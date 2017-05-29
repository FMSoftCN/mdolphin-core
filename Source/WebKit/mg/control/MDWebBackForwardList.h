/*
 ** $Id$
 **
 ** MDWebBackForwardList.h
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: zjwang 
 **  
 ** Create date: 2010-07-13
 */
#ifndef MDWebBackForwardList_H
#define MDWebBackForwardList_H
#include "IMDWebBackForwardList.h"
#include <wtf/RefPtr.h>
namespace WebCore {
    class BackForwardListImpl;
}

class MDWebBackForwardList :public IMDWebBackForwardList{
	public:
		static MDWebBackForwardList* createInstance(PassRefPtr<WebCore::BackForwardListImpl>);
	protected:
		MDWebBackForwardList(PassRefPtr<WebCore::BackForwardListImpl>);
		~MDWebBackForwardList();

	public:
		//Adding and Removing Items
		virtual void addItem(const MDHWebHistoryItem );
		virtual void removeItem(const MDHWebHistoryItem );

		//Moving Backward and Forward
		virtual void goBack();
		virtual void goForward();
		virtual void goToItem(const MDHWebHistoryItem );

		//Querying the Back-Forward List
		virtual MDHWebHistoryItem backItem();
		virtual int backListCount();
		virtual int backListWithLimit(int limit,MDHWebHistoryItem*);
		virtual bool containsItem(const MDHWebHistoryItem );
		virtual MDHWebHistoryItem currentItem();
		virtual MDHWebHistoryItem itemAtIndex(int);
		virtual MDHWebHistoryItem forwardItem();
		virtual int forwardListCount();
		virtual int forwardListWithLimit(int limit ,MDHWebHistoryItem*);

		//Setting Attributes
		virtual int capacity();
		virtual void setCapacity(int);
		virtual bool enabled();
		virtual void setEnabled(bool);

	protected:
		RefPtr<WebCore::BackForwardListImpl> m_backForwardList;
};

#endif
