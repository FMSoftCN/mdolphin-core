// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "file_util.h"
#include <wtf/Vector.h>

#include "basictypes.h"
#include "eintr_wrapper.h"
#include "file_path.h"
#include "scoped_ptr.h"
#include "string_util.h"
#include "cache_time.h"
#include "CString.h"
#include "FileSystemMg.h"
#include "mdolphin_debug.h"
#include "errno.h"

namespace file_util {

bool CopyFile(const FilePath& from_path, const FilePath& to_path)
{
    WebCore::HFile infile = WebCore::openFile(from_path.value().utf8().data(), "r");
    if (infile == NULL)
        return false;

    WebCore::HFile outfile = WebCore::openFile(to_path.value().utf8().data(), "w");
    if (outfile == NULL) {
        WebCore::closeFile(infile);
        return false;
    }

    const unsigned int kBufferSize = 32768;
    Vector<char> buffer(kBufferSize);
    bool result = true;

    while (result) {
        //    ssize_t bytes_read = HANDLE_EINTR(WebCore::readFile(&buffer[0], buffer.size(), 1, infile));
        //FIXME:suyong ssize_t bytes_read =
        unsigned int bytes_read = WebCore::readFile(&buffer[0], 1, buffer.size(),  infile);
        if (bytes_read < 0 ) {
            result = false;
            break;
        }
        if (bytes_read == 0)
            break;
        // Allow for partial writes
        //          ssize_t bytes_written_per_read = 0;
        unsigned int bytes_written_per_read = 0;		
        do {
            //      ssize_t bytes_written_partial = HANDLE_EINTR(write(
            unsigned int bytes_written_partial = WebCore::writeFile(	  	
                    &buffer[bytes_written_per_read],
                    1, bytes_read - bytes_written_per_read, outfile);
            if (bytes_written_partial < 0) {
                result = false;
                break;
            }
            bytes_written_per_read += bytes_written_partial;
        } while (bytes_written_per_read < bytes_read);
    }

    if (WebCore::closeFile(infile) < 0)
        result = false;
    if (WebCore::closeFile(outfile) < 0)
        result = false;

    return result;
}


typedef struct stat stat_wrapper_t;
static int CallStat(const char *path, stat_wrapper_t *sb)
{
    //  return stat(path, sb);
    return WebCore::statFile(path, sb); 
}

bool AbsolutePath(FilePath* path)
{
    char full_path[PATH_MAX];
    if (WebCore::realpathFile(path->value().utf8().data(), full_path) == NULL)
        return false;
    *path = FilePath(full_path);
    return true;
}

// TODO(erikkay): The Windows version of this accepts paths like "foo/bar/*"
// which works both with and without the recursive flag.  I'm not sure we need
// that functionality. If not, remove from file_util_win.cc, otherwise add it
// here.
bool Delete(const FilePath& path, bool recursive)
{
    //const char* path_str= path.value().utf8().data();
    stat_wrapper_t file_info;
    int test = CallStat(path.value().utf8().data(), &file_info);
    if (test != 0) {
        // md_debug("*********** file [%s] stat error ****************\n", path.value().utf8().data());
        // The Windows version defines this condition as success.
        bool ret = (errno == ENOENT || errno == ENOTDIR);
        return ret;
    }
    if (!S_ISDIR(file_info.st_mode))
    {
        if (WebCore::unlinkFile(path.value().utf8().data()) == 0) {
            // md_debug("*** file [%s] delete SUCCEED **********\n", path.value().utf8().data());
            return true;
        } else {
            // md_debug("*** file [%s]delete FAILED**********\n", path.value().utf8().data());
            return false;
        }
    }
    if (!recursive)
        return (WebCore::rmdirFile(path.value().utf8().data()) == 0);

    bool success = true;
    Vector<String> directories;
    directories.append(path.value());
    FileEnumerator traversal(path, true, static_cast<FileEnumerator::FILE_TYPE>(
                FileEnumerator::FILES | FileEnumerator::DIRECTORIES |
                FileEnumerator::SHOW_SYM_LINKS));
    for (FilePath current = traversal.Next(); success && !current.empty();
            current = traversal.Next()) {
        FileEnumerator::FindInfo info;
        traversal.GetFindInfo(&info);

        if (S_ISDIR(info.stat.st_mode))
            directories.append(current.value());
        else
            success = (WebCore::unlinkFile(current.value().utf8().data()) == 0);
    }

    while (success && !directories.isEmpty()) {
        FilePath dir = FilePath(directories.last());
        directories.removeLast();
        success = (WebCore::rmdirFile(dir.value().utf8().data()) == 0);
    }
    return success;
}

bool Move(const FilePath& from_path, const FilePath& to_path)
{
    // Windows compatibility: if to_path exists, from_path and to_path
    // must be the same type, either both files, or both directories.
    stat_wrapper_t to_file_info;
    if (CallStat(to_path.value().utf8().data(), &to_file_info) == 0) {
        stat_wrapper_t from_file_info;
        if (CallStat(from_path.value().utf8().data(), &from_file_info) == 0) {
            if (S_ISDIR(to_file_info.st_mode) != S_ISDIR(from_file_info.st_mode))
                return false;
        } else {
            return false;
        }
    }

    if (WebCore::renameFile(from_path.value().utf8().data(), to_path.value().utf8().data()) == 0)
        return true;

    if (!CopyDirectory(from_path, to_path, true))
        return false;

    Delete(from_path, true);

    return true;
}

bool ReplaceFile(const FilePath& from_path, const FilePath& to_path)
{
    return (rename(from_path.value().utf8().data(), to_path.value().utf8().data()) == 0);
}

bool CopyDirectory(const FilePath& from_path,
        const FilePath& to_path,
        bool recursive)
{
    // Some old callers of CopyDirectory want it to support wildcards.
    // After some discussion, we decided to fix those callers.
    // Break loudly here if anyone tries to do this.
    // TODO(evanm): remove this once we're sure it's ok.
    //  DCHECK(to_path.value().find('*') == std::string::npos);
    //  DCHECK(from_path.value().find('*') == std::string::npos);

    char top_dir[PATH_MAX];
    if (strlcpy(top_dir, from_path.value().utf8().data(),
                arraysize(top_dir)) >= arraysize(top_dir)) {
        return false;
    }

    // This function does not properly handle destinations within the source
    FilePath real_to_path = to_path;
    if (PathExists(real_to_path)) {
        if (!AbsolutePath(&real_to_path))
            return false;
    } else {
        real_to_path = real_to_path.DirName();
        if (!AbsolutePath(&real_to_path))
            return false;
    }
    FilePath real_from_path = from_path;
    if (!AbsolutePath(&real_from_path))
        return false;
    if (real_to_path.value().length() >= real_from_path.value().length() &&
            real_to_path.value().startsWith(real_from_path.value()) == 0)
        return false;

    bool success = true;
    FileEnumerator::FILE_TYPE traverse_type =
        static_cast<FileEnumerator::FILE_TYPE>(FileEnumerator::FILES |
                FileEnumerator::SHOW_SYM_LINKS);
    if (recursive)
        traverse_type = static_cast<FileEnumerator::FILE_TYPE>(
                traverse_type | FileEnumerator::DIRECTORIES);
    FileEnumerator traversal(from_path, recursive, traverse_type);

    // We have to mimic windows behavior here. |to_path| may not exist yet,
    // start the loop with |to_path|.
    FileEnumerator::FindInfo info;
    FilePath current = from_path;
    if (WebCore::statFile(from_path.value().utf8().data(), &info.stat) < 0) {
        //   LOG(ERROR) << "CopyDirectory() couldn't stat source directory: " <<
        //       from_path.value() << " errno = " << errno;
        success = false;
    }
    struct stat to_path_stat;
    FilePath from_path_base = from_path;
    if (recursive && WebCore::statFile(to_path.value().utf8().data(), &to_path_stat) == 0 &&
            S_ISDIR(to_path_stat.st_mode)) {
        // If the destination already exists and is a directory, then the
        // top level of source needs to be copied.
        from_path_base = from_path.DirName();
    }

    // The Windows version of this function assumes that non-recursive calls
    // will always have a directory for from_path.
    //  DCHECK(recursive || S_ISDIR(info.stat.st_mode));

    while (success && !current.empty()) {
        // current is the source path, including from_path, so paste
        // the suffix after from_path onto to_path to create the target_path.
        String suffix(&current.value().utf8().data()[from_path_base.value().length()]);
        // Strip the leading '/' (if any).
        if (!suffix.isEmpty()) {
            //      DCHECK_EQ('/', suffix[0]);
            suffix.remove(0, 1);
        }
        const FilePath target_path = to_path.Append(suffix);

        if (S_ISDIR(info.stat.st_mode)) {
            if (WebCore::mkdirFile(target_path.value().utf8().data(), info.stat.st_mode & 01777) != 0 &&
                    errno != EEXIST) {
                //        LOG(ERROR) << "CopyDirectory() couldn't create directory: " <<
                //            target_path.value() << " errno = " << errno;
                success = false;
            }
        } else if (S_ISREG(info.stat.st_mode)) {
            if (!CopyFile(current, target_path)) {
                //        LOG(ERROR) << "CopyDirectory() couldn't create file: " <<
                //            target_path.value();
                success = false;
            }
        } else {
            //      LOG(WARNING) << "CopyDirectory() skipping non-regular file: " <<
            //          current.value();
        }

        current = traversal.Next();
        traversal.GetFindInfo(&info);
    }
    return success;
}

bool PathExists(const FilePath& path)
{
    stat_wrapper_t file_info;
    return CallStat(path.value().utf8().data(), &file_info) == 0;
}

bool PathIsWritable(const FilePath& path)
{
    FilePath test_path(path);
    stat_wrapper_t file_info;
    if (CallStat(test_path.value().utf8().data(), &file_info) != 0) {
        // If the path doesn't exist, test the parent dir.
        test_path = test_path.DirName();
        // If the parent dir doesn't exist, then return false (the path is not
        // directly writable).
        if (CallStat(test_path.value().utf8().data(), &file_info) != 0)
            return false;
    }
    if (S_IWOTH & file_info.st_mode)
        return true;
    if (WebCore::getegidFile() == file_info.st_gid && (S_IWGRP & file_info.st_mode))
        return true;
    if (WebCore::getegidFile()  == file_info.st_uid && (S_IWUSR & file_info.st_mode))
        return true;
    return false;
}

bool DirectoryExists(const FilePath& path)
{
    stat_wrapper_t file_info;
    if (CallStat(path.value().utf8().data(), &file_info) == 0)
        return S_ISDIR(file_info.st_mode);
    return false;
}

bool CreateDirectory(const FilePath& full_path)
{
#if 1
    Vector<FilePath> subpaths;

    // Collect a list of all parent directories.
    FilePath last_path = full_path;
    subpaths.append(full_path);
    for (FilePath path = FilePath(full_path.DirName());
            path.value() != last_path.value(); path = path.DirName()) {
        subpaths.append(path);
        last_path = path;
    }

    // Iterate through the parents and create the missing ones.
    /*
       for (Vector<FilePath>::iterator i = subpaths.end() - 1;
       i != subpaths.begin(); --i) {
       if (!DirectoryExists(*i)) {
       if (mkdir(i->value().utf8().data(), 0700) != 0)
       return false;
       }
       }
       */
    int size = (int)subpaths.size();
    for (int i = size - 1; i >= 0; --i)
    {
        FilePath path = subpaths[i];
        if (!DirectoryExists(path)) {
            if (WebCore::mkdirFile(path.value().utf8().data(), 0700) != 0)
                return false;
        }

    }

    return true;
#else
    // md_debug("XXXXXXXXXXXXXXXXXCreateDirectory\n");
    std::vector<FilePath> subpaths;

    // Collect a list of all parent directories.
    FilePath last_path = full_path;
    subpaths.push_back(full_path);
    // md_debug("full path %s\n", last_path.value().utf8().data());
    FilePath path = full_path.DirName();
    // md_debug("last path %s\n", last_path.value().utf8().data());
    // md_debug("path %s\n", path.value().utf8().data());

    for (;
            path.value() != last_path.value();
            path = path.DirName()) {
        subpaths.push_back(path);
        last_path = path;
    }

    // Iterate through the parents and create the missing ones.
    for (std::vector<FilePath>::reverse_iterator i = subpaths.rbegin();
            i != subpaths.rend(); ++i) {
        if (!DirectoryExists(*i)) {
            if (mkdir(i->value().utf8().data(), 0700) != 0)
                return false;
        }   
    }
    return true;

#endif
}

///////////////////////////////////////////////
// FileEnumerator

FileEnumerator::FileEnumerator(const FilePath& root_path,
        bool recursive,
        FileEnumerator::FILE_TYPE file_type)
: root_path_(root_path),
    recursive_(recursive),
    file_type_(file_type),
    is_in_find_op_(false),
    current_directory_entry_(0)
{
        // INCLUDE_DOT_DOT must not be specified if recursive.
        //  DCHECK(!(recursive && (INCLUDE_DOT_DOT & file_type_)));
        pending_paths_.append(root_path);
}

FileEnumerator::FileEnumerator(const FilePath& root_path,
        bool recursive,
        FileEnumerator::FILE_TYPE file_type,
        const FilePath::StringType& pattern)
    : root_path_(root_path),
    recursive_(recursive),
    file_type_(file_type),
    pattern_(root_path.Append(pattern)),
    is_in_find_op_(false),
    current_directory_entry_(0)
{
    // INCLUDE_DOT_DOT must not be specified if recursive.
    //  DCHECK(!(recursive && (INCLUDE_DOT_DOT & file_type_)));
    // The Windows version of this code appends the pattern to the root_path,
    // potentially only matching against items in the top-most directory.
    // Do the same here.
    if (pattern.length() == 0)
        pattern_ = FilePath();
    pending_paths_.append(root_path);
}

FileEnumerator::~FileEnumerator()
{
}



///////////////////////////////////////////////
//// FileEnumerator
////
//// Note: the main logic is in file_util_<platform>.cc
//
bool FileEnumerator::ShouldSkip(const FilePath& path)
{
    FilePath::StringType basename = path.BaseName().value();
    return IsDot(path) || (IsDotDot(path) && !(INCLUDE_DOT_DOT & file_type_));
}

bool FileEnumerator::IsDot(const FilePath& path)
{
    return FILE_PATH_LITERAL(".") == path.BaseName().value();
}

bool FileEnumerator::IsDotDot(const FilePath& path)
{
    return FILE_PATH_LITERAL("..") == path.BaseName().value();
}


void FileEnumerator::GetFindInfo(FindInfo* info)
{
    //  DCHECK(info);

    if (current_directory_entry_ >= directory_entries_.size())
        return;

    DirectoryEntryInfo* cur_entry = &directory_entries_[current_directory_entry_];
    memcpy(&(info->stat), &(cur_entry->stat), sizeof(info->stat));
    info->filename = String(cur_entry->filename.value().utf8().data(), 
            cur_entry->filename.value().length());
}

FilePath FileEnumerator::Next()
{
    ++current_directory_entry_;

    // While we've exhausted the entries in the current directory, do the next
    while (current_directory_entry_ >= directory_entries_.size()) {
        if (pending_paths_.isEmpty())
            return FilePath();

        root_path_ = pending_paths_.last();
        root_path_ = root_path_.StripTrailingSeparators();
        pending_paths_.removeLast();

        Vector<DirectoryEntryInfo> entries;
        if (!ReadDirectory(&entries, root_path_, file_type_ & SHOW_SYM_LINKS))
            continue;

        directory_entries_.clear();
        current_directory_entry_ = 0;
        for (Vector<DirectoryEntryInfo>::const_iterator
                i = entries.begin(); i != entries.end(); ++i) {
            FilePath full_path = root_path_.Append(i->filename);
            if (ShouldSkip(full_path))
                continue;

            if (pattern_.value().length() &&
                    WebCore::fnmatchFile(pattern_.value().utf8().data(), full_path.value().utf8().data(),
                        FNM_NOESCAPE))
                continue;

            if (recursive_ && S_ISDIR(i->stat.st_mode))
                pending_paths_.append(full_path);

            if ((S_ISDIR(i->stat.st_mode) && (file_type_ & DIRECTORIES)) ||
                    (!S_ISDIR(i->stat.st_mode) && (file_type_ & FILES)))
                directory_entries_.append(*i);
        }
    }

    return root_path_.Append(directory_entries_[current_directory_entry_
            ].filename);
}

bool FileEnumerator::ReadDirectory(Vector<DirectoryEntryInfo>* entries,
        const FilePath& source, bool show_links)
{
    DIR* dir = WebCore::opendirFile(source.value().utf8().data());
    if (!dir)
        return false;

    struct dirent dent_buf;
    struct dirent* dent;
    while (readdir_rFile(dir, &dent_buf, &dent) == 0 && dent) {
        DirectoryEntryInfo info;
        info.filename = FilePath(dent->d_name);

        FilePath full_name = source.Append(dent->d_name);
        int ret;
        if (show_links)
            ret = WebCore::lstatFile(full_name.value().utf8().data(), &info.stat);
        else
            ret = WebCore::statFile(full_name.value().utf8().data(), &info.stat);
        if (ret < 0) {
            // Print the stat() error message unless it was ENOENT and we're
            // following symlinks.
            if (!(ret == ENOENT && !show_links)) {
                //        PLOG(ERROR) << "Couldn't stat "
                //                    << source.Append(dent->d_name).value();
            }
            memset(&info.stat, 0, sizeof(info.stat));
        }
        entries->append(info);
    }

    WebCore::closedirFile(dir);
    return true;
}

} // namespace file_util
