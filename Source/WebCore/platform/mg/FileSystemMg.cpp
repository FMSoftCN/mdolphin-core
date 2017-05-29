/*
 ** $Id: FileSystemMg.cpp 717 2011-07-05 05:52:02Z liuchunrui $
 **
 ** FileSystemMg.cpp: The platform implementation for FileSystem.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-04 
 */

#include "config.h"

//#if ENABLE(FILESYSTEM)
#include "FileSystemMg.h"
#include "StringBuilder.h"
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <fnmatch.h>

#include <wtf/text/CString.h>
#include <libgen.h>

namespace WebCore {

static int makeDirectoryIfNotExsit(const char* path, long mode)
{
	struct stat st;
	if (stat (path, &st) < 0 && errno == ENOENT) {
		int status;
		char *buf, *parent;

		buf = strdup(path);
		parent = dirname(buf);

		makeDirectoryIfNotExsit(parent, mode);
		status = mkdir(parent,mode);
		free(buf);

		return status;
	}
    return 0;
}

HFile openFile(const char *name, const char *mode)
{
	//check whether the directory exists or not, if not exists, will create it.
   makeDirectoryIfNotExsit(name,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

   return (HFile)fopen(name, mode); 
}

int seekFile(HFile file, long offset, int where)
{
    return fseek((FILE*)file, offset, where);
}

size_t writeFile(void *ptr, size_t size, size_t num, HFile file)
{
    return fwrite(ptr, size, num, (FILE*)file);
}

void *mmapFile(void *start, size_t length, int prot, int flags,
                  HFile file, size_t offset)
{
    int fd = filenoFile(file);
    return mmap(start, length, prot, flags, fd, offset);
}

int munmapFile(void *start, size_t length)
{
     return munmap(start, length);
}

size_t readFile(void *ptr, size_t size, size_t num, HFile file)
{
    return fread(ptr, size, num, (FILE*)file);
}

int closeFile(HFile file)
{
    return fclose((FILE*)file);
}

long ftellFile(HFile file)
{
    return ftell((FILE*)file);
}

int filenoFile(HFile file)
{
     return fileno((FILE*)file);
}

int fflushFile(HFile file)
{
    return fflush((FILE*)file);
}

int ftruncateFile(int fd, size_t length)
{
    return ftruncate(fd, length);
}

int statFile(const char *name, struct stat* buf)
{
   return stat(name, buf); 
}

int unlinkFile(const char *path)
{
    return unlink(path);
}	

int rmdirFile(const char *path)
{
    return rmdir(path);
}

int renameFile(const char *old, const char *newn)
{
    return rename(old, newn);
}

int closedirFile(DIR *dir)
{
      return closedir(dir);
}

int mkdirFile(const char *path, mode_t mode)
{
    return mkdir(path, mode);
}

char *realpathFile(const char *path, char *resolved_path)
{
    return realpath(path, resolved_path);
}

int lstatFile(const char * name, struct stat * buf)
{
   return lstat(name, buf); 
}
DIR *opendirFile(const char *name)
{
     return opendir(name);
}	
gid_t getegidFile(void)
{
    return getegid();
}

int fnmatchFile(const char *pattern, const char *string, int flags)
{
     return fnmatch(pattern, string, flags);
}

#if 0 //don't use readdir_r or readdir by this way, because entry is alloced statically on stack
int readdir_rFile(DIR *dirp, struct dirent *entry,  
               struct dirent **result)
{
	return readdir_r(dirp, entry, result);
}	
#endif

// Global static used to store the base to the plugin path.
// This is set in WebSettings.cpp
String sPluginPath;

CString fileSystemRepresentation(const String& path)
{
    return path.utf8();
}

String openTemporaryFile(const String& prefix, PlatformFileHandle& handle)
{
    char buffer[PATH_MAX];
    const char* tmpDir = getenv("TMPDIR");

    if (!tmpDir)
        tmpDir = "/tmp";

    if (snprintf(buffer, PATH_MAX, "%s/%sXXXXXX", tmpDir, prefix.utf8().data()) >= PATH_MAX) {
        handle = invalidPlatformFileHandle;
        return String();
    }

    handle = mkstemp(buffer);
    if (handle < 0) {
        handle = invalidPlatformFileHandle;
        return String();
    }

    return String(buffer);
}

bool unloadModule(PlatformModule module)
{
    return !dlclose(module);
}

String homeDirectoryPath() 
{
    const char *home = getenv("HOME");
    if (!home) {
        home = getenv("TMPDIR");
        if (!home)
            home = "/tmp";
    }
    return String::fromUTF8(home);
}

} // namespace WebCore

//#endif /* ENABLE(FILESYSTEM) */
