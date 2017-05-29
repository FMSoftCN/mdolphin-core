/*
 ** $Id: ScrollbarThemeMg.cpp 487 2011-02-19 01:07:59Z xwyan $
 **
 ** ScrollbarThemeMg.cpp: The platform implementation for ScrollbarTheme.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-07 
 */

#include "minigui.h"

#include "config.h"
#include "mdolphin.h"
#include "ScrollbarThemeMg.h"
#include "AffineTransform.h"
#include "GraphicsContext.h"
#include "PlatformMouseEvent.h"
#include "Scrollbar.h"

// Generic state constants
#define TS_NORMAL    1
#define TS_HOVER     2
#define TS_ACTIVE    3
#define TS_DISABLED  4

#define SP_BUTTON          1
#define SP_THUMBHOR        2
#define SP_THUMBVERT       3
#define SP_TRACKSTARTHOR   4
#define SP_TRACKENDHOR     5
#define SP_TRACKSTARTVERT  6
#define SP_TRACKENDVERT    7
#define SP_GRIPPERHOR      8
#define SP_GRIPPERVERT     9

#define TS_UP_BUTTON       0
#define TS_DOWN_BUTTON     4
#define TS_LEFT_BUTTON     8
#define TS_RIGHT_BUTTON    12
#define TS_UP_BUTTON_HOVER   17
#define TS_DOWN_BUTTON_HOVER  18
#define TS_LEFT_BUTTON_HOVER  19
#define TS_RIGHT_BUTTON_HOVER   20

namespace WebCore {

enum SCROLLPART{
    ScrollLeftArrowPart,
    ScrollRightArrowPart,
    ScrollUpArrowPart,
    ScrollDownArrowPart,
    HScrollTrackPart,
    VScrollTrackPart,
    HScrollThumbPart,
    VScrollThumbPart
};

enum PARTSTATE {
    LEAVE,
    PRESS,
    HOVER
};

MDScrobarWidth  ScrollbarThemeMg::scrollbarCustomWidthValue = {0, 0, 0};

static void paintScrollThumb (SCROLLPART part, HDC hdc, const IntRect &rect, bool enable, PARTSTATE state)
{
    int style = 0;
    RECT rc = rect;

    if (HOVER == state)
         style = LFRDR_BTN_STATUS_HILITE;
    else
         style = LFRDR_BTN_STATUS_NORMAL;

    switch (part)
    {
        case HScrollThumbPart:
            {
              GetDefaultWindowElementRenderer()->draw_3dbox(hdc, &rc,
                        GetWindowElementAttr(HWND_DESKTOP, WE_MAINC_THREED_BODY), 
                        style | LFRDR_3DBOX_THICKFRAME | LFRDR_3DBOX_FILLED); 
            }
            break;
        case VScrollThumbPart:
            {
              GetDefaultWindowElementRenderer()->draw_3dbox(hdc, &rc,
                        GetWindowElementAttr(HWND_DESKTOP, WE_MAINC_THREED_BODY), 
                        style | LFRDR_3DBOX_THICKFRAME | LFRDR_3DBOX_FILLED); 
            }
            break;
        default:
            /* make complier happy */
            break;
    }
}

static void paintScrollTrack (SCROLLPART part, HDC hdc, const IntRect &rect, bool enable, PARTSTATE state)
{
    switch (part)
    {
        case HScrollTrackPart:
            {
                SetBrushColor (hdc, DWORD2PIXEL(hdc, GetWindowElementAttr(HWND_DESKTOP, WE_MAINC_THREED_BODY)));
                FillBox (hdc, rect.x(), rect.y(), rect.width(), rect.height());
            }
            break;
        case VScrollTrackPart:
            {
                SetBrushColor (hdc, DWORD2PIXEL(hdc, GetWindowElementAttr(HWND_DESKTOP, WE_MAINC_THREED_BODY)));
                FillBox (hdc, rect.x(), rect.y(), rect.width(), rect.height());
            }
            break;
        default:
            /* make complier happy */
            break;
    }
}

static void paintScrollArrows (SCROLLPART part, HDC hdc, const IntRect &rect, bool enable, PARTSTATE state)
{
    int style = 0;
    RECT rc = rect;

    if (PRESS == state) 
         style = LFRDR_BTN_STATUS_PRESSED;
    else if (HOVER == state)
         style = LFRDR_BTN_STATUS_HILITE;
    else
         style = LFRDR_BTN_STATUS_NORMAL;
    
    switch (part)
    {
        case ScrollLeftArrowPart:
            {
                if (enable) {
                    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
                                    &rc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_THREED_BODY), 
                                    style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_LEFT);
                } else {
                    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
                                    &rc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_DISABLED_ITEM),
                                    style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_LEFT);
                }
            }
            break;
        case ScrollRightArrowPart:
            {
                if (enable) {
                    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
                                    &rc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_THREED_BODY), 
                                    style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_RIGHT);
                } else {
                    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
                                    &rc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_DISABLED_ITEM), 
                                    style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_RIGHT);
                }
            }
            break;
        case ScrollUpArrowPart:
            {
                if (enable) {
                    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
                                    &rc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_THREED_BODY), 
                                    style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_UP);
                } else {
                    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
                                    &rc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_DISABLED_ITEM), 
                                    style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_UP);
                }
            }
            break;
        case ScrollDownArrowPart:
            {
                if (enable) {
                    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
                                    &rc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_THREED_BODY), 
                                    style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_DOWN);
                } else {
                    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
                                    &rc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_DISABLED_ITEM), 
                                    style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_DOWN);
                }
            }
            break;
        default:
            /* make complier happy */
            break;
    }

}

static void paintScrollPart (SCROLLPART part, HDC *pdc, const IntRect &rect, bool enable, PARTSTATE state)
{
    switch (part)
    {
    case ScrollLeftArrowPart:
    case ScrollRightArrowPart:
    case ScrollUpArrowPart:
    case ScrollDownArrowPart:
        paintScrollArrows (part, *pdc, rect, enable, state);
        break;
    case HScrollTrackPart:
    case VScrollTrackPart:
        paintScrollTrack (part, *pdc, rect, enable, state);
        break;
    case HScrollThumbPart:
    case VScrollThumbPart:
        paintScrollThumb (part, *pdc, rect, enable, state);
        break;
    }
}

ScrollbarTheme* ScrollbarTheme::nativeTheme()
{
    static ScrollbarThemeMg theme;
    return &theme;
}

ScrollbarThemeMg::~ScrollbarThemeMg()
{
}

int ScrollbarThemeMg::scrollbarThickness(ScrollbarControlSize controlSize)
{
    //FIXME: using static var to speed 
    static int regularSize = -1;
    static int smallSize = -1;

    int sbWidth = 0;
    sbWidth =  (controlSize == RegularScrollbar) ? regularSize : smallSize;

    if (sbWidth < 0) {
        MDScrobarWidth *customScrollbarWidth = ScrollbarThemeMg::scrollbarCustomWidth();
        if (controlSize == RegularScrollbar)
            sbWidth = customScrollbarWidth->wRegular;
        else if(controlSize == SmallScrollbar)
            sbWidth = customScrollbarWidth->wSmall;

        if (sbWidth == 0) 
            sbWidth = GetWindowElementAttr(HWND_DESKTOP, WE_METRICS_SCROLLBAR);
        else if (sbWidth < 0 )
            sbWidth = 0;

        if (controlSize == RegularScrollbar)
            regularSize = sbWidth;
        else if(controlSize == SmallScrollbar)
            smallSize = sbWidth;
    }

    return sbWidth;
}

bool ScrollbarThemeMg::hasThumb(Scrollbar* scrollbar)
{
    return thumbLength(scrollbar) > 0;
}

IntRect ScrollbarThemeMg::backButtonRect(Scrollbar* scrollbar, ScrollbarPart part, bool)
{
    // Windows just has single arrows.
    if (part == BackButtonEndPart)
        return IntRect();

    // Our desired rect is essentially 17x17.
    
    // Our actual rect will shrink to half the available space when
    // we have < 34 pixels left.  This allows the scrollbar
    // to scale down and function even at tiny sizes.
    int thickness = scrollbarThickness(scrollbar->controlSize());
    if (scrollbar->orientation() == HorizontalScrollbar)
        return IntRect(scrollbar->x(), scrollbar->y(),
                       scrollbar->width() < 2 * thickness ? scrollbar->width() / 2 : thickness, thickness);
    return IntRect(scrollbar->x(), scrollbar->y(),
                   thickness, scrollbar->height() < 2 * thickness ? scrollbar->height() / 2 : thickness);
}

IntRect ScrollbarThemeMg::forwardButtonRect(Scrollbar* scrollbar, ScrollbarPart part, bool)
{
    // Windows just has single arrows.
    if (part == ForwardButtonStartPart)
        return IntRect();
    
    // Our desired rect is essentially 17x17.
    
    // Our actual rect will shrink to half the available space when
    // we have < 34 pixels left.  This allows the scrollbar
    // to scale down and function even at tiny sizes.
    int thickness = scrollbarThickness(scrollbar->controlSize());
    if (scrollbar->orientation() == HorizontalScrollbar) {
        int w = scrollbar->width() < 2 * thickness ? scrollbar->width() / 2 : thickness;
        return IntRect(scrollbar->x() + scrollbar->width() - w, scrollbar->y(), w, thickness);
    }
    
    int h = scrollbar->height() < 2 * thickness ? scrollbar->height() / 2 : thickness;
    return IntRect(scrollbar->x(), scrollbar->y() + scrollbar->height() - h, thickness, h);
}

IntRect ScrollbarThemeMg::trackRect(Scrollbar* scrollbar, bool)
{
    int thickness = scrollbarThickness(scrollbar->controlSize());
    if (scrollbar->orientation() == HorizontalScrollbar) {
        if (scrollbar->width() < 2 * thickness)
            return IntRect();
        return IntRect(scrollbar->x() + thickness, scrollbar->y(), scrollbar->width() - 2 * thickness, thickness);
    }
    if (scrollbar->height() < 2 * thickness)
        return IntRect();
    return IntRect(scrollbar->x(), scrollbar->y() + thickness, thickness, scrollbar->height() - 2 * thickness);
}

bool ScrollbarThemeMg::shouldCenterOnThumb(Scrollbar*, const PlatformMouseEvent& evt)
{
    return evt.shiftKey() && evt.button() == LeftButton;
}

bool ScrollbarThemeMg::shouldSnapBackToDragOrigin(Scrollbar* scrollbar, const PlatformMouseEvent& evt)
{
#if 0
    // Find the rect within which we shouldn't snap, by expanding the track rect
    // in both dimensions.
    IntRect rect = trackRect(scrollbar);
    const bool horz = scrollbar->orientation() == HorizontalScrollbar;
    const int thickness = scrollbarThickness(scrollbar->controlSize());
    rect.inflateX((horz ? kOffEndMultiplier : kOffSideMultiplier) * thickness);
    rect.inflateY((horz ? kOffSideMultiplier : kOffEndMultiplier) * thickness);

    // Convert the event to local coordinates.
    IntPoint mousePosition = scrollbar->convertFromContainingWindow(evt.pos());
    mousePosition.move(scrollbar->x(), scrollbar->y());

    // We should snap iff the event is outside our calculated rect.
    return !rect.contains(mousePosition);
#else 
    return false;
#endif
}

void ScrollbarThemeMg::paintTrackBackground(GraphicsContext* context, Scrollbar* scrollbar, const IntRect& rect)
{
    // Just assume a forward track part.  We only paint the track as a single piece when there is no thumb.
    if (!hasThumb(scrollbar))
        paintTrackPiece(context, scrollbar, rect, ForwardTrackPart);
}


void ScrollbarThemeMg::paintTrackPiece(GraphicsContext* context, Scrollbar* scrollbar, const IntRect& rect, ScrollbarPart partType)
{
    //bool start = partType == BackTrackPart;
    SCROLLPART part;
    //int part;
    if (scrollbar->orientation() == HorizontalScrollbar)
        part = HScrollTrackPart;
    else
        part = VScrollTrackPart;

    bool enable = false;
    PARTSTATE state = LEAVE;
    if (scrollbar->enabled())
        enable = true;

    IntRect drawRect = context->getCTM().mapRect(rect);
    paintScrollPart (part, context->platformContext(), drawRect, enable, state);

}

void ScrollbarThemeMg::paintButton(GraphicsContext* context, Scrollbar* scrollbar, const IntRect& rect, ScrollbarPart spart)
{
    //checkAndInitScrollbarTheme();

    bool start = (spart == BackButtonStartPart);
    //int xpState = 0;
    SCROLLPART part;
    //SCROLLPART part;
    //int classicState = 0;
    PARTSTATE state = LEAVE;
    bool enable = false;

    if (scrollbar->orientation() == HorizontalScrollbar)
        part = start ? ScrollLeftArrowPart : ScrollRightArrowPart;
    else
        part = start ? ScrollUpArrowPart : ScrollDownArrowPart;
        //xpState = start ? TS_UP_BUTTON : TS_DOWN_BUTTON;

    if (scrollbar->enabled()) {
        enable = true;
        //xpState += TS_DISABLED;
    } 

    IntRect drawRect = context->getCTM().mapRect(rect);

    if ((scrollbar->hoveredPart() == BackButtonStartPart && start) ||
               (scrollbar->hoveredPart() == ForwardButtonEndPart && !start)) {
        state = HOVER;
    } 
    else if ((scrollbar->pressedPart() == BackButtonStartPart && start) ||
               (scrollbar->pressedPart() == ForwardButtonEndPart && !start)) {
        state = PRESS;
    } 
    
#if 0
    else if ((scrollbar->hoveredPart() == BackButtonStartPart && start) ||
               (scrollbar->hoveredPart() == ForwardButtonEndPart && !start)) {
        if (scrollbar->pressedPart() == scrollbar->hoveredPart()) {
            xpState += TS_ACTIVE;
        } else
            xpState += TS_HOVER;
    } else {
        if (scrollbar->hoveredPart() == NoPart || !runningVista)
            xpState += TS_NORMAL;
        else {
            if (scrollbar->orientation() == HorizontalScrollbar)
                xpState = start ? TS_LEFT_BUTTON_HOVER : TS_RIGHT_BUTTON_HOVER;
            else
                xpState = start ? TS_UP_BUTTON_HOVER : TS_DOWN_BUTTON_HOVER;
        }
    }
#endif

#if 0
    bool alphaBlend = false;
    if (scrollbarTheme)
        alphaBlend = IsThemeBackgroundPartiallyTransparent(scrollbarTheme, SP_BUTTON, xpState);
    HDC hdc = context->getWindowsContext(rect, alphaBlend);

    RECT themeRect(rect);
    if (scrollbarTheme)
        DrawThemeBackground(scrollbarTheme, hdc, SP_BUTTON, xpState, &themeRect, 0);
    else
        ::DrawFrameControl(hdc, &themeRect, DFC_SCROLL, classicState);
    context->releaseWindowsContext(hdc, rect, alphaBlend);
#endif

    paintScrollPart (part, context->platformContext(), drawRect, enable, state); 
}

void ScrollbarThemeMg::paintThumb(GraphicsContext* context, Scrollbar* scrollbar, const IntRect& rect)
{
#if 0
    if (!damageRect.intersects(rect))
        return;
#endif

    SCROLLPART part;
    if (scrollbar->orientation() == HorizontalScrollbar)
        part = HScrollThumbPart;
    else
        part = VScrollThumbPart;

    IntRect drawRect = context->getCTM().mapRect(rect);
    bool enable = false;
    PARTSTATE state = LEAVE;
    if (scrollbar->enabled())
        enable = true;

    if (scrollbar->hoveredPart() == ThumbPart)
        state = HOVER;

    paintScrollPart (part, context->platformContext(), drawRect, enable, state);
}

void ScrollbarThemeMg::setScrollbarCustomWidth(const MDScrobarWidth *scrollbarWidth)
{
   ScrollbarThemeMg::scrollbarCustomWidthValue = *scrollbarWidth;
}


MDScrobarWidth* ScrollbarThemeMg::scrollbarCustomWidth() 
{
    return &(ScrollbarThemeMg::scrollbarCustomWidthValue);
}

}

