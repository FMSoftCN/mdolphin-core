/*
 ** $Id: MDWebSettings.h 760 2011-08-18 07:22:16Z xueshuming $
 **
 ** MDWebSettings.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-12 
 */

#ifndef MDWebSettings_H
#define MDWebSettings_H

#include "IMDWebSettings.h"

#include "WTFString.h"
#include "AtomicString.h"
#include "CString.h"
#include "HashMap.h"
#include "Settings.h"
#include "MDWebView.h"
#include <wtf/Forward.h>

class MDWebSettings : public IMDWebSettings
{
public:
    MDWebSettings(MDWebView* webView) 
        : m_webView(webView) 
        , m_frozen(false)
    { }
    ~MDWebSettings() { }

    void frozen(bool bfrozen)
    {
		if (bfrozen == m_frozen) 
            return ;
        m_frozen = bfrozen;
        if(!bfrozen)
            updateWebView();
    }

    void setValue(const char* name, int ival);
    void setValue(const char* name, bool bval);
    void setValue(const char* name, const char* strval);
    void setValue(const char* name, double dval);

    int   getIntValue(const char* name);
    bool  getBoolValue(const char* name);
    double getDoubleValue(const char* name);
    int   getStrValue(const char* name, char* buffer, int max);

    bool getValueToString(const char* name, char* buff, int max);
    bool setValueFromString(const char* name, const char* strValue);


    bool load(const char* fileName, int type = ETC_FILE){
        if(type == ETC_FILE)
            return loadEtc(fileName);
        return false;
    }
    bool save(const char* fileName, int type = ETC_FILE){
#if ENABLE(FILESYSTEM)
        if(type == ETC_FILE)
            return saveEtc(fileName);
#endif
        return false;
    }


    // the settings interface
#define STR_PROP_DEFINE(Getter, Setter)  \
    int Getter(char* buffer, int max) const { \
        return setStringValue(settings()->Getter(), buffer, max); \
    } \
    void Setter(const char* strValue)      { \
        settings()->Setter(strValue);  \
    }
#if 0
    int standardFontFamily(char* buffer, int max)
    {
        return setStringValue(settings()->standardFontFamily(), buffer, max); 
    }
    void setStandardFontFamily(const char* strValue)
    {
        settings()->setStandardFontFamily(strValue);
        updateWebView();
    }
#else
    STR_PROP_DEFINE(standardFontFamily, setStandardFontFamily)
    STR_PROP_DEFINE(fixedFontFamily, setFixedFontFamily)
    STR_PROP_DEFINE(serifFontFamily, setSerifFontFamily)
    STR_PROP_DEFINE(sansSerifFontFamily, setSansSerifFontFamily)
    STR_PROP_DEFINE(cursiveFontFamily, setCursiveFontFamily)
    STR_PROP_DEFINE(fantasyFontFamily, setFantasyFontFamily)
#if ENABLE(FTPDIR)
    STR_PROP_DEFINE(ftpDirectoryTemplatePath, setFTPDirectoryTemplatePath)
#endif
#endif

#undef STR_PROP_DEFINE

#define TYPE_PROP_DEFINEEX(Type, Getter, Setter, BaseType) \
    Type Getter() const { return (Type)settings()->Getter(); } \
    void Setter(Type v) { settings()->Setter((BaseType)v); }

#define TYPE_PROP_DEFINE(Type, Getter, Setter) TYPE_PROP_DEFINEEX(Type, Getter, Setter, Type)

#define INT_PROP_DEFINEEX(Getter, Setter,BaseType)  TYPE_PROP_DEFINEEX(int, Getter, Setter, BaseType)
#define INT_PROP_DEFINE(Getter, Setter)  INT_PROP_DEFINEEX(Getter, Setter, int)
#define BOOL_PROP_DEFINE(Getter, Setter) TYPE_PROP_DEFINE(bool, Getter, Setter)
#define DBL_PROP_DEFINE(Getter, Setter)  TYPE_PROP_DEFINE(double, Getter, Setter)

    INT_PROP_DEFINE(minimumFontSize, setMinimumFontSize)
    INT_PROP_DEFINE(minimumLogicalFontSize, setMinimumLogicalFontSize)
    INT_PROP_DEFINE(defaultFontSize, setDefaultFontSize)
    INT_PROP_DEFINE(defaultFixedFontSize, setDefaultFixedFontSize)

    BOOL_PROP_DEFINE(loadsImagesAutomatically, setLoadsImagesAutomatically)
    BOOL_PROP_DEFINE(isJavaScriptEnabled, setJavaScriptEnabled)
    BOOL_PROP_DEFINE(isWebSecurityEnabled, setWebSecurityEnabled)
    BOOL_PROP_DEFINE(allowUniversalAccessFromFileURLs, setAllowUniversalAccessFromFileURLs)
    BOOL_PROP_DEFINE(allowFileAccessFromFileURLs, setAllowFileAccessFromFileURLs)
    BOOL_PROP_DEFINE(javaScriptCanOpenWindowsAutomatically, setJavaScriptCanOpenWindowsAutomatically)
    BOOL_PROP_DEFINE(javaScriptCanAccessClipboard, setJavaScriptCanAccessClipboard)
    BOOL_PROP_DEFINE(isSpatialNavigationEnabled, setSpatialNavigationEnabled)
    BOOL_PROP_DEFINE(isMediaEnabled, setMediaEnabled)
    BOOL_PROP_DEFINE(arePluginsEnabled, setPluginsEnabled)
    //added by huangsh begin 2011.4.27
    BOOL_PROP_DEFINE(areShowAllAtOnceEnabled, setShowAllAtOnceEnabled)
    //added by huangsh end 2011.4.27
    BOOL_PROP_DEFINE(localStorageEnabled, setLocalStorageEnabled)
#if ENABLE(DOM_STORAGE)
    INT_PROP_DEFINE(sessionStorageQuota, setSessionStorageQuota)
#endif

    BOOL_PROP_DEFINE(privateBrowsingEnabled, setPrivateBrowsingEnabled)
    BOOL_PROP_DEFINE(caretBrowsingEnabled, setCaretBrowsingEnabled)

   void setDefaultTextEncodingName(const char* strValue) {
        settings()->setDefaultTextEncodingName(strValue);
    }
    int defaultTextEncodingName(char* buffer, int max) const {
        return setStringValue(settings()->defaultTextEncodingName(), buffer, max);
    }
    
    BOOL_PROP_DEFINE(usesEncodingDetector, setUsesEncodingDetector)
    BOOL_PROP_DEFINE(shouldPrintBackgrounds, setShouldPrintBackgrounds)
    BOOL_PROP_DEFINE(textAreasAreResizable, setTextAreasAreResizable)
    INT_PROP_DEFINEEX(editableLinkBehavior, setEditableLinkBehavior, WebCore::EditableLinkBehavior)
    INT_PROP_DEFINEEX(textDirectionSubmenuInclusionBehavior, setTextDirectionSubmenuInclusionBehavior, WebCore::TextDirectionSubmenuInclusionBehavior)

    BOOL_PROP_DEFINE(developerExtrasEnabled, setDeveloperExtrasEnabled)

    INT_PROP_DEFINEEX(fontRenderingMode, setFontRenderingMode, WebCore::FontRenderingMode)
    //INT_PROP_DEFINEEX(zoomMode, setZoomMode,WebCore::ZoomMode)
    INT_PROP_DEFINE(maximumDecodedImageSize, setMaximumDecodedImageSize)
    
    BOOL_PROP_DEFINE(allowScriptsToCloseWindows, setAllowScriptsToCloseWindows)
    BOOL_PROP_DEFINE(downloadableBinaryFontsEnabled, setDownloadableBinaryFontsEnabled)
    BOOL_PROP_DEFINE(acceleratedCompositingEnabled, setAcceleratedCompositingEnabled)
    
    


#undef TYPE_PROP_DEFINE
#undef INT_PROP_DEFINE
#undef BOOL_PROP_DEFINE
#undef DBL_PROP_DEFINE



protected:
    MDWebView* m_webView;
    bool       m_frozen;

    WebCore::Settings * settings();
    const WebCore::Settings * settings() const;

    void updateWebView();

    bool loadEtc(const char* fileName);
#if ENABLE(FILESYSTEM)
    bool saveEtc(const char* fileName);
#endif

    int setStringValue(const AtomicString& str, char* buffer, int max) const{

        if(!buffer || max <= 0)
            return  -1;
        const WTF::String & s = (const WTF::String&)str;
        strncpy(buffer, s.utf8().data(), max-1);
        buffer[max-1] = 0;
        return strlen(buffer);
    }

////meta property
    
    // string property
    typedef int (MDWebSettings::*PGetStrValue)(char*, int) const;
    typedef void (MDWebSettings::*PSetStrValue)(const char*);

    // int property
    typedef int (MDWebSettings::*PGetIntValue)() const;
    typedef void (MDWebSettings::*PSetIntValue)(int);

    // bool property
    typedef bool (MDWebSettings::*PGetBoolValue)() const;
    typedef void (MDWebSettings::*PSetBoolValue)(bool);

    // double property
    typedef double (MDWebSettings::*PGetDoubleValue)() const;
    typedef void (MDWebSettings::*PSetDoubleValue)(double);

    // property meta
    struct PropertyMeta
    {
    private:
        WTF::String _name;
    public:
        virtual bool saveToString(MDWebSettings*, char* buffer, int max) = 0;
        virtual bool loadFromString(MDWebSettings*, const char* str) = 0;

        virtual int getType() = 0;

        const WTF::String & name() const { return _name; }
        void setName(const WTF::String& name) { _name = name; }

    };

    enum {
        PT_ANY = 0,
        PT_STR,
        PT_INT,
        PT_BOOL,
        PT_DOUBLE
    };

    static WTF::HashMap<WTF::String, PropertyMeta*> _properties;
    static PropertyMeta* getPropertyMeta(const char* name, int type);


    //define the template for implement string, int ,double and bool property meta
    template<typename Getter, typename Setter, int type>
    struct TPropertyMeta : public PropertyMeta
    {
        Getter getter;
        Setter  setter;
        TPropertyMeta() : getter(NULL), setter(NULL) { }
        int getType() { return type; }
    };

    //string propertymeta
    struct StrPropertyMeta : public TPropertyMeta<PGetStrValue, PSetStrValue, PT_STR>
    {
        bool saveToString(MDWebSettings* p, char* buffer, int max)
        {
            if(!getter || !buffer || max <= 0)
                return false;
            return (p->*getter)(buffer, max) > 0;
        }

        bool loadFromString(MDWebSettings* p, const char* str)
        {
            if(!setter)
                return false;
            (p->*setter)(str);
            return true;
        }
    };

    //int property meta
    struct IntPropertyMeta : public TPropertyMeta<PGetIntValue, PSetIntValue, PT_INT>
    {
        bool saveToString(MDWebSettings* p, char* buffer, int max)
        {
            if(!getter || !buffer || max <= 0)
                return false;
            int v = (p->*getter)();
            snprintf(buffer, max, "%d", v);
            return true;
        }
        bool loadFromString(MDWebSettings* p, const char* str)
        {
            if(!setter)
                return false;
            (p->*setter)(strtol(str, NULL, 0));
            return true;
        }
    };

    //bool property meta
    struct BoolPropertyMeta : public TPropertyMeta<PGetBoolValue, PSetBoolValue, PT_BOOL>
    {
        bool saveToString(MDWebSettings* p, char* buffer, int max)
        {
            if(!getter || !buffer || max <= 0)
                return false;
            bool v = (p->*getter)();
            strncpy(buffer, v?"true":"false", max-1);
            buffer[max] = 0;
            return true;
        }

        bool loadFromString(MDWebSettings* p, const char* str)
        {
            if(!setter)
                return false;
            bool v ;
            v = (str && (strcasecmp(str, "true") == 0 || strtol(str,NULL, 0) != 0));

            (p->*setter)(v);
            return true;
        }
    };

    //double property meta
    struct DoublePropertyMeta : public TPropertyMeta<PGetDoubleValue, PSetDoubleValue, PT_DOUBLE>
    {
        bool saveToString(MDWebSettings* p, char* buffer, int max)
        {
            if(!getter || !buffer || max <= 0)
                return false;
            double v = (p->*getter)();
            snprintf(buffer, max, "%f", v);
            return true;
        }

        bool loadFromString(MDWebSettings* p, const char* str)
        {
            if(!setter)
                return false;
            double v = 0.0;
            if( str)
                v = strtod(str, NULL);
            (p->*setter)(v);
            return true;
        }
    };

};

#endif

