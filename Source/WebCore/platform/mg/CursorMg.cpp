/*
** $Id$
**
** RenderThemeMg.cpp: theme implements file.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: lvlei 
**
** Create date: 2010-06-02
*/

#include "minigui.h"

#include "config.h"
#include "Cursor.h"

#include "NotImplemented.h"


namespace WebCore {

Cursor::Cursor(const Cursor& other)
    : m_platformCursor(other.m_platformCursor)
{
}

Cursor::~Cursor()
{
}

Cursor::Cursor(Image* img, const IntPoint& hotspot)
    : m_platformCursor(0)
{
}

Cursor& Cursor::operator=(const Cursor& other)
{
    m_platformCursor = other.m_platformCursor;
    return *this;
}

Cursor::Cursor(HCURSOR c)
    : m_platformCursor(c)
{
}

const Cursor& pointerCursor()
{
    static Cursor c = GetSystemCursor(IDC_ARROW);
    return c;
}

const Cursor& crossCursor()
{
    static Cursor c = GetSystemCursor(IDC_CROSS);
    return c;
}

const Cursor& handCursor()
{
    static Cursor c = GetSystemCursor(IDC_HAND_POINT);
    return c;
}

const Cursor& iBeamCursor()
{
    static Cursor c = GetSystemCursor(IDC_IBEAM);
    return c;
}

const Cursor& waitCursor()
{
    static Cursor c = GetSystemCursor(IDC_WAIT);
    return c;
}

const Cursor& helpCursor()
{
    static Cursor c = GetSystemCursor(IDC_HELP);
    return c;
}

const Cursor& eastResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZEWE);
    return c;
}

const Cursor& northResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENS);
    return c;
}

const Cursor& northEastResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENESW);
    return c;
}

const Cursor& northWestResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENWSE);
    return c;
}

const Cursor& southResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENS);
    return c;
}

const Cursor& southEastResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENWSE);
    return c;
}

const Cursor& southWestResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENESW);
    return c;
}

const Cursor& westResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZEWE);
    return c;
}

const Cursor& northSouthResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENS);
    return c;
}

const Cursor& eastWestResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZEWE);
    return c;
}

const Cursor& northEastSouthWestResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENESW);
    return c;
}

const Cursor& northWestSouthEastResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_SIZENWSE);
    return c;
}

const Cursor& columnResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_COLOMN);
    return c;
}

const Cursor& rowResizeCursor()
{
    static Cursor c = GetSystemCursor(IDC_ROW);
    return c;
}

const Cursor& moveCursor()
{
    static Cursor c = GetSystemCursor(IDC_MOVE);
    return c;
}
// not implements 
static Cursor localCursor;
const Cursor& cellCursor() 
{
    notImplemented();
    return localCursor;
}
const Cursor& contextMenuCursor()
{
    notImplemented();
    return localCursor;
}
const Cursor& verticalTextCursor()
{
    notImplemented();
    return localCursor;
}
const Cursor& noDropCursor()
{
    notImplemented();
    return localCursor;
}
const Cursor& notAllowedCursor()
{
    notImplemented();
    return localCursor;
}
const Cursor& progressCursor()
{
    notImplemented();
    return localCursor;
}
const Cursor& aliasCursor()
{
    notImplemented();
    return localCursor;
}
const Cursor& zoomInCursor()
{
    notImplemented();
    return localCursor;
}
const Cursor& zoomOutCursor()
{
    notImplemented();
    return localCursor;
}
const Cursor& copyCursor()
{
    notImplemented();
    return localCursor;
}

const Cursor& middlePanningCursor() { return crossCursor(); }
const Cursor& eastPanningCursor() { return crossCursor(); }
const Cursor& northPanningCursor() { return crossCursor(); }
const Cursor& northEastPanningCursor() { return crossCursor(); }
const Cursor& northWestPanningCursor() { return crossCursor(); }
const Cursor& southPanningCursor() { return crossCursor(); }
const Cursor& southEastPanningCursor() { return crossCursor(); }
const Cursor& southWestPanningCursor() { return crossCursor(); }
const Cursor& westPanningCursor() { return crossCursor(); }
const Cursor& noneCursor()
{
    notImplemented();
    return localCursor;
}

const Cursor& grabCursor()
{
    notImplemented();
    return localCursor;
}

const Cursor& grabbingCursor()
{
    notImplemented();
    return localCursor;
}

}
