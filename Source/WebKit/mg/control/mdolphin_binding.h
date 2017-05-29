/**
 * \file mdolphin_nativebinding.h
 * \author Applications team of Feynman Software
 * \date 2007/12/16
 * 
 * This file includes necessary interfaces of mDolphin.
 *
 \verbatim

    Copyright (C) 2007 Feynman Software.

    All rights reserved by Feynman Software.

    This file is part of mDolphin, a full-featured and high-end embedded web browser.

 \endverbatim
 */

#ifndef MDolphinBinding_h
#define MDolphinBinding_h

#include "mdconfig.h"

#ifdef ENABLE_JSNATIVEBINDING

#include "JSBase.h"
#include "JSContextRef.h"
#include "JSStringRef.h"
#include "JSObjectRef.h"
#include "JSValueRef.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup mdolphin_nativebinding mDolphin Javascript Native Binding
 * @{
 */

/**
 * The JSConstructor defines the type of the native implementation of
 * the JS object constructor.
 */
typedef JSObjectCallAsConstructorCallback JSConstructor;

/**
 * The JSDestructor defines the type of the native implementation of
 * the JS object destructor.
 */
typedef JSObjectFinalizeCallback JSDestructor;

/**
 * The JSMethod describes the type of the native implementation of
 * the JS object methods.
 */
typedef JSStaticFunction JSMethod;

/**
 * The JSNativeFunction defines the type of the native implementation of
 * the JS function.
 */
typedef JSStaticFunction JSNativeFunction;

/**
 * The JSMethod describes the type of the native implementation of
 * the JS object properties.
 */
typedef JSStaticValue JSProperty;

/**
 * A JSNativeClass describes a class of the Javascript native binding objects.
 * A C/C++ programe can use JSNativeClass with mdDefineJSNativeClass API to 
 * create objects that have custom methods and properties implemented in C/C++.
 */
typedef struct _JSNativeClass {
    /** Class name */
    const char    *name;
    /** JavaScript native binding version: default 0 */
    int           version;
    /** Pointer to the constructor for the object that represents this class */
    JSConstructor ctor;
    /** Pointer to the destructor for the object that represents this class */
    JSDestructor  dtor;
    /** Pointer to the methods for the object that represents this class */
    JSMethod      *method;
    /** Pointer to the properties for the object that represents this class */
    JSProperty    *property; 
}JSNativeClass;

typedef struct _JSNativeInterface {
    /** Class name */
    const char    *name;
    /** JavaScript native binding version: default 0 */
    int           version;
    /** Pointer to the methods for the object that represents this class */
    JSMethod      *method;
    /** Pointer to the properties for the object that represents this class */
    JSProperty    *property; 
}JSNativeInterface;

/**
 * \fn BOOL mdDefineJSNativeFunction(JSNativeFunction *func)
 * \brief Define a javascript native binding function.
 *
 * \param func The pointer to the Native Function.
 * \return TRUE if the Native Function was defined, FALSE on otherwise. 
 */
BOOL mdDefineJSNativeFunction(JSNativeFunction *func);

/**
 * \fn BOOL mdUndefineJSNativeFunction(JSNativeFunction *func)
 * \brief Undefine a javascript native binding function.
 *
 * \param func The pointer to the Native Function 
 * \return TRUE if the Native Function was undefined, FALSE on otherwise.
 */
BOOL mdUndefineJSNativeFunction(JSNativeFunction *func);

/**
 * \fn BOOL mdDefineJSNativeClass(JSNativeClass *nativeClass)
 * \brief Define a javascript native binding class.
 *
 * \param nativeClass The pointer to the Native Class.
 * \return TRUE if the Native Class was defined, FALSE on otherwise.
 */

BOOL mdDefineJSNativeClass(JSNativeClass *nativeClass);

/**
 * \fn BOOL mdUndefineJSNativeClass(JSNativeClass *nativeClass)
 * \brief Undefine a javascript native binding class.
 *
 * \param nativeClass The pointer to the Native Class
 * \return TRUE if the Native Class was undefined, FALSE on otherwise.
 */
BOOL mdUndefineJSNativeClass(JSNativeClass *nativeClass);

/**
 * \fn BOOL mdDefineJSNativeInterface(JSNativeInterface *nativeInterface)
 * \brief Define a javascript native binding interface.
 *
 * \param nativeInterface The pointer to the Native Interface.
 * \return TRUE if the Native Interface was defined, FALSE on otherwise. 
 */
BOOL mdDefineJSNativeInterface(JSNativeInterface *nativeInterface);

/**
 * \fn BOOL mdUndefineJSNativeInterface(JSNativeInterface *nativeInterface)
 * \brief Undefine a javascript native binding function.
 *
 * \param func The pointer to the Native Interface.
 * \return TRUE if the Native Interface was undefined, FALSE on otherwise. 
 */
BOOL mdUndefineJSNativeInterface(JSNativeInterface *nativeInterface);

/**
 * \fn JSValueRef mdJSObjectMakeFromInterface(JSContextRef context, JSObjectRef thisObj, 
                    JSStringRef propertyName, JSNativeInterface *interface);
 * \brief Make a javascript object binding to the propertyName of thisObj.
 *
 * \param context The javascript context.
 * \param thisObj The javascript object.
 * \param propertyName The property name of thisObj.
 * \param interface The pointer to the Native Interface, binding to propertyName.
 * \return non-NULL JSValueRef on success, NULL on fail.
 */
JSValueRef mdJSObjectMakeFromInterface(JSContextRef context, JSObjectRef thisObj, 
                    JSStringRef propertyName, JSNativeInterface *interface);

/**
 * \fn JSClassRef mdLookupJSNativeClass(const char* name)
 * \brief Search a javascript native binding class/interface with the class name.
 *
 * \param name The name of the Native Class OR Native Interface.
 * \return non-NULL JSClassRef on success, NULL on fail.
 */
JSClassRef mdLookupJSNativeClass(const char* name);

//////////////////////////////////// compatible mdolphin 2.0 //////////////////////////////

/**
 * \fn int mdolphin_define_jsnativefunction(JSNativeFunction *func)
 * \brief define a javascript native binding function.
 *
 * \param func The description of the Native Function 
 * \return TRUE on success, FALSE on fail.
 */
static inline BOOL mdolphin_define_jsnativefunction(JSNativeFunction *func)
{
    return mdDefineJSNativeFunction(func);
}

/**
 * \fn BOOL mdolphin_undefine_jsnativefunction(JSNativeFunction *func)
 * \brief undefine a javascript native binding function.
 *
 * \param func The description of the Native Function 
 * \return TRUE on success, FALSE on fail.
 */
static inline BOOL mdolphin_undefine_jsnativefunction(JSNativeFunction *func)
{
    return mdUndefineJSNativeFunction(func);
}

/**
 * \fn BOOL mdolphin_define_jsnativeclass(JSNativeClass *nativeClass)
 * \brief define a javascript native binding class.
 *
 * \param nativeClass The description of the Native Class
 * \return TRUE on success, FALSE on fail.
 */
static inline BOOL mdolphin_define_jsnativeclass(JSNativeClass *nativeClass)
{
    return mdDefineJSNativeClass(nativeClass);
}

/**
 * \fn BOOL mdolphin_undefine_jsnativeclass(JSNativeClass *nativeClass)
 * \brief undefine a javascript native binding class.
 *
 * \param nativeClass The description of the Native Class
 * \return TRUE on success, FALSE on fail.
 */
static inline BOOL mdolphin_undefine_jsnativeclass(JSNativeClass *nativeClass)
{
    return mdUndefineJSNativeClass(nativeClass);
}

/**
 * \fn BOOL mdolphin_lookup_jsnativeclass(const char* name)
 * \brief search a javascript native binding class with the class name.
 *
 * \param name The name of the Native Class
 * \return non-NULL JSClassRef on success, NULL on fail.
 */
static inline JSClassRef mdolphin_lookup_jsnativeclass(const char* name)
{
    return  mdLookupJSNativeClass(name);
}

/** @} end of mdolphin_binding*/

#ifdef __cplusplus
}
#endif

#endif /* ENABLE_JSNATIVEBINDING */
#endif /* MDolphinNativeBinding_h */
