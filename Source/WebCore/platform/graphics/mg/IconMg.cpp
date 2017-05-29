/*
 ** $Id: IconMg.cpp 53 2010-06-04 08:11:26Z xwyan $
 **
 ** IconMg.cpp: The platform implementation for Icon.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-04 
 */

#include "minigui.h"

#include "config.h"
#include "Icon.h"

#include "GraphicsContext.h"
#include "PlatformString.h"

namespace WebCore {

Icon::Icon(HICON icon)
    : m_hIcon(icon)
{
    ASSERT(icon);
}

Icon::~Icon()
{
    DestroyIcon(m_hIcon);
}

void Icon::paint(GraphicsContext* context, const IntRect& r)
{
    if (context->paintingDisabled())
        return;

    //HDC hdc = context->getWindowsContext(r);
    HDC hdc = (HDC)(context->platformContext());

    DrawIcon(hdc, r.x(), r.y(), r.width(), r.height(), m_hIcon);

    //context->releaseWindowsContext(hdc, r);
}

}
