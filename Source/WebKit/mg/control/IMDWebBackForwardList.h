/*
 ** $Id$
 **
 ** IMDWebWebBackForwardList.h
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: zjwang 
 **  
 ** Create date: 2010-07-13
 */

#ifndef IMDWebWebBackForwardList_H
#define IMDWebWebBackForwardList_H
#include "IUnknown.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef void* MDHWebHistoryItem;

MDHWebHistoryItem mdWebHistoryItemInitWithURLString(const char* urlString, const char* title, time_t lastVisited);
const char* mdWebHistoryItemOriginalURLString(MDHWebHistoryItem item);
const char* mdWebHistoryItemURLString(MDHWebHistoryItem item);
const char* mdWebHistoryItemTitle(MDHWebHistoryItem item);
time_t mdWebHistoryItemLastVisitedTimeInterval(MDHWebHistoryItem item);
int mdWebHistoryItemSetAlternateTitle(MDHWebHistoryItem item,const char* title);
const char* mdWebHistoryItemAlternateTitle(MDHWebHistoryItem item);
int mdWebHistoryItemIcon(MDHWebHistoryItem item ,MYBITMAP *hBitmap);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

class IMDWebBackForwardList : public IUnknown {
	public:        
		virtual ~IMDWebBackForwardList(){};
		//Adding and Removing Items
		virtual void addItem(const MDHWebHistoryItem )=0;
		virtual void removeItem(const MDHWebHistoryItem )=0;

		//Moving Backward and Forward
		virtual void goBack()=0;
		virtual void goForward()=0;
		virtual void goToItem(const MDHWebHistoryItem )=0;

		//Querying the Back-Forward List
		virtual MDHWebHistoryItem backItem()=0;
		virtual int backListCount()=0;
		//Get an array of items before the current entry the return value is the actual get MDHWebHistoryItem count
		virtual int backListWithLimit(int limit,MDHWebHistoryItem* list)=0;
		virtual bool containsItem(const MDHWebHistoryItem )=0;
		virtual MDHWebHistoryItem currentItem()=0;
		virtual MDHWebHistoryItem itemAtIndex(int)=0;
		virtual MDHWebHistoryItem forwardItem()=0;
		virtual int forwardListCount()=0;
		//Get an array of items after the current entry
		// the return value is the actual get MDHWebHistoryItem count
		virtual int forwardListWithLimit(int limit,MDHWebHistoryItem* list)=0;

		//Setting Attributes
		virtual int capacity()=0;
		virtual void setCapacity(int)=0;
		virtual bool enabled()=0;
		virtual void setEnabled(bool)=0;

};
#endif

#endif
