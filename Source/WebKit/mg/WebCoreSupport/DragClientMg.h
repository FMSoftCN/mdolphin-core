/*
** $Id: DragClientMg.h 546 2007-10-24 09:26:33Z jpzhang $
**
** DragClientMg.h: Drug Client header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-9-20
*/

#ifndef DragClientMg_H
#define DragClientMg_H

#include "DragClient.h"

namespace WebCore {

class DragClientMg : public DragClient
{
public:
    DragClientMg();
    virtual ~DragClientMg();

    virtual void dragControllerDestroyed(){delete this;}

    virtual void willPerformDragDestinationAction(DragDestinationAction, DragData*);
    virtual void willPerformDragSourceAction(DragSourceAction, const IntPoint&, Clipboard*);
    virtual DragDestinationAction actionMaskForDrag(DragData*);
    virtual DragSourceAction dragSourceActionMaskForPoint(const IntPoint& windowPoint);

    virtual void startDrag(DragImageRef dragImage, const IntPoint& dragImageOrigin, 
                                    const IntPoint& eventPos, Clipboard*, Frame*, bool linkDrag = false);
    virtual DragImageRef createDragImageForLink(KURL&, const String& label, Frame*);
};

}
#endif // DragClientMg_H
