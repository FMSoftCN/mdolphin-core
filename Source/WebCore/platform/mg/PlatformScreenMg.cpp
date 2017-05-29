/* 
** $Id$
** 
** PlatformScreenMg.cpp: the native implemention of Screen operation
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: dongjunjie
**
** Create date: 2010-06-01
*/


#include "minigui.h"
#include "config.h"
#include "Screen.h"

#include "IntRect.h"
#include "FloatRect.h"
#include "Frame.h"
#include "FrameView.h"
#include "Page.h"

namespace WebCore {

FloatRect screenRect(Widget* widget)
{
    return FloatRect(g_rcScr.left, g_rcScr.top, RECTW(g_rcScr), RECTH(g_rcScr));
}

FloatRect screenAvailableRect(Widget* widget)
{
    return screenRect(widget);
}

int screenDepth(Widget*)
{
    return (int)GetGDCapability(HDC_SCREEN, GDCAP_DEPTH);
}

int screenDepthPerComponent(Widget*)
{
    return (int)GetGDCapability(HDC_SCREEN, GDCAP_BITSPP);
}

bool screenIsMonochrome(Widget*)
{ 
    return false;
}


} // namespace WebCore
