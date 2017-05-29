/*
** $Id: plugin.cpp 343 2007-08-8 04:58:57Z dengdd $
**
** plugin.cpp: flash plugin control registering file.
**
** Copyright (C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date:2007-8-8
*/

#include "config.h"

#if ENABLE(PLUGIN)
#include "minigui.h"
//#include "mdolphin.h"
#include "PluginView.h"
#include "PluginApiMg.h"

#include "npruntime.h"
#include "npruntime_impl.h"

namespace WebCore {

int _plugin_message(HWND hwnd, int message, WPARAM wParam, LPARAM lParam);

BOOL RegisterPluginControl (void)
{
    WNDCLASS PluginViewClass;
    PluginViewClass.spClassName = MDOLPHIN_PLUGIN;
    PluginViewClass.dwStyle = WS_NONE;
    PluginViewClass.dwExStyle = WS_EX_NONE;
    PluginViewClass.hCursor = GetSystemCursor (IDC_ARROW);
    PluginViewClass.iBkColor = COLOR_lightwhite;
    PluginViewClass.WinProc = _plugin_message;
	return RegisterWindowClass (&PluginViewClass);
}

void UnregisterPluginControl (void)
{
    UnregisterWindowClass (MDOLPHIN_PLUGIN);
}

}


//#if USE(NPOBJECT)


#ifdef __cplusplus
extern "C" {
#endif

void NPN_ReleaseVariantValue (NPVariant *variant)
{
    _NPN_ReleaseVariantValue(variant);
}

NPIdentifier NPN_GetStringIdentifier(const NPUTF8 *name)
{
    return _NPN_GetStringIdentifier(name);
}

void NPN_GetStringIdentifiers(const NPUTF8 **names, int32_t nameCount, NPIdentifier *identifiers)
{
    _NPN_GetStringIdentifiers(names, nameCount, identifiers);
}

NPIdentifier NPN_GetIntIdentifier(int32_t intid)
{
    return _NPN_GetIntIdentifier(intid);
}

bool NPN_IdentifierIsString(NPIdentifier identifier)
{
    return _NPN_IdentifierIsString(identifier);
}

NPUTF8 *NPN_UTF8FromIdentifier(NPIdentifier identifier)
{
    return _NPN_UTF8FromIdentifier(identifier);
}

int32_t NPN_IntFromIdentifier(NPIdentifier identifier)
{
    return _NPN_IntFromIdentifier(identifier);    
}

NPObject *NPN_CreateObject(NPP npp, NPClass *aClass)
{
    return _NPN_CreateObject(npp, aClass);
}

NPObject *NPN_RetainObject (NPObject *obj)
{
    return _NPN_RetainObject(obj);
}

void NPN_ReleaseObject (NPObject *obj)
{
    _NPN_ReleaseObject(obj);
}

bool NPN_Invoke(NPP npp, NPObject *npobj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    return _NPN_Invoke(npp, npobj, methodName, args, argCount, result);
}

bool NPN_InvokeDefault(NPP npp, NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    return _NPN_InvokeDefault(npp, npobj, args, argCount, result);
}

bool NPN_Evaluate(NPP npp, NPObject *npobj, NPString *script, NPVariant *result)
{
    return _NPN_Evaluate(npp, npobj, script, result);
}

bool NPN_GetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName, NPVariant *result)
{
    return _NPN_GetProperty(npp, npobj, propertyName, result);
}

bool NPN_SetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName, const NPVariant *value)
{
    return _NPN_SetProperty(npp, npobj, propertyName, value);
}

bool NPN_RemoveProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName)
{
    return _NPN_RemoveProperty(npp, npobj, propertyName);
}

bool NPN_HasProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName)
{
    return _NPN_HasProperty(npp, npobj, propertyName);
}

bool NPN_HasMethod(NPP npp, NPObject *npobj, NPIdentifier methodName)
{
    return _NPN_HasMethod(npp, npobj, methodName);
}

bool NPN_Enumerate(NPP npp, NPObject *npobj, NPIdentifier **identifier, uint32_t *count)
{
    return _NPN_Enumerate(npp, npobj, identifier, count);
}

void NPN_SetException (NPObject *obj, const NPUTF8 *message)
{
    _NPN_SetException(obj, message);
}

#ifdef __cplusplus
}  /* end extern "C" */
#endif


#endif //enable(plugin)
