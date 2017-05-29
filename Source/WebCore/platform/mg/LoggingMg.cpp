/* 
** $Id: LoggingMg.cpp 44 2010-06-03 08:33:12Z xwyan $
** 
** LoggingMg.cpp: The platform implementation for Logging.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: XiaoweiYan
**
** Create date: 2010-06-01
*/

#include "config.h"
#include "Logging.h"
#include "PlatformString.h"

#include <stdio.h>
#include <string.h>

namespace WebCore {

void InitializeLoggingChannelsIfNecessary()
{
    static bool didInitializeLoggingChannels = false;
    if (didInitializeLoggingChannels)
        return;

    didInitializeLoggingChannels = true;

    char* logEnv = getenv("MD_WEBKIT_LOG");
    if (!logEnv)
        return;

    // we set up the logs anyway because some of our logging, such as
    // soup's is available in release builds
#if defined(NDEBUG)
    fprintf(stderr, "MD_WEBKIT_LOG is not empty, "
        "but this is a release build. Notice that "
        "many log messages will only appear in a debug build.\n");
#endif

    char *env = strtok(logEnv, " ");
    fprintf(stderr, "\nNote: MD_WEBKIT_LOG valid value is:{");

    while (env != NULL)
    {
        if (WTFLogChannel* channel = getChannelFromName(env)) 
        {
            channel->state = WTFLogChannelOn;
            fprintf(stderr, "%s ", env);
        }
        env = strtok(NULL, " ");
    }

    fprintf(stderr, "NotImplemented} \n");
    // to disable logging notImplemented set the DISABLE_NI_WARNING
    // environment variable to 1
    LogNotYetImplemented.state = WTFLogChannelOn;
}

} // namespace WebCore
