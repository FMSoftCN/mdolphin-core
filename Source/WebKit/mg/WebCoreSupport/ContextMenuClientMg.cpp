/*
** $Id: ContextMenuClientMg.cpp 982 2008-02-29 03:34:44Z whzeng $
**
** ContextMenuClientMg.cpp: Context Menu implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-1-5
*/

#include "minigui.h"
#include "mdolphin.h"
#include "config.h"

#include "ContextMenu.h"
#include "ContextMenuClientMg.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "Page.h"
#include "MDWebView.h"

#include "NotImplemented.h"

namespace WebCore {

ContextMenuClientMg::ContextMenuClientMg(MDWebView* webView)
    :m_webView(webView)
{
}

ContextMenuClientMg::~ContextMenuClientMg()
{
}

PlatformMenuDescription ContextMenuClientMg::getCustomMenuFromDefaultItems(ContextMenu* menu)
{
    // warning: this transfers the ownership to the caller
    return menu->releasePlatformDescription();
}

void ContextMenuClientMg::contextMenuItemSelected(ContextMenuItem*, const ContextMenu*)
{
    // We may implement applicaion like SaveAs here.
}
 
void ContextMenuClientMg::downloadURL(const KURL& url)
{
    m_webView->downloadURL(url);
}

void ContextMenuClientMg::searchWithGoogle(const Frame* frame)
{
#if 0 //lijiangwei
    String searchString = frame->selectedText();
    searchString.stripWhiteSpace();
    String encoded = encodeWithURLEscapeSequences(searchString);
    encoded.replace("%20", "+");
    
    String url("http://www.google.com/search?q=");
    url.append(encoded);
    url.append("&ie=UTF-8&oe=UTF-8");

    ResourceRequest request = ResourceRequest(url);
    if (Page* page = frame->page())
        page->mainFrame()->loader()->urlSelected(request, String(), 0, false, false, true, SendReferrer);
#endif
}

void ContextMenuClientMg::lookUpInDictionary(Frame*)
{
    notImplemented();
}

void ContextMenuClientMg::speak(const String&)
{
    notImplemented();
} 

void ContextMenuClientMg::stopSpeaking()
{
    notImplemented();
}

}
