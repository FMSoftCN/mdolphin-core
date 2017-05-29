/*
** $Id$
**
** IntRectMg.cpp: rect implements file.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: lvlei
** 
** Create date: 2010-06-01
*/

#include "minigui.h"

#include "config.h"
#include "IntRect.h"

namespace WebCore {

IntRect::IntRect(const RECT& r)
    : m_location(IntPoint(r.left, r.top)), m_size(IntSize(r.right-r.left, r.bottom-r.top))
{
}

IntRect::operator RECT() const
{
    RECT rect = { x(), y(), x() + width(), y() + height() };
    return rect;
}

}
