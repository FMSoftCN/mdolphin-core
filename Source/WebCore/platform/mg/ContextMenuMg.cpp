/*
** $Id: ContextMenuMg.cpp 618 2011-05-03 07:37:44Z lijiangwei $
**
** ContextMenuMg.cpp: The context menu for minigui.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software
**   
** Current maintainer: lijiangwei 
**  
** Create date: 2010-06-07 
*/
#include "minigui.h"

#include "config.h"
#include "ContextMenu.h"

#include "Document.h"
#include "Frame.h"
#include "FrameView.h"
#include "Node.h"

namespace WebCore {

ContextMenu::ContextMenu()
{
}

ContextMenu::ContextMenu(const PlatformMenuDescription menu)
{
}

ContextMenu::~ContextMenu()
{
}

ContextMenuItem* ContextMenu::itemWithAction(unsigned action)
{
    for (size_t i = 0; i < m_items.size(); ++i) {
        if (m_items[i].action() == static_cast<ContextMenuAction>(action))
            return &m_items[i];
    }
    return 0;
}

ContextMenuItem* ContextMenu::itemAtIndex(unsigned index, const PlatformMenuDescription platformDescription)
{
    return &m_items[index];
}

void ContextMenu::appendItem(ContextMenuItem& item)
{
    m_items.append(item);
}

unsigned ContextMenu::itemCount() const
{
    return m_items.size();
}

void ContextMenu::insertItem(unsigned position, ContextMenuItem& item)
{
    m_items.insert(position, item);
}

void ContextMenu::setPlatformDescription(PlatformMenuDescription)
{
    // doesn't make sense
}

PlatformMenuDescription ContextMenu::platformDescription() const
{
    return m_items;
}

PlatformMenuDescription ContextMenu::releasePlatformDescription()
{
    return PlatformMenuDescription();
}

}
