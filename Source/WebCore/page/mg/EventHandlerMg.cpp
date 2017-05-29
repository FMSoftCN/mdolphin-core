/*
** $Id: EventHandleMg.cpp 62 2010-06-07 06:33:20Z dongjunjie $
**
** EventHandleMg.cpp: event handle implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-2-5
*/

#include "config.h"
#include "EventHandler.h"

#include "Clipboard.h"
#include "Cursor.h"
#include "Document.h"
#include "EventNames.h"
#include "FloatPoint.h"
#include "FrameLoader.h"
#include "FocusController.h"
#include "Frame.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "HTMLFrameSetElement.h"
#include "HitTestRequest.h"
#include "HitTestResult.h"
#include "PlatformKeyboardEvent.h"
#include "MouseEventWithHitTestResults.h"
#include "Page.h"
#include "Scrollbar.h"
#include "PlatformWheelEvent.h"
#include "RenderWidget.h"
#include "NotImplemented.h"

namespace WebCore {

#if ENABLE(DRAG_SUPPORT)
const double EventHandler::TextDragDelay = 0.0;
#endif

bool EventHandler::tabsToAllFormControls(KeyboardEvent* event) const
{
    // We always allow tabs to all controls
    return true;
}

bool EventHandler::passMousePressEventToSubframe(MouseEventWithHitTestResults& mev, Frame* subframe)
{
    subframe->eventHandler()->handleMousePressEvent(mev.event());
    return true;
}

bool EventHandler::passMouseMoveEventToSubframe(MouseEventWithHitTestResults& mev, Frame* subframe, HitTestResult* hoveredNode)
{
    subframe->eventHandler()->handleMouseMoveEvent(mev.event(), hoveredNode);
    return true;
}

bool EventHandler::passMouseReleaseEventToSubframe(MouseEventWithHitTestResults& mev, Frame* subframe)
{
    subframe->eventHandler()->handleMouseReleaseEvent(mev.event());
    return true;
}

bool EventHandler::passWheelEventToWidget(PlatformWheelEvent& wheelEvent, Widget* widget)
{
    if (!widget->isFrameView())
        return false;

    return static_cast<FrameView*>(widget)->frame()->eventHandler()->handleWheelEvent(wheelEvent);
}

bool EventHandler::eventActivatedView(const PlatformMouseEvent& event) const
{
    return event.didActivateWebView();
}

void EventHandler::focusDocumentView()
{
    Page* page = m_frame->page();
    if (page)
        page->focusController()->setFocusedFrame(m_frame);
}

bool EventHandler::passMouseDownEventToWidget(Widget* widget)
{
    notImplemented();
    return false;
}

bool EventHandler::passWidgetMouseDownEventToWidget(const MouseEventWithHitTestResults& event)
{
    // Figure out which view to send the event to.
    // FIXME: what should mg do?
    RenderObject* target = targetNode(event) ? targetNode(event)->renderer() : 0;
    if (!target || !target->isWidget())
        return false;
    return passMouseDownEventToWidget(toRenderWidget(target)->widget());
}

bool EventHandler::passWidgetMouseDownEventToWidget(RenderWidget* renderWidget)
{
    return passMouseDownEventToWidget(renderWidget->widget());
}

unsigned EventHandler::accessKeyModifiers()
{
	return PlatformKeyboardEvent::AltKey;
}

#if ENABLE(DRAG_SUPPORT)
PassRefPtr<Clipboard> EventHandler::createDraggingClipboard() const
{
	return PassRefPtr<Clipboard>(0);
}
#endif


}
