// Copyright (c) 2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_FILE_PATH_H_
#define BASE_FILE_PATH_H_

#include "basictypes.h"
//#include "compiler_specific.h"
#include "PlatformString.h"
#include "CString.h"

using WTF::String;

#define FILE_PATH_LITERAL(x) x
// An abstraction to isolate users from the differences between native
// pathnames on different platforms.
class FilePath {
    public:
        typedef String StringType;

        //typedef StringType::value_type CharType;
        typedef char CharType;

        // Null-terminated array of separators used to separate components in
        // hierarchical paths.  Each character in this array is a valid separator,
        // but kSeparators[0] is treated as the canonical separator and will be used
        // when composing pathnames.
        static const CharType kSeparators[];

        // A special path component meaning "this directory."
        static const CharType kCurrentDirectory[];

        // A special path component meaning "the parent directory."
        static const CharType kParentDirectory[];

        // The character used to identify a file extension.
        // static const CharType kExtensionSeparator;

        FilePath() {}
        FilePath(const FilePath& that)
        {
            path_ = String(that.path_.utf8().data(), that.path_.length());
        }

        explicit FilePath(const StringType& path)
        {
            path_ = String(path.utf8().data(), path.length());
        }

        FilePath& operator=(const FilePath& that)
        {
            path_ = String(that.path_.utf8().data(), that.path_.length());
            return *this;
        }

        bool operator==(const FilePath& that) const;

        bool operator!=(const FilePath& that) const;

        // Returns a copy of this FilePath that does not end with a trailing
        // separator.
        FilePath StripTrailingSeparators() const;

        const StringType& value() const { return path_; }

        bool empty() const { return path_.isEmpty(); }

        // Returns true if |character| is in kSeparators.
        static bool IsSeparator(CharType character);

        FilePath Append(const StringType& component) const;
        FilePath Append(const FilePath& component) const;

        // Although Windows StringType is std::wstring, since the encoding it uses for
        // paths is well defined, it can handle ASCII path components as well.
        // Mac uses UTF8, and since ASCII is a subset of that, it works there as well.
        // On Linux, although it can use any 8-bit encoding for paths, we assume that
        // ASCII is a valid subset, regardless of the encoding, since many operating
        // system paths will always be ASCII.
        FilePath AppendASCII(const String& string) const;

        // Returns a FilePath corresponding to the directory containing the path
        // named by this object, stripping away the file component.  If this object
        // only contains one component, returns a FilePath identifying
        // kCurrentDirectory.  If this object already refers to the root directory,
        // returns a FilePath identifying the root directory.
        FilePath DirName() const;

        // Returns a FilePath corresponding to the last path component of this
        // object, either a file or a directory.  If this object already refers to
        // the root directory, returns a FilePath identifying the root directory;
        // this is the only situation in which BaseName will return an absolute path.
        FilePath BaseName() const;

    private:
        // Remove trailing separators from this object.  If the path is absolute, it
        // will never be stripped any more than to refer to the absolute root
        // directory, so "////" will become "/", not "".  A leading pair of
        // separators is never stripped, to support alternate roots.  This is used to
        // support UNC paths on Windows.
        void StripTrailingSeparatorsInternal();

        StringType path_;
};

#endif  // BASE_FILE_PATH_H_
