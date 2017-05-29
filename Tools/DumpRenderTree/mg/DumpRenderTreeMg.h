
#ifndef DumpRenderTreeMg_h
#define DumpRenderTreeMg_h

#include "minigui.h"

#include "IMDWebView.h"
#include "IMDWebFrame.h"

#include <JavaScriptCore/JSBase.h>

extern IMDWebFrame* mainFrame;
extern IMDWebFrame* topLoadingFrame;
extern unsigned int waitToDumpWatchdog;
extern bool waitForPolicy;
extern HWND webViewWindow;

IMDWebView* createWebViewAndOffscreenWindow(HWND* webViewWindow = 0);
char* JSStringCopyUTF8CString(JSStringRef jsString);

#endif // DumpRenderTreeMg_h
