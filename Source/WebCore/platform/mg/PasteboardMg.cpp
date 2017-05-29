/*
** $Id$
**
** PasteboardMg.cpp: Pateboard implement files.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software.
**   
** Current maintainer: lvlei 
**  
** Create date: 06/04/2010 
*/

#include "minigui.h"
#include "config.h"
#include "Pasteboard.h"

#include "DocumentFragment.h"
#include "Editor.h"
#include "Frame.h"
#include "Image.h"
#include "markup.h"
#include "PlatformString.h"
#include "NotImplemented.h"


namespace WebCore {

Pasteboard::Pasteboard()
{
}

Pasteboard* Pasteboard::generalPasteboard()
{
    static Pasteboard* pasteboard = new Pasteboard;
    return pasteboard;
}

void Pasteboard::writePlainText(const String& selectText)
{
    SetClipBoardData (CBNAME_TEXT, (void*)selectText.utf8().data(), selectText.utf8().length(), CBOP_NORMAL);
}

void Pasteboard::writeSelection(Range* selectedRange, bool canSmartCopyOrDelete, Frame* frame)
{
    //FIXME: selectedRange->toHTML() can get html code but now not used.
    SetClipBoardData (CBNAME_TEXT, (void*)selectedRange->text().utf8().data(), selectedRange->text().utf8().length(), CBOP_NORMAL);
}

void Pasteboard::writeURL(const KURL& url, const String& title, Frame*)
{
    //FIXME: this is a simple implements.
    SetClipBoardData (CBNAME_TEXT, (void*)url.string().utf8().data(), url.string().utf8().length(), CBOP_NORMAL);
}

void Pasteboard::writeImage(Node*, const KURL&, const String& title)
{
    notImplemented();
}

void Pasteboard::clear()
{
    notImplemented();
}

bool Pasteboard::canSmartReplace()
{
    notImplemented();
    return false;
}

PassRefPtr<DocumentFragment> Pasteboard::documentFragment(Frame* frame, PassRefPtr<Range> context, bool allowPlainText, bool& chosePlainText)
{
    chosePlainText = false;
    size_t size = GetClipBoardDataLen(CBNAME_TEXT);
    if (size) {
        char *buffer = new char[size];
        size_t get_size = GetClipBoardData (CBNAME_TEXT, buffer, size);
        String html = String::fromUTF8(buffer, get_size);
        if (! html.isEmpty()) {
            RefPtr<DocumentFragment> fragment = createFragmentFromMarkup(frame->document(), html, "");
            if (fragment)
                return fragment.release();
        }
        
        if (!allowPlainText)
            return 0;
        
        chosePlainText = true;
        RefPtr<DocumentFragment> fragment = createFragmentFromText(context.get(), html);
        if (fragment)
            return fragment.release();
        delete[] buffer;
    }
    return 0;
}

String Pasteboard::plainText(Frame*)
{
    size_t size = GetClipBoardDataLen(CBNAME_TEXT);
    if (size) {
        char buffer[size+1];
        memset(buffer, 0, sizeof(buffer));
        size_t get_size = GetClipBoardData (CBNAME_TEXT, buffer, size);
        return String::fromUTF8(buffer, get_size);
    }
    return String();
}

}
