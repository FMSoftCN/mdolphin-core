/*
 ** $Id$
 **
 ** MDWebFrame.h: The head file of webframe
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-08
 */

#ifndef MDWebFrame_h
#define MDWebFrame_h

#include "config.h"
#include "IMDWebFrame.h"

#include <wtf/PassRefPtr.h>

namespace WebCore {
    class  Frame;
    class  FrameView;
    class  Page;
    class HTMLFrameOwnerElement;
    class FrameLoaderClientMg;
    class SharedBuffer;
}

class MDWebView;
class MDWebFrame;


WebCore::Frame* core(const MDWebFrame* webFrame);
WebCore::Frame* core(MDWebFrame* webFrame);
MDWebFrame* kit(WebCore::Frame*);
// END_MDWEBFRAMEPRIVATE 

//START_MDWEBFRAME
class MDWebFrame : public IMDWebFrame 
{
public:
    MDWebFrame ();                             /* constructor */
    ~MDWebFrame();

//START_MDWEBFRAMEPRIVATE_INIT
    static MDWebFrame* createInstance();
    void destroyWebFrame();
    PassRefPtr<WebCore::Frame> init(MDWebView *view, 
            WebCore::HTMLFrameOwnerElement*, WebCore::FrameLoaderClientMg *client);
    WebCore::Frame* impl();
//END_MDWEBFRAMEPRIVATE_INIT


//START_MDWEBFRAMEPRIVATE_LOAD
    bool loadURL(const char * url);
    void stopLoading(void);
    void reload(void);
    virtual void loadData(const char *data, int len, const char *mimeType, 
            const char *encoding, const char *baseURL, const char* unreachableURL);
//END_MDWEBFRAMEPRIVATE_LOAD


//START_MDWEBFRAMEPRIVATE_INFO
    IMDWebView* webView() const;
    char *url() const;
    MDWebView* topLevelView() const;
//END_MDWEBFRAMEPRIVATE_INFO

    const char* name() const;
    char* renderTreeAsExternalRepresentation();
    IMDWebFrame* findFrame(const char* name); 
    IMDWebFrame* parentFrame(); 
    void layout();
    char* responseMIMEType();
    char* innerText();
    bool childFrames(std::list<IMDWebFrame*>&);

//START_MDWEBFRAME_DATAMEMBER
protected:
    class MDWebFramePrivate;
    MDWebFramePrivate*    d;
private:

//END_MDWEBFRAME_DATAMEMBER

};
//END_MDWEBFRAME

#endif
