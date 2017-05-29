/*
 ** $Id$
 **
 ** ClipboardMg.h: ClipboardMg header file.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 06/04/2010 
 */

#ifndef ClipboardMg_H
#define ClipboardMg_H

#include "Clipboard.h"

namespace WebCore {

class CachedImage;

// State available during IE's events for drag and drop and copy/paste
class ClipboardMg : public Clipboard 
{
public:
    ClipboardMg(ClipboardAccessPolicy policy, ClipboardType type);
    ~ClipboardMg();

    void clearData(const String& type);
    void clearAllData();
    String getData(const String& type, bool& success) const;
    bool setData(const String& type, const String& data);
    
    // extensions beyond IE's API
    HashSet<String> types() const;
    virtual PassRefPtr<FileList> files() const;

    IntPoint dragLocation() const;
    CachedImage* dragImage() const;
    void setDragImage(CachedImage*, const IntPoint&);
    Node* dragImageElement();
    void setDragImageElement(Node*, const IntPoint&);

    virtual DragImageRef createDragImage(IntPoint& dragLoc) const;
#if ENABLE(DRAG_SUPPORT)
    virtual void declareAndWriteDragImage(Element*, const KURL&, const String& title, Frame*);
#endif
    virtual void writeURL(const KURL&, const String&, Frame*);
    virtual void writeRange(Range*, Frame*);

    virtual bool hasData();

    virtual void writePlainText(const String&);
};

}

#endif // ClipboardMg_H
