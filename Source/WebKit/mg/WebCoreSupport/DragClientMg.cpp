/*
** $Id: DragClientMg.cpp 546 2007-10-24 09:26:33Z jpzhang $
**
** DragClientMg.cpp: Drug Client implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-9-20
*/

#include "config.h"
#include "DragClientMg.h"
#include "DragImage.h"

#include "NotImplemented.h"

namespace WebCore
{

DragClientMg::DragClientMg()
{

}

DragClientMg::~DragClientMg()
{

}

void DragClientMg::willPerformDragDestinationAction(DragDestinationAction, DragData*)
{
    notImplemented();
}

void DragClientMg::willPerformDragSourceAction(DragSourceAction, const IntPoint&, Clipboard*)
{
    notImplemented();
}

DragDestinationAction DragClientMg::actionMaskForDrag(DragData*)
{
    notImplemented();
    return DragDestinationActionNone;
}

DragSourceAction DragClientMg::dragSourceActionMaskForPoint(const IntPoint& windowPoint)
{
    notImplemented();
    return DragSourceActionNone;
}

void DragClientMg::startDrag(DragImageRef dragImage, const IntPoint& dragImageOrigin, 
                                    const IntPoint& eventPos, Clipboard*, Frame*, bool linkDrag)
{
    notImplemented();
}

DragImageRef DragClientMg::createDragImageForLink(KURL&, const String&, Frame*)
{
    notImplemented();
    return NULL;
}

}
