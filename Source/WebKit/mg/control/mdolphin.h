/**
 * \file mdolphin.h
 * \author Applications team of Feynman Software
 * \date 2007/11/01
 * 
 * This file includes necessary interfaces of mDolphin.
 *
 \verbatim

    Copyright (C) 2007~2009 Feynman Software.

    All rights reserved by Feynman Software.

    This file is part of mDolphin, a full-featured and high-end embedded web browser,
    which is based on WebKit.

 \endverbatim
 */

#ifndef _MDOLPHIN_H_
#define _MDOLPHIN_H_

#ifndef MDOLPHIN_MAJOR_VERSION
#include "mdconfig.h"
#ifndef __MDOLPHIN_LIB__
#    undef PACKAGE_BUGREPORT
#    undef PACKAGE_NAME
#    undef PACKAGE_STRING
#    undef PACKAGE_TARNAME
#    undef PACKAGE_VERSION
#endif
#endif

#include <stdio.h>
#include <string.h>
#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * The new API of mdophin 3.0
 */

/**
 * \defgroup mdolphin_core mDolphin Core API
 * @{
 */
/**
 * \addtogroup editor element  
 * @{
 */
//keep update with HTMLInputElement::InputType
typedef enum {
    MD_EDITOR_TEXT = 0, // TEXT must be 0.
    MD_EDITOR_PASSWORD,
    MD_EDITOR_ISINDEX,
    MD_EDITOR_CHECKBOX,
    MD_EDITOR_RADIO,
    MD_EDITOR_SUBMIT,
    MD_EDITOR_RESET,
    MD_EDITOR_FILE,
    MD_EDITOR_HIDDEN,
    MD_EDITOR_IMAGE,
    MD_EDITOR_BUTTON,
    MD_EDITOR_SEARCH,
    MD_EDITOR_RANGE,
    MD_EDITOR_EMAIL,
    MD_EDITOR_NUMBER,
    MD_EDITOR_TELEPHONE,
    MD_EDITOR_URL,
    MD_EDITOR_COLOR,
    MD_EDITOR_DATE,
    MD_EDITOR_DATETIME,
    MD_EDITOR_DATETIMELOCAL,
    MD_EDITOR_MONTH,
    MD_EDITOR_TIME,
    MD_EDITOR_WEEK,
    MD_EDITOR_TEXTAREA,
    MD_EDITOR_SELECT
}MDEEditorType;

typedef struct _MDEditorElement {
    int xPos; // x position of edit box's top-left point
    int yPos; // y position of edit box's top-left point
    int width;  // width of edit box
    int height; // height of edit box
    MDEEditorType type; // type of edit box
    int maxLen; // max length of edit box
}MDEditorElement;

/** @} end of editor element */

/**
 * \addtogroup navigation 
 * @{
 */

/**
 * Navigation operations.
 * \sa mdNavigate
 */
typedef enum {
    MD_NAV_BACKWARD,
    MD_NAV_FORWARD,
    MD_NAV_GOTO,
    MD_NAV_RELOAD,
    MD_NAV_STOP
}MDENavigate;

/**
 * \fn BOOL mdNavigate (HWND hWnd, MDENavigate op, const char* url)
 * \brief Navigate the browser.
 *
 * \param hwnd The handle of mDolphin control.
 * \param op The navigation operation, can be one of the following values: 
 *      - MD_NAV_BACKWARD
 *        Go backward.
 *      - MD_NAV_FORWARD 
 *        Go forward.
 *      - MD_NAV_GOTO
 *        Open a specified URL.
 *      - MD_NAV_RELOAD
 *        Reload the current web.
 *      - MD_NAV_STOP
 *        Stop the current download.
 * \param url The specified URL attached to NAV_GOTO operation.
 *
 * \return TRUE on success, FALSE on error.
 */
BOOL mdNavigate(HWND hWnd, MDENavigate op, const char* url);

/** @} end of navigation */

/**
 * \addtogroup rendering
 * @{
 */

/**
 * Rendering mode.
 * \sa mdSetRenderingMode
 */
typedef enum {
    /**
     * Unknown, the mode is unknown
     */
    MD_RDRMODE_UNKNOWN = 0,
    /** 
     * Normal,  will display Web pages in their original layout. 
     */
    MD_RDRMODE_NORMAL = 1,
    /** 
     * Smart-Fit Mode  will render everything in one narrow column, 
     * eliminating the need for horizontal scrolling. This rendering mode is usually 
     * used on mobile phones. 
     */
    MD_RDRMODE_SMARTFIT,
    /** 
     * Global Mode will play a miniature of the entire Web page, 
     * which enables the user to see his/her current position on the page. 
     */
    MD_RDRMODE_GLOBAL,
} MDERenderingMode;

/**
 * \fn BOOL mdSetRenderingMode (HWND hwnd, MDERenderingMode mode, int limit)
 * \brief Set the specified rendering mode on the window. Not support!
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param mode The mode to set, can be one of the following values:
 *      - MD_RDRMODE_NORMAL\n
 *        True Web layout. 
 *      - MD_RDRMODE_SMARTFIT\n
 *        Smart fit mode. 
 *      - MD_RDRMODE_GLOBAL\n 
 *        Global mode.
 * \param limit The max width of the view attached to MD_RDRMODE_SMARTFIT, 
 *        the max width of the view attached to MD_RDRMODE_GLOBAL.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdGetRenderingMode
 */
BOOL mdSetRenderingMode(HWND hwnd, MDERenderingMode mode, int limit);

/**
 * \fn BOOL mdGetRenderingMode(HWND hwnd)
 * \brief Get the specified rendering mode on the window.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \return crrent RenderingMode or MD_RDRMODE_UNKNOWN if failed
 *
 * \sa mdSetRenderingMode
 */
MDERenderingMode mdGetRenderingMode(HWND hwnd);

/** @} end of rendering */

/**
 * \addtogroup text_size
 * @{
 */

/**
 * \fn int mdGetTextSizeMultiplier(HWND hwnd)
 * \brief Get the text size multiplier of the browser.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \return the text size multiplier factor or -1 if failed
 *        Factor expresses int percent as an integer (eg 100 means 100 percent).
 *
 * \sa mdSetTextSizeMultiplier
 */
int mdGetTextSizeMultiplier(HWND hwnd);


/**
 * \fn BOOL mdSetTextSizeMultiplier (HWND hwnd, int factor)
 * \brief Set the text size multiplier of the browser.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param factor The specified text size multiplier. 
 *        Factor expresses int percent as an integer (eg 100 means 100 percent).
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdGetTextSizeMultiplier
 */
BOOL mdSetTextSizeMultiplier (HWND hwnd, int factor);


/** @} end of text_size */

/**
 * \addtogroup character_encoding
 * @{
 */

/** 
 * mDolphin support several character encodings, you can use the following value to set 
 * the default character encoding for your browser. 
 *
 * \sa mdGetTextEncodingName, mdSetTextEncodingName
 */
typedef enum {
    MD_CHARSET_UNKNOWN  = -2, 
    /** 
     * Autodetect character encoding.
     */
    MD_CHARSET_AUTODETECT = -1,
    
    /** 
     * Western (ISO-8859-1).
     */
    MD_CHARSET_ISO8859_1,
    
    /** 
     * Central European (ISO-8859-2).
     */
    MD_CHARSET_ISO8859_2, 
    
    /** 
     * South European (ISO-8859-3).
     */
    MD_CHARSET_ISO8859_3,
    
    /** 
     * Baltic (ISO-8859-4).
     */
    MD_CHARSET_ISO8859_4,
    
    /** 
     * Cyrillic (ISO-8859-5).
     */
    MD_CHARSET_ISO8859_5,
    
    /** 
     * Arabic (ISO-8859-6).
     */
    MD_CHARSET_ISO8859_6,
    
    /** 
     * Greek (ISO-8859-7).
     */
    MD_CHARSET_ISO8859_7,
    
    /** 
     * Hebrew Visual (ISO-8859-8).
     */
    MD_CHARSET_ISO8859_8,
    
    /** 
     * Turkish (ISO-8859-9).
     */
    MD_CHARSET_ISO8859_9,
    
    /** 
     * Nordic (ISO-8859-10).
     */
    MD_CHARSET_ISO8859_10,
    
    /** 
     * Thai (ISO-8859-11).
     */
    MD_CHARSET_ISO8859_11,
    MD_CHARSET_ISO8859_12,
    
    /** 
     * Baltic (ISO-8859-13).
     */
    MD_CHARSET_ISO8859_13,
    
    /** 
     * Celtic (ISO-8859-14).
     */
    MD_CHARSET_ISO8859_14,
    
    /** 
     * Western (ISO-8859-15).
     */
    MD_CHARSET_ISO8859_15,
    
    /** 
     * Romanian (ISO-8859-16).
     */
    MD_CHARSET_ISO8859_16,
    MD_CHARSET_EUC_CN,
    MD_CHARSET_GB1988_0,
    
    /** 
     * Chinese Simplified (GB2312).
     */
    MD_CHARSET_GB2312_0,
    
    /** 
     * Chinese Simplified (GBK).
     */
    MD_CHARSET_GBK,
    
    /** 
     * Chinese Simplified (GB18030).
     */
    MD_CHARSET_GB18030_0,
    
    /** 
     * Chinese Traditional (Big5).
     */
    MD_CHARSET_BIG5,
    MD_CHARSET_KSC5636_0,
    MD_CHARSET_KSC5601_0,
    
    /** 
     * Japanese (EUC-JP).
     */
    MD_CHARSET_EUCJP,
    MD_CHARSET_JISX0201_0,
    MD_CHARSET_JISX0208_0,
    
    /** 
     * Japanese (Shift-JIS).
     */
    MD_CHARSET_SHIFTJIS,
    MD_CHARSET_JISX0201_1,
    MD_CHARSET_JISX0208_1,
    MD_CHARSET_ISO_10646_1,
    
    /** 
     * Unicode (UTF-8).
     */
    MD_CHARSET_UTF8,

} MDEEncodingName;

/**
 * \fn BOOL mdGetTextEncodingName (HWND hWnd)
 * \brief get the current text encoding name of the browser.
 *
 * \param hWnd The handle of mDolphin control.
 * \return MEEncodingName or MD_CHARSET_UNKNOWN.
 *
 * \sa mdSetTextEncodingName
 */
MDEEncodingName mdGetTextEncodingName (HWND hWnd);

/**
 * \fn BOOL mdSetTextEncodingName (HWND hWnd, MDEEncodingName name)
 * \brief Set the text encoding name of the browser.
 *
 * \param hWnd The handle of mDolphin control.
 * \param name The text encoding name.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdGetTextEncodingName
 */
BOOL mdSetTextEncodingName(HWND hWnd, MDEEncodingName name);

/** @} end of character_encoding */

/**
 * \addtogroup preferences 
 * @{
 */

/** 
 \def MD_FIELD_MAX
 *\brief The max length of the string.
 */
#define MD_FIELD_MAX   64

/** 
 * the MDScrobarWidth structure
 * 
 */
typedef struct _MDScrollbarWidth {
    int wRegular; //width of regular
	int wSmall;
	int wMini;
} MDScrobarWidth;

typedef enum {
    MD_STR_DEFAULT_LANGUAGE = 0,    
    MD_STR_STANDARD_FONT,
    MD_STR_SERIF_FONT,
    MD_STR_FIXED_FONT,
    MD_STR_SANS_SERIF_FONT
} MDEStringProp;

typedef enum {
    MD_INT_MEDIUM_FONT_SIZE = 0,
    MD_INT_MEDIUM_FIXED_FONT_SIZE,
    MD_INT_ENCODING_NAME,
    MD_INT_AUTO_LOAD_IMAGES,
    MD_INT_AUTO_DETECT_ENCODING,
    MD_INT_ENABLE_JAVASCRIPT,
    MD_INT_ENABLE_COOKIES,
    MD_INT_ENABLE_PLUGIN,
    MD_INT_ENABLE_JS_POPUP_WINDOW,
    MD_INT_SHOWALL_AT_ONCE,
    MD_INT_SCROBAR_WIDTH_REGULAR,
    MD_INT_SCROBAR_WIDTH_SMALL,
    MD_INT_SCROBAR_WIDTH_MINI,
    MD_INT_ENABLE_SPATIAL_NAVIGATION,
    MD_INT_ENABLE_WEB_SECURITY,
    MD_INT_ENABLE_DEVELOPER_EXTRAS
} MDEIntProp;

/**
 * \fn mdSetStringProperty(HWND hWnd, MDEStringProp property, const char* value)
 * \brief Used to update the current string type property of the browser,
 * \You can call multiple times of this function to set the default language, 
 * \standard font, serif font, fixed font and sans serif font currently.
 *
 * \param hWnd The handle of mDolphin control.
 * \param property The property name you want set, can be one element of MDEStringProp.
 * \
 * \param value The value you want set.
 *
 * \return None.
 *
 * \sa mdGetStringProperty
 */
void mdSetStringProperty(HWND hWnd, MDEStringProp property, const char* value);

/**
 * \fn mdSetIntProperty(HWND hWnd, MDEIntProp property, int value)
 * \brief Used to update the current int type property of the browser,
 * \You can call multiple times of this function to set the Medium font size,
 * \Medium fixed font size, default encoding name and so on.
 *
 * \param hWnd The handle of mDolphin control.
 * \property: The property name you want set, can be one element of MDEIntProp.
 * \
 * \param value The value you want set.
 *
 * \return None.
 *
 * \sa mdGetIntProperty
 */
void mdSetIntProperty(HWND hWnd, MDEIntProp property, int value);

/**
 * \fn void mdGetStringProperty(HWND hWnd, MDEStringProp property, char* buffer, int bufferLen)
 * \brief Used to get the current property with string type of the browser. 
 *
 * \param hWnd The handle of mDolphin control.
 * \param property The property name you want get, can be one element of MDEStringProp.
 * \
 * \param buffer Pointer to a buffer receiving the current information of the property.
 * \param bufferLen The length of buffer, must be equal or greater then MD_FIELD_MAX.
 *
 * \return None.
 *
 * \sa mdSetStringProperty
 */
void mdGetStringProperty(HWND hWnd, MDEStringProp property, char* buffer, int bufferLen);

/**
 * \fn int mdGetIntProperty(HWND hWnd, MDEIntProp property)
 * \brief Used to get the current property with int type of the browser. 
 *
 * \param hWnd The handle of mDolphin control.
 * \param property The property name you want get, can be one element of MDEIntProp.
 *
 * \return The value of the int property.
 *
 * \sa mdSetIntProperty
 */
int mdGetIntProperty(HWND hWnd, MDEIntProp property);

/** @} end of preferences */

/**
 * \addtogroup http_proxy
 * @{
 */

/**
 * The proxy type.
 * \sa MDProxyItem, mdGetPropxy
 */
typedef enum {
    /** HTTP. */
    MD_PROXY_HTTP = 0,
    /** SOCKS4. */
    MD_PROXY_SOCKS4,
    /** SOCKS5. */
    MD_PROXY_SOCKS5,
    MD_PROXY_MAX
} MDEProxyType;

/**
 * The proxy item information structure.
 *
 * \sa mdGetProxy, MDProxy
 */
#define PROXY_MAX_LENGTH 64
typedef struct _MDProxyItem {
    /** Proxy host. */
    char host[PROXY_MAX_LENGTH];
    /** Proxy port. */
    unsigned short port;
    /** User name. */
    char name[PROXY_MAX_LENGTH];
    /** User password. */
    char pwd[PROXY_MAX_LENGTH];
}MDProxyItem;


/**
 * \fn void mdEnableProxy (BOOL isEnabled)
 * \brief Set proxy state.
 *
 * \param isEnabled TRUE to enable proxy, FALSE on otherwise.
 * \return None.
 *
 * \sa mdEnableProxy
 */
void mdEnableProxy(BOOL isEnabled);

/**
 * \fn BOOL mdIsProxyEnabled (void)
 * \brief Check whether proxy is enabled or not.
 *
 * \param void
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdIsProxyEnabled
 */
BOOL mdIsProxyEnabled (void);

/**
 * \fn BOOL mdSetProxy (MDEProxyType type, const MDProxyItem* item)
 * \brief Set the proxy when you use proxy to connnect the network.
 *
 * \param type The proxy type.
 * \param item Proxy item.
 * \return TRUE if you set proxy successfully, otherwise FALSE.
 *
 * \sa mdGetProxy
 */

BOOL mdSetProxy (MDEProxyType type, const MDProxyItem* item);
/**
 * \fn BOOL mdGetProxy (MDEProxyType type, MDProxyItem *item)
 * \brief Get the proxy information.
 *
 * \param type The proxy type.
 * \param item Proxy item.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdSetProxy
 */
BOOL mdGetProxy (MDEProxyType type, MDProxyItem *item);

/** @} end of http_proxy */

/**
 * \addtogroup popup_menu 
 * @{
 */

/** 
 *\def MD_POPMENU_NO
 *\brief One of the popup menu item types. Users don't need to create any popup menu items.
 *\sa MDPE_NO, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_NO                             0x00000000

/** 
 *\def MD_POPMENU_OPEN_LINK
 *\brief One of the popup menu item types. Users can create a popup menu item to open the current link in the local window.
 *\sa MDPE_OPEN_LINK, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_OPEN_LINK                      0x00000001
#define MDPI_OPEN_LINK                         MD_POPMENU_OPEN_LINK

/** 
 *\def MD_POPMENU_OPEN_LINK_IN_NEW_WINDOW
 *\brief One of the popup menu item types. Users can create a popup menu item to open the current link in a new window.
 *\sa MDPE_OPEN_LINK_IN_NEW_WINDOW, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_OPEN_LINK_IN_NEW_WINDOW        0x00000002
#define MDPI_OPEN_LINK_IN_NEW_WINDOW           MD_POPMENU_OPEN_LINK_IN_NEW_WINDOW

/** 
 *\def MD_POPMENU_DOWNLOAD_LINK_TO_DISK
 *\brief One of the popup menu item types. Users can create a popup menu item to downlaod the current link to the local disk.
 *\sa MDPE_DOWNLOAD_LINK_TO_DISK, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_DOWNLOAD_LINK_TO_DISK          0x00000004
#define MDPI_DOWNLOAD_LINK_TO_DISK             MD_POPMENU_DOWNLOAD_LINK_TO_DISK

/** 
 *\def MD_POPMENU_COPY_LINK_TO_CLIPBOARD
 *\brief One of the popup menu item types. Users can create a popup menu item to copy the current link to the clipboard.
 *\sa MDPE_COPY_LINK_TO_CLIPBOARD, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_COPY_LINK_TO_CLIPBOARD         0x00000008
#define MDPI_COPY_LINK_TO_CLIPBOARD            MD_POPMENU_COPY_LINK_TO_CLIPBOARD

/** 
 *\def MD_POPMENU_OPEN_IMAGE_IN_NEW_WINDOW
 *\brief One of the popup menu item types. Users can create a popup menu item to open the current image in a new window.
 *\sa MDPE_OPEN_IMAGE_IN_NEW_WINDOW, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_OPEN_IMAGE_IN_NEW_WINDOW       0x00000010
#define MDPI_OPEN_IMAGE_IN_NEW_WINDOW          MD_POPMENU_OPEN_IMAGE_IN_NEW_WINDOW

/** 
 *\def MD_POPMENU_DOWNLOAD_IMAGE_TO_DISK
 *\brief One of the popup menu item types. Users can create a popup menu item to download the current image to the local disk.
 *\sa MDPE_DOWNLOAD_IMAGE_TO_DISK, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_DOWNLOAD_IMAGE_TO_DISK         0x00000020
#define MDPI_DOWNLOAD_IMAGE_TO_DISK            MD_POPMENU_DOWNLOAD_IMAGE_TO_DISK

/** 
 *\def MD_POPMENU_COPY_IMAGE_TO_CLIPBOARD
 *\brief One of the popup menu item types. Users can create a popup menu item to copy the current image to the clipboard.
 *\sa MDPE_COPY_IMAGE_TO_CLIPBOARD, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_COPY_IMAGE_TO_CLIPBOARD        0x00000040
#define MDPI_COPY_IMAGE_TO_CLIPBOARD           MD_POPMENU_COPY_IMAGE_TO_CLIPBOARD

/** 
 *\def MD_POPMENU_GOBACK
 *\brief One of the popup menu item types. Users can create a popup menu item to go backward.
 *\sa MDPE_GOBACK, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_GOBACK                         0x00000080
#define MDPI_GOBACK                            MD_POPMENU_GOBACK

/** 
 *\def MD_POPMENU_GOFORWARD
 *\brief One of the popup menu item types. Users can create a popup menu item to go forward.
 *\sa MDPE_GOFORWARD, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_GOFORWARD                      0x00000100
#define MDPI_GOFORWARD                         MD_POPMENU_GOFORWARD

/** 
 *\def MD_POPMENU_STOP
 *\brief One of the popup menu item types. Users can create a popup menu item to stop loading the current page.
 *\sa MDPE_STOP, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_STOP                           0x00000200
#define MDPI_STOP                              MD_POPMENU_STOP

/** 
 *\def MD_POPMENU_RELOAD
 *\brief One of the popup menu item types. Users can create a popup menu item to reload the current page.
 *\sa MDPE_RELOAD, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_RELOAD                         0x00000400
#define MDPI_RELOAD                            MD_POPMENU_RELOAD

/** 
 *\def MD_POPMENU_OPEN_FRAME_IN_NEW_WINDOW
 *\brief One of the popup menu item types. Users can create a popup menu item to open the current frame in a new window.
 *\sa MDPE_OPEN_FRAME_IN_NEW_WINDOW, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_OPEN_FRAME_IN_NEW_WINDOW       0x00000800
#define MDPI_OPEN_FRAME_IN_NEW_WINDOW          MD_POPMENU_OPEN_FRAME_IN_NEW_WINDOW

/** 
 *\def MD_POPMENU_COPY
 *\brief One of the popup menu item types. Users can create a popup menu item to copy the current content.
 *\sa MDPE_COPY, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_COPY                           0x00001000
#define MDPI_COPY                              MD_POPMENU_COPY

/** 
 *\def MD_POPMENU_CUT
 *\brief One of the popup menu item types. Users can create a popup menu item to cut the current content.
 *\sa MDPE_CUT, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_CUT                            0x00002000
#define MDPI_CUT                               MD_POPMENU_CUT

/** 
 *\def MD_POPMENU_PASTE
 *\brief One of the popup menu item types. Users can create a popup menu item to paste with the contents on clipboard .
 *\sa MDPE_PASTE, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_PASTE                          0x00004000
#define MDPI_PASTE                             MD_POPMENU_PASTE

/** 
 *\def MD_POPMENU_SEARCH_WEB
 *\brief One of the popup menu item types. Users can create a popup menu item to search the current selected content with Google.
 *\sa MDPE_SEARCH_WEB, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_SEARCH_WEB                     0x00008000
#define MDPI_SEARCH_WEB                        MD_POPMENU_SEARCH_WEB

/** 
 *\def MD_POPMENU_BOLD
 *\brief One of the popup menu item types. Users can create a popup menu item to change font style to bold.
 *\sa MDPE_BOLD, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_BOLD                           0x00010000
#define MDPI_BOLD                              MD_POPMENU_BOLD

/** 
 *\def MD_POPMENU_ITALIC
 *\brief One of the popup menu item types. Users can create a popup menu item to change font style to italic.
 *\sa MDPE_ITALIC, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_ITALIC                         0x00020000
#define MDPI_ITALIC                            MD_POPMENU_ITALIC

/** 
 *\def MD_POPMENU_UNDERLINE
 *\brief One of the popup menu item types. Users can create a popup menu item to change font style to underline.
 *\sa MDPE_UNDERLINE, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_UNDERLINE                      0x00040000
#define MDPI_UNDERLINE                         MD_POPMENU_UNDERLINE

/** 
 *\def MD_POPMENU_DEFAULT_DIRECTION
 *\brief One of the popup menu item types. Users can create a popup menu item to change font direction to default.
 *\sa MDPE_DEFAULT_DIRECTION, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_DEFAULT_DIRECTION              0x00100000
#define MDPI_DEFAULT_DIRECTION                 MD_POPMENU_DEFAULT_DIRECTION

/** 
 *\def MD_POPMENU_LEFT_TO_RIGHT
 *\brief One of the popup menu item types. Users can create a popup menu item to change font direction to LTR.
 *\sa MDPE_LEFT_TO_RIGHT, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_LEFT_TO_RIGHT                  0x00200000
#define MDPI_LEFT_TO_RIGHT                     MD_POPMENU_LEFT_TO_RIGHT

/** 
 *\def MD_POPMENU_RIGHT_TO_LEFT
 *\brief One of the popup menu item types. Users can create a popup menu item to change font direction to RTL.
 *\sa MDPE_RIGHT_TO_LEFT, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_RIGHT_TO_LEFT                  0x00400000
#define MDPI_RIGHT_TO_LEFT                     MD_POPMENU_RIGHT_TO_LEFT

/** 
 *\def MD_POPMENU_INSPECT_ELEMENT
 *\brief One of the popup menu item types. Users can create a popup menu item to inspect element.
 *\sa MDPE_INSPECT_ELEMENT, MDPopupMenu, MDCB_CREATE_POPUP_MENU 
 */
#define MD_POPMENU_INSPECT_ELEMENT                0x00800000
#define MDPI_INSPECT_ELEMENT                   MD_POPMENU_INSPECT_ELEMENT

#define MD_POPMENU_SAVEAS_CURLINK                0x01000000
#define MDPI_SAVEAS_CURLINK                   MD_POPMENU_SAVEAS_CURLINK

/** 
 * The corresponding event types of popup menu.
 */
typedef enum {
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_NO.
     *\sa MD_POPMENU_NO, mdolphin_perform_popup_menu_event
     */
    MD_PME_NO = 0, 
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_OPEN_LINK_IN_NEW_WINDOW.
     *\sa MD_POPMENU_OPEN_LINK_IN_NEW_WINDOW, mdolphin_perform_popup_menu_event
     */
    MD_PME_OPEN_LINK_IN_NEW_WINDOW = 1,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_DOWNLOAD_LINK_TO_DISK.
     *\sa MD_POPMENU_DOWNLOAD_LINK_TO_DISK, mdolphin_perform_popup_menu_event
     */
    MD_PME_DOWNLOAD_LINK_TO_DISK,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_COPY_LINK_TO_CLIPBOARD.
     *\sa MD_POPMENU_COPY_LINK_TO_CLIPBOARD, mdolphin_perform_popup_menu_event
     */
    MD_PME_COPY_LINK_TO_CLIPBOARD,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_OPEN_IMAGE_IN_NEW_WINDOW.
     *\sa MD_POPMENU_OPEN_IMAGE_IN_NEW_WINDOW, mdolphin_perform_popup_menu_event
     */
    MD_PME_OPEN_IMAGE_IN_NEW_WINDOW,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_DOWNLOAD_IMAGE_TO_DISK.
     *\sa MD_POPMENU_DOWNLOAD_IMAGE_TO_DISK, mdolphin_perform_popup_menu_event
     */
    MD_PME_DOWNLOAD_IMAGE_TO_DISK,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_COPY_IMAGE_TO_CLIPBOARD.
     *\sa MD_POPMENU_COPY_IMAGE_TO_CLIPBOARD, mdolphin_perform_popup_menu_event
     */
    MD_PME_COPY_IMAGE_TO_CLIPBOARD,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_OPEN_FRAME_IN_NEW_WINDOW.
     *\sa MD_POPMENU_OPEN_FRAME_IN_NEW_WINDOW, mdolphin_perform_popup_menu_event
     */
    MD_PME_OPEN_FRAME_IN_NEW_WINDOW,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_COPY.
     *\sa MD_POPMENU_COPY, mdolphin_perform_popup_menu_event
     */
    MD_PME_COPY,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_GOBACK.
     *\sa MD_POPMENU_GOBACK, mdolphin_perform_popup_menu_event
     */
    MD_PME_GOBACK,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_GOFORWARD.
     *\sa MD_POPMENU_GOFORWARD, mdolphin_perform_popup_menu_event
     */
    MD_PME_GOFORWARD,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_STOP.
     *\sa MD_POPMENU_STOP, mdolphin_perform_popup_menu_event
     */
    MD_PME_STOP,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_RELOAD.
     *\sa MD_POPMENU_RELOAD, mdolphin_perform_popup_menu_event
     */
    MD_PME_RELOAD,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_CUT.
     *\sa MD_POPMENU_CUT, mdolphin_perform_popup_menu_event
     */
    MD_PME_CUT,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_PASTE.
     *\sa MD_POPMENU_PASTE, mdolphin_perform_popup_menu_event
     */
    MD_PME_PASTE,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_SEARCH_WEB.
     *\sa MD_POPMENU_SEARCH_WEB, mdolphin_perform_popup_menu_event
     */
    MD_PME_SEARCH_WEB=21,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_OPEN_LINK.
     *\sa MD_POPMENU_OPEN_LINK, mdolphin_perform_popup_menu_event
     */
    MD_PME_OPEN_LINK = 2000,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_BOLD.
     *\sa MD_POPMENU_BOLD, mdolphin_perform_popup_menu_event
     */
    MD_PME_BOLD = 2009,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_ITALIC.
     *\sa MD_POPMENU_ITALIC, mdolphin_perform_popup_menu_event
     */
    MD_PME_ITALIC,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_UNDERLINE.
     *\sa MD_POPMENU_UNDERLINE, mdolphin_perform_popup_menu_event
     */
    MD_PME_UNDERLINE,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_DEFAULT_DIRECTION.
     *\sa MD_POPMENU_DEFAULT_DIRECTION, mdolphin_perform_popup_menu_event
     */
    MD_PME_DEFAULT_DIRECTION = 2019,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_LEFT_TO_RIGHT.
     *\sa MD_POPMENU_LEFT_TO_RIGHT, mdolphin_perform_popup_menu_event
     */
    MD_PME_LEFT_TO_RIGHT,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_RIGHT_TO_LEFT.
     *\sa MD_POPMENU_RIGHT_TO_LEFT, mdolphin_perform_popup_menu_event
     */
    MD_PME_RIGHT_TO_LEFT,
    
    /** 
     * The corresponding popup menu item event type of MD_POPMENU_INSPECT_ELEMENT.
     *\sa MD_POPMENU_INSPECT_ELEMENT, mdolphin_perform_popup_menu_event
     */
    MD_PME_INSPECT_ELEMENT = 2024,

    MD_PME_SAVEAS_CURLINK ,

    MD_PME_BASE_APPLICATION_TAG = 10000,
} MDEPopupMenuElementEvent;

/**
 * The popup menu information structure.
 * \sa CB_CREATE_POPUP_MENU
 */
typedef struct _MDPopupMenu {
    /** Coordinates of the popup menu original point. */
	int x, y;
    /** The flag of popup menu item types. */
    unsigned int itemsFlag;
    /** The corresponding enabled state of popup menu items. */
    unsigned int itemsEnabled;
    /** The corresponding checked state of popup menu items. */
    unsigned int itemsChecked;
} MDPopupMenu;

typedef  MDPopupMenu    POPUP_MENU_DATA;

/**
 * \fn BOOL mdPerformPopupMenuEvent (HWND hWnd, MDEPopupMenuElementEvent event)
 * \brief Perform the popup menu event.
 *
 * \param hWnd The handle of mDolphin control.
 * \param event The clicked popup menu item event.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa MDCB_CREATE_POPUP_MENU
 */
BOOL mdPerformPopupMenuEvent (HWND hWnd, MDEPopupMenuElementEvent event);

/** @} end of popup_menu */

/**
 * \addtogroup search_for
 * @{
 */

/**
 * \fn BOOL mdSearchText (HWND hWnd, const char* searchString, unsigned int searchFlag);
 * \brief Searches a document view for a string and highlights the string if it is found.
 *
 * \param hWnd The handle of mDolphin control.
 * \param searchString: The string to search for.
 * \param searchFlag: The mask of search method, can be OR'd with the following values: 
 * SEARCH_NORMAL: Search forward, case-insensitive, avoid wrapping. 
 * SEARCH_BACKWARD: The search direction is backward. 
 * SEARCH_CASE_SENSITIVE: search by case-sensitive. 
 * SEARCH_WRAP_AROUND: search by wrap around.
 * \return TRUE if found, FALSE if not found.
 *
 * \sa mdMarkAllMatchedText
 */
#define SEARCH_NORMAL           0x00000000
#define SEARCH_BACKWARD         0x00000001
#define SEARCH_CASE_SENSITIVE   0x00000002
#define SEARCH_WRAP_AROUND      0x00000004
BOOL mdSearchText (HWND hWnd, const char* searchString, unsigned int searchFlag);

/**
 * \fn unsigned int mdMarkAllMatchedText (HWND hWnd, const char *searchString, 
                unsigned int limitCount, unsigned int markFlag) 
 * \brief Mark all matching strings in a document view and highlights or unhighlights the strings.
 *
 * \param hWnd The handle of mDolphin control.
 * \param searchString The string to search for.
 * \param limitCount The specified maching count. A limit of 0 means no limit.
 * \param markFlag: The mark of matched text method, can be OR'd with the following values: 
 * MARK_NORMAL: ‘case-insensitive’ and ‘highlight the matched text’. 
 * MARK_CASE_SENSITIVE: mark matched text by case-sensitive. 
 * MARK_UNHIGHLIGHT: unhighlight the matched text.
 * \return the matching count, 0 if no match.
 *
 * \sa mdUnmarkAllMatchedText, mdSearchText
 */
#define MARK_NORMAL         0x00000000
#define MARK_CASE_SENSITIVE 0x00000001
#define MARK_UNHIGHLIGHT    0x00000002
unsigned int mdMarkAllMatchedText (HWND hWnd, const char *searchString, 
                unsigned int limitCount, unsigned int markFlag);

/**
 * \fn BOOL mdUnmarkAllMatchedText (HWND hWnd)
 * \brief Unmark all matching text in a document view.
 *
 * \param hWnd The handle of mDolphin control.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdMarkAllMatchedText
 */
BOOL mdUnmarkAllMatchedText (HWND hWnd);

/** @} end of search_for */


/**
 * \addtogroup view_source
 * @{
 */

/**
 * \fn BOOL mdSetSourceView (HWND hWnd, BOOL isSourceMode)
 * \brief Used to place a view into a special source-viewing mode.
 *
 * \param hWnd The handle of mDolphin control.
 * \param isSourceMode TRUE for view source mode, FALSE otherwise.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdIsSourceView
 */
BOOL mdSetSourceView (HWND hWnd, BOOL isSourceMode);

/**
 * \fn BOOL mdIsSourceView (HWND hWnd)
 * \brief Whether or not the view is in source-viewing mode for HTML.
 *
 * \param hWnd The handle of mDolphin control.
 * \return TRUE in view source mode, FALSE otherwise.
 *
 * \sa mdSetSourceView
 */
BOOL mdIsSourceView (HWND hWnd);

/** @} end of view_source */

/**
 * \addtogroup contents_position 
 * @{
 */

/**
 * \fn BOOL mdSetContentPosition(HWND hWnd, int x, int y)
 * \brief Used to set the position of the page contents.
 *
 * \param hWnd The handle f mDolphin control.
 * \param x The x of the contents position.
 * \param y The y of the contents position.
 * \return TRUE on success, FALSE otherwise.
 *
 * \sa mdGetContentPosition
 */
BOOL mdSetContentPosition(HWND hWnd, int x, int y);


/**
 * \fn BOOL mdGetContentPosition(HWND hWnd, int* x, int* y)
 * \brief Get the position of the page contents.
 *
 * \param hWnd The handle of mDolphin control.
 * \param x The x of the contents position.
 * \param y The y of the contents position.
 * \return TRUE on success, FALSE otherwise.
 *
 * \sa mdSetContentPosition
 */
BOOL mdGetContentPosition(HWND hWnd, int* x, int* y);

/**
 * \fn BOOL mdGetContentSize(HWND hWnd, int* width, int* height)
 * \brief Get the size of the page contents.
 *
 * \param hWnd The handle of mDolphin control.
 * \param width The width of the contents.
 * \param height The height of the contents.
 * \return TRUE on success, FALSE otherwise.
 */
BOOL mdGetContentSize(HWND hWnd, int* width, int* height);


/** @} end of contents_position */

/**
 * \addtogroup scale_factor 
 * @{
 */

/**
 * \fn void mdSetScaleFactor(HWND hWnd, float factor)
 * \brief Used to set the current page size multiplier of the browser.
 *
 * \param hWnd The handle of mDolphin control.
 * \param factor The specified page size multiplier, ranges from 0.5 to 3.0.
 *
 * \sa mdGetScaleFactor
 */
void mdSetScaleFactor(HWND hWnd, float factor);

/**
 * \fn float mdGetScaleFactor(HWND hWnd)
 * \brief Used to get the current multiplier of the browser's page size.
 *
 * \param hWnd The handle of mDolphin control.
 * \return The factor of the current multiplier of the browser's page size.
 *
 * \sa mdSetScaleFactor
 */
float mdGetScaleFactor(HWND hWnd);

/** @} end of scale_factor */


#if defined(ENABLE_SCHEMEEXTENSION) && ENABLE_SCHEMEEXTENSION

/**
 * \addtogroup scheme 
 * @{
 */

/** 
 * \var typedef BOOL (*MDCB_SCHEME_HANDLER) (const char *url, void *param)
 * \brief The callback function of the scheme handler.
 *
 * \param url The handled url with the specified scheme.
 * \param param The parameter of the callback function.
 */
typedef BOOL (*MDCB_SCHEME_HANDLER) (const char *url, void *param);

/**
 * \fn BOOL mdRegisterSchemeHandler (const char* scheme, MDCB_SCHEME_HANDLER cb, void* param)
 *
 * \brief Register a callback function for a new protocol. 
 *      The browser will call the callback function when meets the new protocol.
 *
 * \param scheme The scheme of protocol.
 * \param cb The callback of the scheme handler.
 * \param param The parameter of the callback function.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdUnregisterSchemeHandler
 */
BOOL mdRegisterSchemeHandler (const char* scheme, MDCB_SCHEME_HANDLER cb, void* param);

/**
 * \fn BOOL mdUnregisterSchemeHandler (const char* scheme) 
 *
 * \brief Unregister a callback function for a new protocol.
 * \param scheme The scheme of protocol.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdRegisterSchemeHandler
 */
BOOL mdUnregisterSchemeHandler (const char* scheme);

/** @} end of scheme */

#endif

#if defined(ENABLE_SSL) && ENABLE_SSL

/**
 * \addtogroup https 
 * @{
 */

/**
 * \var typedef void MD_SSL_PKEY
 * \brief A type definition for MD_SSL_PKEY.
 */
typedef void MD_SSL_PKEY;

/**
 * \var typedef void MD_CERT_DATA
 * \brief A type definition for MD_CERT_DATA.
 */
typedef void MD_CERT_DATA; 

/**
 * \var typedef void MD_CERT_NAME
 * \brief A type definition for MD_CERT_NAME.
 */
typedef void MD_CERT_NAME;

/**
 * \var typedef int MD_CERT_RESULT
 * \brief A type definition for MD_CERT_RESULT.
 */
typedef int MD_CERT_RESULT;

/**
 * \fn BOOL mdSetCAPath (const char * path) 
 * \brief Specify a certificate directory holding alternate certificates to verify with.
 *
 * \param path The directory holding multiple CA certificates.
 *
 * \return TRUE if you set certificate path successfully, otherwise FALSE.
 */
BOOL mdSetCAPath (const char* path);

/** @} end of https */

#endif
/**
 * \addtogroup minigui_control mDolphin Control 
 * @{
 */

/**
 * \fn BOOL mdRegisterMDolphinControl(void) 
 * \brief Register the mDolphin control class to MiniGUI.
 *
 * \return TRUE if you register successfully, otherwise FALSE.
 *
 * \sa mdUnregisterMDolphinControl
 */
BOOL mdRegisterMDolphinControl (void);

/**
 * \fn void mdUnregisterMDolphinControl (void) 
 * \brief Unregister mDolphin control class from MiniGUI.
 *
 * \return TRUE if there is not any mDolphin control instance, otherwise FALSE.
 *
 * \sa mdRegisterMDolphinControl
 */
BOOL mdUnregisterMDolphinControl (void);

/**
 * \def MDOLPHIN_CTRL
 * \brief The class name of mDolphin view control.
 *
 */
#define MDOLPHIN_CTRL   ("mDolphin")

/** @} end of minigui_control */

/**
 * \addtogroup cache
 * @{
 */

/**
 * Cache type.
 */
typedef enum {
    /** 
     * Memory cache type.
     */
    MD_CACHE_MEM,
    /** 
     * Disk cache type.
     */
    MD_CACHE_DISK,
} MDECache;

/**
 * \fn unsigned int mdSetCacheCapacity (unsigned int totalBytes)
 * \brief Set cache total capacity.
 *
 * \Param totalBytes The cache total capacity in bytes.
 * \Return Return the old cache capacity in bytes.
 *
 * \sa mdGetCahceCapacity
 */
unsigned int mdSetCacheCapacity (unsigned int totalBytes);

/**
 * \fn unsigned int mdGetCacheCapacity (void)
 * \brief Get cache total capacity.
 *
 * \Return The cache total capacity in bytes.
 *
 * \sa mdSetCacheCapacity
 */
unsigned int mdGetCacheCapacity (void);

/**
 * \fn void mdEnableCache(BOOL isEnabled)
 * \brief Set cache state. Disabling the cache will remove all resources from the cache. 
 * They may still live on if they are referenced by some Web page though.
 *
 * \Param isEnabled TRUE for enable cache, FALSE on other wise.
 *
 */
void mdEnableCache (BOOL isEnabled);

/**
 * \fn BOOL mdIsCacheEnabled (void)
 * \brief Return the cache state, TRUE for enabled and FALSE on other wise.
 *
 */
BOOL mdIsCacheEnabled (void);

/**
 * \fn void mdClearCache (void)
 * \brief Remove the current cache in the memory or in the disk.
 *
 */
void mdClearCache (void);

BOOL mdSetCacheType (MDECache type, const char* path, unsigned size);

/** @} end of cache */


/* **************old APIs*************************************************** */

/**
 * \addtogroup editor element  
 * @{
 */
/** @} end of editor element */

/**
 * \addtogroup cookie
 * @{
 */

/**
 * \fn BOOL mdIsCookieEnabled(void);
 * \brief Check whether cookie is enabled or not.
 *
 * \sa mdEnableCookie 
 * \return TRUE if cookie is enabled, otherwise FALSE.
 */
BOOL mdIsCookieEnabled(void);

/**
 * \fn BOOL mdEnableCookie(BOOL isEnabled)
 * \brief Set cookie state.
 *
 * \Param isEnabled TRUE to enable cookie, FALSE on other wise.
 */
void mdEnableCookie(BOOL isEnabled);

/**
 * \fn BOOL mdClearCookie(void);
 * \brief Clear cookie.
 *
 */
void mdClearCookie(void);

/** @} end of cookie */

/**
 * \addtogroup callback
 * @{
 */



/**
 * The callback functions information structure, which contains many callback functions. 
 * Based on these callback functions, users can define some interactive functions.
 *
 * \sa mdolphin_set_callback_info
 */
typedef struct _MDCBInfo {
    /**
     * \fn void (*MDCB_MESSAGE_BOX)(HWND hWnd, const char* message, const char* caption)
     * \brief The callback function, which is used for JavaScript alert() method. 
     *        This function displays a message box within only one button whose title is "OK".
     *
     * \param hWnd The handle of mDolphin control, which will open a message box.
     * \param message The message text with UTF-8 character encoding, 
     *        which will be displayed in the message box.
     * \param caption The caption of the message box.
     *
     * \sa MDCB_CONFIRM_BOX, MDCB_PROMPT_BOX, MDCB_CHOOSEFILE_BOX
     */
    void (*MDCB_MESSAGE_BOX)(HWND hWnd, const char* message, const char* caption);

    /**
     * \fn BOOL (*MDCB_CONFIRM_BOX)(HWND hWnd, const char* message, const char* caption)
     * \brief The callback function, which is used for JavaScript confirm() method. 
     *        This function creates a message box within two buttons whose titles are "OK" 
     *        and "Cancel" respectively.
     *
     * \param hWnd The handle of mDolphin control, which will open a confirm box.
     * \param message The message text with UTF-8 character encoding, 
     *        which will be displayed in the confirm box.
     * \param caption The caption of the confirm box.
     *
     * \return TRUE if you select "OK", otherwise FALSE.
     *
     * \sa MDCB_MESSAGE_BOX, MDCB_PROMPT_BOX, MDCB_CHOOSEFILE_BOX
     */
    BOOL (*MDCB_CONFIRM_BOX)(HWND hWnd, const char* message, const char* caption);

    /**
     * \fn char *(*MDCB_PROMPT_BOX)(HWND hWnd, const char* prompt, const char* defaultInput, 
                    const char* caption)
     * \brief The callback function, which is used for JavaScript prompt() method. 
     *        This function returns the string you commit, otherwise NULL.
     *
     * \param hWnd The handle of mDolphin control, which will display a dialog box that 
     *        prompts the user for input.
     * \param prompt The message with UTF-8 character encoding, which will be 
     *        displayed in the prompt box.
     * \param defaultInput The default input text with UTF-8 character encoding.
     * \param caption The caption of the prompt box.
     *
     * \sa MDCB_MESSAGE_BOX, MDCB_CONFIRM_BOX, MDCB_CHOOSEFILE_BOX
     */
    char *(*MDCB_PROMPT_BOX)(HWND hWnd, const char* prompt, const char* defaultInput, 
                    const char* caption);

    /**
     * \fn void (*MDCB_SET_TITLE) (HWND hWnd, const char* title)
     * \brief The callback function, which is used to set the browsing title of the 
     *        Web page for the window.
     *
     * \param hWnd The handle of mDolphin control.
     * \param title The title of the browsing Web page with UTF-8 character encoding.
     *
     * \sa MDCB_SET_LOCATION, MDCB_SET_STATUS
     */
    void (*MDCB_SET_TITLE) (HWND hWnd, const char* title);

    /**
     * \fn void (*MDCB_SET_LOCATION) (HWND hWnd, const char* url)
     * \brief The callback function, which is used to set the visiting URL for the address field.
     *
     * \param hWnd The handle of mDolphin control.
     * \param url The visiting URL with UTF-8 character encoding.
     *
     * \sa MDCB_SET_TITLE, MDCB_SET_STATUS
     */
    void (*MDCB_SET_LOCATION) (HWND hWnd, const char* url);

    /**
     * \fn void (*MDCB_SET_STATUS) (HWND hWnd, const char* url)
     * \brief The callback function, which is used to display the URL information the mouse is 
     *        hovering over.
     *
     * \param hWnd The handle of mDolphin control.
     * \param url The displaying url with UTF-8 character encoding.
     *
     * \sa MDCB_SET_TITLE, MDCB_SET_LOCATION
     */
    void (*MDCB_SET_STATUS)(HWND hWnd, const char* url);

    /**
     * \fn void (*MDCB_SET_LOADING_STATUS) (HWND hWnd, BOOL isLoading, unsigned int progress)
     * \brief The callback function, which is used to set loading status.
     *
     * \param hWnd The handle of mDolphin control.
     * \param isLoading The loading status. If there are any pending loads, 
     *        isLoading is TRUE. When loading is finished, isLoading is FALSE.
     * \param progress An estimate of the percent complete for a document load. 
     *        This value will range from 0 to 100.
     *
     * \sa MDCB_SET_STATUS, MDCB_SET_HISTORY_STATUS
     */
    void (*MDCB_SET_LOADING_STATUS) (HWND hWnd, BOOL isLoading, unsigned int progress);

    /**
     * \fn void (*MDCB_SET_HISTORY_STATUS) (HWND hWnd, unsigned int backwardListCount, 
                    unsigned int forwardListCount, unsigned int capacity)
     * \brief The callback function, which is used to set history status.
     *
     * \param hWnd The handle of mDolphin control.
     * \param backwardListCount The available backward list count.
     * \param forwardListCount The available forward list count.
     * \param capacity The capacity of history list.
     *
     * \sa MDCB_SET_STATUS, MDCB_SET_LOADING_STATUS
     */
    void (*MDCB_SET_HISTORY_STATUS) (HWND hWnd, unsigned int backwardListCount, 
                unsigned int forwardListCount, unsigned int capacity);

    /**
     * \fn void (*MDCB_ERROR) (HWND hWnd, int errCode, const char * url)
     * \brief The callback function, which is used to deal with error.
     *
     * \param hWnd The handle of mDolphin control.
     * \param errCode The error code.
     * \param url The failed URL.
     *
     * \sa ERROR_CODE 
     */
    void (*MDCB_ERROR) (HWND hWnd, int errCode, const char * url);

    /**
     * \fn BOOL (*MDCB_CHOOSEFILE_BOX) (HWND hWnd, char* fileName, size_t fileNameMaxLen)
     * \brief The callback function, which is used to create a file choosing dialog box to upload file.
     *
     * \param hWnd The handle of mDolphin Control, which will open a file choosing dialog box.
     * \param fileName The pointer to the buffer receiving the full path name of the file 
     *        selected by user.
     * \param fileNameMaxLen The maximum length of the full path name of the file.
     *
     * \return TRUE if users choose a file and click "OK" button, otherwise FALSE.
     */
    BOOL (*MDCB_CHOOSEFILE_BOX) (HWND hWnd, char* fileName, size_t fileNameMaxLen);

    /**
     * \fn BOOL (*MDCB_SAVE_FILE_DATA) (int downloadId, const char* url, const void* dataStream, 
                size_t dataStreamSize, unsigned long totalSize, const char* suggestedFileName, 
                const char* mimeType , BOOL isFinished);
     * \brief This callback function is used for users to download data.
     *        When MIME type is unsupported or content neet to be downloaded, users can decide 
     *        whether saving data to a local file.
     *
     * \param downloadId The download ID. Make a difference among the different downloading contents 
     *        with the same URL.
     * \param url The URL of the downloading page.
     * \param dataStream The downloading data stream.
     * \param dataStreamSize The size of the current downloading data.
     * \param totalSize The total size of the data stream.
     * \param suggestedFileName The suggested file name which is used to save the data.
     * \param mimeType The type of the MIME.
     * \param isFinished If downloading is finished, isFinished is TRUE. Otherwise, isFinished is FALSE.
     *
     * \return TRUE if users want to continue downloading the data stream, otherwise FALSE.
     */
    BOOL  (*MDCB_SAVE_FILE_DATA) (int downloadId, const char* url, const void* dataStream,
                size_t dataStreamSize, unsigned long totalSize, const char* suggestedFileName, 
                const char* mimeType , BOOL isFinished);

    /**
     * \fn BOOL (*MDCB_SET_IME_STATUS) (HWND hWnd, BOOL isShowIME)
     * \brief The callback function, which is used to set IME window status.
     *
     * \param hWnd The handle of mdolphin window. 
     * \param isShowIME The status of IME window. If bstate is TRUE, display the IME window. 
     *        Otherwise, hide the IME window.
     *
     * \return TRUE if users want to use ime, otherwise FALSE.
     */
    BOOL (*MDCB_SET_IME_STATUS) (HWND hWnd, BOOL isShowIME);

    /**
     * \fn BOOL  (*MDCB_PROVIDE_AUTH) (const char *title, char *userName, size_t userNameMaxLen,
     *              char * password, size_t passwordMaxLen)
     * \brief The callback function which is used to set the user name and the password for 
     *        HTTP authentication.
     *
     * \param title The title for authentication.
     * \param userName The user name for authentication.
     * \param userNameMaxLen The maximum length of user name.
     * \param password The password for authentication.
     * \param passwordMaxLen The maximum length of password. 
     *
     * \return TRUE if users want to authentication, otherwise FALSE.
     */
    BOOL  (*MDCB_PROVIDE_AUTH) (const char *title, char *userName, size_t userNameMaxLen, 
            char *password, size_t passwordMaxLen);
    
    /**
     * \fn BOOL (*MDCB_URL_IS_VISITED) (const char* url)
     * \brief The callback function which is used to notify whether the url is visited.
     *
     * \param url The URL will be tested.
     * \return TRUE if url is visited, otherwise FALSE.
     */
    BOOL (*MDCB_URL_IS_VISITED) (const char* url);

    /**
     * \fn BOOL (*MDCB_CREATE_POPUP_MENU) (HWND hWnd, const MDPopupMenu* menuInfo)
     * \brief The callback function, which is used to create a context menu.
     *
     * \param hWnd The handle of mDolphin Control.
     * \param menuInfo The structure of context menu information.
     * \return TRUE if users create context menu successfully, otherwise FALSE.
     *
     * \sa MDPopupMenu, mdolphin_perform_popup_menu_event
     */
    BOOL (*MDCB_CREATE_POPUP_MENU) (HWND hWnd, const MDPopupMenu* data);

#define CWS_TOOLBAR          0x01
#define CWS_LOCATION         0x02
#define CWS_MENUBAR          0x04
#define CWS_STATUS           0x08
#define CWS_SCROLLBARS       0x10
#define CWS_RESIZABLE        0x20
#define CWS_MODAL            0x40

    /**
     * \fn HWND (*MDCB_OPEN_WINDOW) (const char* url, DWORD styles, int x, int y, 
                    int width, int height)
     * \brief The callback function, which is used to open a new window.
     *
     * \param url The URL visited by this window.
     * \param styles The window styles.
     *      - CWS_TOOLBAR\n
     *        Tell the window have tool bar.
     *      - CWS_LOCATION\n
     *        Tell the window have location bar.
     *      - CWS_MENUBAR\n
     *        Tell the window have menu bar.
     *      - CWS_STATUS\n
     *        Tell the window have status bar.
     *      - CWS_SCROLLBARS\n
     *        Tell the window have scroll bar.
     *      - CWS_RESIZABLE\n
     *        Tell the window whether it can be resize.
     *      - CWS_MODAL\n
     *        Tell the window it is a modal dialog.
     * \param x The x position which window on screen.
     * \param y The y position which window on screen.
     * \param width The width of this window. can be -1, means the width is normal.
     * \param height The height of this window. can be -1, means the height is normal.
     *
     * \return The new handle of mDolphin Control.
     */
    HWND (*MDCB_OPEN_WINDOW) (const char* url, DWORD styles, int x, int y, int width, int height);

    /**
     * \fn void (*MDCB_CLOSE_WINDOW) (HWND hWnd)
     * \brief The callback function, which is used to close the specified window.
     *
     * \param hWnd The handle of mDolphin Control.
     */
    void (*MDCB_CLOSE_WINDOW) (HWND hWnd);

    /**
     * \fn void (*MDCB_SET_TOOLTIP) (HWND hWnd, int x, int y, const char *message, BOOL isShow)
     * \brief The callback function, which is used to show a tooltip message when meets HTML "alt" tag.
     *
     * \param hWnd The handle of mDolphin Control.
     * \param x The x position of tooltip in window.
     * \param y The y position of tooltip in window.
     * \param message The tooltip message with UTF-8 character encoding. 
     * \param isShow Whether show tip message or not. 
     */
    void (*MDCB_SET_TOOLTIP) (HWND hWnd, int x, int y, const char* message, BOOL isShow);

    /**
     * \fn const char *(*MDCB_CUSTOM_USERAGENT) (const char *url)
     * \brief The callback function, which is used to set User-Agent according to url.
     *
     * \param url The URI of the visiting site.
     * \return The user defined User Agent encoded by UTF-8 for the mDolphin.
     */
    const char *(*MDCB_CUSTOM_USERAGENT) (const char *url);

    /**
     * \fn void(*MDCB_AFTER_DRAWING) (HWND hWnd, HDC hdc, const RECT* pRect)
     * \brief The callback function, which is used to capture image from DC after paint over.
     *
     * \param hWnd The handle of mdolphin.
     * \param hdc The DC of the conent image.
     * \param pRect The rectangle of the image.
     */
    void (*MDCB_AFTER_DRAWING) (HWND hWnd, HDC hdc, const RECT* pRect);

    /**
     * \fn void(*MDCB_BEFORE_DRAWING) (HWND hWnd, HDC hdc, const RECT* pRect)
     * \brief The callback function, which is used to draw to destination DC 
     *        before paint start.
     *
     * \param hWnd The handle of mdolphin.
     * \param hdc The DC of the conent image.
     * \param pRect The rectangle of the image.
     */
    void (*MDCB_BEFORE_DRAWING) (HWND hWnd, HDC hdc, const RECT* pRect);

    /** 
     * \fn BOOL  (*MDCB_SAVE_AS_FILE_DATA) (char *fileName)
     * \brief The callback function which is used to save as the current webpage.
     *        The difference between MDCB_SAVE_FILE_DATA and MDCB_SAVE_AS_FILE is 
     *        that the former downloads a webpage and the latter do the "save as"
     *        the webpage as well as the elements in the webpage.
     *
     * \param fileName The full file path choosed by user for save the webpage.
     *
     * \return TRUE if users want to continue saving the data stream, otherwise FALSE.
     *
     * \note This interface is only for lead-core and no use for mdolphin 3.0.
     */
    BOOL  (*MDCB_SAVE_AS_FILE_DATA) (char *fileName);

#if defined(ENABLE_SSL) && ENABLE_SSL
    /**
     * \fn BOOL (*MDCB_PROVIDE_CLIENT_CERT) (MD_CERT_DATA **cert, MD_SSL_PKEY **pkey, 
                const MD_CERT_NAME **names, int count)
     * \brief During a handshake a server may request a certificate from the client. 
     *        This callback function is used to set a certificate. The certificate/private 
     *        key combination must be set using the x509 and pkey arguments and TRUE must 
     *        be returned. The certificate will be installed into SSL. Return TRUE if you 
     *        set certificates successful, otherwise FALSE.
     *
     * \param cert The certificate.
     * \param pkey The private key.
     * \param names The client CA names of the list of client CAs sent from the server.
     * \param count The client CA number of the list of client CAs sent from the server.
     */
    BOOL (*MDCB_PROVIDE_CLIENT_CERT) (MD_CERT_DATA **x509, MD_SSL_PKEY **evpKey, 
            const MD_CERT_NAME **certNames, int count);

    /**
     * \fn MD_CERT_RESULT (*MDCB_VERIFY_SERVER_CERT) (MD_CERT_RESULT result, MD_CERT_DATA *cert)
     * \brief The callback function, which is used to decide whether trusting the 
     *        current certificate.
     *
     * \param result The result verified by mDolphin. X509_V_RESULT is same with openssl 
     *        verified code. For more information about verified result, please refer to 
     *        x509_vfy.h in openssl.
     * \param cert The current certificate.
     *
     * \return User verified result.
     */
    int (*MDCB_VERIFY_SERVER_CERT) (int x509result, MD_CERT_DATA *x509);
#endif
    /**
     * \fn void (*MDCB_GET_CARET_RECT) (const RECT* caret)
     * \brief The callback function, which is used to get the caret rect information in device.
     *
     * \param caret The caret rect.
     *
     */
    void (*MDCB_GET_CARET_RECT) (const RECT* caret);

} MDCBInfo;

/**
 * \fn BOOL mdSetCallbackInfo (HWND hWnd, const CB_INFO *cb)
 * \brief Set the callback functions of the browser. 
 *        In order to meet particular requirements or customization, 
 *        users need to define some callback functions.
 *
 * \param hWnd The handle of mDolphin control.
 * \param cb The callback functions structure.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa MDCBInfo
 */
BOOL mdSetCallbackInfo (HWND hWnd, const MDCBInfo *cb);

/** @} end of callback */

/**
 * \addtogroup global
 * @{
 */

/**
 * \fn RECT mdGetCaretRect (HWND hwnd)
 * \brief Get the caret rect information of edit box.
 *
 * \Param hwnd The hwnd of mDolphin control.
 * \Param caretRect The pointer to rectangle of the caret.
 */
RECT mdGetCaretRect(HWND hwnd);

/**
 * \fn BOOL mdolphin_get_page_info (HWND hwnd, char *url, int len);
 * \brief Get the page information. Currently, only url can be got.
 *
 * \Param hwnd The hwnd of mDolphin control.
 * \Param url The returned url string.
 * \Param len The length of the url.
 *
 * \return TRUE if page info was successfully got, and url will be 
 *         the URL of the page, FALSE on other wise.
 */
BOOL mdolphin_get_page_info (HWND hwnd, char *url, int len);

/**
 * \fn BOOL mdSaveAsCurrentPage(HWND hWnd, BOOL bHtmlOnly); 
 * \brief save current page to disc.
 *
 * \param hWnd The hwnd of mDolphin control.
 * \param bHtmlOnly Indicaates if only download main html:
 *        * TURE -  html file;
 *        * FALSE - complete web page.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdSaveAsCurrentPage
 */
BOOL mdSaveAsCurrentPage( HWND hwnd, BOOL bHtmlOnly,const char * savedName);

/**
 * \fn BOOL mdDeleteSavedPage(const char *fullpath); 
 * \brief delete  localpage saved by mdolphin_save_current_page.
 *
 * \param fullpath - The path of webpage to be deleted.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdDeleteSavedPage
 */
BOOL mdDeleteSavedPage(const char *fullpath);

/** @} end of global */

//for backward compatibility.
#if defined(ENABLE_BACKWARD_API) && ENABLE_BACKWARD_API
#include "mdolphin_v2.h"
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MDOLPHIN_H_ */

