/* 
** $Id$
** 
** PopuMenuMg.cpp: the native implement of PopuMenu
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
#include "PopupMenuMg.h"

#include "Document.h"
#include "FloatRect.h"
#include "FontSelector.h"
#include "Frame.h"
#include "FrameView.h"
#include "GraphicsContext.h"
#include "HTMLNames.h"
#include "HostWindow.h"
#include "Page.h"
#include "PlatformMouseEvent.h"
#include "PlatformScreen.h"
#include "RenderTheme.h"
#include "RenderView.h"
#include "Scrollbar.h"
#include "ScrollbarTheme.h"
#include "SimpleFontData.h"
#include "TextRun.h"

//#include "Logging.h"

#define POPUPMENU_CTRL   ("md_popmenu")

namespace WebCore {

using namespace HTMLNames;

static const int optionSpacingMiddle = 1;
static const int popupWindowBorderWidth = 1;
static const int endOfLinePadding = 2;
static const int separatorPadding = 4;
static const int separatorHeight  = 1;

static const int maxPopupHeight = 320;

static const int MSG_HOST_WINDOW_FIRST = MSG_USER;
static const int MSG_HOST_WINDOW_CHAR  =  MSG_HOST_WINDOW_FIRST + MSG_CHAR;
static const int MSG_HOST_WINDOW_MOUSEMOVE = MSG_HOST_WINDOW_FIRST + MSG_MOUSEMOVE;
static const int MSG_HOST_WINDOW_CLOSE = MSG_HOST_WINDOW_FIRST;
static const int MSG_HOST_SIZECHANGED  = MSG_HOST_WINDOW_FIRST + 1;

static BOOL invalidate_exclude_scrollbar(HWND hwnd, Scrollbar *scrollbar, BOOL bEraseBkgnd)
{
    if (!scrollbar)
    {
        return InvalidateRect(hwnd, NULL, bEraseBkgnd);
    }
    else
    {
        RECT rcClient;
        RECT rcScrollbar;
        GetClientRect(hwnd, &rcClient);
        rcScrollbar = scrollbar->frameRect();

        if(rcClient.left == rcScrollbar.left)
        {
            rcClient.left = rcScrollbar.right;
        }
        else
        {
            rcClient.right = rcScrollbar.left;
        }
        return InvalidateRect(hwnd, &rcClient, bEraseBkgnd);
    }
}

PopupMenuMg::PopupMenuMg(PopupMenuClient* client)
    : m_popupClient(client)
    , m_scrollbar(0)
    , m_popup(HWND_INVALID)
    , m_wasClicked(false)
    , m_itemHeight(0)
    , m_scrollOffset(0)
    , m_wheelDelta(0)
    , m_focusedIndex(0)
    , m_scrollbarCapturingMouse(false)
{
}

PopupMenuMg::~PopupMenuMg()
{
    if (m_popup != HWND_INVALID)
        DestroyWindow(m_popup);
}


static int _first_button_down = 0;
void PopupMenuMg::show(const IntRect& r, FrameView* v, int index)
{
    calculatePositionAndSize(r, v);
    if (clientRect().isEmpty())
        return;

	HWND hostWindow = (HWND)v->hostWindow()->platformPageClient();

	if (!m_scrollbar && visibleItems() < client()->listSize()) {
		// We need a scroll bar
		m_scrollbar = client()->createScrollbar(this, VerticalScrollbar, SmallScrollbar);
		m_scrollbar->styleChanged();
	}
    
    if (m_popup == HWND_INVALID) {
        m_popup = CreateWindowEx(POPUPMENU_CTRL,
                  "popmenu",
                  WS_BORDER,
                  0x20000000L, //WS_EX_CTRLASMAIN
                  (int)this, 
                  r.x(), r.y(), r.width(), r.height(),
                  hostWindow, (DWORD)this);
        if(m_popup == HWND_INVALID)
            return;
    }

    MoveWindow(m_popup, m_windowRect.x(), m_windowRect.y(), m_windowRect.width(), m_windowRect.height(), FALSE);

    ShowWindow(m_popup, SW_SHOW);

    SetFocus(m_popup);

    if (client()) {
        int index = client()->selectedIndex();
        if (index >= 0)
            setFocusedIndex(index);
    }

	RefPtr<PopupMenuMg> protect(this);

	MSG msg;

    //Peek and process all the message before begin popupmenu loop

	SetCapture(hostWindow);

    _first_button_down = 1;

	while(GetMessage(&msg, GetMainWindowHandle(hostWindow))) {
		if (msg.message == MSG_HOST_WINDOW_CLOSE)
			break;

		if ((msg.message == MSG_HOST_WINDOW_MOUSEMOVE
			 || msg.message == MSG_HOST_WINDOW_CHAR)
			&& msg.hwnd == m_popup) {
				msg.hwnd = hostWindow;
				msg.message -= MSG_HOST_WINDOW_FIRST;
		}
		else if (msg.message >= MSG_NCMOUSEOFF && msg.message <= MSG_NCRBUTTONDBLCLK) {
			msg.hwnd = m_popup; // Steal mouse messages
		}
		else if (msg.message >= MSG_FIRSTMOUSEMSG && msg.message < MSG_NCMOUSEOFF) {
			// Steal the mouse message
			// Need Tranlsate the coordinate.
			int x = LOSWORD(msg.lParam);
			int y = HISWORD(msg.lParam);
			ScreenToClient(m_popup, &x, &y);
			msg.lParam = MAKELONG(x, y);
			msg.hwnd = m_popup;
		}
		else if (msg.message >= MSG_FIRSTKEYMSG && msg.message <= MSG_LASTKEYMSG) {
			msg.hwnd = m_popup; // Steal key messages
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(!m_popupClient)
			break;

		if (GetCapture() != hostWindow)
			break;
	}

	if (GetCapture() == hostWindow)
		::ReleaseCapture();

	hide();
}

void PopupMenuMg::hide()
{
    if(m_popup != HWND_INVALID)
	{
        ShowWindow(m_popup, SW_HIDE);

		if (client())
			client()->popupDidHide();
		
		//Post quit message to break the message loop
		SendNotifyMessage(m_popup, MSG_HOST_WINDOW_CLOSE, 0, 0);
	}
}

void PopupMenuMg::calculatePositionAndSize(const IntRect& r, FrameView* v)
{
    // rect is in absolute document coordinates, but we want to be in screen coordinates
    IntRect rScreenCoords(v->contentsToWindow(r.location()), r.size());

    // First, determine the popup's height
    int itemCount = client()->listSize();
    m_itemHeight = client()->menuStyle().font().fontMetrics().height() + optionSpacingMiddle;
    int naturalHeight = m_itemHeight * itemCount;
    int popupHeight = MIN(maxPopupHeight, naturalHeight);
    // The popup should show an integral number of items (i.e. no partial items should be visible)
    popupHeight -= popupHeight % m_itemHeight;
    
    // Next determine its width
    int popupWidth = 0;
    for (int i = 0; i < itemCount; ++i) {
        String text = client()->itemText(i);
        if (text.isEmpty())
            continue;

		Font itemFont = client()->menuStyle().font();
		if (client()->itemIsLabel(i)) {
			FontDescription d = itemFont.fontDescription();
			d.setWeight(d.bolderWeight());
			itemFont = Font(d, itemFont.letterSpacing(), itemFont.wordSpacing());
			itemFont.update(m_popupClient->fontSelector());
		}

        popupWidth = MAX(popupWidth, itemFont.width(TextRun(text.characters(), text.length())));
    }

    if (naturalHeight > maxPopupHeight)
        // We need room for a scrollbar
        popupWidth += ScrollbarTheme::nativeTheme()->scrollbarThickness(SmallScrollbar);

    // Add padding to align the popup text with the <select> text
    // Note: We can't add paddingRight() because that value includes the width
    // of the dropdown button, so we must use our own endOfLinePadding constant.
    popupWidth += endOfLinePadding + client()->clientPaddingLeft();

    // Leave room for the border
    popupWidth += 2 * popupWindowBorderWidth;
    popupHeight += 2 * popupWindowBorderWidth;

    // The popup should be at least as wide as the control on the page
    popupWidth = max(rScreenCoords.width(), popupWidth);

    // Always left-align items in the popup.  This matches popup menus on the mac.
    int popupX = rScreenCoords.x();

    IntRect popupRect(popupX, rScreenCoords.maxY(), popupWidth, popupHeight);

    // The popup needs to stay within the bounds of the screen and not overlap any toolbars
    FloatRect screen = screenAvailableRect(v);

    int popupBottom = popupRect.x() + popupRect.width();
    int popupRight = popupRect.y() + popupRect.height();
    ClientToScreen((HWND)v->platformWidget(), &popupRight, &popupBottom);

    // Check that we don't go off the screen vertically
    if (popupBottom > screen.height()) {
        // The popup will go off the screen, so try placing it above the client
        if (rScreenCoords.y() - popupRect.height() < 0) {
            // The popup won't fit above, either, so place it whereever's bigger and resize it to fit
            if ((rScreenCoords.y() + rScreenCoords.height() / 2) < (screen.height() / 2)) {
                // Below is bigger
                popupRect.setHeight((int)ceilf(screen.height() - popupRect.y()));
            } else {
                // Above is bigger
                popupRect.setY(0);
                popupRect.setHeight(rScreenCoords.y());
            }
        } else {
            // The popup fits above, so reposition it
            popupRect.setY(rScreenCoords.y() - popupRect.height());
        }
    }

    // Check that we don't go off the screen horizontally
    if (popupRect.x() < screen.x()) {
        popupRect.setWidth((int)ceilf(popupRect.width() - (screen.x() - popupRect.x())));
        popupRect.setX((int)ceilf(screen.x()));
    }
    m_windowRect = popupRect;
    return;
}

bool PopupMenuMg::setFocusedIndex(int i, bool hotTracking)
{
    if (i < 0 || i >= client()->listSize() || i == focusedIndex())
        return false;

    if (!client()->itemIsEnabled(i))
        return false;

    invalidateItem(focusedIndex());
    invalidateItem(i);

    m_focusedIndex = i;

    if (!hotTracking)
        client()->setTextFromItem(i);

    if (!scrollToRevealSelection())
        //UpdateWindow(m_popup, FALSE);
        invalidate_exclude_scrollbar(m_popup, scrollbar(), FALSE);

    return true;
}

int PopupMenuMg::visibleItems() const
{
    return clientRect().height() / m_itemHeight;
}

int PopupMenuMg::listIndexAtPoint(const IntPoint& point) const
{
    return m_scrollOffset + point.y() / m_itemHeight;
}

int PopupMenuMg::focusedIndex() const
{
    return m_focusedIndex;
}

void PopupMenuMg::focusFirst()
{
    if (!client())
        return;

    int size = client()->listSize();

    for (int i = 0; i < size; ++i)
        if (client()->itemIsEnabled(i)) {
            setFocusedIndex(i);
            break;
        }
}

void PopupMenuMg::focusLast()
{
    if (!client())
        return;

    int size = client()->listSize();

    for (int i = size - 1; i > 0; --i)
        if (client()->itemIsEnabled(i)) {
            setFocusedIndex(i);
            break;
        }
}

bool PopupMenuMg::down(unsigned lines)
{
    if (!client())
        return false;

    int size = client()->listSize();

    int lastSelectableIndex, selectedListIndex;
    lastSelectableIndex = selectedListIndex = focusedIndex();
    for (int i = selectedListIndex + 1; i >= 0 && i < size; ++i)
        if (client()->itemIsEnabled(i)) {
            lastSelectableIndex = i;
            if (i >= selectedListIndex + (int)lines)
                break;
        }

    return setFocusedIndex(lastSelectableIndex);
}

bool PopupMenuMg::up(unsigned lines)
{
    if (!client())
        return false;

    int size = client()->listSize();

    int lastSelectableIndex, selectedListIndex;
    lastSelectableIndex = selectedListIndex = focusedIndex();
    for (int i = selectedListIndex - 1; i >= 0 && i < size; --i)
        if (client()->itemIsEnabled(i)) {
            lastSelectableIndex = i;
            if (i <= selectedListIndex - (int)lines)
                break;
        }

    return setFocusedIndex(lastSelectableIndex);
}

void PopupMenuMg::invalidateItem(int index)
{
    if (m_popup == HWND_INVALID)
        return;

    IntRect damageRect(clientRect());
    damageRect.setY(m_itemHeight * (index - m_scrollOffset));
    damageRect.setHeight(m_itemHeight);
    if (m_scrollbar)
        damageRect.setWidth(damageRect.width() - m_scrollbar->frameRect().width());

    RECT r;
	r.left = damageRect.x();
	r.top  = damageRect.y();
	r.right = r.left + damageRect.width();
	r.bottom = r.top + damageRect.height();
    //InvalidateRect(m_popup, &r, FALSE);
    invalidate_exclude_scrollbar(m_popup, scrollbar(), FALSE);
}

IntRect PopupMenuMg::clientRect() const
{
    IntRect clientRect = m_windowRect;
    clientRect.inflate(-popupWindowBorderWidth);
    clientRect.setLocation(IntPoint(0, 0));
    return clientRect;
}

void PopupMenuMg::incrementWheelDelta(int delta)
{
    m_wheelDelta += delta;
}

void PopupMenuMg::reduceWheelDelta(int delta)
{
    if (m_wheelDelta > 0)
        m_wheelDelta -= delta;
    else if (m_wheelDelta < 0)
        m_wheelDelta += delta;
    else
        return;
}

bool PopupMenuMg::scrollToRevealSelection()
{
    if (!m_scrollbar)
        return false;

    int index = focusedIndex();

    if (index < m_scrollOffset) {
        ScrollableArea::scrollToYOffsetWithoutAnimation(index);
        return true;
    }

    if (index >= m_scrollOffset + visibleItems()) {
        ScrollableArea::scrollToYOffsetWithoutAnimation(index - visibleItems() + 1);
        return true;
    }

    return false;
}

void PopupMenuMg::updateFromElement()
{
    if (m_popup == HWND_INVALID)
        return;

   // InvalidateRect(m_popup, 0, FALSE);
    invalidate_exclude_scrollbar(m_popup, scrollbar(), FALSE);
    if (!scrollToRevealSelection())
        UpdateWindow(m_popup, FALSE);
}
#if 0
bool PopupMenuMg::itemWritingDirectionIsNatural()
{
    return true;
}
#endif
void PopupMenuMg::paint(const IntRect& damageRect, HDC hdc)
{
    GraphicsContext context(&hdc);

    IntRect listRect = damageRect;
    listRect.move(IntSize(0, m_scrollOffset * m_itemHeight));

    for (int y = listRect.y(); y < listRect.maxY(); y += m_itemHeight) {
        int index = y / m_itemHeight;

        if (index >= client()->listSize())
            break;

        Color optionBackgroundColor, optionTextColor;
        PopupMenuStyle itemStyle = client()->itemStyle(index);
        if (index == focusedIndex()) {
            optionBackgroundColor = RenderTheme::defaultTheme()->activeListBoxSelectionBackgroundColor();
            optionTextColor = RenderTheme::defaultTheme()->activeListBoxSelectionForegroundColor();
        } else {
            optionBackgroundColor = itemStyle.backgroundColor();
            optionTextColor = itemStyle.foregroundColor();
        }

        // itemRect is in client coordinates
        IntRect itemRect(0, (index - m_scrollOffset) * m_itemHeight, damageRect.width(), m_itemHeight);

        // Draw the background for this menu item
        if (itemStyle.isVisible())
            context.fillRect(itemRect, optionBackgroundColor, ColorSpaceDeviceRGB);

        if (client()->itemIsSeparator(index)) {
            IntRect separatorRect(itemRect.x() + separatorPadding, itemRect.y() + (itemRect.height() - separatorHeight) / 2, itemRect.width() - 2 * separatorPadding, separatorHeight);
            context.fillRect(separatorRect, optionTextColor, ColorSpaceDeviceRGB);
            continue;
        }

        String itemText = client()->itemText(index);
            
        unsigned length = itemText.length();
        const UChar* string = itemText.characters();
        TextRun textRun(string, length, false, 0, 0, TextRun::AllowTrailingExpansion, itemText.defaultWritingDirection() == WTF::Unicode::RightToLeft);

        context.setFillColor(optionTextColor, ColorSpaceDeviceRGB);
        
        Font itemFont = client()->menuStyle().font();
        if (client()->itemIsLabel(index)) {
            FontDescription d = itemFont.fontDescription();
			d.setWeight(d.bolderWeight());
            itemFont = Font(d, itemFont.letterSpacing(), itemFont.wordSpacing());
            itemFont.update(m_popupClient->fontSelector());
        }
        
        // Draw the item text
        if (itemStyle.isVisible()) {
            int textX = max(0, client()->clientPaddingLeft() - client()->clientInsetLeft());
			if (RenderTheme::defaultTheme()->popupOptionSupportsTextIndent() && itemStyle.textDirection() == LTR)
				textX += itemStyle.textIndent().calcMinValue(itemRect.width());
            int textY = itemRect.y() + itemFont.fontMetrics().ascent() + (itemRect.height() - itemFont.fontMetrics().height()) / 2;
            context.drawBidiText(itemFont, textRun, IntPoint(textX, textY));
        }
    }

    if (m_scrollbar)
        m_scrollbar->paint(&context, damageRect);
}
#if 0
void PopupMenuMg::valueChanged(Scrollbar* scrollbar)
{
    if (m_popup == HWND_INVALID)
        return;

    int offset = scrollbar->value();

    if (m_scrollOffset == offset)
        return;

    int scrolledLines = m_scrollOffset - offset;
    m_scrollOffset = offset;

    IntRect listRect = clientRect();
    if (m_scrollbar)
        listRect.setWidth(listRect.width() - m_scrollbar->frameRect().width());
    RECT r = {listRect.x(), listRect.y(), listRect.x() + listRect.width(), listRect.y() + listRect.height()};
    ScrollWindow(m_popup, 0, scrolledLines * m_itemHeight, &r, NULL);
    if (m_scrollbar) {
        IntRect rect = m_scrollbar->frameRect();
		r.left = rect.x();
		r.top  = rect.y();
		r.right = r.left + rect.width();
		r.bottom = r.top + rect.height();
        //InvalidateRect(m_popup, &r, FALSE);
    }
    invalidate_exclude_scrollbar(m_popup, scrollbar, FALSE);
    //UpdateWindow(m_popup, FALSE);
}

#endif
void PopupMenuMg::invalidateScrollbarRect(Scrollbar* scrollbar, const IntRect& rect)
{
    IntRect scrollRect = rect;
    scrollRect.move(scrollbar->x(), scrollbar->y());
    RECT r = scrollRect;
    ::InvalidateRect(m_popup, &r, false);
}

int PopupMenuMg::scrollSize(ScrollbarOrientation orientation) const
{
    return ((orientation == VerticalScrollbar) && m_scrollbar) ? (m_scrollbar->totalSize() - m_scrollbar->visibleSize()) : 0;
}

int PopupMenuMg::scrollPosition(Scrollbar*) const
{
    return m_scrollOffset;
}

void PopupMenuMg::setScrollOffset(const IntPoint& offset)
{
    scrollTo(offset.y());
}

void PopupMenuMg::scrollTo(int offset)
{
    if (m_popup == HWND_INVALID)
        return;

    if (m_scrollOffset == offset)
        return;

    int scrolledLines = m_scrollOffset - offset;
    m_scrollOffset = offset;

    IntRect listRect = clientRect();
    if (m_scrollbar)
        listRect.setWidth(listRect.width() - m_scrollbar->frameRect().width());
    RECT r = {listRect.x(), listRect.y(), listRect.x() + listRect.width(), listRect.y() + listRect.height()};
    ScrollWindow(m_popup, 0, scrolledLines * m_itemHeight, &r, NULL);
    if (m_scrollbar) {
        IntRect rect = m_scrollbar->frameRect();
		r.left = rect.x();
		r.top  = rect.y();
		r.right = r.left + rect.width();
		r.bottom = r.top + rect.height();
        //InvalidateRect(m_popup, &r, FALSE);
    }
    invalidate_exclude_scrollbar(m_popup, scrollbar(), FALSE);
    //UpdateWindow(m_popup, FALSE);
}

void PopupMenuMg::disconnectClient()
{
        m_popupClient = 0;
}


static int PopupCtrlProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
	PopupMenuMg * popup;


	if(message == MSG_CREATE)
	{
        _first_button_down = 1;
		SetWindowAdditionalData2(hwnd, GetWindowAdditionalData(hwnd));
	}

	popup = reinterpret_cast<PopupMenuMg*>(GetWindowAdditionalData2(hwnd));

	if(!popup)
		return DefaultControlProc(hwnd, message, wParam, lParam);

    switch (message)
    {
    case MSG_PAINT:
        {
            HDC hdc = BeginPaint(hwnd);
            popup->paint(popup->clientRect(), hdc);
            EndPaint(hwnd, hdc);
        }
        break;
    case MSG_KILLFOCUS:
        {
			popup->hide();
        }
        break;
    case MSG_MOUSEMOVE:
		{

			/*if(GetCapture()==hwnd){
				int mouseX=LOWORD(lParam);
				int mouseY=HIWORD(lParam);
				ScreenToClient (hwnd, &mouseX, &mouseY);
				lParam=mouseX|mouseY<<16;
			}*/

			IntPoint mousePoint(LOSWORD(lParam), HISWORD(lParam));
			if (popup->scrollbar()) {
				IntRect scrollBarRect = popup->scrollbar()->frameRect();
				if (popup->scrollbarCapturingMouse() || scrollBarRect.contains(mousePoint)) {
					// Put the point into coordinates relative to the scroll bar
					mousePoint.move(-scrollBarRect.x(), -scrollBarRect.y());
					PlatformMouseEvent event(hwnd, message, wParam, MAKELONG(mousePoint.x(), mousePoint.y()));
					popup->scrollbar()->mouseMoved(event);
					break;
				}
			}

			RECT bounds;
			GetClientRect(hwnd, &bounds);
			if (!PtInRect(&bounds, mousePoint.x(), mousePoint.y())
				/*&& !(wParam & KS_LEFTBUTTON) && popup->client()*/){
                // When the mouse is not inside the popup menu and the left button isn't down, just
                // repost the message to the web view.
			/*  HWND hPage = (HWND)popup->client()->hostWindow()->platformPageClient();	
				if(IsWindow(hPage))
				{
					//send message to hPage
					int x = LOSWORD(lParam);
					int y = HISWORD(lParam);
					ClientToScreen(hwnd, &x, &y);
					ScreenToClient(hPage, &x, &y);
					PostMessage(hwnd, MSG_HOST_WINDOW_MOUSEMOVE, wParam, MAKELONG(x, y));
					break;
				}*/

				break;
            }

            // Translate the coordinate.
			popup->setFocusedIndex(popup->listIndexAtPoint(mousePoint), true);
        }
        break;
    case MSG_LBUTTONDOWN:
        {
			//SetCapture(hwnd);
			IntPoint mousePoint(LOSWORD(lParam), HISWORD(lParam));

			if (popup->scrollbar()) {
				IntRect scrollBarRect = popup->scrollbar()->frameRect();
				if (scrollBarRect.contains(mousePoint)) {
					// Put the point into coordinates relative to the scroll bar
					mousePoint.move(-scrollBarRect.x(), -scrollBarRect.y());
					PlatformMouseEvent event(hwnd, message, wParam, MAKELONG(mousePoint.x(), mousePoint.y()));
					popup->scrollbar()->mouseDown(event);
					popup->setScrollbarCapturingMouse(true);
					break;
				}
			}

			RECT bounds;
			GetClientRect(hwnd, &bounds);
			if (!PtInRect(&bounds, mousePoint.x(), mousePoint.y()))
                break;

			popup->setFocusedIndex(popup->listIndexAtPoint(mousePoint), true);
        }
        break;
    case MSG_LBUTTONUP:
		{
			/*if(GetCapture()==hwnd){
				ReleaseCapture();
				//we must transfer the x,y coordinate 
				//or when click link ,it may not response(the mouse postion is not correct
				int mouseX=LOWORD(lParam);
				int mouseY=HIWORD(lParam);
				ScreenToClient (hwnd, &mouseX, &mouseY);
				lParam=mouseX|mouseY<<16;

			}*/
            if(_first_button_down)
            {
                _first_button_down = 0;
                break;
            }

			IntPoint mousePoint(LOSWORD(lParam), HISWORD(lParam));
			if (popup->scrollbar()) {
				IntRect scrollBarRect = popup->scrollbar()->frameRect();
				if (popup->scrollbarCapturingMouse() || scrollBarRect.contains(mousePoint)) {
					popup->setScrollbarCapturingMouse(false);
					// Put the point into coordinates relative to the scroll bar
					mousePoint.move(-scrollBarRect.x(), -scrollBarRect.y());
					//PlatformMouseEvent event(hwnd, message, wParam, MAKELONG(mousePoint.x(), mousePoint.y()));
					popup->scrollbar()->mouseUp();
					// FIXME: This is a hack to work around Scrollbar not 
					// invalidating correctly when it doesn't have a parent widget
					/*RECT r = {scrollBarRect.x(), scrollBarRect.y(), scrollBarRect.x() + scrollBarRect.width(), scrollBarRect.y() + scrollBarRect.height() };
					InvalidateRect(hwnd, &r, FALSE);*/
					break;
				}
			}
			// Only release capture and hide the popup if the mouse is inside the popup window.
			/*RECT bounds;
			GetClientRect(hwnd, &bounds);
			if (popup->client() && PtInRect(&bounds, mousePoint.x(), mousePoint.y()))*/ {
				popup->hide();
				int index = popup->focusedIndex();
				if (index >= 0)
					popup->client()->valueChanged(index);
			}
        }
        break;
    case MSG_KEYDOWN:
        {
            if ( popup->client()) {
                switch (wParam) {
                    case SCANCODE_CURSORBLOCKUP:
                        popup->up();
                        break;
		            case SCANCODE_CURSORBLOCKDOWN:
                        popup->down();
                        break;
                    case SCANCODE_HOME:
                        popup->focusFirst();
                        break;
                    case SCANCODE_END:
                        popup->focusLast();
                        break;
                    case SCANCODE_PAGEUP:
                        {
                            if (popup->focusedIndex() != popup->scrollOffset()) {
                                int firstVisibleItem = popup->scrollOffset();
                                popup->up(popup->focusedIndex() - firstVisibleItem);
                            } else
                                popup->up(popup->visibleItems());
                        }
                        break;
                    case SCANCODE_PAGEDOWN:
                        {
                            int lastVisibleItem = popup->scrollOffset() + popup->visibleItems() - 1;
                            if (popup->focusedIndex() != lastVisibleItem) {
                                popup->down(lastVisibleItem - popup->focusedIndex());
                            } else
                                popup->down(popup->visibleItems());
                        }
                        break;
                    case SCANCODE_ENTER:
                        {
							popup->hide();
                            int index = popup->focusedIndex();
                            popup->client()->valueChanged(index);
                        }
                        break;
                    case SCANCODE_ESCAPE:
						popup->hide();
                        break;
                    default:
                        /* complier needed*/
                        break;
                }
            }
        }
        break;
    case MSG_SIZECHANGED:
        //Becase PopuMenuCtrl is a WS_EX_CTRLASMAIN control, and 
        //the desktop thread will send MSG_SIZECHANGED by SendASyncMessage,
        //which would call the wndproc of hwnd directly!
        //but, the popup->client()->listSize() must be called in mainthread,
        //so we call SendNotifyMessage translate the MSG_SIZECHANGED to MSG_HOST_SIZECHANGED
        //We cannot call SendMessage or SendSyncMessage, Becuase MoveWindow can cause a MSG_SIZECHAGED
        //message, which maybe make the message-queue dead-locked
        {
            RECT *prc = (RECT*)lParam;
            SendNotifyMessage(hwnd, MSG_HOST_SIZECHANGED, RECTWP(prc), RECTHP(prc));
        }
        break;

    case MSG_HOST_SIZECHANGED:
        {
            if ( popup->scrollbar()) {
                IntSize size((int)wParam,(int)lParam);
                popup->scrollbar()->setFrameRect(IntRect(size.width() - popup->scrollbar()->width(), 0,
                                                        popup->scrollbar()->width(), size.height()));
                int visibleItems = popup->visibleItems();
                popup->scrollbar()->setEnabled(visibleItems < popup->client()->listSize());
                popup->scrollbar()->setSteps(1, max(1, visibleItems - 1));
                popup->scrollbar()->setProportion(visibleItems, popup->client()->listSize());
                //InvalidateRect(hwnd, NULL, TRUE); //reinvalidate
                invalidate_exclude_scrollbar(hwnd, popup->scrollbar(), FALSE);
            }
        }
        return 0;
    }
   return DefaultControlProc (hwnd, message, wParam, lParam);
}

bool RegisterPopmenu(void)
{
    WNDCLASS MenuClass;

    MenuClass.spClassName = POPUPMENU_CTRL;
    MenuClass.dwStyle = WS_NONE;
    MenuClass.dwExStyle = WS_EX_NONE;
    MenuClass.hCursor = GetSystemCursor (IDC_ARROW);
    MenuClass.iBkColor = COLOR_lightwhite;
    MenuClass.WinProc = PopupCtrlProc;

    return RegisterWindowClass (&MenuClass);
}

void UnregisterPopmenmu(void)
{
    UnregisterWindowClass (POPUPMENU_CTRL);
}

}

