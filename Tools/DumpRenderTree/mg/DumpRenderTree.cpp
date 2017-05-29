
#include "minigui.h"

#include "config.h"
#include "mdolphin.h"
#include "MDWebFrame.h"

#include "IMDWebBackForwardList.h"
#include "StringHash.h"
#include "CString.h"

#include "DumpRenderTree.h"

#include "AccessibilityController.h"
#include "GCController.h"
#include "LayoutTestController.h"
#include "PixelDumpSupport.h"

#include "UIDelegate.h"
#include "FrameLoadDelegate.h"

#include "WorkQueue.h"
#include "WorkQueueItem.h"

#include "CString.h"

#include <JavaScriptCore/JavaScript.h>
#include <wtf/Assertions.h>
#include <wtf/Vector.h>

#include <cassert>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <string>
/*==================================================*/
//for delegate 

/*==================================================*/

using namespace std;

volatile bool done;
static bool dumpTree = 1;
static bool dumpPixels;
static bool dumpAllPixels;
static bool printSeparators;
static bool forceComplexText = false;
static bool printSupportedFeatures = false;

//AccessibilityController* axController = 0;
LayoutTestController* gLayoutTestController = 0;
IMDWebFrame* mainFrame = 0;
// This is the topmost frame that is loading, during a given load, or nil when no load is 
// in progress.  Usually this is the same as the main frame, but not always.  In the case
// where a frameset is loaded, and then new content is loaded into one of the child frames,
// that child frame is the "topmost frame that is loading".
IMDWebFrame* topLoadingFrame = 0;
HWND webViewWindow; 

UIDelegate* sharedUIDelegate;
FrameLoadDelegate* sharedFrameLoadDelegate;

// current b/f item at the end of the previous test
static MDHWebHistoryItem prevTestBFItem; 

Vector<HWND>& openWindows()
{
    static Vector<HWND> vector;
    return vector;
}

static void dumpBackForwardList(IMDWebView* mdWebView)
{
    ASSERT(mdWebView);

    printf("\n============== Back Forward List ==============\n");
    IMDWebBackForwardList* bfList = mdWebView->mdWebBackForwardList();
    if (!bfList)
        return;

    // Print out all items in the list after prevTestBFItem, which was from the previous test
    // Gather items from the end of the list, the print them out from oldest to newest

    //TODO:
    //
    printf("===============================================\n");
}

static void dumpBackForwardListForAllWindows()
{
    unsigned count = openWindows().size();
    for (unsigned i = 0; i < count; i++) {
        HWND window = openWindows()[i];
        IMDWebView* mdWebView = reinterpret_cast<IMDWebView*>(GetWindowAdditionalData2(window));
        dumpBackForwardList(mdWebView);
    }
}

static char* autocorrectURL(const char* url)
{
    if (strncmp("http://", url, 7) != 0 && strncmp("https://", url, 8) != 0) {
        string tmpUrl("file://");
        tmpUrl.append(url);
        return strdup(tmpUrl.c_str());
    }
    return strdup(url);
}

unsigned int waitToDumpWatchdog = 0;
static void invalidateAnyPreviousWaitToDumpWatchdog()
{
    KillTimer(HWND_DESKTOP, waitToDumpWatchdog);
}

void dumpFrameScrollPosition(IMDWebFrame* frame)
{

}

static string dumpFramesAsText(IMDWebFrame* frame)
{
    if (!mainFrame)
        return "";

    string result;
    bool isMainFrame = (frame->parentFrame() == mainFrame);
    char* innerText = frame->innerText();

    if (!isMainFrame) {
        result.append(innerText);
        result.append("\n");
    }
    else {
        const char* frameName = frame->name();
        result.append("\n--------\nFrame: '");
        result.append(frameName);
        result.append("'\n--------\n");
        result.append(innerText);
        result.append("\n");
    }
    free(innerText);
    if (gLayoutTestController->dumpChildFramesAsText()) {
        std::list<IMDWebFrame*> children;
        if (frame->childFrames(children)) {
            std::list<IMDWebFrame*>::iterator it;

            for (it = children.begin(); it != children.end(); it++) {
                result.append(dumpFramesAsText(*it));
            }
        }
    }

    return result;
}

void dump()
{
    invalidateAnyPreviousWaitToDumpWatchdog();
    bool dumpAsText = gLayoutTestController->dumpAsText();

    if (dumpTree) {
        string result;
        //get response mime type (text/plain)
        char* responseType = mainFrame->responseMIMEType();
        dumpAsText = strcmp(responseType, "text/plain");
        free(responseType);

        if (dumpAsText)
            gLayoutTestController->setDumpAsText(dumpAsText);

        if (gLayoutTestController->dumpAsText()) {
            InvalidateRect(webViewWindow, 0, TRUE);
            SendMessage(webViewWindow, MSG_PAINT, 0, 0);
            result = dumpFramesAsText(mainFrame);
        } 
        else {
            char* renderTree = mainFrame->renderTreeAsExternalRepresentation();
            result = renderTree;
            free(renderTree);
        }
        
        if (result.length() == 0) {
            const char* errorMessage;
            if (gLayoutTestController->dumpAsText())
                errorMessage = "[documentElement innerText]";
            else if (gLayoutTestController->dumpDOMAsWebArchive())
                errorMessage = "[[mainFrame DOMDocument] webArchive]";
            else if (gLayoutTestController->dumpSourceAsWebArchive())
                errorMessage = "[[mainFrame dataSource] webArchive]";
            else
                errorMessage = "[mainFrame renderTreeAsExternalRepresentation]";
            printf("ERROR: nil result from %s", errorMessage);
        } else {
            printf("%s", result.c_str());
            if (!gLayoutTestController->dumpAsText() && !gLayoutTestController->dumpDOMAsWebArchive() && !gLayoutTestController->dumpSourceAsWebArchive())
                dumpFrameScrollPosition(mainFrame);

            if (gLayoutTestController->dumpBackForwardList())
                dumpBackForwardListForAllWindows();
        }
        if (printSeparators) {
            puts("#EOF"); // terminate the content block
            fputs("#EOF\n", stderr);
            fflush(stdout);
            fflush(stderr);
        }
    }

    if (dumpPixels) {
        if (!gLayoutTestController->dumpAsText() && !gLayoutTestController->dumpDOMAsWebArchive() && !gLayoutTestController->dumpSourceAsWebArchive()) {
            // FIXME: Add support for dumping pixels
            //dumpWebViewAsPixelsAndCompareWithExpected(gLayoutTestController->expectedPixelHash());
        }
    }

    printf("#EOF\n");   // terminate the (possibly empty) pixels block
    fflush(stdout);

//fail:
    // This will exit from our message loop.
    PostQuitMessage(GetParent(webViewWindow));
    done = true;
}

static bool shouldLogFrameLoadDelegates(const char* pathOrURL)
{
    return strstr(pathOrURL, "/loading/") || strstr(pathOrURL, "\\loading\\");
}
#if 0
static bool shouldLogHistoryDelegates(const char* pathOrURL)
{
    return strstr(pathOrURL, "/globalhistory/") || strstr(pathOrURL, "\\globalhistory\\");
}
#endif

static bool shouldOpenWebInspector(const char* pathOrURL)
{
    return strstr(pathOrURL, "/inspector/") || strstr(pathOrURL, "\\inspector\\");
}

static bool shouldEnableDeveloperExtras(const char* pathOrURL)
{
    return shouldOpenWebInspector(pathOrURL) || strstr(pathOrURL, "/inspector-enabled/") || strstr(pathOrURL, "\\inspector-enabled\\");
}

static void resetDefaultsToConsistentValues(HWND hwnd)
{
    mdSetStringProperty(hwnd, MD_STR_STANDARD_FONT, "Times New Roman");
    mdSetStringProperty(hwnd, MD_STR_SERIF_FONT, "Times New Roman");
    mdSetStringProperty(hwnd, MD_STR_FIXED_FONT, "Courier New");
    mdSetStringProperty(hwnd, MD_STR_SANS_SERIF_FONT, "Arial");

    mdSetIntProperty(hwnd, MD_INT_AUTO_LOAD_IMAGES, 1);
    mdSetIntProperty(hwnd, MD_INT_ENABLE_JAVASCRIPT, 1);
    mdSetIntProperty(hwnd, MD_INT_MEDIUM_FONT_SIZE, 16);
    mdSetIntProperty(hwnd, MD_INT_MEDIUM_FIXED_FONT_SIZE, 13);
    mdSetIntProperty(hwnd, MD_INT_ENABLE_JS_POPUP_WINDOW, 1);

    //TODO: other reset setting
    setlocale(LC_ALL, "");

}

static void resetWebViewToConsistentStateBeforeTesting()
{
    IMDWebView* mdWebView = mainFrame->webView();
    if (mdWebView) 
        return;

    HWND viewWindow = mdWebView->viewWindow();

    mdWebView->setPolicyDelegate(0);
    resetDefaultsToConsistentValues(viewWindow);
    SetFocus(viewWindow);
}

static void runTest(const string& testPathOrURL)
{
    ASSERT(!testPathOrURL.empty());

    // Look for "'" as a separator between the path or URL, and the pixel dump hash that follows.
    string pathOrURL(testPathOrURL);
    string expectedPixelHash;

    size_t separatorPos = pathOrURL.find("'");
    if (separatorPos != string::npos) {
        pathOrURL = string(testPathOrURL, 0, separatorPos);
        expectedPixelHash = string(testPathOrURL, separatorPos + 1);
    }

    char* url = autocorrectURL(pathOrURL.c_str());
    const string testURL(url);

    gLayoutTestController = new LayoutTestController(testURL, expectedPixelHash);
    topLoadingFrame = 0;
    done = false;

    gLayoutTestController->setIconDatabaseEnabled(false);

    if (shouldLogFrameLoadDelegates(pathOrURL.c_str()))
        gLayoutTestController->setDumpFrameLoadCallbacks(true);

    IMDWebView* webView = mainFrame->webView();
    resetWebViewToConsistentStateBeforeTesting();

    if (shouldEnableDeveloperExtras(pathOrURL.c_str())) {
        gLayoutTestController->setDeveloperExtrasEnabled(true);
        if (shouldOpenWebInspector(pathOrURL.c_str()))
            gLayoutTestController->showWebInspector();
    }

    prevTestBFItem = 0;
    if (webView) {
        IMDWebBackForwardList* bfList = webView->mdWebBackForwardList();
        if (bfList)
            prevTestBFItem = bfList->currentItem();
    }

    WorkQueue::shared()->clear();
    WorkQueue::shared()->setFrozen(false);

    HWND hMainWindow = GetParent(webView->viewWindow());
    mainFrame->loadURL(testURL.c_str());

    free(url);
    url = NULL;

    MSG msg;
    while (GetMessage(&msg, hMainWindow)) {
        if (msg.message == MSG_MOUSEMOVEIN && !(msg.wParam)) {
            continue;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (shouldEnableDeveloperExtras(pathOrURL.c_str()))
        gLayoutTestController->closeWebInspector();

    resetWebViewToConsistentStateBeforeTesting();
    mainFrame->stopLoading();

    if (::gLayoutTestController->closeRemainingWindowsWhenComplete()) {
        Vector<HWND> windows = openWindows();
        unsigned size = windows.size();
        for (unsigned i = 0; i < size; i++) {
            HWND window = windows[i];

            // Don't try to close the main window
            if (window == hMainWindow)
                continue;

            DestroyMainWindow(window);
        }
    }

//exit:
    gLayoutTestController->deref();
    gLayoutTestController = 0;

    return;
}

static inline void removeWindow(HWND hWnd)
{
    for (int i = openWindows().size() - 1; i >= 0; --i) {
        if (openWindows()[i] == hWnd) {
            openWindows().remove(i);
            break;
        }
    }
}

static int DefaultDumpRenderTreeWndProc(HWND hWnd, 
        int msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case MSG_DESTROY:
            removeWindow(hWnd);
            break;
    }
    return DefaultWindowProc(hWnd, msg, wParam, lParam);
}

IMDWebView* createWebViewAndOffscreenWindow(HWND* mdWebViewWindow)
{
    unsigned maxViewWidth = LayoutTestController::maxViewWidth;
    unsigned maxViewHeight = LayoutTestController::maxViewHeight;
    RECT rc;

    HWND hMainWindow;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = WS_VISIBLE | WS_CAPTION | WS_BORDER;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "DumpRenderTree";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(IDC_ARROW);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DefaultDumpRenderTreeWndProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = maxViewWidth + 20;
    CreateInfo.by = maxViewHeight + 40;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWindow = CreateMainWindow (&CreateInfo);
    if (hMainWindow == HWND_INVALID)
        return NULL;

    SetRect(&rc, 0, 0, maxViewWidth, maxViewHeight);
    IMDWebView *view = CreateWebViewInstance(rc, 100, hMainWindow);

    if (!view) {
        MainWindowThreadCleanup (hMainWindow);
        return NULL;
    }

    //update window procedure
    HWND hwnd = view->viewWindow();
    if (mdWebViewWindow)
        *mdWebViewWindow = hwnd;

    view->setUIDelegate(sharedUIDelegate);
    view->setFrameLoadDelegate(sharedFrameLoadDelegate);

    //show main window
    ShowWindow(hMainWindow, SW_SHOWNORMAL);

    openWindows().append(hMainWindow);
    return view;
}

int MiniGUIMain(int argc, const char* argv[])
{
    Vector<const char*> tests;

    if (argc <= 1)
        return -1;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "DumpRenderTree" , 0 , 0);
#endif

    for (int i = 1; i < argc; ++i) {
        if (!strcasecmp(argv[i], "--dump-all-pixels")) {
            dumpAllPixels = true;
            continue;
        }

        if (!strcasecmp(argv[i], "--pixel-tests")) {
            dumpPixels = true;
            continue;
        }

        if (!strcasecmp(argv[i], "--complex-text")) {
            forceComplexText = true;
            continue;
        }

        if (!strcasecmp(argv[i], "--print-supported-features")) {
            printSupportedFeatures = true;
            continue;
        }

        tests.append(argv[i]);
    }

    sharedFrameLoadDelegate = new FrameLoadDelegate();
    sharedUIDelegate = new UIDelegate();
    IMDWebView *view = createWebViewAndOffscreenWindow(&webViewWindow);
    if (!view) {
        return -1;
    }

    mainFrame = view->mainWebFrame();

    if (tests.size() == 1 && !strcmp(tests[0], "-")) {
        char filenameBuffer[2048];
        printSeparators = true;
        while (fgets(filenameBuffer, sizeof(filenameBuffer), stdin)) {
            char* newLineCharacter = strchr(filenameBuffer, '\n');
            if (newLineCharacter)
                *newLineCharacter = '\0';

            if (strlen(filenameBuffer) == 0)
                continue;

            runTest(filenameBuffer);
        }
    } else {
        printSeparators = tests.size() > 1;
        for (unsigned int i = 0; i < tests.size(); i++)
            runTest(tests[i]);
    }

    delete sharedUIDelegate;
    delete sharedFrameLoadDelegate;

    return -1;
}

