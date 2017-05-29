/*
 * Copyright (C) 2008 Kevin Ollivier <kevino@theolliviers.com> All Rights Reserved.
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

#include "minigui.h"
#include "config.h"
#include "PluginView.h"

#include "Bridge.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "Element.h"
#include "FrameLoader.h"
#include "FrameLoadRequest.h"
#include "FrameTree.h"
#include "Frame.h"
#include "FrameView.h"
#include "GraphicsContext.h"
#include "HTMLNames.h"
#include "HTMLPlugInElement.h"
#include "HostWindow.h"
#include "Image.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include "Page.h"
#include "Chrome.h"
#include "PlatformKeyboardEvent.h"
#include "PlatformMouseEvent.h"
#include "PluginDebug.h"
#include "PluginMainThreadScheduler.h"
#include "PluginPackage.h"
#include "RenderLayer.h"
#include "Settings.h"
#include "JSDOMBinding.h"
#include "ScriptController.h"
#include "npruntime_impl.h"
#include "npapi.h"
#include "runtime_root.h"
#include <runtime/JSLock.h>
#include <runtime/JSValue.h>

#include "PluginApiMg.h"
//#include "Helper.h"

#define DBG()  printf("%s-%d-%s\n", __FILE__, __LINE__, __FUNCTION__)
using namespace WTF;

namespace WebCore {

#if ENABLE(PLUGIN)
int _plugin_message(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    //DBG();
	PluginView * plugin_view = (PluginView *)GetWindowAdditionalData(hwnd);
    if(plugin_view){ 

        //DBG();
        NPEvent npEvent;
        npEvent.hwnd = hwnd;
        npEvent.message = message;
        npEvent.wParam = wParam;
        npEvent.lParam = lParam;
        if (plugin_view->dispatchNPEvent(npEvent))
            return 0;
    }
	return DefaultControlProc (hwnd, message, wParam, lParam);
}

bool PluginView::dispatchNPEvent(NPEvent& event) 
{
    //DBG();
    // sanity check
    if (!m_plugin->pluginFuncs()->event)
        return false;

    PluginView::setCurrentPluginView(this);
    JSC::JSLock::DropAllLocks dropAllLocks(JSC::SilenceAssertionsOnly);
    setCallingPlugin(true);

    bool accepted = m_plugin->pluginFuncs()->event(m_instance, &event);

    setCallingPlugin(false);
    PluginView::setCurrentPluginView(0);
    return accepted;
}

void PluginView::setFocus(bool focused)
{
    ASSERT(platformPluginWidget() == platformWidget());
    Widget::setFocus(focused);
}

void PluginView::show()
{
    LOG(Plugins, "PluginView::show()");
    ASSERT(platformPluginWidget() == platformWidget());
    if (m_parentWindow)
        ShowWindow(m_parentWindow, SW_SHOW);
    Widget::show();
}

void PluginView::hide()
{
    LOG(Plugins, "PluginView::hide()");
    ASSERT(platformPluginWidget() == platformWidget());

    if (m_parentWindow)
        ShowWindow(m_parentWindow, SW_HIDE);
    Widget::hide();
}

void PluginView::paint(GraphicsContext* context, const IntRect& rect)
{
    if (!m_isStarted) {
        paintMissingPluginIcon(context, rect);
        return;
    }

    if (context->paintingDisabled())
        return;

    //setNPWindowIfNeeded();

    if (m_isWindowed)
        return;

}

void PluginView::handleKeyboardEvent(KeyboardEvent*)
{
    JSC::JSLock::DropAllLocks dropAllLocks(JSC::SilenceAssertionsOnly);

    if (m_isWindowed)
        return;
}

void PluginView::handleMouseEvent(MouseEvent*)
{
    JSC::JSLock::DropAllLocks dropAllLocks(JSC::SilenceAssertionsOnly);

    if (m_isWindowed)
        return;
}

void PluginView::setParent(ScrollView* parent)
{
    Widget::setParent(parent);
    if (parent)
        init();
}

void PluginView::setNPWindowRect(const IntRect& rect)
{
    if (!m_isWindowed)
        setNPWindowIfNeeded();
}

#if 0
static bool IsPluginVisable(IntRect &rect)
{
    if( rect.right() == 0 ) return false;
    if( rect.bottom() == 0 ) return false;
    if( rect.x() == rect.right() ) return false;
    if( rect.y() == rect.bottom() ) return false;
    return true;
}
#endif

void PluginView::setNPWindowIfNeeded() 
{
    if (!m_isStarted || !parent() || !m_plugin->pluginFuncs()->setwindow)
        return;

    // If the plugin didn't load sucessfully, no point in calling setwindow
    if (m_status != PluginStatusLoadedSuccessfully)
        return;

    // On Unix, only call plugin's setwindow if it's full-page or windowed
    if (m_mode != NP_FULL && m_mode != NP_EMBED)
        return;

    // Check if the platformPluginWidget still exists
    if (m_isWindowed && !platformPluginWidget())
        return;

    // Check if the platformPluginWidget still exists
    if (m_isWindowed && !platformPluginWidget())
        return;

    if (m_isWindowed) {
        m_npWindow.x = m_windowRect.x();
        m_npWindow.y = m_windowRect.y();
        m_npWindow.width = m_windowRect.width();
        m_npWindow.height = m_windowRect.height();

        m_npWindow.clipRect.left = m_clipRect.x();
        m_npWindow.clipRect.top = m_clipRect.y();
        m_npWindow.clipRect.right = m_clipRect.width();
        m_npWindow.clipRect.bottom = m_clipRect.height();

        //MoveWindow(m_parentwindow, m_windowRect.x(), m_windowRect.y(), m_windowRect.width(), m_windowRect.height(), TRUE);
        //MoveWindow(platformPluginWidget(), m_windowRect.x(), m_windowRect.y(), m_windowRect.width(), m_windowRect.height(), TRUE);

        //printf("[%d, %d, %d, %d %d %d]", m_clipRect.x(), m_clipRect.y() ,m_windowRect.width(), m_windowRect.height(), m_clipRect.width(), m_clipRect.height());
#if 0
        //FIXME: we need used SetWindowRgn to resize the window size , but minigui can't support.
		md_MoveWindow(m_parentWindow ,m_windowRect.x()+m_clipRect.x(), m_windowRect.y()+m_clipRect.y(),
					m_clipRect.width(), m_clipRect.height(), FALSE);
		md_MoveWindow(platformPluginWidget() ,-m_clipRect.x(), -m_clipRect.y() ,m_windowRect.width(),
					m_windowRect.height() ,TRUE);
#else
		MoveWindow(m_parentWindow ,m_windowRect.x()+m_clipRect.x(), m_windowRect.y()+m_clipRect.y(),
					m_clipRect.width(), m_clipRect.height(), FALSE);
		MoveWindow(platformPluginWidget() ,-m_clipRect.x(), -m_clipRect.y() ,m_windowRect.width(),
					m_windowRect.height() ,TRUE);
#endif
        
#if 0
        if (IsPluginVisable(m_clipRect) && !IsWindowVisible(m_parentWindow)) { 
            show();
        }
        else 
            hide();
#endif
        //GtkAllocation allocation = { m_windowRect.x(), m_windowRect.y(), m_windowRect.width(), m_windowRect.height() };
        //gtk_widget_size_allocate(platformPluginWidget(), &allocation);
    }
}

NPError PluginView::handlePostReadFile(Vector<char>&, uint32_t, const char*)
{
    return 0;
}

#if ENABLE(NETSCAPE_PLUGIN_API)
bool PluginView::platformGetValue(NPNVariable, void*, NPError*)
{
    return false;
}

bool PluginView::platformGetValueStatic(NPNVariable, void*, NPError*)
{
    return false;
}
#endif

void PluginView::invalidateRect(NPRect* rect)
{
	if (!rect) {
		invalidate();
		return;
	}

    IntRect r(rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top);
    invalidateWindowlessPluginRect(r);
}

void PluginView::invalidateRect(const IntRect& rect)
{
    if (m_isWindowed) {
        if (platformWidget())
            InvalidateRect(platformWidget(), (RECT *)&rect, false );
        return;
    }
}

void PluginView::invalidateRegion(NPRegion)
{
    invalidate();
}

void PluginView::forceRedraw()
{
	if(platformWidget())
		UpdateWindow(platformWidget(), FALSE);
}

bool PluginView::platformStart()
{

    LOG(Plugins, "PluginView::platformStart()");
    ASSERT(m_isStarted);
    ASSERT(m_status == PluginStatusLoadedSuccessfully);

    if (m_plugin->pluginFuncs()->getvalue) {
        PluginView::setCurrentPluginView(this);
        JSC::JSLock::DropAllLocks dropAllLocks(JSC::SilenceAssertionsOnly);
#if 0
        setCallingPlugin(true);
        m_plugin->pluginFuncs()->getvalue(m_instance, NPPVpluginNeedsXEmbed, &m_needsXEmbed);
        setCallingPlugin(false);
#endif
        PluginView::setCurrentPluginView(0);
    }

    if (m_isWindowed) {
        HTMLPlugInElement* o = static_cast<HTMLPlugInElement*>(m_element);   
        int width = o->pluginWidget()->width();
        int height = o->pluginWidget()->height();
        
        HWND browserWindow = m_parentFrame->page()->chrome()->platformPageClient();

        m_parentWindow = CreateWindowEx( MDOLPHIN_PLUGIN, "pluginParentWindow",
            WS_CHILD, WS_EX_NONE, (int)o,
            0, 0, 0, 0,
            browserWindow, 0); 

        LOG(Plugins, "PluginView::platformStart()");
        //HWND pluginHwnd = CreateWindowEx( MDOLPHIN_PLUGIN, o->name().latin1().data(),
        HWND pluginHwnd = CreateWindowEx( MDOLPHIN_PLUGIN, "pluginWindow",
            WS_CHILD , WS_EX_NONE,(int)m_parentWindow,
            0, 0, width, height,
            m_parentWindow, (DWORD)(this)); 
        setPlatformWidget(pluginHwnd);
        //before handle pluginHwnd was created, function updatePluginWidget() may called, and it will
        //call setNPWindowIfNeeded() to update m_parentWindow and pluginHwnd's window,
        //because of pluginHwnd was not create, the update operaton will fail. enter function 
        //updatePluginWidget() in second time, the functoin setNPWindowIfNeeded() may not be called.
        //so after handle pluginHwnd was create, call function setNPWindowIfNeeded() to force
        //update handle m_parentWindow and pluginHwnd's window.
        setNPWindowIfNeeded();
    }

    show();
    // TODO remove in favor of null events, like mac port?
    if (!(m_plugin->quirks().contains(PluginQuirkDeferFirstSetWindowCall)))
        updatePluginWidget(); // was: setNPWindowIfNeeded(), but this doesn't produce 0x0 rects at first go

    return true;
}

void PluginView::platformDestroy()
{
    LOG(Plugins, "PluginView::platformDestroy()");
    if (platformWidget()) {
        SetWindowAdditionalData(platformWidget(), (DWORD)0);
        DestroyWindow(platformWidget());
    }

    if (m_parentWindow)
        DestroyWindow(m_parentWindow);
    
}

void PluginView::setParentVisible(bool visible)
{
    if (isParentVisible() == visible)
        return;

    Widget::setParentVisible(visible);

    if (isSelfVisible() && platformPluginWidget()) {
        if (visible)
            show();
        else
            hide();
    }
}

void PluginView::updatePluginWidget()
{
    if (!parent())
        return;

    ASSERT(parent()->isFrameView());
    FrameView* frameView = static_cast<FrameView*>(parent());

    IntRect oldWindowRect = m_windowRect;
    IntRect oldClipRect = m_clipRect;

    m_windowRect = IntRect(frameView->contentsToWindow(frameRect().location()), frameRect().size());
    m_clipRect = windowClipRect();
    m_clipRect.move(-m_windowRect.x(), -m_windowRect.y());

    if (m_windowRect == oldWindowRect && m_clipRect == oldClipRect)
        return;

    setNPWindowIfNeeded();
}

void PluginView::halt()
{
}

void PluginView::restart()
{
}

#if 0
#if ENABLE(NETSCAPE_PLUGIN_API)
void PluginView::keepAlive(NPP)
{
}
#endif

#if PLATFORM(MAC) || PLATFORM(CHROMIUM) || PLATFORM(EFL)
void PluginView::privateBrowsingStateChanged(bool)
{
}

void PluginView::setJavaScriptPaused(bool)
{
}
#endif
#endif

#if defined(XP_UNIX) && ENABLE(NETSCAPE_PLUGIN_API)
void PluginView::handleFocusInEvent()
{
}

void PluginView::handleFocusOutEvent()
{
}
#endif

#endif //ENABLE(PLUGIN)

} // namespace WebCore
