/* 
** $Id$
** 
** PlatformMouseEventMg.cpp:
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
#include "PlatformMouseEvent.h"


namespace WebCore {

#define HIGH_BIT_MASK_SHORT 0x8000

static int globalClickCount = 0;
static enum MouseButton globalPrevClickButton = LeftButton;
static DWORD globalPrevClickTime = 0;
static IntPoint globalPrevClickPosition = IntPoint();

static IntPoint positionForEvent(HWND hWnd, LPARAM lParam)
{
    POINT point = {LOSWORD(lParam), HISWORD(lParam)};
    return IntPoint(point.x, point.y);
}

static IntPoint globalPositionForEvent(HWND hWnd, LPARAM lParam)
{
    POINT point = {LOSWORD(lParam), HISWORD(lParam)};
    ClientToScreen(hWnd, &point.x , &point.y);
    return IntPoint(point.x, point.y);
}

static MouseEventType messageToEventType(int message)
{
    switch (message) {
        case MSG_LBUTTONDBLCLK:
        case MSG_RBUTTONDBLCLK:
            //MSDN docs say double click is sent on mouse down
        case MSG_LBUTTONDOWN:
        case MSG_RBUTTONDOWN:
            return MouseEventPressed;

        case MSG_LBUTTONUP:
        case MSG_RBUTTONUP:
            return MouseEventReleased;

        case MSG_MOUSEMOVE:
            return MouseEventMoved;

        default:
            return MouseEventMoved;
    }
}

PlatformMouseEvent::PlatformMouseEvent(HWND hWnd, int message, WPARAM wParam, LPARAM lParam, bool didActivateWebView)
    : m_position(positionForEvent(hWnd, lParam))
    , m_globalPosition(globalPositionForEvent(hWnd, lParam))
    , m_eventType(messageToEventType(message))
    , m_clickCount(0)
    , m_shiftKey(wParam & KS_SHIFT)
    , m_ctrlKey(wParam & KS_CTRL)
    , m_altKey(GetShiftKeyStatus() & KS_ALT)
    , m_metaKey(m_altKey) // FIXME: We'll have to test other browsers
    , m_modifierFlags(wParam)
    , m_didActivateWebView(didActivateWebView)
{
    m_timestamp = GetTickCount()*0.01; // GetTickCount returns milliseconds
    switch (message) {
        case MSG_LBUTTONDOWN:
        case MSG_LBUTTONUP:
        case MSG_LBUTTONDBLCLK:
            m_button = LeftButton;
            break;
        case MSG_RBUTTONDOWN:
        case MSG_RBUTTONUP:
        case MSG_RBUTTONDBLCLK:
            m_button = RightButton;
            break;
        case MSG_MOUSEMOVE:
            if (wParam & SCANCODE_LEFTBUTTON)
                m_button = LeftButton;
            else if (wParam & SCANCODE_RIGHTBUTTON)
                m_button = RightButton;
            else if (wParam & SCANCODE_MIDDLBUTTON)
                m_button = MiddleButton;
            else
                m_button = NoButton;
            break;
        default:
            break;
    }

    switch (message) {
        case MSG_LBUTTONDOWN:
        case MSG_RBUTTONDOWN:
            if (globalPrevClickButton != m_button)
                globalClickCount = 0;
            // FALL THROUGH
        case MSG_LBUTTONDBLCLK:  // For these messages, the OS ensures that the
        case MSG_RBUTTONDBLCLK:
            {
                DWORD curTime = GetTickCount();
                if (curTime - globalPrevClickTime > 40 ||
                    m_position != globalPrevClickPosition)
                    globalClickCount = 0;
                globalPrevClickTime = curTime;
            }
            globalPrevClickButton = m_button;
            globalPrevClickPosition = m_position;
            m_clickCount = ++globalClickCount;
            return;
    }

    m_clickCount = (m_button == globalPrevClickButton) ? globalClickCount :
        ((message == MSG_LBUTTONUP || message == MSG_RBUTTONUP) ? 1 : 0);
}

} // namespace WebCore
