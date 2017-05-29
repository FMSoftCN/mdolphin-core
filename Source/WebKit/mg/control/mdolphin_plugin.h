/**
 * \file mdolphin_plugin.h
 * \author Applications team of Feynman Software
 * \date 2007/11/01
 * 
 * This file includes necessary interfaces of mDolphin plugins.
 *
 \verbatim

    Copyright (C) 2007 Feynman Software.

    All rights reserved by Feynman Software.

    This file is part of mDolphin, a full-featured and high-end embedded web browser.

 \endverbatim
 */



#ifndef _MDOLPHIN_PLUGIN_H_
#define _MDOLPHIN_PLUGIN_H_

#include "mdconfig.h"

#if (defined(ENABLE_PLUGIN) && ENABLE_PLUGIN)

#include "npfunctions.h"

#define MAX_LEN_PLUGINNAME 64
#define MAX_LEN_MIMETYPE 64
#define MAX_LEN_SUFFIXES 128
#define MAX_LEN_DESCRIPTION 128

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


void NPN_ReleaseVariantValue (NPVariant *variant);
NPIdentifier NPN_GetStringIdentifier(const NPUTF8 *name);
void NPN_GetStringIdentifiers(const NPUTF8 **names, int32_t nameCount, NPIdentifier *identifiers);
NPIdentifier NPN_GetIntIdentifier(int32_t intid);
bool NPN_IdentifierIsString(NPIdentifier identifier);
NPUTF8 *NPN_UTF8FromIdentifier(NPIdentifier identifier);
int32_t NPN_IntFromIdentifier(NPIdentifier identifier);
NPObject *NPN_CreateObject(NPP npp, NPClass *aClass);
NPObject *NPN_RetainObject (NPObject *obj);
void NPN_ReleaseObject (NPObject *obj);
bool NPN_Invoke(NPP npp, NPObject *npobj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool NPN_InvokeDefault(NPP npp, NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool NPN_Evaluate(NPP npp, NPObject *npobj, NPString *script, NPVariant *result);
bool NPN_GetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName, NPVariant *result);
bool NPN_SetProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName, const NPVariant *value);
bool NPN_RemoveProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName);
bool NPN_HasProperty(NPP npp, NPObject *npobj, NPIdentifier propertyName);
bool NPN_HasMethod(NPP npp, NPObject *npobj, NPIdentifier methodName);
bool NPN_Enumerate(NPP npp, NPObject *npobj, NPIdentifier **identifier, uint32_t *count);
void NPN_SetException (NPObject *obj, const NPUTF8 *message);

/**
 * mdolphin plugin special interface
 */

typedef char*   (*NP_GetMIMEDescriptionProcPtr)(void);
typedef NPError (*NP_InitializeProcPtr)(NPNetscapeFuncs*, NPPluginFuncs*);
typedef NPError (*NP_ShutdownProcPtr)(void);
typedef NPError (*NP_GetValueProcPtr)(void*, NPPVariable, void*);



/**************************************************************
 *
 *here is the plugin functions
 *
 **************************************************************/

/**
 * \defgroup mdolphin_plugins mDolphin PlugIns 
 * @{
 */

/**
 * The handle of plugin.
 */
typedef unsigned int HPGN;
#define HPGN_INVALID    0

/**
 * The mDolphin plugin information structure.
 *
 * \sa mdolphin_get_plugin_info, mdolphin_get_plugin_info_by_index
 */
typedef struct _PLUGIN_INFO{
    /** Plugin name. */
	char PluginName[MAX_LEN_PLUGINNAME];
    /** Plugin's mime type. */
	char MimeType[MAX_LEN_MIMETYPE];
}PLUGIN_INFO;

/**
 * The mDolphin plugin register structure.
 *
 * \sa mdolphin_register_plugin 
 */
typedef struct _PLUGIN_REGISTER{
    /** Plugin's name. */
	char name[MAX_LEN_PLUGINNAME];
    /** Plugin's mime. */
	char mimetype[MAX_LEN_MIMETYPE];     
    /** Plugin's suffixes. */
	char suffixes[MAX_LEN_SUFFIXES]; 
    /** Plugin's description. */
	char description[MAX_LEN_DESCRIPTION]; 
    /** Plugin's initialize function. (required) */
    NP_InitializeProcPtr init;
    /** Plugin's shutdown function. (required)*/
    NP_ShutdownProcPtr shutdown;
    /** Plugin's get mime description function. (can be NULL) */
    NP_GetMIMEDescriptionProcPtr desc;
    /** Plugin's get value function. (can be NULL)*/
    NP_GetValueProcPtr getval;
}PLUGIN_REGISTER;

/**
 * \fn HPGN mdolphin_register_plugin(const PLUGIN_REGISTER * RegPgn)
 * \brief Register a plugin type on mdolphin.
 *
 * \param RegPgn The register info of a plugin.
 * \return NULL on fail, non-NULL plugin's handle on success.
*/
static inline HPGN mdolphin_register_plugin(const PLUGIN_REGISTER * RegPgn)
{
        return NULL;
}

/**
 * \fn void mdolphin_unregister_plugin(HPGN plugin)
 * \brief Unregister a plugin type. 
 *
 * \param plugin The plugin's handle.
*/
static inline void mdolphin_unregister_plugin(HPGN plugin) {}

/**
 * \fn HPGN mdolphin_get_plugin_from_mimetype(const char* mimeType)
 * \brief Find the registed plugin which support the mimeTypeGet. 
 *
 * \param mimeType The mime type which want to support.
 * \return NULL on fail ,non-NULL plugin's handle on success.
 */
static inline HPGN mdolphin_get_plugin_from_mimetype(const char* mimeType) { return NULL; }

/**
 * \fn BOOL mdolphin_get_plugin_info(HPGN plugin, PLUGIN_INFO* pluginfo)
 * \brief Get the plugin is info,and put it in the struct of PluginInfo.
 *
 * \param plugin The plugin's handle.
 * \param pluginfo The struct to store the plugin's info.
 * \return TRUE on success, FALSE on error.
*/
static inline BOOL mdolphin_get_plugin_info(HPGN plugin, PLUGIN_INFO* pluginfo)
{
    return false;
}

/**
 * \fn unsigned int mdolphin_get_plugin_counts(void)
 * \brief return the numbers of registered plugins in mdolphin.
 *
 * \return The number of the plugins.
*/
static inline unsigned int mdolphin_get_plugin_counts(void)
{
    return -1;
}

/**
 * \fn BOOL mdolphin_get_plugin_info_by_index(unsigned int index, PLUGIN_INFO* pluginfo)
 * \brief Get the plugin's info by the index.
 *
 * \param index The index of plugin which want to get info (0 base). 
 * \param pluginfo The struct to store the plugin's info.
 * \return TRUE on success, FALSE on error.
*/
static inline BOOL mdolphin_get_plugin_info_by_index(unsigned int index, PLUGIN_INFO* pluginfo) 
{
    return false;
}

/** @} end of mdolphin_plugins */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ENABLE_PLUGIN */

#endif  /* _MDOLPHIN_PLUGIN_H_ */

