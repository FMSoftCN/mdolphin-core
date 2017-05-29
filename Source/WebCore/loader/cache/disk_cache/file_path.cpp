// Copyright (c) 2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "file_path.h"
#include "CString.h"
const FilePath::CharType FilePath::kSeparators[] = FILE_PATH_LITERAL("/");

const FilePath::CharType FilePath::kCurrentDirectory[] = FILE_PATH_LITERAL(".");
const FilePath::CharType FilePath::kParentDirectory[] = FILE_PATH_LITERAL("..");


bool FilePath::operator==(const FilePath& that) const
{
    return path_ == that.path_;
}

bool FilePath::operator!=(const FilePath& that) const
{
    return path_ != that.path_;
}

bool FilePath::IsSeparator(CharType character)
{
    for (size_t i = 0; i < arraysize(kSeparators) - 1; ++i) {
        if (character == kSeparators[i]) {
            return true;
        }
    }

    return false;
}

// If this FilePath contains a drive letter specification, returns the
// position of the last character of the drive letter specification,
// otherwise returns npos.  This can only be true on Windows, when a pathname
// begins with a letter followed by a colon.  On other platforms, this always
// returns npos.
int FindDriveLetter(const FilePath::StringType& path)
{
    return -1;
}

FilePath FilePath::Append(const FilePath& component) const
{
    return Append(component.value());
}

FilePath FilePath::Append(const String& component) const
{
    //  DCHECK(!IsPathAbsolute(component));
    if (path_ == kCurrentDirectory) {
        // Append normally doesn't do any normalization, but as a special case,
        // when appending to kCurrentDirectory, just return a new path for the
        // component argument.  Appending component to kCurrentDirectory would
        // serve no purpose other than needlessly lengthening the path, and
        // it's likely in practice to wind up with FilePath objects containing
        // only kCurrentDirectory when calling DirName on a single relative path
        // component.
        return FilePath(component);
    }

    FilePath new_path(path_);
    new_path.StripTrailingSeparatorsInternal();

    // Don't append a separator if the path is empty (indicating the current
    // directory) or if the path component is empty (indicating nothing to
    // append).
    if (component.length() > 0 && new_path.path_.length() > 0) {
        // Don't append a separator if the path still ends with a trailing
        // separator after stripping (indicating the root directory).
        if (!IsSeparator(new_path.path_[new_path.path_.length() - 1])) {
            // Don't append a separator if the path is just a drive letter.
            if (FindDriveLetter(new_path.path_) + 1 != (int)(new_path.path_.length())) {
                new_path.path_.append(kSeparators[0]);
            }
        }
    }

    new_path.path_.append(component);
    return new_path;
}

FilePath FilePath::AppendASCII(const String& string) const
{
    //  DCHECK(IsStringASCII(component));
    //return Append(component.as_string());
    return Append(string);
}

// libgen's dirname and basename aren't guaranteed to be thread-safe and aren't
// guaranteed to not modify their input strings, and in fact are implemented
// differently in this regard on different platforms.  Don't use them, but
// adhere to their behavior.
FilePath FilePath::DirName() const
{
    FilePath new_path(path_);
    new_path.StripTrailingSeparatorsInternal();

    // The drive letter, if any, always needs to remain in the output.  If there
    // is no drive letter, as will always be the case on platforms which do not
    // support drive letters, letter will be npos, or -1, so the comparisons and
    // resizes below using letter will still be valid.
    int letter = FindDriveLetter(new_path.path_);

    int last_separator = new_path.path_.reverseFind(kSeparators);
    if (last_separator == -1) {
        // path_ is in the current directory.
        new_path.path_.truncate(letter + 1);
    } else if (last_separator == letter + 1) {
        // path_ is in the root directory.
        new_path.path_.truncate(letter + 2);
    } else if (last_separator == letter + 2 &&
            IsSeparator(new_path.path_.utf8().data()[letter + 1])) {
        // path_ is in "//" (possibly with a drive letter); leave the double
        // separator intact indicating alternate root.
        new_path.path_.truncate(letter + 3);
    } else if (last_separator != 0) {
        // path_ is somewhere else, trim the basename.
        new_path.path_.truncate(last_separator);
    }

    new_path.StripTrailingSeparatorsInternal();
    if (!new_path.path_.length())
        new_path.path_ = kCurrentDirectory;

    return new_path;
}

FilePath FilePath::BaseName() const
{
    FilePath new_path(path_);
    new_path.StripTrailingSeparatorsInternal();

    // The drive letter, if any, is always stripped.
    // Keep everything after the final separator, but if the pathname is only
    // one character and it's a separator, leave it alone.
#ifdef EXPOSE_COMPILE_WARNING
    int last_separator = new_path.path_.reverseFind(kSeparators);
    if (last_separator != -1 &&
            last_separator < new_path.path_.length() - 1) {
        new_path.path_.remove(0, last_separator + 1);
    }
#else
    unsigned int last_separator = new_path.path_.reverseFind(kSeparators);
    if (last_separator != (unsigned int)-1 &&
            last_separator < new_path.path_.length() - 1) {
        new_path.path_.remove(0, last_separator + 1);
    }
#endif

    return new_path;
}

FilePath FilePath::StripTrailingSeparators() const
{
    FilePath new_path(path_);
    new_path.StripTrailingSeparatorsInternal();

    return new_path;
}

void FilePath::StripTrailingSeparatorsInternal()
{
    // If there is no drive letter, start will be 1, which will prevent stripping
    // the leading separator if there is only one separator.  If there is a drive
    // letter, start will be set appropriately to prevent stripping the first
    // separator following the drive letter, if a separator immediately follows
    // the drive letter.
    int start = 1;

    int last_stripped = -1;
    for (int pos = path_.length();
            pos > start && IsSeparator(path_[pos - 1]);
            --pos) {
        // If the string only has two separators and they're at the beginning,
        // don't strip them, unless the string began with more than two separators.
        if (pos != start + 1 || last_stripped == start + 2 ||
                !IsSeparator(path_.utf8().data()[start - 1])) {
            path_.truncate(pos - 1);
            last_stripped = pos;
        }
    }
    return;
}
