/*
 ** $Id$
 **
 ** MDNativeBindingManager.h: Javascript Native Binding class
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-21 
 */

#ifndef MDNativeBindingManager_h
#define MDNativeBindingManager_h

#if (defined(ENABLE_JSNATIVEBINDING) && ENABLE_JSNATIVEBINDING)

#include "mdolphin_binding.h"
#include "ScriptController.h"


using namespace WTF;
using namespace JSC;

namespace WebCore{

class IMDNativeBindingObject {
public:
    IMDNativeBindingObject(){};
    virtual ~IMDNativeBindingObject(){};

public:
    virtual const char* nativeName() = 0;
    virtual JSStringRef propertyName() = 0;
    virtual JSValueRef propertyValue(JSContextRef context) = 0;
    virtual JSClassRef classRef() = 0;

};

class MDNativeBindingFunction : public IMDNativeBindingObject {
public:
    MDNativeBindingFunction(JSNativeFunction *nativeFunc);
    ~MDNativeBindingFunction();

    const char* nativeName(){ return m_jsNativeFunc.name; } ;
    JSStringRef propertyName();
    JSValueRef propertyValue(JSContextRef context);
    JSClassRef classRef(){return NULL;};
private:
    JSNativeFunction m_jsNativeFunc;
};

class MDNativeBindingClass : public IMDNativeBindingObject {
public:
    MDNativeBindingClass(JSNativeClass *nativeClass);
    ~MDNativeBindingClass();

    const char* nativeName() { return m_jsClass.className; };
    JSStringRef propertyName();
    JSValueRef propertyValue(JSContextRef context);
    JSClassRef classRef() {return m_classRef;};
private:
    JSClassDefinition m_jsClass;
    JSClassRef m_classRef;
};


class MDNativeBindingInterface : public IMDNativeBindingObject {
public:
    MDNativeBindingInterface(JSNativeInterface *nativeInterface);
    ~MDNativeBindingInterface();

    const char* nativeName() { return m_jsClass.className; };
    JSStringRef propertyName();
    JSValueRef propertyValue(JSContextRef context);
    JSClassRef classRef() {return m_classRef;};
private:
    JSClassDefinition m_jsClass;
    JSClassRef m_classRef;
};

class MDNativeBindingManager {
public:
    static MDNativeBindingManager* sharedInstance();
    static void destroyInstance();
    
    bool registerJSNativeClass(JSNativeClass *nativeClass);
    bool unregisterJSNativeClass(JSNativeClass *nativeClass);

    bool registerJSNativeInterface(JSNativeInterface *nativeInterface);
    bool unregisterJSNativeInterface(JSNativeInterface *nativeInterface);

    bool registerJSNativeFunction(JSNativeFunction *nativeFunc);
    bool unregisterJSNativeFunction(JSNativeFunction *nativeFunc);

    int registerNativeJSObjectsToContext(ScriptController *script, DOMWrapperWorld* world);

    int lookupJSNativeObject(const char* name);

    JSClassRef  lookupJSNativeClass(const char* name);

private:
    MDNativeBindingManager();
    ~MDNativeBindingManager();

    typedef Vector<IMDNativeBindingObject*> MDJSClassRegisterTable;
    MDJSClassRegisterTable m_jsTable ;

    void registerJSNativeObject(IMDNativeBindingObject* object) { m_jsTable.append(object); }; 
    void unregisterJSNativeObject (int i);


};

}

#endif

#endif
