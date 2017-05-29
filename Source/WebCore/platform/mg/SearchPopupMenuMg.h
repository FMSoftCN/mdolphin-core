/*
 ** $Id: SearchPopupMenuMg.h 590 2011-04-22 05:29:20Z lijiangwei $
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

#ifndef SearchPopupMenuMg_h
#define SearchPopupMenuMg_h

#include "PopupMenuMg.h"
#include "SearchPopupMenu.h"

namespace WebCore {

class SearchPopupMenuMg : public SearchPopupMenu {
public:
    SearchPopupMenuMg(PopupMenuClient*);

    virtual PopupMenu* popupMenu();
    virtual void saveRecentSearches(const AtomicString& name, const Vector<String>& searchItems);
    virtual void loadRecentSearches(const AtomicString& name, Vector<String>& searchItems);
    virtual bool enabled();

private:
    RefPtr<PopupMenuMg> m_popup;
};

}
#endif
