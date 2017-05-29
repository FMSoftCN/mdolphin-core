// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file contains utility functions for dealing with the local
// filesystem.

#ifndef BASE_FILE_UTIL_H_
#define BASE_FILE_UTIL_H_

#include "build_config.h"

#include <sys/stat.h>

#include <stdio.h>

#include <wtf/Vector.h>

#include "FileSystemMg.h"
#include "basictypes.h"
#include "file_path.h"
#include "scoped_ptr.h"
#include "cache_time.h"

namespace base {
class Time;
}

namespace file_util {

//-----------------------------------------------------------------------------
// Functions that operate purely on a path string w/o touching the filesystem:

// Returns true if the given path ends with a path separator character.
//bool EndsWithSeparator(const FilePath& path);


// WARNING: USING THIS WITH recursive==true IS EQUIVALENT
//          TO "rm -rf", SO USE WITH CAUTION.
//  delete a file or path
bool Delete(const FilePath& path, bool recursive);
/*
// Deprecated temporary compatibility function.
bool Delete(const std::wstring& path, bool recursive);
*/
// Moves the given path, whether it's a file or a directory.
// If a simple rename is not possible, such as in the case where the paths are
// on different volumes, this will attempt to copy and delete. Returns
// true for success.
bool Move(const FilePath& from_path, const FilePath& to_path);
/*
// Renames file |from_path| to |to_path|. Both paths must be on the same
// volume, or the function will fail. Destination file will be created
// if it doesn't exist. Prefer this function over Move when dealing with
// temporary files. On Windows it preserves attributes of the target file.
// Returns true on success.
bool ReplaceFile(const FilePath& from_path, const FilePath& to_path);
*/
// Copies a single file. Use CopyDirectory to copy directories.
bool CopyFile(const FilePath& from_path, const FilePath& to_path);
// Copies the given path, and optionally all subdirectories and their contents
// as well.
// If there are files existing under to_path, always overwrite.
// Returns true if successful, false otherwise.
// Don't use wildcards on the names, it may stop working without notice.
//
// If you only need to copy a file use CopyFile, it's faster.
bool CopyDirectory(const FilePath& from_path, const FilePath& to_path,
        bool recursive);
// Returns true if the given path exists on the local filesystem,
// false otherwise.
bool PathExists(const FilePath& path);

// Get the temporary directory provided by the system.
//bool GetTempDir(FilePath* path);

// Creates a directory, as well as creating any parent directories, if they
// don't exist. Returns 'true' on successful creation, or if the directory
// already exists.  The directory is only readable by the current user.
bool CreateDirectory(const FilePath& full_path);
// Deprecated temporary compatibility function.

// Used to hold information about a given file path.  See GetFileInfo below.
struct FileInfo {
    // The size of the file in bytes.  Undefined when is_directory is true.
    int64 size;

    // True if the file corresponds to a directory.
    bool is_directory;

    // The last modified time of a file.
    base::Time last_modified;

    // Add additional fields here as needed.
};

//FILE* OpenFile(const FilePath& filename, const char* mode);

// A class for enumerating the files in a provided path. The order of the
// results is not guaranteed.
//
// DO NOT USE FROM THE MAIN THREAD of your application unless it is a test
// program where latency does not matter. This class is blocking.
class FileEnumerator {
    public:
        typedef struct {
            struct stat stat;
            String filename;
        } FindInfo;

        enum FILE_TYPE {
            FILES                 = 1 << 0,
            DIRECTORIES           = 1 << 1,
            INCLUDE_DOT_DOT       = 1 << 2,
            SHOW_SYM_LINKS        = 1 << 4,
        };

        // |root_path| is the starting directory to search for. It may or may not end
        // in a slash.
        //
        // If |recursive| is true, this will enumerate all matches in any
        // subdirectories matched as well. It does a breadth-first search, so all
        // files in one directory will be returned before any files in a
        // subdirectory.
        //
        // |file_type| specifies whether the enumerator should match files,
        // directories, or both.
        //
        // |pattern| is an optional pattern for which files to match. This
        // works like shell globbing. For example, "*.txt" or "Foo???.doc".
        // However, be careful in specifying patterns that aren't cross platform
        // since the underlying code uses OS-specific matching routines.  In general,
        // Windows matching is less featureful than others, so test there first.
        // If unspecified, this will match all files.
        // NOTE: the pattern only matches the contents of root_path, not files in
        // recursive subdirectories.
        // TODO(erikkay): Fix the pattern matching to work at all levels.
        FileEnumerator(const FilePath& root_path,
                bool recursive,
                FileEnumerator::FILE_TYPE file_type);
        FileEnumerator(const FilePath& root_path,
                bool recursive,
                FileEnumerator::FILE_TYPE file_type,
                const FilePath::StringType& pattern);
        ~FileEnumerator();

        // Returns an empty string if there are no more results.
        FilePath Next();

        // Write the file info into |info|.
        void GetFindInfo(FindInfo* info);

    private:
        FilePath root_path_;
        bool recursive_;
        FILE_TYPE file_type_;
        FilePath pattern_;  // Empty when we want to find everything.

        // Set to true when there is a find operation open. This way, we can lazily
        // start the operations when the caller calls Next().
        bool is_in_find_op_;

        // A stack that keeps track of which subdirectories we still need to
        // enumerate in the breadth-first search.
        Vector<FilePath> pending_paths_;

        // Returns true if the given path should be skipped in enumeration.
        bool ShouldSkip(const FilePath& path);

        // Returns true if the given path's base name is ".".
        bool IsDot(const FilePath& path);

        // Returns true if the given path's base name is "..".
        bool IsDotDot(const FilePath& path);

        typedef struct {
            FilePath filename;
            struct stat stat;
        } DirectoryEntryInfo;

        // Read the filenames in source into the vector of DirectoryEntryInfo's
        static bool ReadDirectory(Vector<DirectoryEntryInfo>* entries,
                const FilePath& source, bool show_links);

        // The files in the current directory
        Vector<DirectoryEntryInfo> directory_entries_;

        // The next entry to use from the directory_entries_ vector
        size_t current_directory_entry_;

        DISALLOW_COPY_AND_ASSIGN(FileEnumerator);
};

}  // namespace file_util

#endif  // BASE_FILE_UTIL_H_

