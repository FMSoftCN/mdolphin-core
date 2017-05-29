/*
 ** $Id$
 **
 ** ClipboardMg.cpp: ClipboardMg implements file.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 06/04/2010 
 */

#include "config.h"
#include "ClipboardMg.h"

#include "DragData.h"
#include "Editor.h"
#include "FileList.h"
#include "NotImplemented.h"
#include "StringHash.h"

namespace WebCore {

PassRefPtr<Clipboard> Editor::newGeneralClipboard(ClipboardAccessPolicy policy, Frame* frame)
{
    return new ClipboardMg(policy, Clipboard::CopyAndPaste);
}

ClipboardMg::ClipboardMg(ClipboardAccessPolicy policy, ClipboardType type) 
    : Clipboard(policy, type)
{
    notImplemented();
}

ClipboardMg::~ClipboardMg()
{
    notImplemented();
}

void ClipboardMg::clearData(const String& type)
{
    notImplemented();
}

void ClipboardMg::writePlainText(const String&)
{
    notImplemented();
}

void ClipboardMg::clearAllData() 
{
    //the following code comes from mdolphin V2.0
    //Pasteboard::generalPasteboard()->clear();
    notImplemented();
}

String ClipboardMg::getData(const String& type, bool& success) const 
{
    notImplemented();
    success = false;
    return String();
}

bool ClipboardMg::setData(const String& type, const String& data) 
{
    notImplemented();
    return false;
}

// extensions beyond IE's API
HashSet<String> ClipboardMg::types() const 
{
    notImplemented();
    return HashSet<String>();
}

PassRefPtr<FileList> ClipboardMg::files() const
{
    notImplemented();
    return 0;
}

IntPoint ClipboardMg::dragLocation() const 
{ 
    notImplemented();
    return IntPoint(0,0);
}

CachedImage* ClipboardMg::dragImage() const 
{
    notImplemented();
    return 0; 
}

void ClipboardMg::setDragImage(CachedImage*, const IntPoint&) 
{
    notImplemented();
}

Node* ClipboardMg::dragImageElement() 
{
    notImplemented();
    return 0; 
}

void ClipboardMg::setDragImageElement(Node*, const IntPoint&)
{
    notImplemented();
}

DragImageRef ClipboardMg::createDragImage(IntPoint& dragLoc) const
{ 
    notImplemented();
    return 0;
}

PassRefPtr<Clipboard> Clipboard::create(ClipboardAccessPolicy policy, DragData* dragData, Frame* frame)
{
    return new ClipboardMg(policy, Clipboard::CopyAndPaste);
}

#if ENABLE(DRAG_SUPPORT)
void ClipboardMg::declareAndWriteDragImage(Element*, const KURL&, const String&, Frame*) 
{
    notImplemented();
}
#endif

void ClipboardMg::writeURL(const KURL& url, const String& string, Frame* frame) 
{
    //the following code comes from mdolphin V2.0
    //Pasteboard::generalPasteboard()->writeURL(url, string, frame);
    notImplemented();
}

void ClipboardMg::writeRange(Range*, Frame*) 
{
    notImplemented();
}

bool ClipboardMg::hasData() 
{
    notImplemented();
    return false;
}
}
