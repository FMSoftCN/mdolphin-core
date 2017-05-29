/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "TestShell.h"

#include "LayoutTestController.h"
#include "WebViewHost.h"
#include "base/md5.h" // FIXME: Wrap by webkit_support.
#include "base/string16.h"
#include "gfx/codec/png_codec.h" // FIXME: Remove dependecy. WebCore/platform/image-encoder is better?
#include "net/base/escape.h" // FIXME: Remove dependency.
#include "public/WebDataSource.h"
#include "public/WebDocument.h"
#include "public/WebElement.h"
#include "public/WebFrame.h"
#include "public/WebHistoryItem.h"
#include "public/WebRuntimeFeatures.h"
#include "public/WebScriptController.h"
#include "public/WebSettings.h"
#include "public/WebSize.h"
#include "public/WebString.h"
#include "public/WebURLRequest.h"
#include "public/WebURLResponse.h"
#include "public/WebView.h"
#include "skia/ext/bitmap_platform_device.h"
#include "skia/ext/platform_canvas.h"
#include "webkit/support/webkit_support.h"
#include <algorithm>
#include <cctype>
#include <vector>

using namespace WebKit;
using namespace std;

// Content area size for newly created windows.
static const int testWindowWidth = 800;
static const int testWindowHeight = 600;

// The W3C SVG layout tests use a different size than the other layout tests.
static const int SVGTestWindowWidth = 480;
static const int SVGTestWindowHeight = 360;

static const char layoutTestsPattern[] = "/LayoutTests/";
static const string::size_type layoutTestsPatternSize = sizeof(layoutTestsPattern) - 1;
static const char fileUrlPattern[] = "file:/";
static const char fileTestPrefix[] = "(file test):";
static const char dataUrlPattern[] = "data:";
static const string::size_type dataUrlPatternSize = sizeof(dataUrlPattern) - 1;

TestShell::TestShell()
    : m_testIsPending(false)
    , m_testIsPreparing(false)
    , m_focusedWidget(0)
{
    m_accessibilityController.set(new AccessibilityController(this));
    m_layoutTestController.set(new LayoutTestController(this));
    m_eventSender.set(new EventSender(this));
    m_plainTextController.set(new PlainTextController());
    m_textInputController.set(new TextInputController(this));
    m_notificationPresenter.set(new NotificationPresenter(this));

    m_webViewHost = createWebView();
    m_webView = m_webViewHost->webView();
}

TestShell::~TestShell()
{
    loadURL(GURL("about:blank"));
    // Call GC twice to clean up garbage.
    callJSGC();
    callJSGC();

    // Destroy the WebView before its WebViewHost.
    m_webView->close();
}

void TestShell::resetWebSettings(WebView& webView)
{
    // Match the settings used by Mac DumpRenderTree, with the exception of
    // fonts.
    WebSettings* settings = webView.settings();
#if OS(MAC_OS_X)
    WebString serif = WebString::fromUTF8("Times");
    settings->setCursiveFontFamily(WebString::fromUTF8("Apple Chancery"));
    settings->setFantasyFontFamily(WebString::fromUTF8("Papyrus"));
#else
    // NOTE: case matters here, this must be 'times new roman', else
    // some layout tests fail.
    WebString serif = WebString::fromUTF8("times new roman");

    // These two fonts are picked from the intersection of
    // Win XP font list and Vista font list :
    //   http://www.microsoft.com/typography/fonts/winxp.htm
    //   http://blogs.msdn.com/michkap/archive/2006/04/04/567881.aspx
    // Some of them are installed only with CJK and complex script
    // support enabled on Windows XP and are out of consideration here.
    // (although we enabled both on our buildbots.)
    // They (especially Impact for fantasy) are not typical cursive
    // and fantasy fonts, but it should not matter for layout tests
    // as long as they're available.
    settings->setCursiveFontFamily(WebString::fromUTF8("Comic Sans MS"));
    settings->setFantasyFontFamily(WebString::fromUTF8("Impact"));
#endif
    settings->setSerifFontFamily(serif);
    settings->setStandardFontFamily(serif);
    settings->setFixedFontFamily(WebString::fromUTF8("Courier"));

    settings->setDefaultTextEncodingName(WebString::fromUTF8("ISO-8859-1"));
    settings->setDefaultFontSize(16);
    settings->setDefaultFixedFontSize(13);
    settings->setMinimumFontSize(1);
    settings->setMinimumLogicalFontSize(9);
    settings->setJavaScriptCanOpenWindowsAutomatically(true);
    settings->setJavaScriptCanAccessClipboard(true);
    settings->setDOMPasteAllowed(true);
    settings->setDeveloperExtrasEnabled(false);
    settings->setNeedsSiteSpecificQuirks(true);
    settings->setShrinksStandaloneImagesToFit(false);
    settings->setUsesEncodingDetector(false);
    settings->setTextAreasAreResizable(false);
    settings->setJavaEnabled(false);
    settings->setAllowScriptsToCloseWindows(false);
    settings->setXSSAuditorEnabled(false);
    settings->setDownloadableBinaryFontsEnabled(true);
    settings->setLocalStorageEnabled(true);
    settings->setOfflineWebApplicationCacheEnabled(true);
    settings->setAllowFileAccessFromFileURLs(true);

    // LayoutTests were written with Safari Mac in mind which does not allow
    // tabbing to links by default.
    webView.setTabsToLinks(false);

    // Allow those layout tests running as local files, i.e. under
    // LayoutTests/http/tests/local, to access http server.
    settings->setAllowUniversalAccessFromFileURLs(true);

    settings->setJavaScriptEnabled(true);
    settings->setPluginsEnabled(true);
    settings->setWebSecurityEnabled(true);
    settings->setEditableLinkBehaviorNeverLive();
    settings->setFontRenderingModeNormal();
    settings->setShouldPaintCustomScrollbars(true);
    settings->setTextDirectionSubmenuInclusionBehaviorNeverIncluded();

    settings->setLoadsImagesAutomatically(true);
    settings->setImagesEnabled(true);
}

void TestShell::runFileTest(const TestParams& params)
{
    ASSERT(params.testUrl.isValid());
    m_testIsPreparing = true;
    m_params = params;
    string testUrl = m_params.testUrl.spec();

    bool inspectorTestMode = testUrl.find("/inspector/") != string::npos
        || testUrl.find("\\inspector\\") != string::npos;
    m_webView->settings()->setDeveloperExtrasEnabled(inspectorTestMode);
    loadURL(m_params.testUrl);

    m_testIsPreparing = false;
    waitTestFinished();
}

static inline bool isSVGTestURL(const WebURL& url)
{
    return url.isValid() && string(url.spec()).find("W3C-SVG-1.1") != string::npos;
}

void TestShell::resizeWindowForTest(WebViewHost* window, const WebURL& url)
{
    int width, height;
    if (isSVGTestURL(url)) {
        width = SVGTestWindowWidth;
        height = SVGTestWindowHeight;
    } else {
        width = testWindowWidth;
        height = testWindowHeight;
    }
    window->setWindowRect(WebRect(0, 0, width + virtualWindowBorder * 2, height + virtualWindowBorder * 2));
}

void TestShell::resetTestController()
{
    m_accessibilityController->reset();
    m_layoutTestController->reset();
    m_eventSender->reset();
    m_webViewHost->reset();
    m_notificationPresenter->reset();
}

void TestShell::loadURL(const WebURL& url)
{
    m_webViewHost->loadURLForFrame(url, WebString());
}

void TestShell::reload()
{
    m_webViewHost->navigationController()->reload();
}

void TestShell::goToOffset(int offset)
{
     m_webViewHost->navigationController()->goToOffset(offset);
}

int TestShell::navigationEntryCount() const
{
    return m_webViewHost->navigationController()->entryCount();
}

void TestShell::callJSGC()
{
    m_webView->mainFrame()->collectGarbage();
}

void TestShell::setFocus(WebWidget* widget, bool enable)
{
    // Simulate the effects of InteractiveSetFocus(), which includes calling
    // both setFocus() and setIsActive().
    if (enable) {
        if (m_focusedWidget != widget) {
            if (m_focusedWidget)
                m_focusedWidget->setFocus(false);
            webView()->setIsActive(enable);
            widget->setFocus(enable);
            m_focusedWidget = widget;
        }
    } else {
        if (m_focusedWidget == widget) {
            widget->setFocus(enable);
            webView()->setIsActive(enable);
            m_focusedWidget = 0;
        }
    }
}

void TestShell::testFinished()
{
    if (!m_testIsPending)
        return;
    m_testIsPending = false;
    dump();
    webkit_support::QuitMessageLoop();
}

void TestShell::testTimedOut()
{
    fprintf(stderr, "FAIL: Timed out waiting for notifyDone to be called\n");
    fprintf(stdout, "FAIL: Timed out waiting for notifyDone to be called\n");
    testFinished();
}

static string dumpDocumentText(WebFrame* frame)
{
    // We use the document element's text instead of the body text here because
    // not all documents have a body, such as XML documents.
    WebElement documentElement = frame->document().documentElement();
    if (documentElement.isNull())
        return string();
    return documentElement.innerText().utf8();
}

static string dumpFramesAsText(WebFrame* frame, bool recursive)
{
    string result;

    // Add header for all but the main frame. Skip empty frames.
    if (frame->parent() && !frame->document().documentElement().isNull()) {
        result.append("\n--------\nFrame: '");
        result.append(frame->name().utf8().data());
        result.append("'\n--------\n");
    }

    result.append(dumpDocumentText(frame));
    result.append("\n");

    if (recursive) {
        for (WebFrame* child = frame->firstChild(); child; child = child->nextSibling())
            result.append(dumpFramesAsText(child, recursive));
    }

    return result;
}

static void dumpFrameScrollPosition(WebFrame* frame, bool recursive)
{
    WebSize offset = frame->scrollOffset();
    if (offset.width > 0 || offset.height > 0) {
        if (frame->parent())
            printf("frame '%s' ", frame->name().utf8().data());
        printf("scrolled to %d,%d\n", offset.width, offset.height);
    }

    if (!recursive)
        return;
    for (WebFrame* child = frame->firstChild(); child; child = child->nextSibling())
        dumpFrameScrollPosition(child, recursive);
}

struct ToLower {
    char16 operator()(char16 c) { return tolower(c); }
};

// FIXME: Eliminate std::transform(), std::vector, and std::sort().

// Returns True if item1 < item2.
static bool HistoryItemCompareLess(const WebHistoryItem& item1, const WebHistoryItem& item2)
{
    string16 target1 = item1.target();
    string16 target2 = item2.target();
    std::transform(target1.begin(), target1.end(), target1.begin(), ToLower());
    std::transform(target2.begin(), target2.end(), target2.begin(), ToLower());
    return target1 < target2;
}

static string dumpHistoryItem(const WebHistoryItem& item, int indent, bool isCurrent)
{
    string result;

    if (isCurrent) {
        result.append("curr->");
        result.append(indent - 6, ' '); // 6 == "curr->".length()
    } else {
        result.append(indent, ' ');
    }

    string url = item.urlString().utf8();
    size_t pos;
    if (!url.find(fileUrlPattern) && ((pos = url.find(layoutTestsPattern)) != string::npos)) {
        // adjust file URLs to match upstream results.
        url.replace(0, pos + layoutTestsPatternSize, fileTestPrefix);
    } else if (!url.find(dataUrlPattern)) {
        // URL-escape data URLs to match results upstream.
        string path = EscapePath(url.substr(dataUrlPatternSize));
        url.replace(dataUrlPatternSize, url.length(), path);
    }

    result.append(url);
    if (!item.target().isEmpty()) {
        result.append(" (in frame \"");
        result.append(item.target().utf8());
        result.append("\")");
    }
    if (item.isTargetItem())
        result.append("  **nav target**");
    result.append("\n");

    const WebVector<WebHistoryItem>& children = item.children();
    if (!children.isEmpty()) {
        // Must sort to eliminate arbitrary result ordering which defeats
        // reproducible testing.
        // FIXME: WebVector should probably just be a std::vector!!
        std::vector<WebHistoryItem> sortedChildren;
        for (size_t i = 0; i < children.size(); ++i)
            sortedChildren.push_back(children[i]);
        std::sort(sortedChildren.begin(), sortedChildren.end(), HistoryItemCompareLess);
        for (size_t i = 0; i < sortedChildren.size(); ++i)
            result += dumpHistoryItem(sortedChildren[i], indent + 4, false);
    }

    return result;
}

static void dumpBackForwardList(const TestNavigationController& navigationController, string& result)
{
    result.append("\n============== Back Forward List ==============\n");
    for (int index = 0; index < navigationController.entryCount(); ++index) {
        int currentIndex = navigationController.lastCommittedEntryIndex();
        WebHistoryItem historyItem = navigationController.entryAtIndex(index)->contentState();
        if (historyItem.isNull()) {
            historyItem.initialize();
            historyItem.setURLString(navigationController.entryAtIndex(index)->URL().spec().utf16());
        }
        result.append(dumpHistoryItem(historyItem, 8, index == currentIndex));
    }
    result.append("===============================================\n");
}

string TestShell::dumpAllBackForwardLists()
{
    string result;
    for (unsigned i = 0; i < m_windowList.size(); ++i)
        dumpBackForwardList(*m_windowList[i]->navigationController(), result);
    return result;
}

void TestShell::dump()
{
    WebScriptController::flushConsoleMessages();

    // Dump the requested representation.
    WebFrame* frame = m_webView->mainFrame();
    if (!frame)
        return;
    bool shouldDumpAsText = m_layoutTestController->shouldDumpAsText();
    bool dumpedAnything = false;
    if (m_params.dumpTree) {
        dumpedAnything = true;
        printf("Content-Type: text/plain\n");
        // Text output: the test page can request different types of output
        // which we handle here.
        if (!shouldDumpAsText) {
            // Plain text pages should be dumped as text
            string mimeType = frame->dataSource()->response().mimeType().utf8();
            shouldDumpAsText = mimeType == "text/plain";
        }
        if (shouldDumpAsText) {
            bool recursive = m_layoutTestController->shouldDumpChildFramesAsText();
            string dataUtf8 = dumpFramesAsText(frame, recursive);
            if (fwrite(dataUtf8.c_str(), 1, dataUtf8.size(), stdout) != dataUtf8.size())
                FATAL("Short write to stdout, disk full?\n");
        } else {
            printf("%s", frame->renderTreeAsText().utf8().data());
            bool recursive = m_layoutTestController->shouldDumpChildFrameScrollPositions();
            dumpFrameScrollPosition(frame, recursive);
        }
        if (m_layoutTestController->shouldDumpBackForwardList())
            printf("%s", dumpAllBackForwardLists().c_str());
    }
    if (dumpedAnything && m_params.printSeparators)
        printf("#EOF\n");

    if (m_params.dumpPixels && !shouldDumpAsText) {
        // Image output: we write the image data to the file given on the
        // command line (for the dump pixels argument), and the MD5 sum to
        // stdout.
        dumpedAnything = true;
        m_webView->layout();
        if (m_layoutTestController->testRepaint()) {
            WebSize viewSize = m_webView->size();
            int width = viewSize.width;
            int height = viewSize.height;
            if (m_layoutTestController->sweepHorizontally()) {
                for (WebRect column(0, 0, 1, height); column.x < width; column.x++)
                    m_webViewHost->paintRect(column);
            } else {
                for (WebRect line(0, 0, width, 1); line.y < height; line.y++)
                    m_webViewHost->paintRect(line);
            }
        } else
            m_webViewHost->paintInvalidatedRegion();

        // See if we need to draw the selection bounds rect. Selection bounds
        // rect is the rect enclosing the (possibly transformed) selection.
        // The rect should be drawn after everything is laid out and painted.
        if (m_layoutTestController->shouldDumpSelectionRect()) {
            // If there is a selection rect - draw a red 1px border enclosing rect
            WebRect wr = frame->selectionBoundsRect();
            if (!wr.isEmpty()) {
                // Render a red rectangle bounding selection rect
                SkPaint paint;
                paint.setColor(0xFFFF0000); // Fully opaque red
                paint.setStyle(SkPaint::kStroke_Style);
                paint.setFlags(SkPaint::kAntiAlias_Flag);
                paint.setStrokeWidth(1.0f);
                SkIRect rect; // Bounding rect
                rect.set(wr.x, wr.y, wr.x + wr.width, wr.y + wr.height);
                m_webViewHost->canvas()->drawIRect(rect, paint);
            }
        }

        string md5sum = dumpImage(m_webViewHost->canvas(), m_params.pixelHash);
    }
    printf("#EOF\n"); // For the image.
    fflush(stdout);
    fflush(stderr);
}

string TestShell::dumpImage(skia::PlatformCanvas* canvas, const string& expectedHash)
{
    skia::BitmapPlatformDevice& device =
        static_cast<skia::BitmapPlatformDevice&>(canvas->getTopPlatformDevice());
    const SkBitmap& sourceBitmap = device.accessBitmap(false);

    SkAutoLockPixels sourceBitmapLock(sourceBitmap);

    // Fix the alpha. The expected PNGs on Mac have an alpha channel, so we want
    // to keep it. On Windows, the alpha channel is wrong since text/form control
    // drawing may have erased it in a few places. So on Windows we force it to
    // opaque and also don't write the alpha channel for the reference. Linux
    // doesn't have the wrong alpha like Windows, but we ignore it anyway.
#if OS(WINDOWS)
    bool discardTransparency = true;
    device.makeOpaque(0, 0, sourceBitmap.width(), sourceBitmap.height());
#elif OS(MAC_OS_X)
    bool discardTransparency = false;
#elif OS(UNIX)
    bool discardTransparency = true;
#endif

    // Compute MD5 sum.  We should have done this before calling
    // device.makeOpaque on Windows.  Because we do it after the call, there are
    // some images that are the pixel identical on windows and other platforms
    // but have different MD5 sums.  At this point, rebaselining all the windows
    // tests is too much of a pain, so we just check in different baselines.
    MD5Context ctx;
    MD5Init(&ctx);
    MD5Update(&ctx, sourceBitmap.getPixels(), sourceBitmap.getSize());

    MD5Digest digest;
    MD5Final(&digest, &ctx);
    string md5hash = MD5DigestToBase16(digest);
    printf("\nActualHash: %s\n", md5hash.c_str());
    if (!expectedHash.empty())
        printf("\nExpectedHash: %s\n", expectedHash.c_str());

    // Only encode and dump the png if the hashes don't match. Encoding the image
    // is really expensive.
    if (md5hash.compare(expectedHash)) {
        std::vector<unsigned char> png;
        gfx::PNGCodec::ColorFormat colorFormat = gfx::PNGCodec::FORMAT_BGRA;
        gfx::PNGCodec::Encode(
            reinterpret_cast<const unsigned char*>(sourceBitmap.getPixels()),
            colorFormat, sourceBitmap.width(), sourceBitmap.height(),
            static_cast<int>(sourceBitmap.rowBytes()), discardTransparency, &png);

        printf("Content-Type: image/png\n");
        printf("Content-Length: %lu\n", png.size());
        // Write to disk.
        if (fwrite(&png[0], 1, png.size(), stdout) != png.size())
            FATAL("Short write to stdout.\n");
    }

    return md5hash;
}

void TestShell::bindJSObjectsToWindow(WebFrame* frame)
{
    m_accessibilityController->bindToJavascript(frame, WebString::fromUTF8("accessibilityController"));
    m_layoutTestController->bindToJavascript(frame, WebString::fromUTF8("layoutTestController"));
    m_eventSender->bindToJavascript(frame, WebString::fromUTF8("eventSender"));
    m_plainTextController->bindToJavascript(frame, WebString::fromUTF8("plainText"));
    m_textInputController->bindToJavascript(frame, WebString::fromUTF8("textInputController"));
}

int TestShell::layoutTestTimeout()
{
    return 10 * 1000;
}

WebViewHost* TestShell::createWebView()
{
    return createNewWindow(WebURL());
}

WebViewHost* TestShell::createNewWindow(const WebURL& url)
{
    WebViewHost* host = new WebViewHost(this);
    WebView* view = WebView::create(host);
    host->setWebWidget(view);
    resetWebSettings(*view);
    view->initializeMainFrame(host);
    m_windowList.append(host);
    host->loadURLForFrame(url, WebString());
    return host;
}

void TestShell::closeWindow(WebViewHost* window)
{
    size_t i = m_windowList.find(window);
    if (i == notFound) {
        ASSERT_NOT_REACHED();
        return;
    }
    m_windowList.remove(i);
    window->webWidget()->close();
    delete window;
}

void TestShell::closeRemainingWindows()
{
    // Iterate through the window list and close everything except the main
    // ihwindow. We don't want to delete elements as we're iterating, so we copy
    // to a temp vector first.
    Vector<WebViewHost*> windowsToDelete;
    for (unsigned i = 0; i < m_windowList.size(); ++i) {
        if (m_windowList[i] != webViewHost())
            windowsToDelete.append(m_windowList[i]);
    }
    ASSERT(windowsToDelete.size() + 1 == m_windowList.size());
    for (unsigned i = 0; i < windowsToDelete.size(); ++i)
        closeWindow(windowsToDelete[i]);
    ASSERT(m_windowList.size() == 1);
}

int TestShell::windowCount()
{
    return m_windowList.size();
}
