/*
** $Id: MDWebInspector.cpp 1 2011-05-17 03:29:10Z lijiangwei $
**
** MDWebInspector.cpp: web inspector object implement.
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
** 
** All rights reserved by Feynman Software
**   
** Current maintainer: lijiangwei 
**  
** Create date: 2011-05-18 
*/

#include "minigui.h"
#include "mdolphin.h"
#include "config.h"

#include "MDWebInspector.h"
#include "MDWebView.h"

MDWebInspector* MDWebInspector::createInstance()
{
	MDWebInspector* instance = new MDWebInspector();
	return instance;
}

MDWebInspector::MDWebInspector()
    : m_inspectorWebView(NULL)
	, m_inspectorFrontendClient(NULL)
{
}

MDWebInspector::~MDWebInspector()
{
	close();
}

void MDWebInspector::close()
{
	if (m_inspectorWebView)
	{
		SendMessage(m_inspectorWebView->viewWindow(), MSG_CLOSE, 0, 0);
	}
	m_inspectorWebView = NULL;
}

void MDWebInspector::setInspectorWebView(IMDWebView* inspectorWebView)
{
    m_inspectorWebView = inspectorWebView;
}

void MDWebInspector::setInspectorFrontendClient(WebCore::InspectorFrontendClientMg* inspectorFrontend)
{
    m_inspectorFrontendClient = inspectorFrontend;
}
