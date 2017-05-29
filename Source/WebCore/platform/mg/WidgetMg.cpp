/* 
** $Id$
** 
** WidgetMg.cpp: the native implemention of Widget
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
#include "Widget.h"

#include "Cursor.h"
#include "Document.h"
#include "Element.h"
#include "GraphicsContext.h"
#include "IntRect.h"
#include "FrameView.h"
#include "NotImplemented.h"

namespace WebCore {


Widget::Widget(PlatformWidget widget/*=0*/)
{
	init(widget);
}

Widget::~Widget() 
{
	ASSERT(!parent());
}

HCURSOR lastSetCursor = 0;

void Widget::setCursor(const Cursor& cursor)
{
    if (HCURSOR c = (HCURSOR)cursor.impl()) {
        //if (c != lastSetCursor) {
            SetCursor(c);
            lastSetCursor = c;
        //}
    }
}


void Widget::show()
{
    if (platformWidget())
        ShowWindow(platformWidget(), SW_SHOW);
}

void Widget::hide()
{
    if (platformWidget())
        ShowWindow(platformWidget(), SW_HIDE);
}

void Widget::setFocus(bool focused)
{
}

void Widget::paint(GraphicsContext*, const IntRect&)
{
}

void Widget::setIsSelected(bool) 
{
    notImplemented();
}

IntRect Widget::frameRect() const
{
	return m_frame;
}

void Widget::setFrameRect(const IntRect& rect)
{
	m_frame = rect;
}


} // namespace WebCore
