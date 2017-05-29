/*
** $Id: MDolphinFunctions.cpp 1505 2008-11-11 05:20:38Z jpzhang $
**
** MDolphin.cpp: mDolphin control API file.
**
** Copyright (C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date:2007-10-25
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "minigui.h"

#include "config.h"
#include "mdolphin.h"
#include "MDWebView.h"
#include "MDWebFrame.h"

#include "MDolphinEncoding.h"
#include "MDolphinDelegates.h"
#include "SchemeExtension.h"
#include "MDWebSettings.h"
#include "ScrollbarThemeMg.h"
#include "Language.h"
#include "MDCommonFunc.h"
#include "MemoryCache.h"
#include "ProxyMg.h"

#include "CookieJar.h"

#if ENABLE(FILESYSTEM)
#include <sys/types.h>
#include <sys/stat.h>
#include "FileSystem.h"
#endif

#if ENABLE(SSL)
#include "CertificateMg.h"
#endif

using namespace Control;
using namespace WebCore;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    BOOL mdNavigate(HWND hWnd, MDENavigate op, const char* url)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        HWND hMainWnd;
        MSG msg;
        if (!webView)
            return FALSE;

        hMainWnd = GetMainWindowHandle(hWnd);
        //throw all previous char message.
        while(PeekMessage(&msg, hMainWnd, MSG_CHAR, MSG_CHAR, PM_REMOVE)) { }

        switch (op) {
        case MD_NAV_BACKWARD:
            webView->goBack();
            return TRUE;
        case MD_NAV_FORWARD:
            webView->goForward();
            return TRUE;
        case MD_NAV_GOTO: {
            if (!webView->loadURL(url))
                return FALSE;
            else
                return TRUE;
        }
        case MD_NAV_RELOAD:
            webView->reload();
            return TRUE;
        case MD_NAV_STOP:
            webView->stopLoading();
            return TRUE;
        }

        //set the focus in the navigate hwnd,so we can input without moving mouse and clicking the mouse
        SetFocus(hWnd);

        return FALSE;
    }

    MDERenderingMode mdGetRenderingMode(HWND hwnd)
    {
#if 0
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hwnd);
        if (!webView || !mode)
            return ;
#if 0
        switch (webView->renderMode()) {
        case Render_Normal:
            *mode = MD_SCREENVIEW_MODE;
            break;
        case Render_SmartFit:
            *mode = MD_SMALLVIEW_MODE;
            break;
        case Render_GlobalView:
            *mode = MD_VIRTUALVIEW_MODE;
            break;
        }
#else
        *mode = MD_SCREENVIEW_MODE;
#endif
        return TRUE;
#endif
        return MD_RDRMODE_NORMAL;
    }

    BOOL mdSetRenderingMode(HWND hwnd, MDERenderingMode mode, int limit)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hwnd);
        if (!webView)
            return FALSE;
#if 0
        switch(mode) {
        case MD_SCREENVIEW_MODE:
            webView->setRenderMode(Render_Normal, 0);
            break;
        case MD_SMALLVIEW_MODE:
            webView->setRenderMode(Render_SmartFit, (void*)limit);
            break;
        case MD_VIRTUALVIEW_MODE:
            webView->setRenderMode(Render_GlobalView, (void*)limit);
            break;
        }
#endif
        return TRUE;
    }

    int mdGetTextSizeMultiplier(HWND hwnd)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hwnd);
        if (!webView )
            return -1;

        return webView->textSizeMultiplier()*100;
    }

    BOOL mdSetTextSizeMultiplier(HWND hwnd, int factor)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hwnd);
        if (!webView)
            return FALSE;

        webView->setTextSizeMultiplier(factor/100.);
        return TRUE;
    }

    BOOL mdSetCallbackInfo(HWND hWnd, const MDCBInfo *cb)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView || !cb)
            return FALSE;

        set_global_callbacks(cb);
        return TRUE;
    }

    BOOL mdSetTextEncodingName(HWND hWnd, MDEEncodingName name)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return FALSE;
        if ((((int)name) <= MD_CHARSET_UNKNOWN) || (((int)name) > EncodingTableSize))
            return FALSE;

        if (name == MD_CHARSET_AUTODETECT)
            webView->setCustomTextEncodingName("");
        else
            webView->setCustomTextEncodingName(EncodingTable[name]);

        return TRUE;
    }

    MDEEncodingName mdGetTextEncodingName(HWND hWnd)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return (MDEEncodingName)-1;

        char*p =  webView->customTextEncodingName();
        if(!p)
            return (MDEEncodingName)-1;

        int i = 0;
        for (; i < EncodingTableSize; i++) {
            if (strcmp(EncodingTable[i], p) == 0) {
                freeUtf8Char(p);
                return MDEEncodingName(i);
            }
        }

        return (MDEEncodingName)-1;

    }

    void mdGetStringProperty(HWND hWnd, MDEStringProp property, char* buffer, int bufferLen)
    {
        if (!hWnd || (HWND_INVALID == hWnd) || !buffer || (bufferLen < MD_FIELD_MAX))
            return;

        MDWebView* webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return;

        MDWebSettings *settings = webView->mdWebSettings();

        switch (property) {
        case MD_STR_DEFAULT_LANGUAGE:
            strncpy (buffer, defaultLanguage().utf8().data(), MD_FIELD_MAX);
            buffer[MD_FIELD_MAX - 1] = '\0';
            break;
        case MD_STR_STANDARD_FONT:
            settings->standardFontFamily(buffer, MD_FIELD_MAX);
            break;
        case MD_STR_SERIF_FONT:
            settings->serifFontFamily(buffer, MD_FIELD_MAX);
            break;
        case MD_STR_FIXED_FONT:
            settings->fixedFontFamily(buffer, MD_FIELD_MAX);
            break;
        case MD_STR_SANS_SERIF_FONT:
            settings->sansSerifFontFamily(buffer, MD_FIELD_MAX);
            break;
        default:
            break;
        }
    }

    void mdSetStringProperty(HWND hWnd, MDEStringProp property, const char* value)
    {
        if (!hWnd || (HWND_INVALID == hWnd) || !value)
            return;

        MDWebView* webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return;

        MDWebSettings *settings = webView->mdWebSettings();
        settings->frozen(true);

        switch (property) {
        case MD_STR_DEFAULT_LANGUAGE:
            overrideDefaultLanguage(String(value));
            break;
        case MD_STR_STANDARD_FONT:
            settings->setStandardFontFamily(value);
            break;
        case MD_STR_SERIF_FONT:
            settings->setSerifFontFamily(value);
            break;
        case MD_STR_FIXED_FONT:
            settings->setFixedFontFamily(value);
            break;
        case MD_STR_SANS_SERIF_FONT:
            settings->setSansSerifFontFamily(value);
            break;
        default:
            break;
        }
        settings->frozen(false);
    }

    int mdGetIntProperty(HWND hWnd, MDEIntProp property)
    {
        if (!hWnd || (HWND_INVALID == hWnd))
            return -3;  //TODO return value

        MDWebView* webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return -3;  //TODO

        MDWebSettings* settings = webView->mdWebSettings();

        switch (property) {
        case MD_INT_MEDIUM_FONT_SIZE:
            return settings->defaultFontSize();
        case MD_INT_MEDIUM_FIXED_FONT_SIZE:
            return settings->defaultFixedFontSize();
        case MD_INT_ENCODING_NAME:
            char defaultEncoding[MD_FIELD_MAX];
            settings->defaultTextEncodingName(defaultEncoding, MD_FIELD_MAX);
            for (int i = 0; i < EncodingTableSize; i++) {
                if (!strcmp(EncodingTable[i], defaultEncoding))
                    return i;
            }
            return MD_CHARSET_UNKNOWN;
        case MD_INT_AUTO_LOAD_IMAGES:
            return settings->loadsImagesAutomatically();
        case MD_INT_AUTO_DETECT_ENCODING:
            return settings->usesEncodingDetector();
        case MD_INT_ENABLE_JAVASCRIPT:
            return settings->isJavaScriptEnabled();
        case MD_INT_ENABLE_COOKIES:
            return cookiesEnabled((Document*)0) ? TRUE : FALSE;
#if ENABLE(PLUGIN)
        case MD_INT_ENABLE_PLUGIN:
            return settings->arePluginsEnabled();
#endif
        case MD_INT_ENABLE_JS_POPUP_WINDOW:
            return !settings->javaScriptCanOpenWindowsAutomatically();
        case MD_INT_SHOWALL_AT_ONCE:
            //added by huangsh start 2011.4.27
            return settings->areShowAllAtOnceEnabled();
            //added by huangsh end 2011.4.27
            break;
        case MD_INT_SCROBAR_WIDTH_REGULAR:
            return (*(ScrollbarThemeMg::scrollbarCustomWidth())).wRegular;
        case MD_INT_SCROBAR_WIDTH_SMALL:
            return (*(ScrollbarThemeMg::scrollbarCustomWidth())).wSmall;
        case MD_INT_SCROBAR_WIDTH_MINI:
            return (*(ScrollbarThemeMg::scrollbarCustomWidth())).wMini;
        case MD_INT_ENABLE_SPATIAL_NAVIGATION:
            return settings->isSpatialNavigationEnabled();
        case MD_INT_ENABLE_WEB_SECURITY:
            return settings->isWebSecurityEnabled();
        case MD_INT_ENABLE_DEVELOPER_EXTRAS:
            return settings->developerExtrasEnabled();
        default:
            break;
        }
        return -3; //TODO return value
    }

    void mdSetIntProperty(HWND hWnd, MDEIntProp property, int value)
    {
        MDScrobarWidth scroBarWidth;

        if (!hWnd || (HWND_INVALID == hWnd))
            return;

        MDWebView* webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return;

        MDWebSettings *settings = webView->mdWebSettings();
        settings->frozen(true);

        switch (property) {
        case MD_INT_MEDIUM_FONT_SIZE:
            settings->setDefaultFontSize(value);
            break;
        case MD_INT_MEDIUM_FIXED_FONT_SIZE:
            settings->setDefaultFixedFontSize(value);
            break;
        case MD_INT_ENCODING_NAME:
            if (value >= 0 && value < EncodingTableSize)
                settings->setDefaultTextEncodingName(EncodingTable[value]);
            else
                settings->setDefaultTextEncodingName(""); // default is autodetect encode
            break;
        case MD_INT_AUTO_LOAD_IMAGES:
            settings->setLoadsImagesAutomatically(value);
            break;
        case MD_INT_AUTO_DETECT_ENCODING:
            settings->setUsesEncodingDetector(value);
            break;
        case MD_INT_ENABLE_JAVASCRIPT:
            settings->setJavaScriptEnabled(value);
            break;
        case MD_INT_ENABLE_COOKIES:
            enableCookies(value);
            break;
#if ENABLE(PLUGIN)
        case MD_INT_ENABLE_PLUGIN:
            settings->setPluginsEnabled(value);
#endif
            break;
        case MD_INT_ENABLE_JS_POPUP_WINDOW:
            settings->setJavaScriptCanOpenWindowsAutomatically(!value);
            break;
        case MD_INT_SHOWALL_AT_ONCE:
            //added by huangsh start 2011.4.27
            settings->setShowAllAtOnceEnabled(value);
            //added by huangsh end 2011.4.27
            break;
        case MD_INT_SCROBAR_WIDTH_REGULAR:
            scroBarWidth = *(ScrollbarThemeMg::scrollbarCustomWidth());
            scroBarWidth.wRegular = value;
            ScrollbarThemeMg::setScrollbarCustomWidth(&scroBarWidth);
            break;
        case MD_INT_SCROBAR_WIDTH_SMALL:
            scroBarWidth = *(ScrollbarThemeMg::scrollbarCustomWidth());
            scroBarWidth.wSmall = value;
            ScrollbarThemeMg::setScrollbarCustomWidth(&scroBarWidth);
            break;
        case MD_INT_SCROBAR_WIDTH_MINI:
            scroBarWidth = *(ScrollbarThemeMg::scrollbarCustomWidth());
            scroBarWidth.wMini = value;
            ScrollbarThemeMg::setScrollbarCustomWidth(&scroBarWidth);
            break;
        case MD_INT_ENABLE_SPATIAL_NAVIGATION:
            settings->setSpatialNavigationEnabled(value);
            break;
        case MD_INT_ENABLE_WEB_SECURITY:
            settings->setWebSecurityEnabled(value);
            break;
        case MD_INT_ENABLE_DEVELOPER_EXTRAS:
            settings->setDeveloperExtrasEnabled(value);
            break;
        default:
            break;
        }
        settings->frozen(false);
    }

    BOOL mdPerformPopupMenuEvent(HWND hWnd, MDEPopupMenuElementEvent event)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return FALSE;

        webView->performContextAction(event);
        return TRUE;
    }

    BOOL mdSearchText (HWND hWnd, const char* searchString, unsigned int searchFlag)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView || !searchString)
            return FALSE;

        return (webView->search(searchString, (searchFlag&SEARCH_BACKWARD?false:true) , (searchFlag&SEARCH_CASE_SENSITIVE ? true : false), (searchFlag&SEARCH_WRAP_AROUND? true : false)) ? TRUE : FALSE);
    }

    unsigned int mdMarkAllMatchedText(HWND hWnd, const char *searchString, unsigned int limitCount, unsigned int markFlag)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView || !searchString)
            return FALSE;

        return webView->markAllMatches(searchString, (markFlag & MARK_CASE_SENSITIVE? true : false), (markFlag & MARK_UNHIGHLIGHT? false: true), limitCount);
    }

    BOOL mdUnmarkAllMatchedText(HWND hWnd)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return FALSE;

        webView->unmarkAllMatches();
        return TRUE;
    }

    BOOL mdSetSourceView(HWND hWnd, BOOL isSourceMode)
    {
#if ENABLE(VIEWSOURCE)
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return FALSE;
        webView->setInViewSourceMode((isSourceMode ? true : false));
        return TRUE;
#else
        return FALSE;
#endif
    }

    BOOL mdIsSourceView(HWND hWnd)
    {
#if ENABLE(VIEWSOURCE)
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return FALSE;
        return (webView->isInViewSourceMode() ? TRUE : FALSE);
#else
        return FALSE;
#endif
    }

    BOOL mdGetContentPosition(HWND hWnd, int* x, int* y)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return FALSE;

        webView->contentPos(x, y);
        return TRUE;
    }

    BOOL mdSetContentPosition(HWND hWnd, int x, int y)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return FALSE;

        webView->setContentPos(x, y);
        return TRUE;
    }

    BOOL mdGetContentSize(HWND hWnd, int* width, int* height)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return FALSE;

        webView->contentsSize(width, height);
        return TRUE;
    }


    float mdGetScaleFactor(HWND hWnd)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return -1;

        return webView->pageSizeMultiplier();
    }

    void mdSetScaleFactor(HWND hWnd, float zoom)
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hWnd);
        if (!webView)
            return;

        // Fix bug 5383.
        // Set limitation for zoom according to Chrome.
        if (zoom < 0.5)
            zoom = 0.5;
        else if (zoom > 3.0)
            zoom = 3.0;

        webView->setPageSizeMultiplier(zoom);
    }

    BOOL mdSaveAsCurrentPage( HWND hwnd, BOOL bHtmlOnly,const char* savedName )
    {
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hwnd);
        if (!webView)
            return FALSE;
        webView->saveas(bHtmlOnly,savedName);
        return TRUE;
    }

    BOOL mdDeleteSavedPage(const char *fullpath)
    {
        int len = strlen(fullpath);
        struct stat buffer;
        int         status;
        int pos = len - 1;
        char *basepath;

        status = stat(fullpath, &buffer);
        if (status != 0)
            return false;

        if (!S_ISREG(buffer.st_mode))
            return false;

        for (; pos >= 0; pos--) {
            if (fullpath[pos] == '/' || fullpath[pos] == '\\') {
                pos = len;
                break;
            }

            if (fullpath[pos] == '.') {
                break;
            }
        }

        basepath = (char *) malloc(pos + 7);
        strncpy(basepath, fullpath, pos);
        basepath[pos] = '\0';
        strncat(basepath, "_files", 6);
        basepath[pos + 6]= '\0';

        char delcmd[256];
        memset(delcmd, 0, 256);
        snprintf(delcmd, 255, "rm -fr \"%s\"", basepath);
        free(basepath);

        if (system(delcmd) == -1)
            printf ("ERROR: {%s} failure. \n", delcmd);
        if(deleteFile(fullpath))
            return true;
        return false;
    }

    BOOL mdolphin_get_page_info(HWND hwnd, char *url, int len)
    {
        if (!url)
            return FALSE;
        MDWebView * webView = (MDWebView *)GetWindowAdditionalData2(hwnd);
        if (!webView)
            return FALSE;

        char *tmpUrl = webView->mainFrameURL();
        if (!tmpUrl)
            return FALSE;

        int length = strlen(tmpUrl);

        if (len <= length)
            return FALSE;
        strcpy(url, tmpUrl);

        freeUtf8Char(tmpUrl);

        return TRUE;
    }

    BOOL mdRegisterMDolphinControl(void)
    {
        return MDWebView::registerWebViewWindowClass();
    }

    BOOL mdUnregisterMDolphinControl(void)
    {
        return MDWebView::unregisterWebViewWindowClass();
    }

    void mdEnableProxy(BOOL isEnabled)
    {
        setProxyEnabled(isEnabled);
    }

    BOOL mdIsProxyEnabled(void)
    {
        return proxyEnabled();
    }

    BOOL mdSetProxy (MDEProxyType type, const MDProxyItem* item)
    {
        ProxyItem tmpItem;
        tmpItem.m_user = item->name;
        tmpItem.m_password = item->pwd;
        tmpItem.m_host = item->host;

        char port[6];
        memset(port,0,sizeof(port));
        sprintf(port,"%d",item->port);

        tmpItem.m_port = port;
        tmpItem.m_type = type;

        setProxy(tmpItem);
        return TRUE;
    }

    BOOL mdGetProxy (MDEProxyType type, MDProxyItem* item)
    {
        if (!item)
            return proxyEnabled();

        if (type > MD_PROXY_MAX)
            return FALSE;

        memset(item,0,sizeof(MDProxyItem));
        const ProxyItem *pitem = proxy(type);
        const char *p = const_cast<char*>(pitem->m_host.data());
        if(p)
            strncpy(item->host,p,PROXY_MAX_LENGTH-1);

        p = const_cast<char*>(pitem->m_user.data());
        if(p)
            strncpy(item->name,p,PROXY_MAX_LENGTH-1);

        p = const_cast<char*>(pitem->m_password.data());
        if(p)
            strncpy(item->pwd,p,PROXY_MAX_LENGTH-1);

        p = const_cast<char*>(pitem->m_port.data());
        if(p)
            item->port = atoi(const_cast<char*>(pitem->m_port.data()));
        return TRUE;
    }

    void mdEnableCache (BOOL isEnabled)
    {
        memoryCache()->setDisabled(!isEnabled);
    }

    unsigned int mdGetCacheCapacity (void)
    {
        if (memoryCache()->disabled())
            return 0;
        unsigned int totalBytes;
        memoryCache()->getCapacities(0, 0, &totalBytes);
        return totalBytes;
    }

    unsigned int mdSetCacheCapacity (unsigned int totalBytes)
    {
        unsigned oldCapacity;
        memoryCache()->getCapacities(0, 0, &oldCapacity);
        memoryCache()->setCapacities(0, totalBytes, totalBytes);

        return oldCapacity;
    }

    void mdClearCache(void)
    {
        memoryCache()->setDisabled(true);
        memoryCache()->setDisabled(false);
    }

    BOOL mdSetCacheType(MDECache type, const char* path, unsigned size)
    {
        switch (type)
       {
        case MEM_CACHE:
            return memoryCache()->setCacheType(MemCache);
#if ENABLE(DISK_CACHE)
        case DISK_CACHE:
            return memoryCache()->setCacheType(DiskCache, path, size);
#endif
        default:
            return false;
        }
    }

    BOOL mdIsCacheEnabled (void)
    {
        return !memoryCache()->disabled();
    }

    BOOL mdIsCookieEnabled(void)
    {
        return (cookiesEnabled((Document *)0) ? TRUE : FALSE);
    }

    void mdEnableCookie(BOOL isEnabled)
    {
        return enableCookies(isEnabled);
    }

    void mdClearCookie(void)
    {
        return clearCookies();
    }


#if ENABLE(SCHEMEEXTENSION)
    BOOL mdRegisterSchemeHandler(const char *scheme, MDCB_SCHEME_HANDLER cb, void *param)
    {
        return RegisterSchemeHandler(scheme, cb, param);
    }

    BOOL mdUnregisterSchemeHandler(const char *scheme)
    {
        return UnregisterSchemeHandler(scheme);
    }

#if ENABLE(SSL) && ENABLE(SSLFILE)
    BOOL mdSetCAPath(const char* path)
    {
        if(!path)
            return FALSE;
        setCAPath(path);
        return TRUE;
    }
#endif

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

