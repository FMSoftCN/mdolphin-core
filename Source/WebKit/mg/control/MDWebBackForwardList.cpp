/*
 ** $Id$
 **
 ** MDWebBackForwardList.cpp
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: zjwang 
 **  
 ** Create date: 2010-07-13
 */
#include "minigui.h"
#include "config.h"

#include "MDWebBackForwardList.h"
#include "BackForwardListImpl.h"
#include "HistoryItem.h" 
#include "PlatformString.h"
using namespace WebCore;
MDWebBackForwardList* MDWebBackForwardList::createInstance(PassRefPtr<BackForwardListImpl> backForwardList)
{
	MDWebBackForwardList* instance = new MDWebBackForwardList(backForwardList);
	return instance;
}

MDWebBackForwardList::MDWebBackForwardList(PassRefPtr<BackForwardListImpl> backForwardList)
    :m_backForwardList(backForwardList)
{
}
MDWebBackForwardList::~MDWebBackForwardList()
{    
}

void MDWebBackForwardList::addItem(const MDHWebHistoryItem i)
{
	//convert handle to historyitem
	HistoryItem *item=(HistoryItem*) i;
	if(!item)
		m_backForwardList->addItem(item);
}

void MDWebBackForwardList::removeItem(const MDHWebHistoryItem i )
{
	//convert handle to historyitem
	HistoryItem *item=(HistoryItem*) i;
	if(!item)
		m_backForwardList->removeItem(item);
}

void MDWebBackForwardList::goBack()
{
	m_backForwardList->goBack();
}

void MDWebBackForwardList::goForward()
{
	m_backForwardList->goForward();
}

void MDWebBackForwardList::goToItem(const MDHWebHistoryItem i)
{
	//convert handle to historyitem
	HistoryItem *item=(HistoryItem*) i;
	if(!item)
		m_backForwardList->goToItem(item);
}
//backItem的实现
MDHWebHistoryItem MDWebBackForwardList::backItem()
{    
	HistoryItem* item = m_backForwardList->backItem();
	//convert   historyitem to handle
	// if it's failed ,the item 's value is NULL
	return (MDHWebHistoryItem)item;   
}
//backListCount的实现
int MDWebBackForwardList::backListCount()
{    
	return m_backForwardList->backListCount();
}

int MDWebBackForwardList::backListWithLimit(int limit,MDHWebHistoryItem* list)
{    
	HistoryItemVector historyItemVector;
	m_backForwardList->backListWithLimit(limit, historyItemVector);
	int listCount = static_cast<int>(historyItemVector.size());
	if (list)
		for (unsigned i = 0; i < historyItemVector.size(); i++)
			list[i] =(MDHWebHistoryItem)(historyItemVector[i].get());
	return listCount;

}
bool MDWebBackForwardList::containsItem(const MDHWebHistoryItem i )
{     
	HistoryItem *item=(HistoryItem*) i;
	return  m_backForwardList->containsItem(item);
}
MDHWebHistoryItem MDWebBackForwardList:: currentItem()
{    
	HistoryItem* historyItem = m_backForwardList->currentItem();
	return (MDHWebHistoryItem)historyItem;
}
MDHWebHistoryItem MDWebBackForwardList::itemAtIndex(int index)
{    
	HistoryItem* historyItem = m_backForwardList->itemAtIndex(index);
	return (MDHWebHistoryItem)historyItem;
}
MDHWebHistoryItem MDWebBackForwardList:: forwardItem()
{    
	HistoryItem* historyItem = m_backForwardList->forwardItem();
	return (MDHWebHistoryItem)historyItem;
}
int MDWebBackForwardList::forwardListCount()
{    
	return m_backForwardList->forwardListCount();
}
int MDWebBackForwardList::forwardListWithLimit(int limit,MDHWebHistoryItem* list)
{    
	HistoryItemVector historyItemVector;
	m_backForwardList->forwardListWithLimit(limit, historyItemVector);
	int listCount = static_cast<int>(historyItemVector.size());
	if (list)
		for (unsigned i = 0; i < historyItemVector.size(); i++)
			list[i] =(MDHWebHistoryItem)(historyItemVector[i].get());
	return listCount;
}
int MDWebBackForwardList::capacity()
{    
	return m_backForwardList->capacity();

}
void  MDWebBackForwardList::setCapacity(int size)
{    
	m_backForwardList->setCapacity(size);
}
bool MDWebBackForwardList::enabled()
{    
	return m_backForwardList->enabled();
}
void MDWebBackForwardList::setEnabled(bool enable)
{    
	m_backForwardList->setEnabled(enable);
}
MDHWebHistoryItem mdWebHistoryItemInitWithURLString(const char* urlString, const char* title, time_t lastVisited)
{
	HistoryItem *historyItem=HistoryItem::create(urlString, title,lastVisited).releaseRef();
	return (MDHWebHistoryItem)historyItem;

}
const char* mdWebHistoryItemOriginalURLString(MDHWebHistoryItem item)
{
	if(!item)
		return NULL;
	HistoryItem *historyItem=(HistoryItem*)item;
		String str = historyItem->originalURLString();
	if(!str)
		return NULL;
	char *url=(char*)malloc(str.length()+1);
	strcpy(url,str.utf8().data());
	return url;

}
const char* mdWebHistoryItemURLString(MDHWebHistoryItem item)
{
	if(!item)
		return NULL;
	HistoryItem *historyItem=(HistoryItem*)item;
		String str = historyItem->urlString();
	if(!str)
		return NULL;
	char *url=(char*)malloc(str.length()+1);
	strcpy(url,str.utf8().data());
	return url;

}
const char* mdWebHistoryItemTitle(MDHWebHistoryItem item)
{
	if(!item)
		return NULL;
	HistoryItem *historyItem=(HistoryItem*)item;
		String str = historyItem->title();
	if(!str)
		return NULL;
	char *url=(char*)malloc(str.length()+1);
	strcpy(url,str.utf8().data());
	return url;

}

time_t mdWebHistoryItemLastVisitedTimeInterval(MDHWebHistoryItem item)
{
	if(!item)
		return NULL;
	HistoryItem *historyItem=(HistoryItem*)item;
		return historyItem->lastVisitedTime();
}

int mdWebHistoryItemSetAlternateTitle(MDHWebHistoryItem item,const char* title)
{
	if(!item)
		return -1;
	HistoryItem *historyItem=(HistoryItem*)item;
	historyItem->setAlternateTitle(title);

    return -1;
}

const char* mdWebHistoryItemAlternateTitle(MDHWebHistoryItem item)
{
	if(!item)
		return NULL;
	HistoryItem *historyItem=(HistoryItem*)item;
	return historyItem->alternateTitle().utf8().data();
}

int mdWebHistoryItemIcon(MDHWebHistoryItem item ,MYBITMAP *hBitmap)
{
    return -1;
}
