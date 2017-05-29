/*
** $Id: InspectorClientMg.cpp 546 2007-10-24 09:26:33Z jpzhang $
**
** InspectorClientMg.cpp: Inspector Client implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-9-20
*/

#include "minigui.h"

#include "config.h"
#include "mdolphin.h"

#include "InspectorClientMg.h"
#include "Frame.h"
#include "InspectorController.h"
#include "MDWebView.h"
#include "IMDWebUIDelegate.h"
#include "MDWebInspector.h"
#include "Page.h"
#include "NotImplemented.h"

namespace WebCore
{

#if ENABLE(INSPECTOR)
class InspectorFrontendSettingsMg : public InspectorFrontendClientLocal::Settings 
{
private:
    virtual ~InspectorFrontendSettingsMg() { }
    virtual String getProperty(const String&)
    {
        notImplemented();
        return String();
    }

    virtual void setProperty(const String&, const String&)
    {
        notImplemented();
    }
};

InspectorClientMg::InspectorClientMg(MDWebView* webView)
    : m_inspectedWebView(webView)
    , m_frontendPage(0)
    , m_frontendClient(0)
{
}

InspectorClientMg::~InspectorClientMg()
{
    if (m_frontendClient) {
        m_frontendClient->disconnectInspectorClient();
        m_frontendClient = 0;
    }
}

void InspectorClientMg::inspectorDestroyed()
{
    delete this;
}

static WNDPROC old_WebViewProc = NULL;
static int InspectorWndProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    MDWebView* view = reinterpret_cast<MDWebView*>(GetWindowAdditionalData2(hwnd));
    switch (message){
        case MSG_CLOSE:
            DestroyWindow(hwnd);
            break;
        case MSG_DESTROY:
            if (view)
            {
                InspectorFrontendClientMg* inspectorFrontendClient = view->mdWebInspector()->getInspectorFrontendClient();
                if (inspectorFrontendClient)
                {
                    inspectorFrontendClient->destroyInspectorWindow(true);
                }
            }
            break;
        case MSG_RBUTTONDOWN:
        case MSG_RBUTTONUP:
            return TRUE;
    }           
    return (*old_WebViewProc)(hwnd, message, wParam, lParam);
}

void InspectorClientMg::openInspectorFrontend(InspectorController* controller)
{
    HWND inspectedWebHwnd = m_inspectedWebView->viewWindow();
    RECT rect;
    GetWindowRect(inspectedWebHwnd, &rect);
    MoveWindow(inspectedWebHwnd, 0, 0, RECTW(rect), RECTH(rect)/2, TRUE);
    const WINDOWINFO* wndInfo = GetWindowInfo(inspectedWebHwnd);

    HWND newHwnd  = CreateWindowEx (MDOLPHIN_CTRL, 
            "", 
            WS_VISIBLE | WS_CAPTION,
            WS_EX_NOTDRAGGABLE,
            wndInfo->id, 0, RECTH(rect)/2, RECTW(rect), RECTH(rect)/2, 
            GetParent(inspectedWebHwnd), 
            0); 

    old_WebViewProc = SetWindowCallbackProc(newHwnd, InspectorWndProc);
    MDWebView* inspectorWebView= (MDWebView *)GetWindowAdditionalData2(newHwnd);

    WTF::String ainspectorPath = "file://" + inspectorFilesPath() + "inspector.html";
    inspectorWebView->loadURL(ainspectorPath.utf8().data());
    m_inspectedWebView->reload();

    MDWebInspector* webInspector = inspectorWebView->mdWebInspector();
    
    m_frontendPage = core(inspectorWebView);
    m_frontendClient = new InspectorFrontendClientMg(m_inspectedWebView, inspectorWebView, webInspector, m_frontendPage, this);
    m_frontendPage->inspectorController()->setInspectorFrontendClient(m_frontendClient);
    webInspector->setInspectorFrontendClient(m_frontendClient);

    m_inspectedWebView->mdWebInspector()->setInspectorWebView(inspectorWebView);

    // The inspector must be in it's own PageGroup to avoid deadlock while debugging.
    m_frontendPage->setGroupName("");
}

void InspectorClientMg::releaseFrontendPage()
{
    m_frontendPage = 0;
}

void InspectorClientMg::restoreInspectedWebView()
{
    HWND inspectedWebHwnd = m_inspectedWebView->viewWindow();
    RECT rect;
    GetWindowRect(GetParent(inspectedWebHwnd), &rect);
    MoveWindow(inspectedWebHwnd, 0, 0, RECTW(rect), RECTH(rect), TRUE);
}

void InspectorClientMg::highlight(Node* node)
{
    InvalidateRect (m_inspectedWebView->viewWindow(), NULL, TRUE);
}

void InspectorClientMg::hideHighlight()
{
    InvalidateRect (m_inspectedWebView->viewWindow(), NULL, TRUE);
}

bool InspectorClientMg::sendMessageToFrontend(const String& message)
{
    return doDispatchMessageOnFrontendPage(m_frontendPage, message);
}

const WTF::String& InspectorClientMg::inspectorFilesPath()
{
    if (!m_inspectorFilesPath.isNull())
    {
        return m_inspectorFilesPath;
    }

    const char* inspectorPath= getenv("WEBKIT_INSPECTOR_PATH");
    m_inspectorFilesPath = inspectorPath ? inspectorPath : "/usr/local/share/minigui/webinspector/";
    if (m_inspectorFilesPath[m_inspectorFilesPath.length() -1] != '/')
    {
        m_inspectorFilesPath += "/";
    }
    return m_inspectorFilesPath;
}

InspectorFrontendClientMg::InspectorFrontendClientMg(MDWebView* inspectedWebView, MDWebView* inspectorWebView, 
        MDWebInspector* webInspector, Page* inspectorPage, InspectorClientMg* inspectorClient)
    : InspectorFrontendClientLocal(core(inspectedWebView)->inspectorController(), inspectorPage, new InspectorFrontendSettingsMg())
    , m_inspectorWebView(inspectorWebView)
    , m_inspectedWebView(inspectedWebView)
    , m_webInspector(webInspector)
    , m_inspectorClient(inspectorClient)
{
}

InspectorFrontendClientMg::~InspectorFrontendClientMg()
{
    if (m_inspectorClient) {
        m_inspectorClient->disconnectFrontendClient();
        m_inspectorClient = 0;
    }
    ASSERT(!m_webInspector);
}

void InspectorFrontendClientMg::destroyInspectorWindow(bool notifyInspectorController)
{
    if (!m_webInspector)
        return;
    m_webInspector->setInspectorFrontendClient(NULL);
    m_webInspector = 0;

    m_inspectorWebView = 0;

    if (notifyInspectorController)
        core(m_inspectedWebView)->inspectorController()->disconnectFrontend();

    m_inspectedWebView->mdWebInspector()->setInspectorWebView(NULL);

    if (m_inspectorClient)
    {
        m_inspectorClient->restoreInspectedWebView();
        m_inspectorClient->releaseFrontendPage();
    }
}

String InspectorFrontendClientMg::localizedStringsURL()
{
    // FIXME: support l10n of localizedStrings.js
    WTF::String localizedURL = "file://" + m_inspectorClient->inspectorFilesPath() + "localizedStrings.js";

    return localizedURL;
}

String InspectorFrontendClientMg::hiddenPanels()
{
    notImplemented();
    return String();
}

void InspectorFrontendClientMg::bringToFront()
{
    // about z order 
    notImplemented();
}

void InspectorFrontendClientMg::closeWindow()
{
    destroyInspectorWindow(true);
}

void InspectorFrontendClientMg::disconnectFromBackend()
{
    destroyInspectorWindow(false);
}

void InspectorFrontendClientMg::attachWindow()
{
    notImplemented();
}

void InspectorFrontendClientMg::detachWindow()
{
    notImplemented();
}

void InspectorFrontendClientMg::setAttachedWindowHeight(unsigned height)
{
    notImplemented();
}

void InspectorFrontendClientMg::inspectedURLChanged(const String& newURL)
{
    if (!m_inspectorWebView)
        return;
    notImplemented();
}
#endif

}

