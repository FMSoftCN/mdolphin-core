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
** Create date: 2010-06-01
*/

#include "minigui.h"

#include "config.h"
#include "RenderThemeMg.h"

#include "AffineTransform.h"
#include "GraphicsContext.h"
#include <wtf/MathExtras.h>
#include "RenderWidget.h"
#include "Element.h"
#include "Color.h"
#include "NotImplemented.h"
#include "PaintInfo.h"
#include "RenderMediaControls.h"
#include "UserAgentStyleSheets.h"
#include "RenderProgress.h"

// Generic state constants
#define TS_NORMAL    0x00
#define TS_HOVER     0x01
#define TS_ACTIVE    0x02
#define TS_DISABLED  0x04
#define TS_FOCUSED   0x08
#define TS_CHECKED   0x10

// Button constants
#define BP_BUTTON    1
#define BP_RADIO     2
#define BP_CHECKBOX  3

namespace WebCore {


PassRefPtr<RenderTheme> RenderThemeMg::create()
{
    return adoptRef(new RenderThemeMg());
}

PassRefPtr<RenderTheme> RenderTheme::themeForPage(Page* page)
{
    static RenderTheme* rt = RenderThemeMg::create().releaseRef();
    return rt;
}

RenderThemeMg::RenderThemeMg()
{
}

RenderThemeMg::~RenderThemeMg()
{
}

bool RenderThemeMg::supportsHover(const RenderStyle*) const
{
    return true;
}

int RenderThemeMg::baselinePosition(const RenderObject* o) const
{
    if (!o->isBox())
        return 0;

    if (o->style()->appearance() == CheckboxPart || o->style()->appearance() == RadioPart)
        return toRenderBox(o)->marginTop() + toRenderBox(o)->height() - 2; // Same as in old khtml
    return RenderTheme::baselinePosition(o);
}

bool RenderThemeMg::controlSupportsTints(const RenderObject* o) const
{
    if (!isEnabled(o))
        return false;

    // Checkboxes only have tint when checked.
    if (o->style()->appearance() == CheckboxPart)
        return isChecked(o);

    // For now assume other controls have tint if enabled.
    return true;
}

bool RenderThemeMg::supportsControlTints() const
{
    return true;
}

void RenderThemeMg::adjustRepaintRect(const RenderObject* o, IntRect& r)
{
    RenderTheme::adjustRepaintRect(o, r);
}


Color RenderThemeMg::platformActiveSelectionBackgroundColor() const
{
    DWORD color = GetWindowElementAttr(HWND_NULL, WE_BGC_SELECTED_ITEM);
    return Color(GetRValue(color), GetGValue(color), 
            GetBValue(color), GetAValue(color));
}

Color RenderThemeMg::platformInactiveSelectionBackgroundColor() const
{
    DWORD color = GetWindowElementAttr(HWND_NULL, WE_BGC_SELECTED_LOSTFOCUS);
    return Color(GetRValue(color), GetGValue(color), 
            GetBValue(color), GetAValue(color));
}

Color RenderThemeMg::platformActiveSelectionForegroundColor() const
{
    DWORD color = GetWindowElementAttr(HWND_NULL, WE_FGC_SELECTED_ITEM);
    return Color(GetRValue(color), GetGValue(color), 
            GetBValue(color), GetAValue(color));
}

Color RenderThemeMg::platformInactiveSelectionForegroundColor() const
{
    DWORD color = GetWindowElementAttr(HWND_NULL, WE_FGC_SELECTED_ITEM);
    return Color(GetRValue(color), GetGValue(color), 
            GetBValue(color), GetAValue(color));
}

int RenderThemeMg::minimumMenuListSize(RenderStyle* style) const
{
    return RenderTheme::minimumMenuListSize(style);
}

void RenderThemeMg::adjustSliderThumbSize(RenderObject* o) const
{
#if ENABLE(VIDEO)
	RenderMediaControls::adjustMediaSliderThumbSize(o);
#else
	UNUSED_PARAM(o);
#endif
}

void RenderThemeMg::adjustSliderThumbStyle(CSSStyleSelector* selecter, RenderStyle* style, Element* e) const
{
    RenderTheme::adjustSliderThumbStyle(selecter, style, e);
}

int RenderThemeMg::popupInternalPaddingLeft(RenderStyle*) const
{
    return 0;
}

int RenderThemeMg::popupInternalPaddingRight(RenderStyle*) const
{
    return 0;
}

int RenderThemeMg::popupInternalPaddingTop(RenderStyle*) const
{
    return 0;
}

int RenderThemeMg::popupInternalPaddingBottom(RenderStyle*) const
{
    return 0;
}

bool RenderThemeMg::paintCheckbox(RenderObject* o, const PaintInfo& i, const IntRect& rc)
{
    HDC hdc = *i.context->platformContext();
    
    RECT r = i.context->getCTM().mapRect(rc);

    unsigned btnstate = determineState(o);

    int status = LFRDR_BTN_STATUS_NORMAL;

    if (btnstate & TS_CHECKED) 
        status |= LFRDR_BTN_STATUS_SELECTED;
    if (btnstate & TS_DISABLED)
        status |= LFRDR_BTN_STATUS_DISABLED;
    if (btnstate & TS_HOVER)
        status |= LFRDR_BTN_STATUS_HILITE;
    if (btnstate & TS_ACTIVE)
        status |= LFRDR_BTN_STATUS_PRESSED;

    GetDefaultWindowElementRenderer()->draw_check_button(HWND_DESKTOP, hdc, &r, status);

    if (btnstate & TS_FOCUSED) { //focused
        Vector<IntRect> focusRingRects;
        focusRingRects.append(rc);
        i.context->drawFocusRing(focusRingRects, 0, 0, Color(Color::darkGray));
    }   

    return false;
}

void RenderThemeMg::setCheckboxSize(RenderStyle* style) const
{
    // If the width and height are both specified, then we have nothing to do.
    if (!style->width().isIntrinsicOrAuto() && !style->height().isAuto())
        return;

    if (style->width().isIntrinsicOrAuto())
        style->setWidth(Length(13, Fixed));

    if (style->height().isAuto())
        style->setHeight(Length(13, Fixed));
}

bool RenderThemeMg::paintRadio(RenderObject* o, const PaintInfo& i, const IntRect& rc)
{
    HDC hdc = *i.context->platformContext();

    RECT r = i.context->getCTM().mapRect(rc);

    unsigned btnstate = determineState(o);

    int status = LFRDR_BTN_STATUS_NORMAL;

    if (btnstate & TS_CHECKED) 
        status |= LFRDR_BTN_STATUS_SELECTED;
    if (btnstate & TS_DISABLED)
        status |= LFRDR_BTN_STATUS_DISABLED;
    if (btnstate & TS_HOVER)
        status |= LFRDR_BTN_STATUS_HILITE;
    if (btnstate & TS_ACTIVE)
        status |= LFRDR_BTN_STATUS_PRESSED;
    
    GetDefaultWindowElementRenderer()->draw_radio_button(HWND_DESKTOP, hdc, &r, status);

    if (btnstate & TS_FOCUSED) { //focused
        Vector<IntRect> focusRingRects;
        focusRingRects.append(rc);
        i.context->drawFocusRing(focusRingRects, 0, 0, Color(Color::darkGray));
    }

    return false;
}

void RenderThemeMg::setRadioSize(RenderStyle* style) const
{
    // If the width and height are both specified, then we have nothing to do.
    if (!style->width().isIntrinsicOrAuto() && !style->height().isAuto())
        return;

    if (style->width().isIntrinsicOrAuto())
        style->setWidth(Length(13, Fixed));

    if (style->height().isAuto())
        style->setHeight(Length(13, Fixed));
}

void RenderThemeMg::adjustButtonStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    RenderTheme::adjustButtonStyle(selector, style, e);
}

void RenderThemeMg::setButtonSize(RenderStyle* style) const
{
    RenderTheme::setButtonSize(style);
}

bool RenderThemeMg::paintButton(RenderObject* o, const PaintInfo& i, const IntRect& rc)
{
    unsigned btnstate = determineState(o);
    RECT r = i.context->getCTM().mapRect(rc);
    
    int status = 0;
    
    if (btnstate & TS_ACTIVE) //pressed
        status = BST_PUSHED;
    else if (btnstate & TS_DISABLED) //disabled
        status = BST_DISABLE;
    else if (btnstate & TS_HOVER) //hoverd
        status = BST_HILITE;
    else if (btnstate & TS_FOCUSED) //focused
        status = BST_FOCUS;
    else
        status = BST_NORMAL;

    GetDefaultWindowElementRenderer()->draw_push_button(HWND_DESKTOP, 
                    *(i.context->platformContext()), 
                    &r, 0xFFC0C0C0, 0xFFFFFFFF, status);

    if (btnstate & TS_FOCUSED) { //focused
        Vector<IntRect> focusRingRects;
        focusRingRects.append(rc);
        i.context->drawFocusRing(focusRingRects, 0, 0, Color(Color::darkGray));
    }

    return false; //I want draw it my self, thanks
}

void RenderThemeMg::adjustTextFieldStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    RenderTheme::adjustTextFieldStyle(selector, style, e);
}

bool RenderThemeMg::paintTextField(RenderObject* o, const PaintInfo& i, const IntRect& rc)
{
    HDC hdc = *i.context->platformContext();
    RECT r = i.context->getCTM().mapRect(rc);

    DWORD color = GetWindowElementAttr(HWND_DESKTOP, WE_MAINC_THREED_BODY); 
    GetDefaultWindowElementRenderer()->draw_3dbox(hdc, &r, color, 
            LFRDR_3DBOX_THICKFRAME | LFRDR_BTN_STATUS_PRESSED); 

    if (!isEnabled(o)) {
        gal_pixel oldBrushColor = SetBrushColor(hdc, RGBA2Pixel(hdc, GetRValue(color), 
                    GetGValue(color), GetBValue(color), GetAValue(color)));
        int offset = 2;
        FillBox(hdc, r.left + offset, r.top + offset, RECTW(r)- offset*2, RECTH(r)- offset*2);
        SetBrushColor(hdc, oldBrushColor);
    }
    
    return false;
}

void RenderThemeMg::adjustTextAreaStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    RenderTheme::adjustTextAreaStyle(selector, style, e);
}

bool RenderThemeMg::paintTextArea(RenderObject* o, const PaintInfo& i, const IntRect& rc)
{
    HDC hdc = *i.context->platformContext();
    RECT r = i.context->getCTM().mapRect(rc);
    GetDefaultWindowElementRenderer()->draw_3dbox(hdc, &r,
            GetWindowElementAttr(HWND_DESKTOP, WE_MAINC_THREED_BODY), 
            LFRDR_BTN_STATUS_PRESSED|LFRDR_3DBOX_THICKFRAME); 
    return false;
}

bool RenderThemeMg::paintMenuList(RenderObject* o, const PaintInfo& i, const IntRect& rc)
{
    int style = LFRDR_BTN_STATUS_NORMAL;
    IntRect r = i.context->getCTM().mapRect(rc);
    IntRect btn_rect = i.context->getCTM().mapRect(IntRect(0, 0, 16, 16));

    HDC hdc = *i.context->platformContext();
    unsigned btnstate = determineState(o);

    r.setWidth(r.width() - btn_rect.width());

    RECT mgrc = r;
    GetDefaultWindowElementRenderer()->draw_3dbox(hdc, &mgrc,
            GetWindowElementAttr(HWND_DESKTOP, WE_MAINC_THREED_BODY), 
            LFRDR_BTN_STATUS_PRESSED|LFRDR_3DBOX_THICKFRAME); 

    if (TS_ACTIVE == btnstate) 
         style |= LFRDR_BTN_STATUS_PRESSED;
    else if (TS_HOVER == btnstate)
         style |= LFRDR_BTN_STATUS_HILITE;

    btn_rect.setX(r.x()+r.width());
    btn_rect.setY(r.y());

    mgrc = btn_rect;
    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
            &mgrc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_THREED_BODY), 
            style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_DOWN);

    if (btnstate & TS_FOCUSED) { //focused
        Vector<IntRect> focusRingRects;
        focusRingRects.append(rc);
        i.context->drawFocusRing(focusRingRects, 0, 0, Color(Color::darkGray));
    }

    return false;
}

void RenderThemeMg::adjustMenuListStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    style->setMinHeight(Length(16, Fixed));
    style->setPaddingRight(Length(16, Fixed));
}

bool RenderThemeMg::paintMenuListButton(RenderObject* o, const PaintInfo& i, const IntRect& rc)
{
    IntRect r = i.context->getCTM().mapRect(rc);
    HDC hdc = *i.context->platformContext();
    IntRect btn_rect = i.context->getCTM().mapRect(IntRect(0, 0, 16, 16));
    btn_rect.setX(r.x()+r.width()-btn_rect.width());
    btn_rect.setY(r.y());

    unsigned btnstate = determineState(o);
    int style = LFRDR_BTN_STATUS_NORMAL;
    if (TS_ACTIVE == btnstate) 
         style |= LFRDR_BTN_STATUS_PRESSED;
    else if (TS_HOVER == btnstate)
         style |= LFRDR_BTN_STATUS_HILITE;

    RECT mgrc = btn_rect;
    GetDefaultWindowElementRenderer()->draw_arrow(HWND_DESKTOP, hdc, 
            &mgrc, GetWindowElementAttr(HWND_DESKTOP, WE_FGC_THREED_BODY), 
            style | LFRDR_ARROW_HAVESHELL | LFRDR_ARROW_DOWN);
    return false;
}

void RenderThemeMg::adjustMenuListButtonStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    style->setMinHeight(Length(16, Fixed));
    style->setPaddingRight(Length(16, Fixed));
}

bool RenderThemeMg::paintSliderTrack(RenderObject* o, const PaintInfo& pi, const IntRect& r)
{
    notImplemented();
    return RenderTheme::paintSliderTrack(o, pi, r);
}

bool RenderThemeMg::paintSliderThumb(RenderObject* o, const PaintInfo& pi, const IntRect& r)
{
    notImplemented();
    return RenderTheme::paintSliderThumb(o, pi, r);
}

bool RenderThemeMg::paintSearchField(RenderObject* o, const PaintInfo& pi, const IntRect& r)
{
    notImplemented();
    return RenderTheme::paintSearchField(o, pi, r);
}

void RenderThemeMg::adjustSearchFieldStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    notImplemented();
    RenderTheme::adjustSearchFieldStyle(selector, style, e);
}

void RenderThemeMg::adjustSearchFieldCancelButtonStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    notImplemented();
    RenderTheme::adjustSearchFieldCancelButtonStyle(selector, style, e);
}

bool RenderThemeMg::paintSearchFieldCancelButton(RenderObject* o, const PaintInfo& pi, const IntRect& r)
{
    notImplemented();
    return RenderTheme::paintSearchFieldCancelButton(o, pi, r);
}

void RenderThemeMg::adjustSearchFieldDecorationStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    notImplemented();
    RenderTheme::adjustSearchFieldDecorationStyle(selector, style, e);
}

bool RenderThemeMg::paintSearchFieldDecoration(RenderObject* o, const PaintInfo& pi, const IntRect& r)
{
    notImplemented();
    return RenderTheme::paintSearchFieldDecoration(o, pi, r);
}

void RenderThemeMg::adjustSearchFieldResultsDecorationStyle(CSSStyleSelector* selector, RenderStyle* style, Element* e) const
{
    notImplemented();
    RenderTheme::adjustSearchFieldResultsDecorationStyle(selector, style, e);
}

bool RenderThemeMg::paintSearchFieldResultsDecoration(RenderObject* o, const PaintInfo& pi, const IntRect& r)
{
    notImplemented();
    return RenderTheme::paintSearchFieldResultsDecoration(o, pi, r);
}


void RenderThemeMg::systemFont(int propId, FontDescription& fontDescription) const
{
    notImplemented();
}

unsigned RenderThemeMg::determineState(RenderObject* o)
{
    unsigned result = TS_NORMAL;
    if (!isEnabled(o))
        result |= TS_DISABLED;
    else if (isPressed(o))
        result |= TS_ACTIVE;
    else if (isHovered(o))
        result |= TS_HOVER;
    
    if (supportsFocusRing(o->style()) && isFocused(o))
        result |= TS_FOCUSED;

    if (isChecked(o))
        result |= TS_CHECKED; 

    return result;
}

#if ENABLE(VIDEO)

String RenderThemeMg::extraMediaControlsStyleSheet()
{
	return String(mediaControlsGtkUserAgentStyleSheet, sizeof(mediaControlsGtkUserAgentStyleSheet));
}

bool RenderThemeMg::paintMediaFullscreenButton(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaFullscreenButton, o, paintInfo, r);
}

bool RenderThemeMg::paintMediaMuteButton(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaMuteButton, o, paintInfo, r);
}

bool RenderThemeMg::paintMediaPlayButton(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaPlayButton, o, paintInfo, r);
}

bool RenderThemeMg::paintMediaSeekBackButton(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaSeekBackButton, o, paintInfo, r);
}

bool RenderThemeMg::paintMediaSeekForwardButton(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaSeekForwardButton, o, paintInfo, r);
}

bool RenderThemeMg::paintMediaSliderTrack(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaSlider, o, paintInfo, r);
}

bool RenderThemeMg::paintMediaSliderThumb(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaSliderThumb, o, paintInfo, r);
}

bool RenderThemeMg::paintMediaVolumeSliderTrack(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaVolumeSlider, o, paintInfo, r);
}

bool RenderThemeMg::paintMediaVolumeSliderThumb(RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    return RenderMediaControls::paintMediaControlsPart(MediaVolumeSliderThumb, o, paintInfo, r);
}
#endif

#if ENABLE(PROGRESS_TAG)
static const int progressActivityBlocks = 5;
static const int progressAnimationFrames = 10;
static const double progressAnimationInterval = 0.125;
double RenderThemeMg::animationRepeatIntervalForProgressBar(RenderProgress*) const
{
    return progressAnimationInterval;
}

double RenderThemeMg::animationDurationForProgressBar(RenderProgress*) const
{
    return progressAnimationInterval * progressAnimationFrames * 2; // "2" for back and forth;
}

void RenderThemeMg::adjustProgressBarStyle(CSSStyleSelector*, RenderStyle* style, Element*) const
{
    style->setBoxShadow(0);
}

IntRect RenderThemeMg::calculateProgressRect(RenderObject* renderObject, const IntRect& fullBarRect)
{
    static double pos = 0.1;
    RenderProgress* renderProgress = toRenderProgress(renderObject);

    IntRect progressRect(fullBarRect);
    if (renderProgress->isDeterminate()) {
        int progressWidth = progressRect.width() * renderProgress->position();
        if (renderObject->style()->direction() == RTL)
            progressRect.setX(progressRect.x() + progressRect.width() - progressWidth);
        progressRect.setWidth(progressWidth);
        return progressRect;
    }

    double animationProgress = renderProgress->animationProgress();

    // Never let the progress rect shrink smaller than 2 pixels.
    int newWidth = max(2, progressRect.width() / progressActivityBlocks);
    int movableWidth = progressRect.width() - newWidth;
    progressRect.setWidth(newWidth);

    // We want the first 0.5 units of the animation progress to represent the
    // forward motion and the second 0.5 units to represent the backward motion,
    // thus we multiply by two here to get the full sweep of the progress bar with
    // each direction.
    if (animationProgress < 0.5)
        progressRect.setX(progressRect.x() + (animationProgress * 2 * movableWidth));
    else
        progressRect.setX(progressRect.x() + ((1.0 - animationProgress) * 2 * movableWidth));
    return progressRect;
}

bool RenderThemeMg::paintProgressBar(RenderObject* renderObject, const PaintInfo& paintInfo, const IntRect& rect)
{
    HDC hdc = *paintInfo.context->platformContext();
    RECT r = paintInfo.context->getCTM().mapRect(rect);

    IntRect fullProgressBarRect(r);
    paintInfo.context->setFillColor(Color(Color::white), ColorSpaceDeviceRGB);
    paintInfo.context->setStrokeColor(Color(Color::gray), ColorSpaceDeviceRGB);
    paintInfo.context->drawRect(fullProgressBarRect);

    IntRect progressRect = RenderThemeMg::calculateProgressRect(renderObject, fullProgressBarRect);

    if (!progressRect.isEmpty())
    {
        paintInfo.context->fillRect(progressRect, Color(Color::lightGray), ColorSpaceDeviceRGB);
    }
    return false;
}
#endif
}

