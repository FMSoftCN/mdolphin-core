#!/usr/bin/env python
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

"""Chromium Win implementation of the Port interface."""

import logging
import os
import sys

import chromium

_log = logging.getLogger("webkitpy.layout_tests.port.chromium_win")


class ChromiumWinPort(chromium.ChromiumPort):
    """Chromium Win implementation of the Port class."""

    def __init__(self, port_name=None, options=None):
        if port_name is None:
            port_name = 'chromium-win' + self.version()
        if options and not hasattr(options, 'configuration'):
            options.configuration = 'Release'
        chromium.ChromiumPort.__init__(self, port_name, options)

    def baseline_search_path(self):
        port_names = []
        if self._name == 'chromium-win-xp':
            port_names.append("chromium-win-xp")
        if self._name in ('chromium-win-xp', 'chromium-win-vista'):
            port_names.append("chromium-win-vista")
        # FIXME: This may need to include mac-snowleopard like win.py.
        port_names.extend(["chromium-win", "chromium", "win", "mac"])
        return map(self._webkit_baseline_path, port_names)

    def check_build(self, needs_http):
        result = chromium.ChromiumPort.check_build(self, needs_http)
        if not result:
            _log.error('For complete Windows build requirements, please '
                       'see:')
            _log.error('')
            _log.error('    http://dev.chromium.org/developers/how-tos/'
                       'build-instructions-windows')
        return result

    def get_absolute_path(self, filename):
        """Return the absolute path in unix format for the given filename."""
        abspath = os.path.abspath(filename)
        return abspath.replace('\\', '/')

    def relative_test_filename(self, filename):
        path = filename[len(self.layout_tests_dir()) + 1:]
        return path.replace('\\', '/')

    def test_platform_name(self):
        # We return 'win-xp', not 'chromium-win-xp' here, for convenience.
        return 'win' + self.version()

    def version(self):
        if not hasattr(sys, 'getwindowsversion'):
            return ''
        winver = sys.getwindowsversion()
        if winver[0] == 6 and (winver[1] == 1):
            return '-7'
        if winver[0] == 6 and (winver[1] == 0):
            return '-vista'
        if winver[0] == 5 and (winver[1] == 1 or winver[1] == 2):
            return '-xp'
        return ''

    #
    # PROTECTED ROUTINES
    #

    def _build_path(self, *comps):
        p = self.path_from_chromium_base('webkit', *comps)
        if os.path.exists(p):
            return p
        return self.path_from_chromium_base('chrome', *comps)

    def _lighttpd_path(self, *comps):
        return self.path_from_chromium_base('third_party', 'lighttpd', 'win',
                                            *comps)

    def _path_to_apache(self):
        return self.path_from_chromium_base('third_party', 'cygwin', 'usr',
                                            'sbin', 'httpd')

    def _path_to_apache_config_file(self):
        return os.path.join(self.layout_tests_dir(), 'http', 'conf',
                            'cygwin-httpd.conf')

    def _path_to_lighttpd(self):
        return self._lighttpd_path('LightTPD.exe')

    def _path_to_lighttpd_modules(self):
        return self._lighttpd_path('lib')

    def _path_to_lighttpd_php(self):
        return self._lighttpd_path('php5', 'php-cgi.exe')

    def _path_to_driver(self, configuration=None):
        if not configuration:
            configuration = self._options.configuration
        return self._build_path(configuration, 'test_shell.exe')

    def _path_to_helper(self):
        return self._build_path(self._options.configuration, 'layout_test_helper.exe')

    def _path_to_image_diff(self):
        return self._build_path(self._options.configuration, 'image_diff.exe')

    def _path_to_wdiff(self):
        return self.path_from_chromium_base('third_party', 'cygwin', 'bin',
                                            'wdiff.exe')

    def _shut_down_http_server(self, server_pid):
        """Shut down the lighttpd web server. Blocks until it's fully
        shut down.

        Args:
            server_pid: The process ID of the running server.
        """
        # FIXME: Why are we ignoring server_pid and calling
        # _kill_all instead of Executive.kill_process(pid)?
        self._executive.kill_all("LightTPD.exe")
        self._executive.kill_all("httpd.exe")
