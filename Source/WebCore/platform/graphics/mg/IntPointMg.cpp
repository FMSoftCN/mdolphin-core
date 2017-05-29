/*
** $Id$
**
** IntPointMg.cpp: point implements file.
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
#include "IntPoint.h"

namespace WebCore {

IntPoint::IntPoint(const POINT& p)
    : m_x(p.x)
    , m_y(p.y)
{
}

IntPoint::operator POINT() const
{
    POINT p = {m_x, m_y};
    return p;
}


}
