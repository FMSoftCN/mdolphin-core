
#include "minigui.h"

#include "config.h"
#include "mdolphin.h"
#include "WorkQueueItem.h"

#include "string.h"
#include "DumpRenderTree.h"
#include "IMDWebBackForwardList.h"

// Returns a newly allocated UTF-8 character buffer which must be freed with free()
char* JSStringCopyUTF8CString(JSStringRef jsString)
{
    size_t dataSize = JSStringGetMaximumUTF8CStringSize(jsString);
    char* utf8 = (char*)malloc(dataSize);
    JSStringGetUTF8CString(jsString, utf8, dataSize);

    return utf8;
}

bool LoadItem::invoke() const
{
    char* targetString = JSStringCopyUTF8CString(m_target.get());

    IMDWebFrame* targetFrame;
    if (!strlen(targetString))
        targetFrame = mainFrame;
    else {
        targetFrame = mainFrame->findFrame(targetString);
        if (!targetFrame) {
            free(targetString);
            return false;
        }
    }
    free(targetString);

    char* urlString = JSStringCopyUTF8CString(m_url.get());
    targetFrame->loadURL(urlString);
    free(urlString);

    return true;
}

bool ReloadItem::invoke() const
{
    mainFrame->reload();
    return true;
}

bool ScriptItem::invoke() const
{
    IMDWebView* webView = mainFrame->webView();
    if (!webView)
        return false;
    char* scriptString = JSStringCopyUTF8CString(m_script.get());
    webView->executeScript(scriptString);
    free(scriptString);
    return true;
}

bool BackForwardItem::invoke() const
{
    IMDWebView* webView = mainFrame->webView();
    if (!webView)
        return false;

    if (m_howFar == 1) {
        webView->goForward();
    } else if (m_howFar == -1) {
        webView->goBack();
    }
    else {
        IMDWebBackForwardList* webBackForwardList = webView->mdWebBackForwardList();
        MDHWebHistoryItem item = webBackForwardList->itemAtIndex(m_howFar);
        webBackForwardList->goToItem(item);
    }
    
    return true;
}
