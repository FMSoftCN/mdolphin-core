/*
** $Id: ContextMenuClientMg.h 638 2007-11-15 09:29:43Z jpzhang $
**
** ContextMenuClientMg.h: Context Menu header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-1-5
*/

#ifndef ContextMenuClientMg_H
#define ContextMenuClientMg_H

#include "ContextMenuClient.h"

class MDWebView;
namespace WebCore {
    class ContextMenu;

class ContextMenuClientMg : public ContextMenuClient
{
public:
    ContextMenuClientMg(MDWebView*);
    virtual ~ContextMenuClientMg();

    virtual void contextMenuDestroyed(){delete this;}

    virtual PlatformMenuDescription getCustomMenuFromDefaultItems(ContextMenu*);
    virtual void contextMenuItemSelected(ContextMenuItem*, const ContextMenu*);

    virtual void downloadURL(const KURL& url);
    virtual void lookUpInDictionary(Frame*);
    virtual void searchWithGoogle(const Frame*);
    virtual void speak(const String&);
    virtual void stopSpeaking();
    virtual bool isSpeaking() {return false;}
private:
    MDWebView* m_webView;
};

}

#endif // ContextMenuClientMg_H
