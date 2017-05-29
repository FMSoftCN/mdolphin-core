/*
 ** $Id: SearchPopupMenuMg.cpp 619 2011-05-03 09:01:47Z lijiangwei $
 **
 ** SearchPopupMenuMg.cpp: The platform implementation for SearchPopupMenu.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-07 
 */

#include "config.h"
#include "SearchPopupMenuMg.h"

#include "NotImplemented.h"

namespace WebCore {

SearchPopupMenuMg::SearchPopupMenuMg(PopupMenuClient* client)
    : m_popup(adoptRef(new PopupMenuMg(client)))
{
    notImplemented();
}

PopupMenu* SearchPopupMenuMg::popupMenu()
{
    return m_popup.get();
}

void SearchPopupMenuMg::saveRecentSearches(const AtomicString&, const Vector<String>&)
{
    notImplemented();
}

void SearchPopupMenuMg::loadRecentSearches(const AtomicString&, Vector<String>&)
{
    notImplemented();
}

bool SearchPopupMenuMg::enabled()
{
    notImplemented();
    return true;
}

}
