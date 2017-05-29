/*
 ** $Id$
 **
 ** MDNativeBindingManager.cpp: Javascript Native Binding class
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-07-21 
 */

#include "minigui.h"

#include "mdconfig.h"
#include "config.h"
#include "Vector.h"
#include "MDNativeBindingManager.h"
#include "APICast.h"
#include "HashMap.h"
#include "WTFString.h"

#if ENABLE(JSNATIVEBINDING)

using namespace WTF;

namespace WebCore{

MDNativeBindingFunction::MDNativeBindingFunction(JSNativeFunction *nativeFunc)
{
    ASSERT(nativeFunc);

    memcpy((void *)&m_jsNativeFunc, (void *)nativeFunc, sizeof(*nativeFunc));
}

MDNativeBindingFunction::~MDNativeBindingFunction()
{
}

JSStringRef MDNativeBindingFunction::propertyName()
{
    return JSStringCreateWithUTF8CString(m_jsNativeFunc.name);
}

JSValueRef MDNativeBindingFunction::propertyValue(JSContextRef context)
{
    JSStringRef strRef = JSStringCreateWithUTF8CString(m_jsNativeFunc.name);
    JSValueRef valueRef = JSObjectMakeFunctionWithCallback(context, strRef, m_jsNativeFunc.callAsFunction);
    JSStringRelease(strRef);
    return valueRef;

}

MDNativeBindingClass::MDNativeBindingClass(JSNativeClass *nativeClass)
{

    ASSERT(nativeClass);

    memset(&m_jsClass, 0x0, sizeof(m_jsClass));

    m_jsClass.className = nativeClass->name;
    m_jsClass.staticFunctions = static_cast<JSStaticFunction*>(nativeClass->method);
    m_jsClass.staticValues = nativeClass->property;
    m_jsClass.callAsConstructor = nativeClass->ctor;
    m_jsClass.finalize = nativeClass->dtor;

    m_classRef = JSClassCreate(&m_jsClass);
}

MDNativeBindingClass::~MDNativeBindingClass()
{
    JSClassRelease(m_classRef);
}

JSStringRef MDNativeBindingClass::propertyName()
{
    return JSStringCreateWithUTF8CString(m_jsClass.className);
}

JSValueRef MDNativeBindingClass::propertyValue(JSContextRef context)
{
    return JSObjectMakeConstructor(context,
            m_classRef, m_jsClass.callAsConstructor);
}

//interface
MDNativeBindingInterface::MDNativeBindingInterface(JSNativeInterface *nativeInterface)
{

    ASSERT(nativeInterface);

    memset(&m_jsClass, 0x0, sizeof(m_jsClass));

    m_jsClass.className = nativeInterface->name;
    m_jsClass.staticFunctions = static_cast<JSStaticFunction*>(nativeInterface->method);
    m_jsClass.staticValues = nativeInterface->property;

    m_classRef = JSClassCreate(&m_jsClass);
}

MDNativeBindingInterface::~MDNativeBindingInterface()
{
    JSClassRelease(m_classRef);
}

JSStringRef MDNativeBindingInterface::propertyName()
{
    return JSStringCreateWithUTF8CString(m_jsClass.className);
}

JSValueRef MDNativeBindingInterface::propertyValue(JSContextRef context)
{
    return JSObjectMake(context, m_classRef,  NULL);
}

//end interface

MDNativeBindingManager* MDNativeBindingManager::sharedInstance()
{
    static MDNativeBindingManager s_MDNativeBindingManager;
    return &s_MDNativeBindingManager;
}

void MDNativeBindingManager::destroyInstance()
{

}

bool MDNativeBindingManager::registerJSNativeClass(JSNativeClass *nativeClass)
{
    if(!nativeClass || lookupJSNativeObject(nativeClass->name) >= 0)
        return false;

     MDNativeBindingClass *bindingClass = new MDNativeBindingClass(nativeClass);
     registerJSNativeObject(bindingClass);
     return true;
}

int MDNativeBindingManager::lookupJSNativeObject(const char * name)
{
    if (!name)
        return -1;

    IMDNativeBindingObject *object = NULL;
    const char* objName = NULL;
    size_t size = m_jsTable.size();
    for (size_t i=0; i<size; i++) {
        object  = m_jsTable.at(i);
        objName =  object->nativeName(); 

        if (objName) 
            if (! strcmp(name, objName)) 
                return i;
    }

    return -1;
}

JSClassRef  MDNativeBindingManager::lookupJSNativeClass(const char* name)
{
    int i; 
    if((i = lookupJSNativeObject(name)) < 0)
        return NULL;

   IMDNativeBindingObject *object =  m_jsTable.at(i);
   return object->classRef();
}

bool MDNativeBindingManager::unregisterJSNativeClass(JSNativeClass *nativeClass)
{
    int i; 
    if(!nativeClass || (i = lookupJSNativeObject(nativeClass->name)) < 0)
        return false;

    unregisterJSNativeObject(i);
    return true;
}

bool MDNativeBindingManager::registerJSNativeInterface(JSNativeInterface *nativeInterface)
{

    if(!nativeInterface || lookupJSNativeObject(nativeInterface->name) >= 0)
        return false;

     MDNativeBindingInterface *bindingInterface = new MDNativeBindingInterface(nativeInterface);
     registerJSNativeObject(bindingInterface);
     return true;
}

bool MDNativeBindingManager::unregisterJSNativeInterface(JSNativeInterface *nativeInterface) 
{

    int i; 
    if(!nativeInterface || (i = lookupJSNativeObject(nativeInterface->name)) < 0)
        return false;

    unregisterJSNativeObject(i);
    return true;
}

bool MDNativeBindingManager::registerJSNativeFunction(JSNativeFunction *nativeFunc)
{
    if(!nativeFunc || lookupJSNativeObject(nativeFunc->name) >= 0)
        return false;

     MDNativeBindingFunction *bindingFunc = new MDNativeBindingFunction(nativeFunc);
     registerJSNativeObject(bindingFunc);

     return true;
}

bool MDNativeBindingManager::unregisterJSNativeFunction(JSNativeFunction *nativeFunc)
{
    int i; 
    if(!nativeFunc || (i = lookupJSNativeObject(nativeFunc->name)) < 0)
        return false;

     unregisterJSNativeObject(i);
     return true;
}


void MDNativeBindingManager::unregisterJSNativeObject(int i)
{
    //FIXME:  cancel  deleting js property from global object  
    // you should the js property in all JS context not only current JS Context
    // and make sure there is not instance of current native object 
    // please read http://rdwiki.rd.minigui.com/bin/view/Applications/MDCoreDDV3dot0dot0JSNativeBinding

    delete m_jsTable.at(i);
    m_jsTable.remove(i);
}

int MDNativeBindingManager::registerNativeJSObjectsToContext(ScriptController *script, DOMWrapperWorld* world)
{
    DOMWrapperWorld* nativeWorld = (world)? world : mainThreadNormalWorld();
    JSDOMWindow*    window =  (script->globalObject(nativeWorld));
    //toJSDOMWindow
    JSContextRef context = reinterpret_cast<JSContextRef>(window->globalExec()); 
    JSObjectRef globalObject = JSContextGetGlobalObject(context);

    IMDNativeBindingObject *object = NULL;
    size_t size = m_jsTable.size();
    
    for (size_t i=0; i<size; i++) {
        object = m_jsTable.at(i);
        JSStringRef propertyName = object->propertyName(); //entry->m_propertyName;
        JSObjectSetProperty(
                context,
                globalObject,
                propertyName,
                object->propertyValue(context),
                kJSPropertyAttributeDontEnum | kJSPropertyAttributeDontDelete, NULL);

        JSStringRelease(propertyName);
    }

    return 0;
}

MDNativeBindingManager::MDNativeBindingManager()
{

}

MDNativeBindingManager::~MDNativeBindingManager()
{
    for (size_t i = 0; i<m_jsTable.size(); i++)
        delete m_jsTable.at(i);
}

}

static inline  WTF::String JSStringRefToString(JSStringRef propertyName) 
{
  return  WTF::String(JSStringGetCharactersPtr(propertyName));  
}

typedef HashMap<WTF::String, void*> InterfacePropertyObject;
JSValueRef JSObjectMakeFromInterface(JSContextRef context, JSObjectRef thisObj, 
                    JSStringRef propertyName, JSNativeInterface *interface) 
{
        //FIXME: this code will lead a  recursion Infinitly
        /*
        if (JSObjectHasProperty(context, thisObj, propertyName))
            return JSObjectGetProperty(context, thisObj, propertyName, NULL);
        */
        
        InterfacePropertyObject* propertyObject = (InterfacePropertyObject*)JSObjectGetPrivate(thisObj);
        if (!propertyObject) {
            propertyObject = new  InterfacePropertyObject;
            JSObjectSetPrivate(thisObj, (void *)propertyObject);
        }

        JSObjectRef object;
        WTF::String identName = JSStringRefToString(propertyName);

        if (propertyObject->find(identName) == propertyObject->end()) {
            JSClassDefinition jsClass;
            memset(&jsClass, 0, sizeof(jsClass));

            jsClass.className = interface->name;
            jsClass.staticFunctions = static_cast<JSStaticFunction*>(interface->method);
            jsClass.staticValues = interface->property;

            JSClassRef classRef = JSClassCreate(&jsClass);

            object = JSObjectMake(context, classRef, NULL);
            
            /*
            JSObjectSetProperty(
                    context,
                    thisObj,
                    propertyName,
                    object,
                    kJSPropertyAttributeDontEnum | kJSPropertyAttributeDontDelete, NULL);
            */

            propertyObject->add(identName,(void *)object);
        }
        else {
            object =(JSObjectRef) propertyObject->get(identName);
        }

        return object;
}

#ifdef __cplusplus
extern "C" {
#endif

using namespace  WebCore;
BOOL mdDefineJSNativeClass(JSNativeClass *nativeClass)
{
    MDNativeBindingManager* instance = MDNativeBindingManager::sharedInstance();
    return ((instance->registerJSNativeClass(nativeClass)) ? TRUE : FALSE);
}

JSClassRef mdLookupJSNativeClass(const char* name)
{
    MDNativeBindingManager* instance = MDNativeBindingManager::sharedInstance();
    return instance->lookupJSNativeClass(name);
}

BOOL mdUndefineJSNativeClass(JSNativeClass *nativeClass)
{
    MDNativeBindingManager* instance = MDNativeBindingManager::sharedInstance();
    return (instance->unregisterJSNativeClass(nativeClass) ? TRUE : FALSE);
}

BOOL mdDefineJSNativeInterface(JSNativeInterface *nativeInterface)
{
    MDNativeBindingManager* instance = MDNativeBindingManager::sharedInstance();
    return ((instance->registerJSNativeInterface(nativeInterface)) ? TRUE : FALSE);
}

BOOL mdUndefineJSNativeInterface(JSNativeInterface *nativeInterface)
{
    MDNativeBindingManager* instance = MDNativeBindingManager::sharedInstance();
    return (instance->unregisterJSNativeInterface(nativeInterface) ? TRUE : FALSE);
}

BOOL mdDefineJSNativeFunction(JSNativeFunction *func)
{
    MDNativeBindingManager* instance = MDNativeBindingManager::sharedInstance();
    return (instance->registerJSNativeFunction(func) ? TRUE : FALSE);
}

BOOL mdUndefineJSNativeFunction(JSNativeFunction *func)
{
    MDNativeBindingManager* instance = MDNativeBindingManager::sharedInstance();
    return (instance->unregisterJSNativeFunction(func) ? TRUE : FALSE);
}

JSValueRef mdJSObjectMakeFromInterface(JSContextRef context, JSObjectRef thisObj, 
                    JSStringRef propertyName, JSNativeInterface *interface) 
{
    return JSObjectMakeFromInterface(context, thisObj, propertyName,interface) ;
}

#ifdef __cplusplus
}
#endif

#endif //ENABLE(JSNATIVEBINDING)
