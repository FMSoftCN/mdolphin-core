/*
 ** $Id: MDWebSettings.h 236 2010-07-28 01:14:50Z lijiangwei $
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

#ifndef IMDWebSettings_H
#define MDWebSettings_H

class IMDWebSettings
{
public:
    virtual void frozen(bool bfrozen) = 0;

    virtual void setValue(const char* name, int ival) = 0;
    virtual void setValue(const char* name, bool bval) = 0;
    virtual void setValue(const char* name, const char* strval) = 0;
    virtual void setValue(const char* name, double dval) = 0;

    virtual int   getIntValue(const char* name) = 0;
    virtual bool  getBoolValue(const char* name) = 0;
    virtual double getDoubleValue(const char* name) = 0;
    virtual int   getStrValue(const char* name, char* buffer, int max) = 0;

    virtual bool getValueToString(const char* name, char* buff, int max) = 0;
    virtual bool setValueFromString(const char* name, const char* strValue) = 0;


    enum {
        ETC_FILE,
        XML_FILE //don't suppport now
    };

    virtual bool load(const char* fileName, int type = ETC_FILE) = 0; 
    virtual bool save(const char* fileName, int type = ETC_FILE) = 0; 


    virtual void setStandardFontFamily(const char *) = 0;
    virtual int standardFontFamily(char *, int) const  = 0;

    virtual void setFixedFontFamily(const char *) = 0;
    virtual int fixedFontFamily(char *, int) const  = 0;

    virtual void setSerifFontFamily(const char *) = 0;
    virtual int serifFontFamily(char *, int) const  = 0;

    virtual void setSansSerifFontFamily(const char *) = 0;
    virtual int sansSerifFontFamily(char *, int) const  = 0;

    virtual void setCursiveFontFamily(const char *) = 0;
    virtual int cursiveFontFamily(char *, int) const  = 0;

    virtual void setFantasyFontFamily(const char *) = 0;
    virtual int fantasyFontFamily(char *, int) const  = 0;

    virtual void setMinimumFontSize(int) = 0;
    virtual int minimumFontSize() const  = 0;

    virtual void setMinimumLogicalFontSize(int) = 0;
    virtual int minimumLogicalFontSize() const  = 0;

    virtual void setDefaultFontSize(int) = 0;
    virtual int defaultFontSize() const  = 0;

    virtual void setDefaultFixedFontSize(int) = 0;
    virtual int defaultFixedFontSize() const  = 0;

    // Unlike areImagesEnabled, this only suppresses the network load of
    // the image URL.  A cached image will still be rendered if requested.
    virtual void setLoadsImagesAutomatically(bool) = 0;
    virtual bool loadsImagesAutomatically() const  = 0;

    virtual void setJavaScriptEnabled(bool) = 0;
    // Instead of calling isJavaScriptEnabled directly, please consider calling
    // ScriptController::canExecuteScripts, which takes things like the
    // HTML sandbox attribute into account.
    virtual bool isJavaScriptEnabled() const  = 0;

    virtual void setWebSecurityEnabled(bool) = 0;
    virtual bool isWebSecurityEnabled() const  = 0;

    virtual void setAllowUniversalAccessFromFileURLs(bool) = 0;
    virtual bool allowUniversalAccessFromFileURLs() const  = 0;

    virtual void setAllowFileAccessFromFileURLs(bool) = 0;
    virtual bool allowFileAccessFromFileURLs() const  = 0;

    virtual void setJavaScriptCanOpenWindowsAutomatically(bool) = 0;
    virtual bool javaScriptCanOpenWindowsAutomatically() const  = 0;

    virtual void setJavaScriptCanAccessClipboard(bool) = 0;
    virtual bool javaScriptCanAccessClipboard() const  = 0;

    virtual void setSpatialNavigationEnabled(bool) = 0;
    virtual bool isSpatialNavigationEnabled() const  = 0;

    virtual void setMediaEnabled(bool) = 0;
    virtual bool isMediaEnabled() const  = 0;

    virtual void setPluginsEnabled(bool) = 0;
    virtual bool arePluginsEnabled() const  = 0;

    //added by huangsh begin 2011.4.27
    virtual bool areShowAllAtOnceEnabled() const  = 0;
    //added by huangsh end  2011.4.27
    
    virtual void setLocalStorageEnabled(bool) = 0;
    virtual bool localStorageEnabled() const  = 0;

#if ENABLE(DOM_STORAGE)        

    // Allow clients concerned with memory consumption to set a quota on session storage
    // since the memory used won't be released until the Page is destroyed.
    // Default is noQuota.
    virtual void setSessionStorageQuota(int) = 0;
    virtual int sessionStorageQuota() const  = 0;
#endif

    virtual void setPrivateBrowsingEnabled(bool) = 0;
    virtual bool privateBrowsingEnabled() const  = 0;

    virtual void setCaretBrowsingEnabled(bool) = 0;
    virtual bool caretBrowsingEnabled() const  = 0;

    virtual void setDefaultTextEncodingName(const char *) = 0;
    virtual int defaultTextEncodingName(char *, int) const  = 0;

    virtual void setUsesEncodingDetector(bool) = 0;
    virtual bool usesEncodingDetector() const  = 0;

    virtual void setShouldPrintBackgrounds(bool) = 0;
    virtual bool shouldPrintBackgrounds() const  = 0;

    virtual void setTextAreasAreResizable(bool) = 0;
    virtual bool textAreasAreResizable() const  = 0;

#if ENABLE(DASHBOARD_SUPPORT)
    virtual void setUsesDashboardBackwardCompatibilityMode(bool) = 0;
    virtual bool usesDashboardBackwardCompatibilityMode() const  = 0;
#endif


#if ENABLE(FTPDIR)
    virtual void setFTPDirectoryTemplatePath(const char *) = 0;
    virtual int ftpDirectoryTemplatePath(char *, int) const  = 0;
#endif

    virtual void setMaximumDecodedImageSize(int size)  = 0;
    virtual int maximumDecodedImageSize() const  = 0;

    virtual void setAllowScriptsToCloseWindows(bool) = 0;
    virtual bool allowScriptsToCloseWindows() const  = 0;

    virtual void setDownloadableBinaryFontsEnabled(bool) = 0;
    virtual bool downloadableBinaryFontsEnabled() const  = 0;

    virtual void setAcceleratedCompositingEnabled(bool) = 0;
    virtual bool acceleratedCompositingEnabled() const  = 0;

};



#endif

