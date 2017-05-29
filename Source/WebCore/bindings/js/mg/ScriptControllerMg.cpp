/*
 ** $Id: ScriptControllerMg.cpp 6  2011-06-07 05:37:20Z lijiangwei $
 **
 ** ScriptControllerMg.cpp: The platform implementation for ScriptController.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lijiangwei 
 **  
 ** Create date: 2011-03-21 
 */
#include "config.h"
#include "ScriptController.h"

#include "Bridge.h"
#include "PluginView.h"
#include "runtime_root.h"

namespace WebCore {

PassRefPtr<JSC::Bindings::Instance> ScriptController::createScriptInstanceForWidget(Widget* widget)
{
    if (!widget->isPluginView())
        return 0;

    return static_cast<PluginView*>(widget)->bindingInstance();
}

}
