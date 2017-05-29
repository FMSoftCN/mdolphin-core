/*
** $Id: FileSystemMg.h 569 2007-12-24 08:33:58Z whzeng$
**
** FileSystemMg.h: operate file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-12-24
*/
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <dirent.h>
#include <sys/mman.h>

#include "config.h"

#if ENABLE(FILESYSTEM)

#include "FileSystem.h"

namespace WebCore {

#define readdir_rFile readdir_r
#define readdir_File  readdir

typedef void * HFile;

HFile openFile(const char *name, const char* mode);
size_t writeFile(void *ptr, size_t offset, size_t size, HFile);
size_t readFile(void *ptr, size_t offset, size_t size, HFile);
int seekFile(HFile file, long offset, int where);
int closeFile(HFile file); 
int fflushFile(HFile file);
int filenoFile(HFile file);
long ftellFile(HFile file);
int ftruncateFile(int fd, size_t length);
int statFile(const char *name, struct stat* buf);
int unlinkFile(const char *path);
int rmdirFile(const char *path);
int renameFile(const char *old, const char *newn);
int closedirFile(DIR *dir);
void *mmapFile(void *start, size_t length, int prot, int flags,
                  HFile file, size_t offset);
int munmapFile(void *start, size_t length);
int mkdirFile(const char *path, mode_t mode);
char *realpathFile(const char *path, char *resolved_path);
int lstatFile(const char * name, struct stat * buf);
DIR* opendirFile(const char *name);
gid_t getegidFile(void);
int fnmatchFile(const char *pattern, const char *string, int flags);
#if 0
int readdir_rFile(DIR *dirp, struct dirent *entry,  
               struct dirent **result);
#endif
} /* namespace WebCore */

#endif /* ENABLE(FILESYSTEM) */
