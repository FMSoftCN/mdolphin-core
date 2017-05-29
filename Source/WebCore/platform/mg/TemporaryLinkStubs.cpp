/*
 ** $Id: TemporaryLinkStubs.cpp 53 2010-06-04 08:11:26Z xwyan $
 **
 ** TemporaryLinkStubs.cpp: The platform implementation for TemporaryLinkStubs.
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

#include "AXObjectCache.h"
#include "Editor.h"
#include "FTPDirectoryDocument.h"
#include "FrameView.h"
#include "KURL.h"
#include "NotImplemented.h"
#include "PluginView.h"
#include "ScrollbarTheme.h"
#include "SharedBuffer.h"

#include <float.h>

using namespace WebCore;

namespace WebCore {

void getSupportedKeySizes(Vector<String>&)
{
    notImplemented();
}

String signedPublicKeyAndChallengeString(unsigned keySizeIndex, const String &challengeString, const KURL &url)
{
    return String();
}

float userIdleTime()
{
    notImplemented();
    return FLT_MAX;
}

}

