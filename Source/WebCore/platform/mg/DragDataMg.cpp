/*
** $Id: DragDataMg.cpp 590 2011-04-22 05:29:20Z lijiangwei $
**
** DragDataMg.cpp: Dragdata implements file.
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
#include "DragData.h"

#include "ClipboardMg.h"
#include "PlatformString.h"
#include "DocumentFragment.h"
#include "NotImplemented.h"

namespace WebCore {

#if 0
DragData::DragData(DragDataRef data, const IntPoint& clientPosition, const IntPoint& globalPosition, 
    DragOperation sourceOperationMask)
    : m_clientPosition(clientPosition)
    , m_globalPosition(globalPosition)
    , m_platformDragData(data)
    , m_draggingSourceOperationMask(sourceOperationMask)
{  
}
#endif

#if 0
PassRefPtr<Clipboard> DragData::createClipboard(ClipboardAccessPolicy) const
{
    notImplemented();
    PassRefPtr<Clipboard> T;
    return T;
    //Clipboard *T = NULL;
    //return adoptRef(new Clipboard());
}
#endif
bool DragData::containsURL(Frame* frame, FilenameConversionPolicy filenamePolicy) const
{
    notImplemented();
    return false;
}

bool DragData::containsPlainText() const
{
    notImplemented();
    return false;
}

bool DragData::containsCompatibleContent() const
{
    notImplemented();
    return false;
}

String DragData::asURL(Frame*, FilenameConversionPolicy filenamePolicy, String* title) const
{
    notImplemented();
    return String();
}

String DragData::asPlainText(Frame*) const
{
    notImplemented();
    return String();
}

void DragData::asFilenames(Vector<String>&) const
{
    notImplemented();
}

Color DragData::asColor() const
{
    notImplemented();
    return Color();
}

PassRefPtr<DocumentFragment> DragData::asFragment(Frame* frame, PassRefPtr<Range>, bool, bool&) const
{
    notImplemented();
    return NULL;
}

bool DragData::canSmartReplace() const
{
    notImplemented();
    return false;
}

bool DragData::containsColor() const
{
    notImplemented();
    return false;
}

bool DragData::containsFiles() const
{
    notImplemented();
    return false;
}

}
