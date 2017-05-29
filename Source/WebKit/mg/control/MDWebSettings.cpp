/*
 ** $Id: MDWebSettings.cpp 760 2011-08-18 07:22:16Z xueshuming $
 **
 ** MDWebSettings.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-12 
 */
#include "minigui.h"
#include "mdolphin.h"

#include "config.h"
#include "MDWebSettings.h"
#include "MDWebView.h"
#include "Page.h"

#include "StringHash.h"
#include "CString.h"
#include "FileSystemMg.h"

using namespace WTF;
using namespace WebCore;

HashMap<String, MDWebSettings::PropertyMeta*> MDWebSettings::_properties;


#define ADD_PROPMETA(prop, PropMeta, Getter, Setter) do { \
    static PropMeta prop##_meta;                          \
    prop##_meta.setName(#prop);                           \
    prop##_meta.getter = &MDWebSettings::Getter;          \
    prop##_meta.setter = &MDWebSettings::Setter;          \
    (void)_properties.add(prop##_meta.name(), &prop##_meta); \
}while(0)

MDWebSettings::PropertyMeta*  MDWebSettings::getPropertyMeta(const char* name, int type)
{
    if(name == NULL || type < PT_ANY || type > PT_DOUBLE)
        return NULL;

    if(_properties.isEmpty())
    {
#if 1
        do {
            static StrPropertyMeta standardFontFamily_meta;
            standardFontFamily_meta.setName("standardFontFamily");
            standardFontFamily_meta.getter = &MDWebSettings::standardFontFamily; 
            standardFontFamily_meta.setter = &MDWebSettings::setStandardFontFamily;
            (void)_properties.add(standardFontFamily_meta.name(), &standardFontFamily_meta);
        }while(0);
#else
        ADD_PROPMETA(standardFontFamily, StrPropertyMeta, standardFontFamily, setStandardFontFamily);
#endif
        ADD_PROPMETA(fixedFontFamily, StrPropertyMeta, fixedFontFamily, setFixedFontFamily);
        ADD_PROPMETA(serifFontFamily, StrPropertyMeta, serifFontFamily, setSerifFontFamily);
        ADD_PROPMETA(sansSerifFontFamily,StrPropertyMeta, sansSerifFontFamily,setSansSerifFontFamily);
        ADD_PROPMETA(cursiveFontFamily, StrPropertyMeta,cursiveFontFamily, setCursiveFontFamily);
        ADD_PROPMETA(fantasyFontFamily, StrPropertyMeta,fantasyFontFamily,setFantasyFontFamily);
#if ENABLE(FTPDIR)
        ADD_PROPMETA(ftpDirectoryTemplatePath, StrPropertyMeta, ftpDirectoryTemplatePath, setFTPDirectoryTemplatePath);
#endif

        ADD_PROPMETA(minimumFontSize, IntPropertyMeta, minimumFontSize, setMinimumFontSize);
        ADD_PROPMETA(minimumLogicalFontSize, IntPropertyMeta, minimumLogicalFontSize, setMinimumLogicalFontSize);
        ADD_PROPMETA(defaultFontSize, IntPropertyMeta, defaultFontSize, setDefaultFontSize);
        ADD_PROPMETA(defaultFixedFontSize, IntPropertyMeta, defaultFixedFontSize, setDefaultFixedFontSize);

        ADD_PROPMETA(loadsImagesAutomatically, BoolPropertyMeta, loadsImagesAutomatically, setLoadsImagesAutomatically);
        ADD_PROPMETA(isJavaScriptEnabled, BoolPropertyMeta, isJavaScriptEnabled, setJavaScriptEnabled);
        ADD_PROPMETA(isWebSecurityEnabled, BoolPropertyMeta, isWebSecurityEnabled, setWebSecurityEnabled);
        ADD_PROPMETA(allowUniversalAccessFromFileURLs, BoolPropertyMeta, allowUniversalAccessFromFileURLs, setAllowFileAccessFromFileURLs);
        ADD_PROPMETA(javaScriptCanOpenWindowsAutomatically, BoolPropertyMeta, javaScriptCanOpenWindowsAutomatically, setJavaScriptCanOpenWindowsAutomatically);
        ADD_PROPMETA(isSpatialNavigationEnabled, BoolPropertyMeta, isSpatialNavigationEnabled, setSpatialNavigationEnabled);
        ADD_PROPMETA(isJavaScriptEnabled, BoolPropertyMeta, isJavaScriptEnabled, setJavaScriptEnabled);
        ADD_PROPMETA(isMediaEnabled, BoolPropertyMeta, isMediaEnabled, setMediaEnabled);
        ADD_PROPMETA(arePluginsEnabled, BoolPropertyMeta, arePluginsEnabled, setPluginsEnabled);
        //add start 2011-05-13
        ADD_PROPMETA(areShowAllAtOnceEnabled, BoolPropertyMeta, areShowAllAtOnceEnabled, setShowAllAtOnceEnabled);
        //add end 2011-05-13
        ADD_PROPMETA(localStorageEnabled, BoolPropertyMeta, localStorageEnabled, setLocalStorageEnabled);
#if ENABLE(DOM_STORAGE)
        ADD_PROPMETA(sessionStorageQuota, IntPropertyMeta, sessionStorageQuota, setSessionStorageQuota);
#endif

        ADD_PROPMETA(privateBrowsingEnabled, BoolPropertyMeta, privateBrowsingEnabled, setPrivateBrowsingEnabled);
        ADD_PROPMETA(caretBrowsingEnabled, BoolPropertyMeta, caretBrowsingEnabled, setCaretBrowsingEnabled);

        ADD_PROPMETA(defaultTextEncodingName, StrPropertyMeta, defaultTextEncodingName, setDefaultTextEncodingName);

        ADD_PROPMETA(usesEncodingDetector, BoolPropertyMeta, usesEncodingDetector, setUsesEncodingDetector);
        ADD_PROPMETA(textAreasAreResizable, BoolPropertyMeta, textAreasAreResizable, setTextAreasAreResizable);
        ADD_PROPMETA(editableLinkBehavior, IntPropertyMeta, editableLinkBehavior, setEditableLinkBehavior);
        ADD_PROPMETA(textDirectionSubmenuInclusionBehavior, IntPropertyMeta, textDirectionSubmenuInclusionBehavior, setTextDirectionSubmenuInclusionBehavior);

        ADD_PROPMETA(developerExtrasEnabled, BoolPropertyMeta, developerExtrasEnabled, setDeveloperExtrasEnabled);

        ADD_PROPMETA(fontRenderingMode, IntPropertyMeta, fontRenderingMode, setFontRenderingMode);
        //ADD_PROPMETA(zoomMode, IntPropertyMeta, zoomMode, setZoomMode);
        ADD_PROPMETA(maximumDecodedImageSize, IntPropertyMeta, maximumDecodedImageSize, setMaximumDecodedImageSize);
        
        ADD_PROPMETA(allowScriptsToCloseWindows, BoolPropertyMeta, allowScriptsToCloseWindows, setAllowScriptsToCloseWindows);
        ADD_PROPMETA(downloadableBinaryFontsEnabled, BoolPropertyMeta, downloadableBinaryFontsEnabled, setDownloadableBinaryFontsEnabled);
        ADD_PROPMETA(acceleratedCompositingEnabled, BoolPropertyMeta, acceleratedCompositingEnabled, setAcceleratedCompositingEnabled);
        

        //....
    }
    
    PropertyMeta* pm = _properties.get(name);
    return  (pm && ((type == PT_ANY) || (type == pm->getType()))) ? pm : NULL;
}

#undef ADD_PROPMETA

//////////////////////////////////////////////////////////////////////////

WebCore::Settings * MDWebSettings::settings() {
    return m_webView->page()->settings();
}
const WebCore::Settings * MDWebSettings::settings() const {
    return m_webView->page()->settings();
}

void MDWebSettings::updateWebView() {
    if(!m_frozen && m_webView)
        m_webView->reload();
}

void MDWebSettings::setValue(const char* name, int ival)
{
    MDWebSettings::IntPropertyMeta* pm = (MDWebSettings::IntPropertyMeta*)getPropertyMeta(name, PT_INT);
    if(pm && pm->setter)
        (this->*(pm->setter))(ival);
}

void MDWebSettings::setValue(const char* name, bool bval)
{
    MDWebSettings::BoolPropertyMeta* pm = (MDWebSettings::BoolPropertyMeta*)getPropertyMeta(name, PT_BOOL);
    if(pm && pm->setter)
        (this->*(pm->setter))(bval);

}

void MDWebSettings::setValue(const char* name, const char* strval)
{
    MDWebSettings::StrPropertyMeta* pm = (MDWebSettings::StrPropertyMeta*)getPropertyMeta(name, PT_STR);
    if(pm && pm->setter)
        (this->*(pm->setter))(strval);
}

void MDWebSettings::setValue(const char* name, double dval)
{
    MDWebSettings::DoublePropertyMeta* pm = (MDWebSettings::DoublePropertyMeta*)getPropertyMeta(name, PT_DOUBLE);
    if(pm && pm->setter)
        (this->*(pm->setter))(dval);
}

int MDWebSettings::getIntValue(const char* name)
{
    MDWebSettings::IntPropertyMeta* pm = (MDWebSettings::IntPropertyMeta*)getPropertyMeta(name, PT_INT);
    if(pm && pm->getter)
        return (this->*(pm->getter))();
    return 0;
}

bool MDWebSettings::getBoolValue(const char* name)
{
    MDWebSettings::BoolPropertyMeta* pm = (MDWebSettings::BoolPropertyMeta*)getPropertyMeta(name, PT_BOOL);
    if(pm && pm->getter)
        return (this->*(pm->getter))();
    return false;
}
double MDWebSettings::getDoubleValue(const char* name)
{
    MDWebSettings::DoublePropertyMeta* pm = (MDWebSettings::DoublePropertyMeta*)getPropertyMeta(name, PT_DOUBLE);
    if(pm && pm->getter)
        return (this->*(pm->getter))();
    return 0.0;
}
int MDWebSettings::getStrValue(const char* name, char* buffer, int max)
{
    MDWebSettings::StrPropertyMeta* pm = (MDWebSettings::StrPropertyMeta*)getPropertyMeta(name, PT_STR);
    if(pm && pm->getter)
        return (this->*(pm->getter))(buffer, max);
    return 0;
}


bool MDWebSettings::getValueToString(const char* name, char* buff, int max)
{
    if(!name || !buff || max <= 0)
        return false;

    MDWebSettings::PropertyMeta* pm = getPropertyMeta(name, PT_ANY);
    if(!pm)
        return false;

    return pm->saveToString(this, buff, max);
}

bool MDWebSettings::setValueFromString(const char* name, const char* strValue)
{
    if(!name)
        return false;

    MDWebSettings::PropertyMeta* pm = getPropertyMeta(name, PT_ANY);
    if(!pm)
        return false;

    return pm->loadFromString(this, strValue);
}

bool MDWebSettings::loadEtc(const  char* fileName)
{
    bool bret = false;
    GHANDLE hetc = LoadEtcFile(fileName);

    if(hetc == 0)
        return false;

    ETCSECTION *psec = (ETCSECTION*)FindSectionInEtc(hetc, "WebSettings", FALSE);
    if(!psec)
        goto FAILED;

    frozen(true);

    for(int i = 0; i < psec->key_nr; i ++)
    {
        MDWebSettings::PropertyMeta *pm = getPropertyMeta(psec->keys[i], PT_ANY);
        if (pm)
            pm->loadFromString(this, psec->values[i]);
    }

    bret = true;

    frozen(false);

FAILED:
    UnloadEtcFile(hetc);
    return bret;

}

#if ENABLE(FILESYSTEM)
bool MDWebSettings::saveEtc(const char* fileName)
{
	GHANDLE hetc = LoadEtcFile(fileName);

    if(!hetc)
    {
        //create file
        HFile file = openFile(fileName, "w");
        if(!file)
            return false;
        closeFile(file);
        hetc = LoadEtcFile(fileName);
    }

    GHANDLE hsec = FindSectionInEtc(hetc, "WebSettings", TRUE);

    for(HashMap<String, PropertyMeta*>::iterator it = _properties.begin() ;
            it != _properties.end(); ++it)
    {
        char szbuff[256];
        PropertyMeta* pm = it->second;
        const String& str = pm->name();
        if(pm && pm->saveToString(this, szbuff, sizeof(szbuff)-1))
            SetValueToEtcSec(hsec, str.utf8().data(), szbuff);
    }

    SaveSectionToEtcFile(fileName, (ETCSECTION*)hsec);
    UnloadEtcFile(hetc);
    return true;
}
#endif

////////////////////////////////////////////////////////////////
//C API Support
#include "mdolphin-settings.h"

#define CHECK_WEBSETTINGS(hWebSettings, RET)  if(!hWebSettings) RET;
#define WEBSETTINGS(hWebSettings)  ((MDWebSettings*)(hWebSettings))

void mdSetWebSettingsStrValue(MDHWebSettings hWebSettings, const char* name, const char* strValue)
{
    CHECK_WEBSETTINGS(hWebSettings, return)

    WEBSETTINGS(hWebSettings)->setValue(name, strValue);
}

int  mdGetWebSettingsStrValue(MDHWebSettings hWebSettings, const char* name, char* buffer, int max)
{
    CHECK_WEBSETTINGS(hWebSettings, return 0)
    return WEBSETTINGS(hWebSettings)->getStrValue(name, buffer, max);
}

void mdSetWebSettingsIntValue(MDHWebSettings hWebSettings, const char* name, int ival)
{
    CHECK_WEBSETTINGS(hWebSettings, return)
    WEBSETTINGS(hWebSettings)->setValue(name, ival);

}
int  mdGetWebSettingsIntValue(MDHWebSettings hWebSettings, const char* name)
{
    CHECK_WEBSETTINGS(hWebSettings, return 0)
    return WEBSETTINGS(hWebSettings)->getIntValue(name);
}

void mdSetWebSettingsBoolValue(MDHWebSettings hWebSettings, const char* name, BOOL bval)
{
    CHECK_WEBSETTINGS(hWebSettings, return)
    WEBSETTINGS(hWebSettings)->setValue(name, bval);

}
BOOL mdGetWebSettingsBoolValue(MDHWebSettings hWebSettings, const char* name)
{
    CHECK_WEBSETTINGS(hWebSettings, return FALSE)
    return WEBSETTINGS(hWebSettings)->getBoolValue(name);
}

void   mdSetWebSettingsDoubleValue(MDHWebSettings hWebSettings, const char* name, double dval)
{
    CHECK_WEBSETTINGS(hWebSettings, return)
    WEBSETTINGS(hWebSettings)->setValue(name, dval);

}
double mdGetWebSettingsDoubleValue(MDHWebSettings hWebSettings, const char* name)
{
    CHECK_WEBSETTINGS(hWebSettings, return 0.0)
    return WEBSETTINGS(hWebSettings)->getDoubleValue(name);

}


BOOL mdWebSettingsLoad(MDHWebSettings hWebSettings, const char* fileName, int type)
{
    CHECK_WEBSETTINGS(hWebSettings, return FALSE)
    return WEBSETTINGS(hWebSettings)->load(fileName, type);
}

BOOL mdWebSettingsSave(MDHWebSettings hWebSettings, const char* fileName, int type)
{
    CHECK_WEBSETTINGS(hWebSettings, return FALSE)
    return WEBSETTINGS(hWebSettings)->save(fileName, type);
}


void mdWebSettingsFrozen(MDHWebSettings hWebSettings, BOOL bFrozen)
{
    CHECK_WEBSETTINGS(hWebSettings, return)

    return WEBSETTINGS(hWebSettings)->frozen(bFrozen);

}

#define WEBSETTING_STR_PROP(Prop, Getter, Setter)  \
int  mdGetWebSettings##Prop(MDHWebSettings hWebSettings, char* buffer, int max) \
{ \
    CHECK_WEBSETTINGS(hWebSettings, return 0); \
    return WEBSETTINGS(hWebSettings)->Getter(buffer, max); \
} \
void mdSetWebSettings##Prop(MDHWebSettings hWebSettings, const char* strValue) \
{ \
    CHECK_WEBSETTINGS(hWebSettings, return); \
    return WEBSETTINGS(hWebSettings)->Setter(strValue); \
}

WEBSETTING_STR_PROP(StandardFontFamily, standardFontFamily, setStandardFontFamily)


#undef WEBSETTING_STR_PROP

