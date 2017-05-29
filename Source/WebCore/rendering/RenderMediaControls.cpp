/*
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"

#if ENABLE(VIDEO)

#include "RenderMediaControls.h"

#include "GraphicsContext.h"
#include "HTMLMediaElement.h"
#include "HTMLNames.h"
#include "RenderTheme.h"

// FIXME: Unify more of the code for Mac and Win.
#if PLATFORM(WIN)

#include <CoreGraphics/CoreGraphics.h>
#include <WebKitSystemInterface/WebKitSystemInterface.h>

// The Windows version of WKSI defines these functions as capitalized, while the Mac version defines them as lower case.
// FIXME: Is this necessary anymore?
#define wkMediaControllerThemeAvailable(themeStyle) WKMediaControllerThemeAvailable(themeStyle)
#define wkHitTestMediaUIPart(part, themeStyle, bounds, point) WKHitTestMediaUIPart(part, themeStyle, bounds, point)
#define wkMeasureMediaUIPart(part, themeStyle, bounds, naturalSize) WKMeasureMediaUIPart(part, themeStyle, bounds, naturalSize)
#define wkDrawMediaUIPart(part, themeStyle, context, rect, state) WKDrawMediaUIPart(part, themeStyle, context, rect, state)
#define wkDrawMediaSliderTrack(themeStyle, context, rect, timeLoaded, currentTime, duration, state) WKDrawMediaSliderTrack(themeStyle, context, rect, timeLoaded, currentTime, duration, state)

#endif

#if PLATFORM(MG)
#include "Gradient.h"
#include "minigui.h"
#include "BitmapImage.h"
#endif
 
using namespace std;
 
namespace WebCore {

#if PLATFORM(WIN)

static WKMediaControllerThemeState determineState(RenderObject* o)
{
    int result = 0;
    RenderTheme* theme = o->theme();
    if (!theme->isEnabled(o) || theme->isReadOnlyControl(o))
        result |= WKMediaControllerFlagDisabled;
    if (theme->isPressed(o))
        result |= WKMediaControllerFlagPressed;
    if (theme->isFocused(o))
        result |= WKMediaControllerFlagFocused;
    return static_cast<WKMediaControllerThemeState>(result);
}

// Utility to scale when the UI part are not scaled by wkDrawMediaUIPart
static FloatRect getUnzoomedRectAndAdjustCurrentContext(RenderObject* o, const PaintInfo& paintInfo, const IntRect &originalRect)
{
    float zoomLevel = o->style()->effectiveZoom();
    FloatRect unzoomedRect(originalRect);
    if (zoomLevel != 1.0f) {
        unzoomedRect.setWidth(unzoomedRect.width() / zoomLevel);
        unzoomedRect.setHeight(unzoomedRect.height() / zoomLevel);
        paintInfo.context->translate(unzoomedRect.x(), unzoomedRect.y());
        paintInfo.context->scale(FloatSize(zoomLevel, zoomLevel));
        paintInfo.context->translate(-unzoomedRect.x(), -unzoomedRect.y());
    }
    return unzoomedRect;
}

static const int mediaSliderThumbWidth = 13;
static const int mediaSliderThumbHeight = 14;

void RenderMediaControls::adjustMediaSliderThumbSize(RenderObject* o)
{
    ControlPart part = o->style()->appearance();

    if (part != MediaSliderThumbPart && part != MediaVolumeSliderThumbPart)
        return;

    CGSize size;
    wkMeasureMediaUIPart(part == MediaSliderThumbPart ? MediaSliderThumb : MediaVolumeSliderThumb, WKMediaControllerThemeQuickTime, 0, &size);

    float zoomLevel = o->style()->effectiveZoom();
    o->style()->setWidth(Length(static_cast<int>(size.width * zoomLevel), Fixed));
    o->style()->setHeight(Length(static_cast<int>(size.height * zoomLevel), Fixed));
}

bool RenderMediaControls::paintMediaControlsPart(MediaControlElementType part, RenderObject* o, const PaintInfo& paintInfo, const IntRect& r)
{
    static const int themeStyle = WKMediaControllerThemeQuickTime;
    paintInfo.context->save();
    switch (part) {
        case MediaFullscreenButton:
            wkDrawMediaUIPart(WKMediaUIPartFullscreenButton, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaShowClosedCaptionsButton:
        case MediaHideClosedCaptionsButton:
            if (MediaControlToggleClosedCaptionsButtonElement* btn = static_cast<MediaControlToggleClosedCaptionsButtonElement*>(o->node())) {
                bool captionsVisible = btn->displayType() == MediaHideClosedCaptionsButton;
                wkDrawMediaUIPart(captionsVisible ? WKMediaUIPartHideClosedCaptionsButton : WKMediaUIPartShowClosedCaptionsButton, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            }
            break;
        case MediaMuteButton:
        case MediaUnMuteButton:
            if (MediaControlMuteButtonElement* btn = static_cast<MediaControlMuteButtonElement*>(o->node())) {
                bool audioEnabled = btn->displayType() == MediaMuteButton;
                wkDrawMediaUIPart(audioEnabled ? WKMediaUIPartMuteButton : WKMediaUIPartUnMuteButton, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            }
            break;
        case MediaPauseButton:
        case MediaPlayButton:
            if (MediaControlPlayButtonElement* btn = static_cast<MediaControlPlayButtonElement*>(o->node())) {
                bool canPlay = btn->displayType() == MediaPlayButton;
                wkDrawMediaUIPart(canPlay ? WKMediaUIPartPlayButton : WKMediaUIPartPauseButton, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            }
            break;
        case MediaRewindButton:
            wkDrawMediaUIPart(WKMediaUIPartRewindButton, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaSeekBackButton:
            wkDrawMediaUIPart(WKMediaUIPartSeekBackButton, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaSeekForwardButton:
            wkDrawMediaUIPart(WKMediaUIPartSeekForwardButton, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaSlider: {
            if (HTMLMediaElement* mediaElement = toParentMediaElement(o)) {
                FloatRect unzoomedRect = getUnzoomedRectAndAdjustCurrentContext(o, paintInfo, r);
                wkDrawMediaSliderTrack(themeStyle, paintInfo.context->platformContext(), unzoomedRect, mediaElement->percentLoaded() * mediaElement->duration(), mediaElement->currentTime(), mediaElement->duration(), determineState(o));
            }
            break;
        }
        case MediaSliderThumb:
            wkDrawMediaUIPart(WKMediaUIPartTimelineSliderThumb, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaVolumeSliderContainer:
            wkDrawMediaUIPart(WKMediaUIPartVolumeSliderContainer, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaVolumeSlider:
            wkDrawMediaUIPart(WKMediaUIPartVolumeSlider, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaVolumeSliderThumb:
            wkDrawMediaUIPart(WKMediaUIPartVolumeSliderThumb, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaTimelineContainer:
            wkDrawMediaUIPart(WKMediaUIPartBackground, themeStyle, paintInfo.context->platformContext(), r, determineState(o));
            break;
        case MediaCurrentTimeDisplay:
            ASSERT_NOT_REACHED();
            break;
        case MediaTimeRemainingDisplay:
            ASSERT_NOT_REACHED();
            break;
        case MediaControlsPanel:
            ASSERT_NOT_REACHED();
            break;
    }
    paintInfo.context->restore();

    return false;
}

#endif

IntPoint RenderMediaControls::volumeSliderOffsetFromMuteButton(RenderBox* muteButtonBox, const IntSize& size)
{
    static const int xOffset = -4;
    static const int yOffset = 5;

    float zoomLevel = muteButtonBox->style()->effectiveZoom();
    int y = yOffset * zoomLevel + muteButtonBox->offsetHeight() - size.height();
    FloatPoint absPoint = muteButtonBox->localToAbsolute(FloatPoint(muteButtonBox->offsetLeft(), y), true, true);
    if (absPoint.y() < 0)
        y = muteButtonBox->height();
    return IntPoint(xOffset * zoomLevel, y);
}

#if PLATFORM(MG)
typedef WTF::HashMap<const char*, Image*> MediaControlImageMap;
static MediaControlImageMap* gMediaControlImageMap = 0;

static Image* platformResource(const char* name)
{
    if (!gMediaControlImageMap)
        gMediaControlImageMap = new MediaControlImageMap();
    if (Image* image = gMediaControlImageMap->get(name))
        return image;

    if (Image* image = Image::loadPlatformResource(name).releaseRef()) {
        gMediaControlImageMap->set(name, image);
        return image;
    }

    ASSERT_NOT_REACHED();
    return 0;
}

static bool hasSource(const HTMLMediaElement* mediaElement)
{
    return mediaElement->networkState() != HTMLMediaElement::NETWORK_EMPTY
        && mediaElement->networkState() != HTMLMediaElement::NETWORK_NO_SOURCE;
}

static bool paintMediaButton(GraphicsContext* context, const IntRect& rect, Image* image)
{
    IntRect imageRect = image->rect();
    context->drawImage(image, DeviceColorSpace, rect);
    return true;
}

static bool paintMediaMuteButton(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
    HTMLMediaElement* mediaElement = toParentMediaElement(object);
    if (!mediaElement)
      return false;

    static Image* soundFull = platformResource("audio-volume-high");
    static Image* soundNone = platformResource("audio-volume-muted");
    static Image* soundDisabled = platformResource("audio-volume-muted");

    if (!hasSource(mediaElement) || !mediaElement->hasAudio())
        return paintMediaButton(paintInfo.context, rect, soundDisabled);

    return paintMediaButton(paintInfo.context, rect, mediaElement->muted() ? soundNone: soundFull);
}

static bool paintSeekForwardButton(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
   HTMLMediaElement* mediaElement = toParentMediaElement(object);
    if (!mediaElement)
        return false;
    static Image* mediaSeekForward= platformResource("media-seek-forward");
	return paintMediaButton(paintInfo.context, rect, mediaSeekForward);
}
static bool paintSeekBackButton(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
	HTMLMediaElement* mediaElement = toParentMediaElement(object);
	if (!mediaElement)
		return false;
    static Image* mediaSeekBackward= platformResource("media-seek-backward");
	return paintMediaButton(paintInfo.context, rect, mediaSeekBackward);
}
static bool paintMediaPlayButton(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
    HTMLMediaElement* mediaElement = toParentMediaElement(object);
    if (!mediaElement)
        return false;

    static Image* mediaPlay = platformResource("media-playback-start");
    static Image* mediaPause = platformResource("media-playback-pause");
    static Image* mediaPlayDisabled = platformResource("media-playback-stop");

    if (!hasSource(mediaElement))
        return paintMediaButton(paintInfo.context, rect, mediaPlayDisabled);

    return paintMediaButton(paintInfo.context, rect, mediaElement->paused() ? mediaPlay : mediaPause);
}

static bool paintMediaSlider(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
    HTMLMediaElement* mediaElement = toParentMediaElement(object);
    if (!mediaElement)
        return false;

    RenderStyle* style = object->style();
    GraphicsContext* context = paintInfo.context;

    // Draw the border of the time bar.
    // FIXME: this should be a rounded rect but need to fix GraphicsContextSkia first.
    // https://bugs.webkit.org/show_bug.cgi?id=30143
	context->save();
	context->setShouldAntialias(true);
	context->setStrokeStyle(SolidStroke);
	context->setStrokeColor(style->borderLeftColor(), DeviceColorSpace);
	context->setStrokeThickness(style->borderLeftWidth());
	context->setFillColor(style->backgroundColor(), DeviceColorSpace);
    context->setFillColor(Color::gray, DeviceColorSpace);
    context->drawRect(rect);
    context->restore();
	return true;

    // Draw the buffered ranges.
    // FIXME: Draw multiple ranges if there are multiple buffered ranges.
    IntRect bufferedRect = rect;
    bufferedRect.inflate(-style->borderLeftWidth());
    bufferedRect.setWidth((bufferedRect.width() * mediaElement->percentLoaded()));

    // Don't bother drawing an empty area.
    if (!bufferedRect.isEmpty()) {
        IntPoint sliderTopLeft = bufferedRect.location();
        IntPoint sliderTopRight = sliderTopLeft;
        sliderTopRight.move(0, bufferedRect.height());

        RefPtr<Gradient> gradient = Gradient::create(sliderTopLeft, sliderTopRight);
        Color startColor = object->style()->color();
        gradient->addColorStop(0.0, startColor);
        gradient->addColorStop(1.0, Color(startColor.red() / 2, startColor.green() / 2, startColor.blue() / 2, startColor.alpha()));

        context->save();
        context->setStrokeStyle(NoStroke);
        context->setFillGradient(gradient);
        context->fillRect(bufferedRect);
        context->restore();
    }

    return true;
}

static bool paintMediaSliderThumb(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
    if (!object->parent()->isSlider())
        return false;

    HTMLMediaElement* mediaElement = toParentMediaElement(object->parent());
    if (!mediaElement)
        return false;

    if (!hasSource(mediaElement))
        return true;
#if 1 
    static Image* mediaSliderThumb = platformResource("mediaSliderThumb");
    return paintMediaButton(paintInfo.context, rect, mediaSliderThumb);
#else
	GraphicsContext* context = paintInfo.context;
	context->save();
	context->setShouldAntialias(true);
	context->setFillColor(Color::black, DeviceColorSpace);
	context->drawRect(rect);
	context->restore();
#endif
}

static bool paintMediaVolumeSlider(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
    HTMLMediaElement* mediaElement = toParentMediaElement(object);
    if (!mediaElement)
        return false;

    GraphicsContext* context = paintInfo.context;
    Color originalColor = context->strokeColor();
    if (originalColor != Color::white)
        context->setStrokeColor(Color::white, DeviceColorSpace);

    int x = rect.x() + rect.width() / 2;
    context->drawLine(IntPoint(x, rect.y()),  IntPoint(x, rect.y() + rect.height()));

    if (originalColor != Color::white)
        context->setStrokeColor(originalColor, DeviceColorSpace);
    return true;
}

static bool paintMediaVolumeSliderThumb(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
    if (!object->parent()->isSlider())
        return false;

#if 0
    paintInfo.context->fillRoundedRect(rect, IntSize(3, 3), IntSize(3, 3), IntSize(3, 3), IntSize(3, 3), Color::white, DeviceColorSpace);
    return false;
#else
    static Image* mediaVolumeSliderThumb = platformResource("mediaVolumeSliderThumb");
    return paintMediaButton(paintInfo.context, rect, mediaVolumeSliderThumb);
#endif
}

static bool paintMediaTimelineContainer(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
    HTMLMediaElement* mediaElement = toParentMediaElement(object);
    if (!mediaElement)
        return false;

    if (!rect.isEmpty()) {
        GraphicsContext* context = paintInfo.context;
        Color originalColor = context->strokeColor();
        float originalThickness = context->strokeThickness();
        StrokeStyle originalStyle = context->strokeStyle();

        context->setStrokeStyle(SolidStroke);

        // Draw the left border using CSS defined width and color.
        context->setStrokeThickness(object->style()->borderLeftWidth());
        context->setStrokeColor(object->style()->borderLeftColor().rgb(), DeviceColorSpace);
        context->drawLine(IntPoint(rect.x() + 1, rect.y()),
                          IntPoint(rect.x() + 1, rect.y() + rect.height()));

        // Draw the right border using CSS defined width and color.
        context->setStrokeThickness(object->style()->borderRightWidth());
        context->setStrokeColor(object->style()->borderRightColor().rgb(), DeviceColorSpace);
        context->drawLine(IntPoint(rect.x() + rect.width() - 1, rect.y()),
                          IntPoint(rect.x() + rect.width() - 1, rect.y() + rect.height()));

        context->setStrokeColor(originalColor, DeviceColorSpace);
        context->setStrokeThickness(originalThickness);
        context->setStrokeStyle(originalStyle);
    }
    return true;
}

bool RenderMediaControls::shouldRenderMediaControlPart(ControlPart part, Element* e)
{
    UNUSED_PARAM(e);

    switch (part) {
    case MediaMuteButtonPart:
    case MediaPlayButtonPart:
    case MediaSliderPart:
    case MediaSliderThumbPart:
    case MediaVolumeSliderContainerPart:
    case MediaVolumeSliderPart:
    case MediaVolumeSliderThumbPart:
    case MediaControlsBackgroundPart:
    case MediaCurrentTimePart:
    case MediaTimeRemainingPart:
        return true;
    default:
        ;
    }
    return false;
}

bool RenderMediaControls::paintMediaControlsPart(MediaControlElementType part, RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect)
{
    switch (part) {
    case MediaMuteButton:
    case MediaUnMuteButton:
        return paintMediaMuteButton(object, paintInfo, rect);
    case MediaPauseButton:
    case MediaPlayButton:
        return paintMediaPlayButton(object, paintInfo, rect);
    case MediaSlider:
        return paintMediaSlider(object, paintInfo, rect);
    case MediaSliderThumb:
        return paintMediaSliderThumb(object, paintInfo, rect);
    case MediaVolumeSlider:
        return paintMediaVolumeSlider(object, paintInfo, rect);
    case MediaVolumeSliderThumb:
        return paintMediaVolumeSliderThumb(object, paintInfo, rect);
    case MediaTimelineContainer:
        return paintMediaTimelineContainer(object, paintInfo, rect);
    case MediaSeekBackButton:
        return paintSeekBackButton(object, paintInfo, rect);
    case MediaSeekForwardButton:
        return paintSeekForwardButton(object, paintInfo, rect);
    case MediaFullscreenButton:
    case MediaVolumeSliderContainer:
    case MediaCurrentTimeDisplay:
    case MediaTimeRemainingDisplay:
    case MediaControlsPanel:
    case MediaRewindButton:
    case MediaReturnToRealtimeButton:
    case MediaStatusDisplay:
    case MediaShowClosedCaptionsButton:
    case MediaHideClosedCaptionsButton:
        ASSERT_NOT_REACHED();
        break;
    }
    return false;
}

void RenderMediaControls::adjustMediaSliderThumbSize(RenderObject* object)
{
    static Image* mediaSliderThumb = platformResource("mediaSliderThumb");
    static Image* mediaVolumeSliderThumb = platformResource("mediaVolumeSliderThumb");

    Image* thumbImage = 0;
    if (object->style()->appearance() == MediaSliderThumbPart)
        thumbImage = mediaSliderThumb;
    else if (object->style()->appearance() == MediaVolumeSliderThumbPart)
        thumbImage = mediaVolumeSliderThumb;

    float zoomLevel = object->style()->effectiveZoom();
    if (thumbImage) {
        object->style()->setWidth(Length(static_cast<int>(thumbImage->width() * zoomLevel), Fixed));
        object->style()->setHeight(Length(static_cast<int>(thumbImage->height() * zoomLevel), Fixed));
    }
}

#endif  // #if PLATFORM(MG)

}

#endif
