# Copyright (c) 2009, Google Inc. All rights reserved.
# Copyright (c) 2009 Apple Inc. All rights reserved.
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
#
# Python module for interacting with an SCM system (like SVN or Git)

import os
import re

# FIXME: Instead of using run_command directly, most places in this
# class would rather use an SCM.run method which automatically set
# cwd=self.checkout_root.
from webkitpy.common.system.executive import Executive, run_command, ScriptError
from webkitpy.common.system.user import User
from webkitpy.common.system.deprecated_logging import error, log


def detect_scm_system(path):
    absolute_path = os.path.abspath(path)

    if SVN.in_working_directory(absolute_path):
        return SVN(cwd=absolute_path)
    
    if Git.in_working_directory(absolute_path):
        return Git(cwd=absolute_path)
    
    return None


def first_non_empty_line_after_index(lines, index=0):
    first_non_empty_line = index
    for line in lines[index:]:
        if re.match("^\s*$", line):
            first_non_empty_line += 1
        else:
            break
    return first_non_empty_line


class CommitMessage:
    def __init__(self, message):
        self.message_lines = message[first_non_empty_line_after_index(message, 0):]

    def body(self, lstrip=False):
        lines = self.message_lines[first_non_empty_line_after_index(self.message_lines, 1):]
        if lstrip:
            lines = [line.lstrip() for line in lines]
        return "\n".join(lines) + "\n"

    def description(self, lstrip=False, strip_url=False):
        line = self.message_lines[0]
        if lstrip:
            line = line.lstrip()
        if strip_url:
            line = re.sub("^(\s*)<.+> ", "\1", line)
        return line

    def message(self):
        return "\n".join(self.message_lines) + "\n"


class CheckoutNeedsUpdate(ScriptError):
    def __init__(self, script_args, exit_code, output, cwd):
        ScriptError.__init__(self, script_args=script_args, exit_code=exit_code, output=output, cwd=cwd)


def commit_error_handler(error):
    if re.search("resource out of date", error.output):
        raise CheckoutNeedsUpdate(script_args=error.script_args, exit_code=error.exit_code, output=error.output, cwd=error.cwd)
    Executive.default_error_handler(error)


# SCM methods are expected to return paths relative to self.checkout_root.
class SCM:
    def __init__(self, cwd):
        self.cwd = cwd
        self.checkout_root = self.find_checkout_root(self.cwd)
        self.dryrun = False

    # SCM always returns repository relative path, but sometimes we need
    # absolute paths to pass to rm, etc.
    def absolute_path(self, repository_relative_path):
        return os.path.join(self.checkout_root, repository_relative_path)

    # FIXME: This belongs in Checkout, not SCM.
    def scripts_directory(self):
        return os.path.join(self.checkout_root, "WebKitTools", "Scripts")

    # FIXME: This belongs in Checkout, not SCM.
    def script_path(self, script_name):
        return os.path.join(self.scripts_directory(), script_name)

    def ensure_clean_working_directory(self, force_clean):
        if not force_clean and not self.working_directory_is_clean():
            # FIXME: Shouldn't this use cwd=self.checkout_root?
            print run_command(self.status_command(), error_handler=Executive.ignore_error)
            raise ScriptError(message="Working directory has modifications, pass --force-clean or --no-clean to continue.")
        
        log("Cleaning working directory")
        self.clean_working_directory()
    
    def ensure_no_local_commits(self, force):
        if not self.supports_local_commits():
            return
        commits = self.local_commits()
        if not len(commits):
            return
        if not force:
            error("Working directory has local commits, pass --force-clean to continue.")
        self.discard_local_commits()

    def run_status_and_extract_filenames(self, status_command, status_regexp):
        filenames = []
        # We run with cwd=self.checkout_root so that returned-paths are root-relative.
        for line in run_command(status_command, cwd=self.checkout_root).splitlines():
            match = re.search(status_regexp, line)
            if not match:
                continue
            # status = match.group('status')
            filename = match.group('filename')
            filenames.append(filename)
        return filenames

    def strip_r_from_svn_revision(self, svn_revision):
        match = re.match("^r(?P<svn_revision>\d+)", unicode(svn_revision))
        if (match):
            return match.group('svn_revision')
        return svn_revision

    def svn_revision_from_commit_text(self, commit_text):
        match = re.search(self.commit_success_regexp(), commit_text, re.MULTILINE)
        return match.group('svn_revision')

    @staticmethod
    def in_working_directory(path):
        raise NotImplementedError, "subclasses must implement"

    @staticmethod
    def find_checkout_root(path):
        raise NotImplementedError, "subclasses must implement"

    @staticmethod
    def commit_success_regexp():
        raise NotImplementedError, "subclasses must implement"

    def working_directory_is_clean(self):
        raise NotImplementedError, "subclasses must implement"

    def clean_working_directory(self):
        raise NotImplementedError, "subclasses must implement"

    def status_command(self):
        raise NotImplementedError, "subclasses must implement"

    def add(self, path):
        raise NotImplementedError, "subclasses must implement"

    def changed_files(self, git_commit=None, squash=None):
        raise NotImplementedError, "subclasses must implement"

    def changed_files_for_revision(self):
        raise NotImplementedError, "subclasses must implement"

    def added_files(self):
        raise NotImplementedError, "subclasses must implement"

    def conflicted_files(self):
        raise NotImplementedError, "subclasses must implement"

    def display_name(self):
        raise NotImplementedError, "subclasses must implement"

    def create_patch(self, git_commit=None, squash=None):
        raise NotImplementedError, "subclasses must implement"

    def committer_email_for_revision(self, revision):
        raise NotImplementedError, "subclasses must implement"

    def contents_at_revision(self, path, revision):
        raise NotImplementedError, "subclasses must implement"

    def diff_for_revision(self, revision):
        raise NotImplementedError, "subclasses must implement"

    def apply_reverse_diff(self, revision):
        raise NotImplementedError, "subclasses must implement"

    def revert_files(self, file_paths):
        raise NotImplementedError, "subclasses must implement"

    def should_squash(self, squash):
        raise NotImplementedError, "subclasses must implement"

    def commit_with_message(self, message, username=None, git_commit=None, squash=None):
        raise NotImplementedError, "subclasses must implement"

    def svn_commit_log(self, svn_revision):
        raise NotImplementedError, "subclasses must implement"

    def last_svn_commit_log(self):
        raise NotImplementedError, "subclasses must implement"

    # Subclasses must indicate if they support local commits,
    # but the SCM baseclass will only call local_commits methods when this is true.
    @staticmethod
    def supports_local_commits():
        raise NotImplementedError, "subclasses must implement"

    def svn_merge_base():
        raise NotImplementedError, "subclasses must implement"

    def commit_locally_with_message(self, message):
        error("Your source control manager does not support local commits.")

    def discard_local_commits(self):
        pass

    def local_commits(self):
        return []


class SVN(SCM):
    # FIXME: We should move these values to a WebKit-specific config. file.
    svn_server_host = "svn.webkit.org"
    svn_server_realm = "<http://svn.webkit.org:80> Mac OS Forge"

    def __init__(self, cwd):
        SCM.__init__(self, cwd)
        self.cached_version = None
    
    @staticmethod
    def in_working_directory(path):
        return os.path.isdir(os.path.join(path, '.svn'))
    
    @classmethod
    def find_uuid(cls, path):
        if not cls.in_working_directory(path):
            return None
        return cls.value_from_svn_info(path, 'Repository UUID')

    @classmethod
    def value_from_svn_info(cls, path, field_name):
        svn_info_args = ['svn', 'info', path]
        info_output = run_command(svn_info_args).rstrip()
        match = re.search("^%s: (?P<value>.+)$" % field_name, info_output, re.MULTILINE)
        if not match:
            raise ScriptError(script_args=svn_info_args, message='svn info did not contain a %s.' % field_name)
        return match.group('value')

    @staticmethod
    def find_checkout_root(path):
        uuid = SVN.find_uuid(path)
        # If |path| is not in a working directory, we're supposed to return |path|.
        if not uuid:
            return path
        # Search up the directory hierarchy until we find a different UUID.
        last_path = None
        while True:
            if uuid != SVN.find_uuid(path):
                return last_path
            last_path = path
            (path, last_component) = os.path.split(path)
            if last_path == path:
                return None

    @staticmethod
    def commit_success_regexp():
        return "^Committed revision (?P<svn_revision>\d+)\.$"

    def has_authorization_for_realm(self, realm=svn_server_realm, home_directory=os.getenv("HOME")):
        # Assumes find and grep are installed.
        if not os.path.isdir(os.path.join(home_directory, ".subversion")):
            return False
        find_args = ["find", ".subversion", "-type", "f", "-exec", "grep", "-q", realm, "{}", ";", "-print"];
        find_output = run_command(find_args, cwd=home_directory, error_handler=Executive.ignore_error).rstrip()
        return find_output and os.path.isfile(os.path.join(home_directory, find_output))

    def svn_version(self):
        if not self.cached_version:
            self.cached_version = run_command(['svn', '--version', '--quiet'])
        
        return self.cached_version

    def working_directory_is_clean(self):
        return run_command(["svn", "diff"], cwd=self.checkout_root, decode_output=False) == ""

    def clean_working_directory(self):
        # svn revert -R is not as awesome as git reset --hard.
        # It will leave added files around, causing later svn update
        # calls to fail on the bots.  We make this mirror git reset --hard
        # by deleting any added files as well.
        added_files = reversed(sorted(self.added_files()))
        # added_files() returns directories for SVN, we walk the files in reverse path
        # length order so that we remove files before we try to remove the directories.
        run_command(["svn", "revert", "-R", "."], cwd=self.checkout_root)
        for path in added_files:
            # This is robust against cwd != self.checkout_root
            absolute_path = self.absolute_path(path)
            # Completely lame that there is no easy way to remove both types with one call.
            if os.path.isdir(path):
                os.rmdir(absolute_path)
            else:
                os.remove(absolute_path)

    def status_command(self):
        return ['svn', 'status']

    def _status_regexp(self, expected_types):
        field_count = 6 if self.svn_version() > "1.6" else 5
        return "^(?P<status>[%s]).{%s} (?P<filename>.+)$" % (expected_types, field_count)

    def add(self, path):
        # path is assumed to be cwd relative?
        run_command(["svn", "add", path])

    def changed_files(self, git_commit=None, squash=None):
        return self.run_status_and_extract_filenames(self.status_command(), self._status_regexp("ACDMR"))

    def changed_files_for_revision(self, revision):
        # As far as I can tell svn diff --summarize output looks just like svn status output.
        # No file contents printed, thus utf-8 auto-decoding in run_command is fine.
        status_command = ["svn", "diff", "--summarize", "-c", revision]
        return self.run_status_and_extract_filenames(status_command, self._status_regexp("ACDMR"))

    def conflicted_files(self):
        return self.run_status_and_extract_filenames(self.status_command(), self._status_regexp("C"))

    def added_files(self):
        return self.run_status_and_extract_filenames(self.status_command(), self._status_regexp("A"))

    @staticmethod
    def supports_local_commits():
        return False

    def display_name(self):
        return "svn"

    def create_patch(self, git_commit=None, squash=None):
        """Returns a byte array (str()) representing the patch file.
        Patch files are effectively binary since they may contain
        files of multiple different encodings."""
        return run_command([self.script_path("svn-create-patch")],
            cwd=self.checkout_root, return_stderr=False,
            decode_output=False)

    def committer_email_for_revision(self, revision):
        return run_command(["svn", "propget", "svn:author", "--revprop", "-r", revision]).rstrip()

    def contents_at_revision(self, path, revision):
        """Returns a byte array (str()) containing the contents
        of path @ revision in the repository."""
        remote_path = "%s/%s" % (self._repository_url(), path)
        return run_command(["svn", "cat", "-r", revision, remote_path], decode_output=False)

    def diff_for_revision(self, revision):
        # FIXME: This should probably use cwd=self.checkout_root
        return run_command(['svn', 'diff', '-c', revision])

    def _repository_url(self):
        return self.value_from_svn_info(self.checkout_root, 'URL')

    def apply_reverse_diff(self, revision):
        # '-c -revision' applies the inverse diff of 'revision'
        svn_merge_args = ['svn', 'merge', '--non-interactive', '-c', '-%s' % revision, self._repository_url()]
        log("WARNING: svn merge has been known to take more than 10 minutes to complete.  It is recommended you use git for rollouts.")
        log("Running '%s'" % " ".join(svn_merge_args))
        # FIXME: Should this use cwd=self.checkout_root?
        run_command(svn_merge_args)

    def revert_files(self, file_paths):
        # FIXME: This should probably use cwd=self.checkout_root.
        run_command(['svn', 'revert'] + file_paths)

    def should_squash(self, squash):
        # SVN doesn't support the concept of squashing.
        return False

    def commit_with_message(self, message, username=None, git_commit=None, squash=None):
        # squash and git-commit are not used by SVN.
        if self.dryrun:
            # Return a string which looks like a commit so that things which parse this output will succeed.
            return "Dry run, no commit.\nCommitted revision 0."
        svn_commit_args = ["svn", "commit"]
        if not username and not self.has_authorization_for_realm():
            username = User.prompt("%s login: " % self.svn_server_host, repeat=5)
            if not username:
                raise Exception("You need to specify the username on %s to perform the commit as." % self.svn_server_host)
        if username:
            svn_commit_args.extend(["--username", username])
        svn_commit_args.extend(["-m", message])
        # FIXME: Should this use cwd=self.checkout_root?
        return run_command(svn_commit_args, error_handler=commit_error_handler)

    def svn_commit_log(self, svn_revision):
        svn_revision = self.strip_r_from_svn_revision(svn_revision)
        return run_command(['svn', 'log', '--non-interactive', '--revision', svn_revision]);

    def last_svn_commit_log(self):
        # BASE is the checkout revision, HEAD is the remote repository revision
        # http://svnbook.red-bean.com/en/1.0/ch03s03.html
        return self.svn_commit_log('BASE')

# All git-specific logic should go here.
class Git(SCM):
    def __init__(self, cwd):
        SCM.__init__(self, cwd)

    @classmethod
    def in_working_directory(cls, path):
        return run_command(['git', 'rev-parse', '--is-inside-work-tree'], cwd=path, error_handler=Executive.ignore_error).rstrip() == "true"

    @classmethod
    def find_checkout_root(cls, path):
        # "git rev-parse --show-cdup" would be another way to get to the root
        (checkout_root, dot_git) = os.path.split(run_command(['git', 'rev-parse', '--git-dir'], cwd=path))
        # If we were using 2.6 # checkout_root = os.path.relpath(checkout_root, path)
        if not os.path.isabs(checkout_root): # Sometimes git returns relative paths
            checkout_root = os.path.join(path, checkout_root)
        return checkout_root

    @classmethod
    def read_git_config(cls, key):
        # FIXME: This should probably use cwd=self.checkout_root.
        return run_command(["git", "config", key],
            error_handler=Executive.ignore_error).rstrip('\n')

    @staticmethod
    def commit_success_regexp():
        return "^Committed r(?P<svn_revision>\d+)$"

    def discard_local_commits(self):
        # FIXME: This should probably use cwd=self.checkout_root
        run_command(['git', 'reset', '--hard', self.svn_branch_name()])
    
    def local_commits(self):
        # FIXME: This should probably use cwd=self.checkout_root
        return run_command(['git', 'log', '--pretty=oneline', 'HEAD...' + self.svn_branch_name()]).splitlines()

    def rebase_in_progress(self):
        return os.path.exists(os.path.join(self.checkout_root, '.git/rebase-apply'))

    def working_directory_is_clean(self):
        # FIXME: This should probably use cwd=self.checkout_root
        return run_command(['git', 'diff', 'HEAD', '--name-only']) == ""

    def clean_working_directory(self):
        # FIXME: These should probably use cwd=self.checkout_root.
        # Could run git clean here too, but that wouldn't match working_directory_is_clean
        run_command(['git', 'reset', '--hard', 'HEAD'])
        # Aborting rebase even though this does not match working_directory_is_clean
        if self.rebase_in_progress():
            run_command(['git', 'rebase', '--abort'])

    def status_command(self):
        # git status returns non-zero when there are changes, so we use git diff name --name-status HEAD instead.
        # No file contents printed, thus utf-8 autodecoding in run_command is fine.
        return ["git", "diff", "--name-status", "HEAD"]

    def _status_regexp(self, expected_types):
        return '^(?P<status>[%s])\t(?P<filename>.+)$' % expected_types

    def add(self, path):
        # path is assumed to be cwd relative?
        run_command(["git", "add", path])

    def _merge_base(self, git_commit, squash):
        if git_commit:
            # FIXME: Calling code should turn commit ranges into a list of commit IDs
            # and then treat each commit separately.
            if '..' not in git_commit:
                git_commit = git_commit + "^.." + git_commit
            return git_commit

        if self.should_squash(squash):
            return self.svn_merge_base()

        # FIXME: Non-squash behavior should match commit_with_message. It raises an error
        # if there are working copy changes and --squash or --no-squash wasn't passed in.
        # If --no-squash, then it should proceed with each local commit as a separate patch.
        return 'HEAD'

    def changed_files(self, git_commit=None, squash=None):
        status_command = ['git', 'diff', '-r', '--name-status', '-C', '-M', "--no-ext-diff", "--full-index", self._merge_base(git_commit, squash)]
        return self.run_status_and_extract_filenames(status_command, self._status_regexp("ADM"))

    def _changes_files_for_commit(self, git_commit):
        # --pretty="format:" makes git show not print the commit log header,
        changed_files = run_command(["git", "show", "--pretty=format:", "--name-only", git_commit]).splitlines()
        # instead it just prints a blank line at the top, so we skip the blank line:
        return changed_files[1:]

    def changed_files_for_revision(self, revision):
        commit_id = self.git_commit_from_svn_revision(revision)
        return self._changes_files_for_commit(commit_id)

    def conflicted_files(self):
        # We do not need to pass decode_output for this diff command
        # as we're passing --name-status which does not output any data.
        status_command = ['git', 'diff', '--name-status', '-C', '-M', '--diff-filter=U']
        return self.run_status_and_extract_filenames(status_command, self._status_regexp("U"))

    def added_files(self):
        return self.run_status_and_extract_filenames(self.status_command(), self._status_regexp("A"))

    @staticmethod
    def supports_local_commits():
        return True

    def display_name(self):
        return "git"

    def create_patch(self, git_commit=None, squash=None):
        """Returns a byte array (str()) representing the patch file.
        Patch files are effectively binary since they may contain
        files of multiple different encodings."""
        # FIXME: This should probably use cwd=self.checkout_root
        return run_command(['git', 'diff', '--binary', "--no-ext-diff", "--full-index", "-M", self._merge_base(git_commit, squash)], decode_output=False)

    @classmethod
    def git_commit_from_svn_revision(cls, revision):
        # FIXME: This should probably use cwd=self.checkout_root
        git_commit = run_command(['git', 'svn', 'find-rev', 'r%s' % revision]).rstrip()
        # git svn find-rev always exits 0, even when the revision is not found.
        if not git_commit:
            raise ScriptError(message='Failed to find git commit for revision %s, your checkout likely needs an update.' % revision)
        return git_commit

    def contents_at_revision(self, path, revision):
        """Returns a byte array (str()) containing the contents
        of path @ revision in the repository."""
        return run_command(["git", "show", "%s:%s" % (self.git_commit_from_svn_revision(revision), path)], decode_output=False)

    def diff_for_revision(self, revision):
        git_commit = self.git_commit_from_svn_revision(revision)
        return self.create_patch(git_commit)

    def committer_email_for_revision(self, revision):
        git_commit = self.git_commit_from_svn_revision(revision)
        committer_email = run_command(["git", "log", "-1", "--pretty=format:%ce", git_commit])
        # Git adds an extra @repository_hash to the end of every committer email, remove it:
        return committer_email.rsplit("@", 1)[0]

    def apply_reverse_diff(self, revision):
        # Assume the revision is an svn revision.
        git_commit = self.git_commit_from_svn_revision(revision)
        # I think this will always fail due to ChangeLogs.
        run_command(['git', 'revert', '--no-commit', git_commit], error_handler=Executive.ignore_error)

    def revert_files(self, file_paths):
        run_command(['git', 'checkout', 'HEAD'] + file_paths)

    def should_squash(self, squash):
        if squash is not None:
            # Squash is specified on the command-line.
            return squash

        config_squash = Git.read_git_config('webkit-patch.squash')
        if (config_squash and config_squash is not ""):
            return config_squash.lower() == "true"

        # Only raise an error if there are actually multiple commits to squash.
        num_local_commits = len(self.local_commits())
        if num_local_commits > 1 or num_local_commits > 0 and not self.working_directory_is_clean():
            working_directory_message = "" if self.working_directory_is_clean() else " and working copy changes"
            raise ScriptError(message="""There are %s local commits%s. Do one of the following:
1) Use --squash or --no-squash
2) git config webkit-patch.squash true/false
""" % (num_local_commits, working_directory_message))

        return None

    def commit_with_message(self, message, username=None, git_commit=None, squash=None):
        # Username is ignored during Git commits.
        if git_commit:
            # Need working directory changes to be committed so we can checkout the merge branch.
            if not self.working_directory_is_clean():
                # FIXME: webkit-patch land will modify the ChangeLogs to correct the reviewer.
                # That will modify the working-copy and cause us to hit this error.
                # The ChangeLog modification could be made to modify the existing local commit?
                raise ScriptError(message="Working copy is modified. Cannot commit individual git_commits.")
            return self._commit_on_branch(message, git_commit)

        squash = self.should_squash(squash)
        if squash:
            run_command(['git', 'reset', '--soft', self.svn_branch_name()])
            self.commit_locally_with_message(message)
        elif not self.working_directory_is_clean():
            if not len(self.local_commits()):
                # There are only working copy changes. Assume they should be committed.
                self.commit_locally_with_message(message)
            elif squash is None:
                # The user didn't explicitly say to squash or not squash. There are local commits
                # and working copy changes. Not clear what the user wants.
                raise ScriptError(message="""There are local commits and working copy changes. Do one of the following:
1) Commit/revert working copy changes.
2) Use --squash or --no-squash
3) git config webkit-patch.squash true/false
""")

        # FIXME: This will commit all local commits, each with it's own message. We should restructure
        # so that each local commit has the appropriate commit message based off it's ChangeLogs.
        return self.push_local_commits_to_server()

    def _commit_on_branch(self, message, git_commit):
        branch_ref = run_command(['git', 'symbolic-ref', 'HEAD']).strip()
        branch_name = branch_ref.replace('refs/heads/', '')
        commit_ids = self.commit_ids_from_commitish_arguments([git_commit])

        # We want to squash all this branch's commits into one commit with the proper description.
        # We do this by doing a "merge --squash" into a new commit branch, then dcommitting that.
        MERGE_BRANCH = 'webkit-patch-land'
        self.delete_branch(MERGE_BRANCH)

        # We might be in a directory that's present in this branch but not in the
        # trunk.  Move up to the top of the tree so that git commands that expect a
        # valid CWD won't fail after we check out the merge branch.
        os.chdir(self.checkout_root)

        # Stuff our change into the merge branch.
        # We wrap in a try...finally block so if anything goes wrong, we clean up the branches.
        commit_succeeded = True
        try:
            run_command(['git', 'checkout', '-q', '-b', MERGE_BRANCH, self.svn_branch_name()])

            for commit in commit_ids:
                # We're on a different branch now, so convert "head" to the branch name.
                commit = re.sub(r'(?i)head', branch_name, commit)
                # FIXME: Once changed_files and create_patch are modified to separately handle each
                # commit in a commit range, commit each cherry pick so they'll get dcommitted separately.
                run_command(['git', 'cherry-pick', '--no-commit', commit])

            run_command(['git', 'commit', '-m', message])
            output = self.push_local_commits_to_server()
        except Exception, e:
            log("COMMIT FAILED: " + str(e))
            output = "Commit failed."
            commit_succeeded = False
        finally:
            # And then swap back to the original branch and clean up.
            self.clean_working_directory()
            run_command(['git', 'checkout', '-q', branch_name])
            self.delete_branch(MERGE_BRANCH)

        return output

    def svn_commit_log(self, svn_revision):
        svn_revision = self.strip_r_from_svn_revision(svn_revision)
        return run_command(['git', 'svn', 'log', '-r', svn_revision])

    def last_svn_commit_log(self):
        return run_command(['git', 'svn', 'log', '--limit=1'])

    # Git-specific methods:

    def delete_branch(self, branch):
        if run_command(['git', 'show-ref', '--quiet', '--verify', 'refs/heads/' + branch], return_exit_code=True) == 0:
            run_command(['git', 'branch', '-D', branch])

    def svn_merge_base(self):
        return run_command(['git', 'merge-base', self.svn_branch_name(), 'HEAD']).strip()

    def svn_branch_name(self):
        # FIXME: This should so something like: Git.read_git_config('svn-remote.svn.fetch').split(':')[1]
        # but that doesn't work if the git repo is tracking multiple svn branches.
        return 'trunk'

    def commit_locally_with_message(self, message):
        run_command(['git', 'commit', '--all', '-F', '-'], input=message)

    def push_local_commits_to_server(self):
        dcommit_command = ['git', 'svn', 'dcommit']
        if self.dryrun:
            dcommit_command.append('--dry-run')
        output = run_command(dcommit_command, error_handler=commit_error_handler)
        # Return a string which looks like a commit so that things which parse this output will succeed.
        if self.dryrun:
            output += "\nCommitted r0"
        return output

    # This function supports the following argument formats:
    # no args : rev-list trunk..HEAD
    # A..B    : rev-list A..B
    # A...B   : error!
    # A B     : [A, B]  (different from git diff, which would use "rev-list A..B")
    def commit_ids_from_commitish_arguments(self, args):
        if not len(args):
            args.append('%s..HEAD' % self.svn_branch_name())

        commit_ids = []
        for commitish in args:
            if '...' in commitish:
                raise ScriptError(message="'...' is not supported (found in '%s'). Did you mean '..'?" % commitish)
            elif '..' in commitish:
                commit_ids += reversed(run_command(['git', 'rev-list', commitish]).splitlines())
            else:
                # Turn single commits or branch or tag names into commit ids.
                commit_ids += run_command(['git', 'rev-parse', '--revs-only', commitish]).splitlines()
        return commit_ids

    def commit_message_for_local_commit(self, commit_id):
        commit_lines = run_command(['git', 'cat-file', 'commit', commit_id]).splitlines()

        # Skip the git headers.
        first_line_after_headers = 0
        for line in commit_lines:
            first_line_after_headers += 1
            if line == "":
                break
        return CommitMessage(commit_lines[first_line_after_headers:])

    def files_changed_summary_for_commit(self, commit_id):
        return run_command(['git', 'diff-tree', '--shortstat', '--no-commit-id', commit_id])
