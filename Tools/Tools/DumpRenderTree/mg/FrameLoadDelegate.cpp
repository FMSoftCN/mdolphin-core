#include "minigui.h"

#include "config.h"
#include "mdolphin.h"
#include "FrameLoadDelegate.h"

#include "AccessibilityController.h"
#include "DumpRenderTree.h"
#include "EventSender.h"
#include "GCController.h"
#include "LayoutTestController.h"
#include "WorkQueueItem.h"
#include "WorkQueue.h"
#include <wtf/Vector.h>
#include <stdio.h>
#include <string>

using std::string;

static FrameLoadDelegate* g_delegateWaitingOnTimer;
#if 0
void FrameLoadDelegate::processWork()
{
    // if another load started, then wait for it to complete.
    if (topLoadingFrame)
        return;

    // if we finish all the commands, we're ready to dump state
    if (WorkQueue::shared()->processWork() && !::gLayoutTestController->waitToDump())
        dump();
}
#endif

FrameLoadDelegate::FrameLoadDelegate()
    : m_gcController(new GCController)
{

}

static BOOL processWorkTimer(HWND hWnd, int id, DWORD)
{
    KillTimer(hWnd, id);
    g_delegateWaitingOnTimer = 0;

    // if another load started, then wait for it to complete.
    if (topLoadingFrame)
        return TRUE;

    // if we finish all the commands, we're ready to dump state
    if (WorkQueue::shared()->processWork() && !::gLayoutTestController->waitToDump()) {
        dump();
    }

    return TRUE;
}

string descriptionSuitableForTestResult(IMDWebFrame* webFrame)
{
    IMDWebView* webView = webFrame->webView();
    if (!webView)
        return string();

    IMDWebFrame* mainFrame = webView->mainWebFrame();
    if (!mainFrame)
        return string();

    const char* name = webFrame->name();

    if (!name || name[0] == '\0') {
        return (webFrame == mainFrame) ? "main frame" : string();
    }

    string frameName = (webFrame == mainFrame) ? "main frame" : "frame";
    frameName += " \"" + string(name) + "\""; 

    return frameName;
}

void FrameLoadDelegate::didFinishLoadForFrame(IMDWebView* webView, IMDWebFrame* webFrame)
{
    if (!done && gLayoutTestController->dumpFrameLoadCallbacks())
        printf("%s - didFinishLoadForFrame\n", 
                descriptionSuitableForTestResult(webFrame).c_str());

    if (webFrame != topLoadingFrame)
        return;

    topLoadingFrame = 0;
    WorkQueue::shared()->setFrozen(true);

    if (gLayoutTestController->waitToDump())
        return;

    if (WorkQueue::shared()->count()) {
        ASSERT(!g_delegateWaitingOnTimer);
        g_delegateWaitingOnTimer = this;
        SetTimerEx(webView->viewWindow(), 101, 0, processWorkTimer);
    }
    else 
        dump();
}
void FrameLoadDelegate::didFailLoad(IMDWebView* webView, IMDWebFrame* webFrame,
        int code, const char* url)
{
    if (!done && gLayoutTestController->dumpFrameLoadCallbacks())
        printf("%s - didFailLoad\n", 
                descriptionSuitableForTestResult(webFrame).c_str());

    if (webFrame != topLoadingFrame)
        return;

    topLoadingFrame = 0;
    WorkQueue::shared()->setFrozen(true);

    if (gLayoutTestController->waitToDump())
        return;

    if (WorkQueue::shared()->count()) {
        ASSERT(!g_delegateWaitingOnTimer);
        g_delegateWaitingOnTimer = this;
        SetTimerEx(webView->viewWindow(), 101, 0, processWorkTimer);
    }
    else 
        dump();
}

void FrameLoadDelegate::didStartProvisionalLoadForFrame(IMDWebView* webView, 
        IMDWebFrame* webFrame)
{
    if (!done && gLayoutTestController->dumpFrameLoadCallbacks())
        printf("%s - didStartProvisionalLoadForFrame\n", 
                descriptionSuitableForTestResult(webFrame).c_str());
    // Make sure we only set this once per test.  If it gets cleared, and then set again, we might
    // end up doing two dumps for one test.
    if (!topLoadingFrame && !done)
        topLoadingFrame = webFrame;
}

void FrameLoadDelegate::didCommitLoadForFrame(IMDWebView* webView, 
        IMDWebFrame* webFrame)
{
    if (!done && gLayoutTestController->dumpFrameLoadCallbacks()) {
        printf("%s - didCommitLoadForFrame\n", 
            descriptionSuitableForTestResult(webFrame).c_str());
    }
}
void FrameLoadDelegate::didReceiveTitle(IMDWebView* webView,
    IMDWebFrame* webFrame, const char* title)
{
    if (!done && gLayoutTestController->dumpFrameLoadCallbacks())
        printf("%s - didReceiveTitle: %s\n", 
                descriptionSuitableForTestResult(webFrame).c_str(), title);

    if (::gLayoutTestController->dumpTitleChanges() && !done)
        printf("TITLE CHANGED: %s\n", title ? title : "");
}

void FrameLoadDelegate::didFinishDocumentLoad(IMDWebView* webView, 
        IMDWebFrame* webFrame)
{
    if (!done && gLayoutTestController->dumpFrameLoadCallbacks())
        printf("%s - didFinishDocumentLoadForFrame\n",
                descriptionSuitableForTestResult(webFrame).c_str());

    if (!done) {
        int pendingFrameUnloadEvents = 0;//webFrame->pendingFrameUnloadEventCount();
        if (pendingFrameUnloadEvents)
            printf("%s - has %u onunload handler(s)\n",
                descriptionSuitableForTestResult(webFrame).c_str(), pendingFrameUnloadEvents);
    }
}

void FrameLoadDelegate::didHandleOnloadEventsForFrame(IMDWebView* webView, 
        IMDWebFrame* webFrame)
{
    if (!done && gLayoutTestController->dumpFrameLoadCallbacks())
        printf("%s - didHandleOnloadEventsForFrame\n",
                descriptionSuitableForTestResult(webFrame).c_str());
}

BOOL FrameLoadDelegate::didClearWindowObject(IMDWebView* webView,
        IMDWebFrame* webFrame, JSContextRef context,
        JSObjectRef windowObject)
{
    JSValueRef exception = 0;
    ASSERT(gLayoutTestController);

    IMDWebFrame* parentFrame = mainFrame->parentFrame();
    gLayoutTestController->makeWindowObject(context, windowObject, &exception);
    ASSERT(!exception);

    m_gcController->makeWindowObject(context, windowObject, &exception);
    ASSERT(!exception);

    JSStringRef eventSenderStr = JSStringCreateWithUTF8CString("eventSender");
    JSValueRef eventSender = makeEventSender(context, !parentFrame);
    JSObjectSetProperty(context, windowObject, eventSenderStr, eventSender, 
            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete, 0);
    JSStringRelease(eventSenderStr);

    return TRUE;
}

