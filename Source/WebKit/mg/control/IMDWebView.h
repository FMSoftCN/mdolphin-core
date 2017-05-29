/*
 ** $Id$
 **
 ** IMDWebView.h: the interface file of MDWebView
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-08
 */

#ifndef IMDWebView_h
#define IMDWebView_h

#include "IUnknown.h"

class MDWebSettings;
class IMDWebBackForwardList;
class IMDWebFrame;

class IMDWebUIDelegate;
class IMDWebCustomDelegate;
class IMDWebEditorDelegate;
class IMDWebHistoryDelegate;
class IMDWebDownloadDelegate;
class IMDWebPolicyDelegate;
class IMDWebFrameLoadDelegate;
class IMDWebView;

#ifdef __cplusplus
extern "C" {
#endif

IMDWebView* mdCreateWebViewInstance(RECT rect, int id, HWND hParent);
IMDWebView* mdGetWebViewInstance(HWND hWnd);
void GCCollectJSObjects();
void GCCollectJSObjectsOnAlternateThread(bool waitUntilDone);
size_t GCCollectJSObjectsCount();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class IMDWebView: public IUnknown
{
public:
 //START_MDWEBVIEW_INITWINDOW
    virtual HWND viewWindow() {return NULL;};
//END_MDWEBVIEW_INITWINDOW

//START_MDWEBVIEW_NAVIGATION
    virtual bool loadURL(const char * sUrl) {return false;};
    virtual void stopLoading(void) {};
    virtual void reload(void){};
    virtual void goBack(void){};
    virtual void goForward(void){};
    virtual bool canGoBack(void){ return false;};
    virtual bool canGoForward(void){ return false;};
//END_MDWEBVIEW_NAVIGATION

//START_MDWEBVIEW_SEARCH
    virtual bool search(const char *str, bool forward, bool caseSensitive, bool wrap){ return false;};
    virtual unsigned int markAllMatches(const char *str, bool caseSensitive, 
                         bool highlight, unsigned int limit) = 0;
    virtual void unmarkAllMatches(void){};
//END_MDWEBVIEW_SEARCH

//START_MDWEBVIEW_CONTENTPOSITIPON
    virtual void contentsSize(int* w, int* h){}; 
    virtual void contentPos(int *x, int *y){};
    virtual void setContentPos(int x, int y){};
    virtual void scrollBy(int offsetX, int offsetY){};
//END_MDWEBVIEW_CONTENTPOSITIPON

//START_MDWEBVIEW_ZOOM
    virtual void setTextSizeMultiplier(float multiplier) = 0;
    virtual float textSizeMultiplier() = 0;
 // zoompage
    virtual void setPageSizeMultiplier(float multiplier) = 0;
    virtual float pageSizeMultiplier() = 0;
//END_MDWEBVIEW_ZOOM


//START_MDWEBVIEW_PROGRESS
    virtual double estimatedProgress() = 0;
    virtual bool isLoading() = 0;
//END_MDWEBVIEW_PROGRESS

//START_MDWEBVIEW_PERFERENCESANDBACKFORWARDLIST
    virtual MDWebSettings* mdWebSettings(){return NULL;};
    virtual IMDWebBackForwardList* mdWebBackForwardList(){return NULL;};
//END_MDWEBVIEW_PERFERENCESANDBACKFORWARDLIST
    
//START_MDWEBVIEW_PAGEINFO
    virtual void setCustomTextEncodingName(const char *encodingName) = 0;
    virtual char* customTextEncodingName(){ return NULL;};  
    virtual char* mainFrameURL(){return NULL;};
    virtual IMDWebFrame*  mainWebFrame() = 0;
//END_MDWEBVIEW_PAGEINFO


//START_MDWEBVIEW_MIME
    virtual bool canShowMIMEType(const char *mime){ return false;};
    //END_MDWEBVIEW_MIME

    //START_MDWEBVIEW_MENUCONTEXT
    virtual void performContextAction(MDEPopupMenuElementEvent event){};
    //END_MDWEBVIEW_MENUCONTEXT
//START_MDWEBVIEW_GETANDSETDELEGATE
//
    virtual void setUIDelegate(IMDWebUIDelegate* d){};
    virtual IMDWebUIDelegate*  uiDelegate(){return NULL;};
    virtual void setEditorDelegate(IMDWebEditorDelegate* d){};
    virtual IMDWebEditorDelegate* editorDelegate( ){return NULL;};
    virtual void setDownloadDelegate(IMDWebDownloadDelegate* d){};
    virtual IMDWebDownloadDelegate* downloadDelegate( ){return NULL;};
    virtual void setHistoryDelegate(IMDWebHistoryDelegate* d){};
    virtual IMDWebHistoryDelegate* historyDelegate( ){return NULL;};
    virtual void setCustomDelegate(IMDWebCustomDelegate* d){};
    virtual IMDWebCustomDelegate* customDelegate(){return NULL;};
    virtual void setPolicyDelegate(IMDWebPolicyDelegate* d){};
    virtual IMDWebPolicyDelegate* policyDelegate(){return NULL;};
    virtual void setFrameLoadDelegate(IMDWebFrameLoadDelegate* d){};
    virtual IMDWebFrameLoadDelegate* frameLoadDelegate(){return NULL;};
//END_MDWEBVIEW_GETANDSETDELEGATE

    virtual void executeScript(const char* script) = 0;
    virtual bool getFocusedEditorInfo(MDEditorElement* elment){return false;};
};

#endif

#endif
