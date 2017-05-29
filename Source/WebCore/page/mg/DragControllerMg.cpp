/*
** $Id$
**
** DragControllerMg.cpp: Drug Controller implements file.
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
#include "DragController.h"

#include "DragData.h"
#include "Frame.h"
#include "FrameView.h"
#include "Page.h"




namespace WebCore {

const int DragController::LinkDragBorderInset = 2;
const int DragController::MaxOriginalImageArea = 1500 * 1500;
const int DragController::DragIconRightInset = 7;
const int DragController::DragIconBottomInset = 3;

const float DragController::DragImageAlpha = 0.75f;

DragOperation DragController::dragOperation(DragData* dragData)
{
    //FIXME: This logic is incomplete
     if (dragData->containsURL(0))
        return DragOperationCopy;

    return DragOperationNone;
}

bool DragController::isCopyKeyDown(DragData*) 
{
    return false;
}
    
const IntSize& DragController::maxDragImageSize()
{
    static const IntSize maxDragImageSize(200, 200);
    
    return maxDragImageSize;
}

void DragController::cleanupAfterSystemDrag()
{
    //dragEnded();
}

}
