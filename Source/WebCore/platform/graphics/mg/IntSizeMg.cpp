/*
** $Id$
**
** IntSizeMg.cpp: size implements file.
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
#include "IntSize.h"

namespace WebCore {

IntSize::IntSize(const SIZE& s)
    : m_width(s.cx)
    , m_height(s.cy)
{
}

IntSize::operator SIZE() const
{
    SIZE s = {m_width, m_height};
    return s;
}

}
