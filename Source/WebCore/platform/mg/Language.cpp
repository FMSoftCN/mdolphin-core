/*
 ** $Id: Language.cpp 369 2010-11-25 01:52:08Z lijiangwei $
 **
 ** Language.cpp: The platform implementation for Language.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-04 
 */

#include "config.h"
#include "Language.h"

#include "PlatformString.h"

namespace WebCore {

static String g_defaultLanguage = "en";

String defaultLanguage()
{
    return g_defaultLanguage;
}

void setDefaultLanguage(const String & language)
{
    g_defaultLanguage = language;
}

}
