# Copyright (C) 2010 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Module for handling messages, threads, processes, and concurrency for run-webkit-tests.

Testing is accomplished by having a manager (TestRunner) gather all of the
tests to be run, and sending messages to a pool of workers (TestShellThreads)
to run each test. Each worker communicates with one driver (usually
DumpRenderTree) to run one test at a time and then compare the output against
what we expected to get.

This modules provides a message broker that connects the manager to the
workers: it provides a messaging abstraction and message loops, and
handles launching threads and/or processes depending on the
requested configuration.
"""

import logging
import time

from webkitpy.common.system import stack_utils

import dump_render_tree_thread

_log = logging.getLogger(__name__)


def get(port, options):
    """Return an instance of a WorkerMessageBroker."""
    worker_model = options.worker_model
    if worker_model == 'old-inline':
        return InlineBroker(port, options)
    if worker_model == 'old-threads':
        return MultiThreadedBroker(port, options)
    raise ValueError('unsupported value for --worker-model: %s' % worker_model)


class _WorkerState(object):
    def __init__(self, name):
        self.name = name
        self.thread = None


class WorkerMessageBroker(object):
    def __init__(self, port, options):
        self._port = port
        self._options = options
        self._num_workers = int(self._options.child_processes)

        # This maps worker names to their _WorkerState values.
        self._workers = {}

    def _threads(self):
        return tuple([w.thread for w in self._workers.values()])

    def start_workers(self, test_runner):
        """Starts up the pool of workers for running the tests.

        Args:
            test_runner: a handle to the manager/TestRunner object
        """
        self._test_runner = test_runner
        for worker_number in xrange(self._num_workers):
            worker = _WorkerState('worker-%d' % worker_number)
            worker.thread = self._start_worker(worker_number, worker.name)
            self._workers[worker.name] = worker
        return self._threads()

    def _start_worker(self, worker_number, worker_name):
        raise NotImplementedError

    def run_message_loop(self):
        """Loop processing messages until done."""
        raise NotImplementedError

    def cancel_workers(self):
        """Cancel/interrupt any workers that are still alive."""
        pass

    def cleanup(self):
        """Perform any necessary cleanup on shutdown."""
        pass


class InlineBroker(WorkerMessageBroker):
    def _start_worker(self, worker_number, worker_name):
        # FIXME: Replace with something that isn't a thread.
        thread = dump_render_tree_thread.TestShellThread(self._port,
            self._options, worker_number, worker_name,
            self._test_runner._current_filename_queue,
            self._test_runner._result_queue)
        # Note: Don't start() the thread! If we did, it would actually
        # create another thread and start executing it, and we'd no longer
        # be single-threaded.
        return thread

    def run_message_loop(self):
        thread = self._threads()[0]
        thread.run_in_main_thread(self._test_runner,
                                  self._test_runner._current_result_summary)
        self._test_runner.update()


class MultiThreadedBroker(WorkerMessageBroker):
    def _start_worker(self, worker_number, worker_name):
        thread = dump_render_tree_thread.TestShellThread(self._port,
            self._options, worker_number, worker_name,
            self._test_runner._current_filename_queue,
            self._test_runner._result_queue)
        thread.start()
        return thread

    def run_message_loop(self):
        threads = self._threads()
        wedged_threads = set()

        # Loop through all the threads waiting for them to finish.
        some_thread_is_alive = True
        while some_thread_is_alive:
            some_thread_is_alive = False
            t = time.time()
            for thread in threads:
                if thread.isAlive():
                    if thread in wedged_threads:
                        continue

                    some_thread_is_alive = True
                    next_timeout = thread.next_timeout()
                    if next_timeout and t > next_timeout:
                        stack_utils.log_thread_state(_log.error, thread.getName(), thread.id(), "is wedged")
                        thread.clear_next_timeout()
                        wedged_threads.add(thread)

                exception_info = thread.exception_info()
                if exception_info is not None:
                    # Re-raise the thread's exception here to make it
                    # clear that testing was aborted. Otherwise,
                    # the tests that did not run would be assumed
                    # to have passed.
                    raise exception_info[0], exception_info[1], exception_info[2]

            self._test_runner.update()

            if some_thread_is_alive:
                time.sleep(0.01)

        if wedged_threads:
            _log.warning("All remaining threads are wedged, bailing out.")

    def cancel_workers(self):
        threads = self._threads()
        for thread in threads:
            thread.cancel()
