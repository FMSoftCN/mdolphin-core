/*
 * Copyright (C) 2008 Kevin Ollivier <kevino@theolliviers.com> All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "PluginPackage.h"

#include <stdio.h>
#include <dlfcn.h>

#include "MIMETypeRegistry.h"
#include "NotImplemented.h"
#include "npruntime_impl.h"
#include "PluginDebug.h"
#include <wtf/text/CString.h>

#define DBG()  printf("%s-%d-%s\n", __FILE__, __LINE__, __FUNCTION__)

namespace WebCore {

#if ENABLE(PLUGIN)
void PluginPackage::determineQuirks(const String&)
{
}

typedef char*   (*NP_GetMIMEDescriptionFuncPtr)(void);
typedef NPError (*NP_InitializeProcPtr)(NPNetscapeFuncs*, NPPluginFuncs*);


bool PluginPackage::fetchInfo()
{
    if (!load())
        return false;

    NP_GetMIMEDescriptionFuncPtr NP_GetMIMEDescription = 0;
    NPP_GetValueProcPtr NPP_GetValue = 0;

    NP_GetMIMEDescription = (NP_GetMIMEDescriptionFuncPtr)dlsym(m_module, "NP_GetMIMEDescription");
    NPP_GetValue = (NPP_GetValueProcPtr)dlsym(m_module, "NP_GetValue");

    if (!NP_GetMIMEDescription || !NPP_GetValue)
        return false;

    char* buffer = 0;
    NPError err = NPP_GetValue(0, NPPVpluginNameString, &buffer);
    if (err == NPERR_NO_ERROR)
        m_name = buffer;

    buffer = 0;
    err = NPP_GetValue(0, NPPVpluginDescriptionString, &buffer);
    if (err == NPERR_NO_ERROR) {
        m_description = buffer;
        determineModuleVersionFromDescription();
    }

    const char* types = NP_GetMIMEDescription();
    if (!types)
        return true;

    Vector<String> mimeDescs;
    String strTypes(types);
    strTypes.split(';', false, mimeDescs);
    int mimeDescsLength = mimeDescs.size();
    
    for (int i = 0; i< mimeDescsLength; i++) {
        Vector<String> mimeData;
        mimeDescs[i].split(':', true, mimeData);

        if (mimeData.size() < 3) {
            fprintf(stderr, "mimeData length < 3");
            continue;
        }

        Vector<String> extensions;
        mimeData[1].split(',', extensions);
        m_mimeToExtensions.add(mimeData[0], extensions);

        m_mimeToDescriptions.add(mimeData[0], mimeData[2]);

        determineQuirks(mimeData[0]);
    }

    return true;
}

bool PluginPackage::load()
{
    if (m_isLoaded) {
        m_loadCount++;
        return true;
    }

    m_module = dlopen(m_path.utf8().data(), RTLD_LAZY);

    printf("Module Load %s\n", (m_path.utf8()).data());
    if (!m_module) {
        LOG(Plugins,"Module Load Failed :%s\n", (m_path.utf8()).data());
        perror("Module Load Failed");
        return false;
    }

    m_isLoaded = true;

    NP_InitializeProcPtr NP_Initialize = 0;
    m_NPP_Shutdown = 0;

    NP_Initialize = (NP_InitializeProcPtr)dlsym(m_module, "NP_Initialize");
    m_NPP_Shutdown = (NPP_ShutdownProcPtr)dlsym(m_module, "NP_Shutdown");

    if (!NP_Initialize || !m_NPP_Shutdown)
        goto abort;

    NPError npErr;


    memset(&m_pluginFuncs, 0, sizeof(m_pluginFuncs));
    m_pluginFuncs.size = sizeof(m_pluginFuncs);


    initializeBrowserFuncs();
    npErr = NP_Initialize(&m_browserFuncs, &m_pluginFuncs);

    if (npErr != NPERR_NO_ERROR)
        goto abort;
    m_loadCount++;
    return true;





abort:
    unloadWithoutShutdown();
    return false;
}

#if ENABLE(NETSCAPE_PLUGIN_API)
uint16_t PluginPackage::NPVersion() const
{
    return 0;
}
#endif

#endif //ENABLE(PLUGIN)
}
