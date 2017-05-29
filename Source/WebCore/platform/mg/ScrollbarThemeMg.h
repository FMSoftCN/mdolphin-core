/*
 ** $Id: ScrollbarThemeMg.h 251 2010-08-04 03:31:05Z lvlei $
 **
 ** ScrollbarThemeMg.h: The platform definition for ScrollbarTheme.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-07 
 */

#ifndef ScrollbarThemeMg_h
#define ScrollbarThemeMg_h

#include "ScrollbarThemeComposite.h"


namespace WebCore {

class ScrollbarThemeMg : public ScrollbarThemeComposite {
public:
    virtual ~ScrollbarThemeMg();

    virtual int scrollbarThickness(ScrollbarControlSize = RegularScrollbar);
    virtual bool hasButtons(Scrollbar*) { return true; }
    virtual bool hasThumb(Scrollbar*);

    virtual IntRect backButtonRect(Scrollbar*, ScrollbarPart, bool painting = false);
    virtual IntRect forwardButtonRect(Scrollbar*, ScrollbarPart, bool painting = false);
    virtual IntRect trackRect(Scrollbar*, bool painting = false);

    virtual bool shouldCenterOnThumb(Scrollbar*, const PlatformMouseEvent&);
    virtual bool shouldSnapBackToDragOrigin(Scrollbar*, const PlatformMouseEvent&);

    virtual void paintTrackBackground(GraphicsContext*, Scrollbar*, const IntRect&);
    virtual void paintTrackPiece(GraphicsContext*, Scrollbar*, const IntRect&, ScrollbarPart);
    virtual void paintButton(GraphicsContext*, Scrollbar*, const IntRect&, ScrollbarPart);
    virtual void paintThumb(GraphicsContext*, Scrollbar*, const IntRect&);

    //FIXME: add by feynman
    static void setScrollbarCustomWidth(const MDScrobarWidth *);
    static MDScrobarWidth* scrollbarCustomWidth();
    static MDScrobarWidth scrollbarCustomWidthValue;

};

}
#endif
