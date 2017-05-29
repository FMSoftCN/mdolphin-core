/*
 ** $Id: mdolphin-settings.h 184 2010-07-15 02:58:18Z dongjunjie $
 **
 ** mdolphin-settings.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: dongjunjie 
 **  
 ** Create date: 2010-07-12 
 */

#ifndef MDOLPHIN_SETTINGS_H
#define MDOLPHIN_SETTINGS_H

/*
 *  the C API of class MDWebSettings
 */

#ifdef __cplusplus
extern "C" {
#endif
typedef void *MDHWebSettings;


MDHWebSettings * mdGetWebSettings(HWND hwnd);

void mdSetWebSettingsStrValue(MDHWebSettings hWebSettings, const char* name, const char* strValue);
int  mdGetWebSettingsStrValue(MDHWebSettings hWebSettings, const char* name, char* buffer, int max);

void mdSetWebSettingsIntValue(MDHWebSettings hWebSettings, const char* name, int ival);
int  mdGetWebSettingsIntValue(MDHWebSettings hWebSettings, const char* name);

void mdSetWebSettingsBoolValue(MDHWebSettings hWebSettings, const char* name, BOOL bval);
BOOL mdGetWebSettingsBoolValue(MDHWebSettings hWebSettings, const char* name);

void   mdSetWebSettingsDoubleValue(MDHWebSettings hWebSettings, const char* name, double dval);
double mdGetWebSettingsDoubleValue(MDHWebSettings hWebSettings, const char* name);

#define MD_WEBSETTINGS_ETCFILE 0
#define MD_WEBSETTINGS_XMLFILE 1
BOOL mdWebSettingsLoad(MDHWebSettings hWebSettings, const char* fileName, int type);
BOOL mdWebSettingsSave(MDHWebSettings hWebSettings, const char* fileName, int type);

void mdWebSettingsFrozen(MDHWebSettings hWebSettings, BOOL bFrozen);


int  mdGetWebSettingsStandardFontFamily(MDHWebSettings hWebSettings, char* buffer, int max);
void mdSetWebSettingsStandardFontFamily(MDHWebSettings hWebSettings, const char* strValue);

//.....


#ifdef __cplusplus
}
#endif


#endif


