#ifndef _MDOLPHIN_BACKWARD_API_H_
#define _MDOLPHIN_BACKWARD_API_H_

#if defined(ENABLE_BACKWARD_API) && ENABLE_BACKWARD_API

typedef MDENavigate                 NAV_OPERATION;
typedef MDERenderingMode            RENDERING_MODE;
typedef MDEEncodingName             ENCODING_NAME;
typedef MDScrobarWidth              ScrobarWidth_t;
typedef MDEProxyType                PROXY_TYPE;
typedef MDCBInfo                    CB_INFO;
typedef MDEditorElement             EditorElementInfo;
typedef MDEPopupMenuElementEvent    POPUP_MENU_EVENT_TYPE;
#if defined(ENABLE_SCHEMEEXTENSION) && ENABLE_SCHEMEEXTENSION
typedef MDCB_SCHEME_HANDLER         CB_SCHEME_HANDLER;
#endif

#define NAV_BACKWARD                MD_NAV_BACKWARD
#define NAV_FORWARD                 MD_NAV_FORWARD
#define NAV_GOTO                    MD_NAV_GOTO
#define NAV_RELOAD                  MD_NAV_RELOAD
#define NAV_STOP                    MD_NAV_STOP

#define MD_SCREENVIEW_MODE          MD_RDRMODE_NORMAL
#define MD_SMALLVIEW_MODE           MD_RDRMODE_SMARTFIT
#define MD_VIRTUALVIEW_MODE         MD_RDRMODE_GLOBAL

#define RegularScrollbarWidth       wRegular
#define SmallScrollbarWidth         wSmall
#define MiniScrollbarWidth          wMini

#define FIELD_MAX                   MD_FIELD_MAX
#define PROXY_HTTP                  MD_PROXY_HTTP
#define PROXY_SOCKS4                MD_PROXY_SOCKS4
#define PROXY_SOCKS5                MD_PROXY_SOCKS5
#define PROXY_MAX                   MD_PROXY_MAX

#define MDPI_NO                         MD_POPMENU_NO
#define MDPE_NO                         MD_PME_NO
#define MDPE_OPEN_LINK_IN_NEW_WINDOW    MD_PME_OPEN_LINK_IN_NEW_WINDOW
#define MDPE_DOWNLOAD_LINK_TO_DISK      MD_PME_DOWNLOAD_LINK_TO_DISK
#define MDPE_COPY_LINK_TO_CLIPBOARD     MD_PME_COPY_LINK_TO_CLIPBOARD
#define MDPE_OPEN_IMAGE_IN_NEW_WINDOW   MD_PME_OPEN_IMAGE_IN_NEW_WINDOW
#define MDPE_DOWNLOAD_IMAGE_TO_DISK     MD_PME_DOWNLOAD_IMAGE_TO_DISK
#define MDPE_COPY_IMAGE_TO_CLIPBOARD    MD_PME_COPY_IMAGE_TO_CLIPBOARD
#define MDPE_OPEN_FRAME_IN_NEW_WINDOW   MD_PME_OPEN_FRAME_IN_NEW_WINDOW
#define MDPE_COPY                       MD_PME_COPY
#define MDPE_GOBACK                     MD_PME_GOBACK
#define MDPE_GOFORWARD                  MD_PME_GOFORWARD
#define MDPE_STOP                       MD_PME_STOP
#define MDPE_RELOAD                     MD_PME_RELOAD
#define MDPE_CUT                        MD_PME_CUT
#define MDPE_PASTE                      MD_PME_PASTE
#define MDPE_SEARCH_WEB                 MD_PME_SEARCH_WEB
#define MDPE_OPEN_LINK                  MD_PME_OPEN_LINK
#define MDPE_BOLD                       MD_PME_BOLD
#define MDPE_ITALIC                     MD_PME_ITALIC
#define MDPE_UNDERLINE                  MD_PME_UNDERLINE
#define MDPE_DEFAULT_DIRECTION          MD_PME_DEFAULT_DIRECTION
#define MDPE_LEFT_TO_RIGHT              MD_PME_LEFT_TO_RIGHT
#define MDPE_RIGHT_TO_LEFT              MD_PME_RIGHT_TO_LEFT
#define MDPE_INSPECT_ELEMENT            MD_PME_INSPECT_ELEMENT
#define MDPE_SAVEAS_CURLINK             MD_PME_SAVEAS_CURLINK 
#define MDPE_BASE_APPLICATION_TAG       MD_PME_BASE_APPLICATION_TAG


/* old SETUP_INFO */
typedef struct _SETUP_INFO {
    /** Default language. */
    char default_language[FIELD_MAX];
    /** Standard font. */
    char std_font[FIELD_MAX];
    /** Serif font. */
    char serif_font[FIELD_MAX];
    /** Fixed font. */
    char fixed_font[FIELD_MAX];
    /** Sans serif font. */
    char sans_serif_font[FIELD_MAX];
    /** Medium font size. */
    int medium_fontsize;
    /** Medium fixed font size. */
    int medium_fixed_fontsize;
    /** Default encoding name. */
    ENCODING_NAME default_encoding;
    /** Is autoload images. */
    BOOL autoload_images;
    /** Is autodetect encoding. */
    BOOL autodetect_encoding;
    /** Is enable javascript. */
    BOOL enable_javascript;
    /** Is enable cookies. */
    BOOL enable_cookies;
    /** Is enable plugin. */
    BOOL enable_plugin;
    /** block popup window. */
    BOOL block_popupwindow;
    /** show all at once.*/
    BOOL show_all_at_once;
    ScrobarWidth_t scrollbar_width;
    BOOL enable_websecurity;
    BOOL enable_developer_extras;
} SETUP_INFO;

/**
 * the editor element type enum.
 * \sa CB_SET_IME_STATUS
 */
typedef enum {
    EDITOR_TEXT,
    EDITOR_PASSWORD,
    EDITOR_ISINDEX,
    EDITOR_CHECKBOX,
    EDITOR_RADIO,
    EDITOR_SUBMIT,
    EDITOR_RESET,
    EDITOR_FILE,
    EDITOR_HIDDEN,
    EDITOR_IMAGE,
    EDITOR_BUTTON,
    EDITOR_SEARCH,
    EDITOR_RANGE,
    EDITOR_TEXTAREA,
    EDITOR_SELECT
} EditorElementType;

#define CB_MESSAGE_BOX          MDCB_MESSAGE_BOX   
#define CB_CONFIRM_BOX          MDCB_CONFIRM_BOX   
#define CB_PROMPT_BOX           MDCB_PROMPT_BOX   
#define CB_SET_TITLE            MDCB_SET_TITLE   
#define CB_SET_LOCATION         MDCB_SET_LOCATION   
#define CB_SET_STATUS           MDCB_SET_STATUS   
#define CB_SET_LOADING_STATUS   MDCB_SET_LOADING_STATUS   
#define CB_SET_HISTORY_STATUS   MDCB_SET_HISTORY_STATUS   
#define CB_ERROR                MDCB_ERROR   
#define CB_CHOOSEFILE_BOX       MDCB_CHOOSEFILE_BOX   
#define CB_SAVE_FILE_DATA       MDCB_SAVE_FILE_DATA   
#define CB_SET_IME_STATUS       MDCB_SET_IME_STATUS   
#define CB_PROVIDE_AUTH         MDCB_PROVIDE_AUTH   
#define CB_URL_IS_VISITED       MDCB_URL_IS_VISITED   
#define CB_CREATE_POPUP_MENU    MDCB_CREATE_POPUP_MENU   
#define CB_OPEN_WINDOW          MDCB_OPEN_WINDOW   
#define CB_CLOSE_WINDOW         MDCB_CLOSE_WINDOW   
#define CB_SET_TOOLTIP          MDCB_SET_TOOLTIP   
#define CB_CUSTOM_USERAGENT     MDCB_CUSTOM_USERAGENT   
#define CB_CAPTURE_IMAGE        MDCB_AFTER_DRAWING
#define CB_OWNER_DRAW           MDCB_BEFORE_DRAWING
#define CB_SAVE_AS_FILE_DATA    MDCB_SAVE_AS_FILE_DATA   
#define CB_PROVIDE_CLIENT_CERT  MDCB_PROVIDE_CLIENT_CERT   
#define CB_VERIFY_SERVER_CERT   MDCB_VERIFY_SERVER_CERT   
#define CB_GET_CARET_RECT       MDCB_GET_CARET_RECT   

static inline BOOL mdolphin_navigate(HWND hwnd, NAV_OPERATION op, const char* url, BOOL lockHistory)
{
    return mdNavigate(hwnd, op, url);
}

/**
 * \fn BOOL mdolphin_set_rendering_mode (HWND hwnd, RENDERING_MODE mode, int limit)
 * \brief Set the specified rendering mode on the window.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param mode The mode to set, can be one of the following values:
 *      - MD_SCREENVIEW_MODE\n
 *        True Web layout. 
 *      - MD_SMALLVIEW_MODE\n
 *        Smart fit mode. 
 *      - MD_VIRTUALVIEW_MODE\n 
 *        Global view.
 * \param limit The max width of the view attached to MD_SMALLVIEW_MODE, 
 *        the max width of the view attached to MD_VIRTUALVIEW_MODE.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_set_rendering_mode
 */
static inline BOOL mdolphin_set_rendering_mode(HWND hwnd, RENDERING_MODE mode, int limit)
{
    return mdSetRenderingMode(hwnd, mode, limit);
}

/**
 * \fn BOOL mdolphin_get_rendering_mode(HWND hwnd, RENDERING_MODE *mode)
 * \brief Get the specified rendering mode on the window.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param mode Pointer to a buffer receiving the current rendering mode.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_set_rendering_mode
 */
static inline BOOL mdolphin_get_rendering_mode(HWND hwnd, RENDERING_MODE *mode)
{
    return mdGetRenderingMode(hwnd);
}


/**
 * \fn BOOL mdolphin_get_text_encoding_name (HWND hwnd, ENCODING_NAME *name)
 * \brief Get the text encoding name of the browser.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param name Pointer to a buffer receiving the text encoding name.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_set_text_encoding_name
 */
static inline BOOL mdolphin_get_text_encoding_name (HWND hwnd, ENCODING_NAME *name)
{
    MDEEncodingName n;
    if(!name)
        return FALSE;
    n = mdGetTextEncodingName(hwnd);
    if (n == MD_CHARSET_UNKNOWN)
        return FALSE;
    *name = n;
    return TRUE;
}

/**
 * \fn BOOL mdolphin_set_text_encoding_name (HWND hwnd, ENCODING_NAME name)
 * \brief Set the text encoding name of the browser.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param name The text encoding name.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_set_text_encoding_name
 */

static inline BOOL mdolphin_set_text_encoding_name(HWND hwnd, ENCODING_NAME name)
{
    return mdSetTextEncodingName(hwnd, name);
}

/**
 * \fn BOOL mdolphin_fetch_setup (HWND hwnd, SETUP_INFO *setup)
 * \brief Get the current setup information.
 *
 * \param hwnd The handle of mDolphin control.
 * \param setup Pointer to a buffer receiving the current setup information.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_import_setup
 */
static inline BOOL mdolphin_fetch_setup (HWND hwnd, SETUP_INFO *setup)
{
    if (!hwnd || (HWND_INVALID == hwnd) || !setup)
        return FALSE;
    
    mdGetStringProperty(hwnd, MD_STR_DEFAULT_LANGUAGE, setup->default_language, FIELD_MAX);
    mdGetStringProperty(hwnd, MD_STR_STANDARD_FONT, setup->std_font, FIELD_MAX);
    mdGetStringProperty(hwnd, MD_STR_SERIF_FONT, setup->serif_font, FIELD_MAX);
    mdGetStringProperty(hwnd, MD_STR_FIXED_FONT, setup->fixed_font, FIELD_MAX);
    mdGetStringProperty(hwnd, MD_STR_SANS_SERIF_FONT, setup->sans_serif_font, FIELD_MAX);

    setup->medium_fontsize       = mdGetIntProperty(hwnd, MD_INT_MEDIUM_FONT_SIZE);
    setup->medium_fixed_fontsize = mdGetIntProperty(hwnd, MD_INT_MEDIUM_FIXED_FONT_SIZE);
    setup->default_encoding      = (ENCODING_NAME)mdGetIntProperty(hwnd, MD_INT_ENCODING_NAME);
    setup->autoload_images       = mdGetIntProperty(hwnd, MD_INT_AUTO_LOAD_IMAGES);
    setup->autodetect_encoding   = mdGetIntProperty(hwnd, MD_INT_AUTO_DETECT_ENCODING);
    setup->enable_javascript     = mdGetIntProperty(hwnd, MD_INT_ENABLE_JAVASCRIPT);
    setup->enable_cookies        = mdGetIntProperty(hwnd, MD_INT_ENABLE_COOKIES);
    setup->enable_plugin         = mdGetIntProperty(hwnd, MD_INT_ENABLE_PLUGIN);
    setup->block_popupwindow     = mdGetIntProperty(hwnd, MD_INT_ENABLE_JS_POPUP_WINDOW);
    setup->show_all_at_once      = mdGetIntProperty(hwnd, MD_INT_SHOWALL_AT_ONCE);
    setup->scrollbar_width.wRegular = mdGetIntProperty(hwnd, MD_INT_SCROBAR_WIDTH_REGULAR);
    setup->scrollbar_width.wSmall   = mdGetIntProperty(hwnd, MD_INT_SCROBAR_WIDTH_SMALL);
    setup->scrollbar_width.wMini    = mdGetIntProperty(hwnd, MD_INT_SCROBAR_WIDTH_MINI);

    setup->enable_websecurity = mdGetIntProperty(hwnd, MD_INT_ENABLE_WEB_SECURITY);
    setup->enable_developer_extras = mdGetIntProperty(hwnd, MD_INT_ENABLE_DEVELOPER_EXTRAS);

 
    return TRUE;
}

/**
 * \fn BOOL mdolphin_import_setup (HWND hwnd, const SETUP_INFO *setup)
 * \brief Set the new setup information.
 *
 * \param hwnd The handle of mDolphin control.
 * \param setup Pointer to the new setup information.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_fetch_setup
 */
static inline BOOL mdolphin_import_setup (HWND hwnd, const SETUP_INFO *setup)
{
    if (!hwnd || (HWND_INVALID == hwnd) || !setup)
        return FALSE;
    
    mdSetStringProperty(hwnd, MD_STR_DEFAULT_LANGUAGE, setup->default_language);
    mdSetStringProperty(hwnd, MD_STR_STANDARD_FONT, setup->std_font);
    mdSetStringProperty(hwnd, MD_STR_SERIF_FONT, setup->serif_font);
    mdSetStringProperty(hwnd, MD_STR_FIXED_FONT, setup->fixed_font);
    mdSetStringProperty(hwnd, MD_STR_SANS_SERIF_FONT, setup->sans_serif_font);

    mdSetIntProperty(hwnd, MD_INT_MEDIUM_FONT_SIZE, setup->medium_fontsize);
    mdSetIntProperty(hwnd, MD_INT_MEDIUM_FIXED_FONT_SIZE, setup->medium_fixed_fontsize);
    mdSetIntProperty(hwnd, MD_INT_ENCODING_NAME, setup->default_encoding);
    mdSetIntProperty(hwnd, MD_INT_AUTO_LOAD_IMAGES, setup->autoload_images);
    mdSetIntProperty(hwnd, MD_INT_AUTO_DETECT_ENCODING, setup->autodetect_encoding);
    mdSetIntProperty(hwnd, MD_INT_ENABLE_JAVASCRIPT, setup->enable_javascript);
    mdSetIntProperty(hwnd, MD_INT_ENABLE_COOKIES, setup->enable_cookies);
    mdSetIntProperty(hwnd, MD_INT_ENABLE_PLUGIN, setup->enable_plugin);
    mdSetIntProperty(hwnd, MD_INT_ENABLE_JS_POPUP_WINDOW, setup->block_popupwindow);
    mdSetIntProperty(hwnd, MD_INT_SHOWALL_AT_ONCE, setup->show_all_at_once);
    mdSetIntProperty(hwnd, MD_INT_SCROBAR_WIDTH_REGULAR, setup->scrollbar_width.wRegular);
    mdSetIntProperty(hwnd, MD_INT_SCROBAR_WIDTH_SMALL, setup->scrollbar_width.wSmall);
    mdSetIntProperty(hwnd, MD_INT_SCROBAR_WIDTH_MINI, setup->scrollbar_width.wMini);
    mdSetIntProperty(hwnd, MD_INT_ENABLE_WEB_SECURITY, setup->enable_websecurity);
    mdSetIntProperty(hwnd, MD_INT_ENABLE_DEVELOPER_EXTRAS, setup->enable_developer_extras);

    return TRUE;
}

typedef struct _PROXY_ITEM {
    /** Proxy host. */
    char *host;
    /** Proxy port. */
    char *port;
    /** User name. */
    char *name;
    /** User password. */
    char *pwd;
    /** Proxy type, which can be HTTP, SOCKS4, or SOCKS5. */
    MDEProxyType  type; 
}PROXY_ITEM;

/**
 * The proxy information structure.
 *
 * \sa MDProxyItem, mdGetProxy
 */
typedef struct _PROXY_INFO {
    /** Num of proxy items. */
    int  num;
    /** Proxy item. */
    PROXY_ITEM *items;
} PROXY_INFO;
/**
 * \fn BOOL mdolphin_set_proxy (const PROXY_INFO *proxy_info, BOOL enable)
 * \brief Set the proxy when you use proxy to connnect the network.
 *
 * \param proxy_info The proxy information.
 * \param enable Active proxy.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_set_proxy
 */
static inline BOOL mdolphin_set_proxy (const PROXY_INFO *proxy_info, BOOL enable)
{
	BOOL bReturn=TRUE;

	if (! proxy_info || proxy_info->num <= 0)
		return FALSE;

	mdEnableProxy(enable);

	int num = (proxy_info->num > MD_PROXY_MAX) ? MD_PROXY_MAX:proxy_info->num;
    int i = 0;
	for (i = 0; i<num; i++)
	{
		PROXY_ITEM item = proxy_info->items[i];
		MDProxyItem mditem;
		memset(&mditem,0,sizeof(mditem));

		if(item.host)
			strncpy(mditem.host,item.host,PROXY_MAX_LENGTH-1);
		if(item.name)
			strncpy(mditem.name,item.name,PROXY_MAX_LENGTH-1);
		if(item.pwd)
			strncpy(mditem.pwd,item.pwd,PROXY_MAX_LENGTH-1);

		if(item.port)
			mditem.port=atoi(item.port);

	    bReturn=mdSetProxy(item.type,&mditem);
	    if(bReturn==FALSE)
	    	return bReturn;
	}

	return bReturn;
}

/**
 * \fn BOOL mdolphin_get_proxy (PROXY_TYPE type, PROXY_ITEM *item)
 * \brief Get the proxy information.
 *
 * \param type The proxy type.
 * \param item Proxy item.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_get_proxy
 */
static inline BOOL mdolphin_get_proxy (PROXY_TYPE type, PROXY_ITEM *item)
{
	//PROXY_ITEM is not compaitable with MDProxyItem
	//in 2.0 ,it used PROXY_ITEM,and in the application it is  also not free memory that 
	//the pointers item->host ,item->name,item->pwd,item->port point to.
	//so here we will use static memory instead of  malloc memory.
	static MDProxyItem mditem;
	static char pPort[6]={0};
	if(!item)
		return mdGetProxy(type, NULL);
	BOOL bReturn = mdGetProxy(type,&mditem);
	if(bReturn){
		memset(item,0,sizeof(PROXY_ITEM));
		if(strlen(mditem.host)>0)
			item->host=mditem.host;
		if(strlen(mditem.name)>0)
			item->name=mditem.name;
		if(strlen(mditem.pwd)>0)
			item->pwd=mditem.pwd;
		snprintf(pPort,sizeof(pPort),"%d",mditem.port);
		item->port=pPort;
	}
    return bReturn;
}

/**
 * \fn BOOL mdolphin_perform_popup_menu_event (HWND hwnd, POPUP_MENU_EVENT_TYPE event)
 * \brief Perform the popup menu event.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param event The clicked popup menu item event.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_perform_popup_menu_event
 */
static inline BOOL mdolphin_perform_popup_menu_event (HWND hwnd, POPUP_MENU_EVENT_TYPE event)
{
    return mdPerformPopupMenuEvent(hwnd, (MDEPopupMenuElementEvent)event);
}

/**
 * \fn BOOL mdolphin_search_for (HWND hwnd, const char* str, BOOL forward, 
                BOOL caseSensitive, BOOL wrap) 
 * \brief Searches a document view for a string and highlights the string if it is found.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param str The string to search for.
 * \param forward TRUE to search forward, FALSE to search backwards.
 * \param caseSensitive TRUE to for case-sensitive search, FALSE for case-insensitive search.
 * \param wrap TRUE to wrap around, FALSE to avoid wrapping.
 * \return TRUE if found, FALSE if not found.
 *
 * \sa mdolphin_mark_all_matches_for_text
 */
static inline BOOL mdolphin_search_for (HWND hwnd, const char* str, BOOL forward, 
                BOOL caseSensitive, BOOL wrap)
{
	unsigned int searchFlag=SEARCH_NORMAL;
	if(!forward)
		searchFlag|=SEARCH_BACKWARD ;
	if(caseSensitive)
		searchFlag|=SEARCH_CASE_SENSITIVE ;
	if(wrap)
		searchFlag|=SEARCH_WRAP_AROUND;
    return mdSearchText(hwnd, str,searchFlag);
}

/**
 * \fn unsigned int mdolphin_mark_all_matches_for_text (HWND hwnd, const char *search, 
                BOOL caseSensitive, BOOL highlight, unsigned int limit) 
 * \brief Mark all matching strings in a document view and highlights or unhighlights the strings.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param search The string to search for.
 * \param caseSensitive TRUE to for case-sensitive search, FALSE for case-insensitive search.
 * \param highlight TRUE to highlight, FALSE to unhighlight.
 * \param limitThe specified maching count. A limit of 0 means no limit.
 * \return the matching count, 0 if no match.
 *
 * \sa mdolphin_unmark_all_text_matches, mdolphin_search_for
 */
static inline unsigned int mdolphin_mark_all_matches_for_text (HWND hwnd, const char *search, 
                BOOL caseSensitive, BOOL highlight, unsigned int limit)
{
	unsigned int markFlag=MARK_NORMAL;
	if(!caseSensitive)
		markFlag|=MARK_CASE_SENSITIVE;
	if(!highlight)
		markFlag|=MARK_UNHIGHLIGHT;
    return mdMarkAllMatchedText(hwnd, search,limit,markFlag);
}

/**
 * \fn BOOL mdolphin_unmark_all_text_matches (HWND hwnd)
 * \brief Unmark all matching text in a document view.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_mark_all_matches_for_text
 */
static inline BOOL mdolphin_unmark_all_text_matches (HWND hwnd)
{
    return mdUnmarkAllMatchedText(hwnd);
}

/**
 * \fn BOOL mdolphin_set_in_view_source_mode (HWND hwnd, BOOL flag)
 * \brief Used to place a view into a special source-viewing mode.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param flag TRUE for view source mode, FALSE otherwise.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_in_view_source_mode
 */
static inline BOOL mdolphin_set_in_view_source_mode (HWND hwnd, BOOL flag)
{
    return mdSetSourceView(hwnd, flag);
}


/**
 * \fn BOOL mdolphin_in_view_source_mode (HWND hwnd)
 * \brief Whether or not the view is in source-view mode for HTML.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \return TRUE in view source mode, FALSE otherwise.
 *
 * \sa mdolphin_set_in_view_source_mode
 */
static inline BOOL mdolphin_in_view_source_mode (HWND hwnd)
{
    return mdIsSourceView(hwnd);
}

/**
 * \fn BOOL mdolphin_set_contents_position(HWND hwnd, int x, int y)
 * \brief Used to set the position of the page contents.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param x The x of the contents position.
 * \param y The y of the contents position.
 * \return TRUE on success, FALSE otherwise.
 *
 * \sa mdolphin_get_contents_position
 */
static inline BOOL mdolphin_set_contents_position(HWND hwnd, int x, int y)
{
    return mdSetContentPosition(hwnd, x, y);
}


/**
 * \fn BOOL mdolphin_get_contents_position(HWND hwnd, int* x, int* y)
 * \brief get the position of the page contents.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param x The x of the contents position.
 * \param y The y of the contents position.
 * \return TRUE on success, FALSE otherwise.
 *
 * \sa mdolphin_set_contents_position
 */
static inline BOOL mdolphin_get_contents_position(HWND hwnd, int* x, int* y)
{
    return mdGetContentPosition(hwnd, x, y);
}

/**
 * \fn BOOL mdolphin_get_contents_size(HWND hwnd, int* width, int* height)
 * \brief get the length of the page contents.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param width The width of the contents.
 * \param height The height of the contents.
 * \return TRUE on success, FALSE otherwise.
 */
static inline BOOL mdolphin_get_contents_size(HWND hwnd, int* width, int* height)
{
    return mdGetContentSize(hwnd, width, height);
}

/**
 * \fn void mdolphin_set_scale_factor(HWND hwnd, float x, float y)
 * \brief Used to set the scale factor of the page contents.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param x The x of the horizontal scale factor.
 * \param y The y of the vertical scale factor.
 *
 * \sa mdolphin_get_scale_factor
 */
static inline void mdolphin_set_scale_factor(HWND hwnd, float zoom)
{
    mdSetScaleFactor(hwnd, zoom);
    return;
}

/**
 * \fn BOOL mdolphin_get_text_size_multiplier(HWND hwnd, int *factor)
 * \brief Get the text size multiplier of the browser.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param factor Pointer to a buffer receiving the text size multiplier. 
 *        Factor expresses int percent as an integer (eg 100 means 100 percent).
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_get_text_size_multiplier
 */

static inline BOOL mdolphin_get_text_size_multiplier (HWND hwnd, int *factor)
{
    float f;
    if(!factor)
        return FALSE;
    f = mdGetScaleFactor(hwnd);
    if ( f < 0)
        return FALSE;
    *factor = (int)(f*100);
    return TRUE;
}
/**
 * \fn BOOL mdolphin_get_scale_factor(HWND hwnd, float* x, float* y)
 * \brief get the scale factor of the page contents.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param x The x of the horizontal scale factor.
 * \param y The y of the vertical scale factor.
 * \return TRUE on success, FALSE otherwise.
 *
 * \sa mdolphin_set_scale_factor
 */
static inline BOOL mdolphin_get_scale_factor(HWND hwnd, float* x, float* y)
{
    float f = mdGetScaleFactor(hwnd);
    if (f < -1)
        return FALSE;
    if(x)
      *x = f;
    if(y)
      *y = f;

    return TRUE;
    
}

/**
 * \fn BOOL mdolphin_set_text_size_multiplier (HWND hwnd, int factor)
 * \brief Set the text size multiplier of the browser.
 *
 * \param hwnd The hwnd of mDolphin control.
 * \param factor The specified text size multiplier. 
 *        Factor expresses int percent as an integer (eg 100 means 100 percent).
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_set_text_size_multiplier
 */
static inline BOOL mdolphin_set_text_size_multiplier (HWND hwnd, int factor)
{
    float f;
    f = factor/100.0;
    mdSetScaleFactor(hwnd, f);
    
    return TRUE;
}

#if defined(ENABLE_SCHEMEEXTENSION) && ENABLE_SCHEMEEXTENSION
/**
 * \fn BOOL mdolphin_register_scheme_handler (const char* scheme, CB_SCHEME_HANDLER cb, void* data)
 *
 * \brief Register network protocol scheme handler.
 *
 * \param scheme The scheme of protocol.
 * \param cb The callback of the scheme handler.
 * \param data The parameter of the callback function.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_unregister_scheme_handler
 */
static inline BOOL mdolphin_register_scheme_handler (const char* scheme, CB_SCHEME_HANDLER cb, void* data)
{
    return mdRegisterSchemeHandler(scheme, (MDCB_SCHEME_HANDLER)cb, data);
}

/**
 * \fn BOOL mdolphin_unregister_scheme_handler (const char* scheme) 
 *
 * \brief Unregister network protocol scheme handler.
 * \param scheme The scheme of protocol.
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_register_scheme_handler
 */
static inline BOOL mdolphin_unregister_scheme_handler (const char* scheme)
{
    return mdUnregisterSchemeHandler(scheme);
}
#endif

/**
 * \fn BOOL mdolphin_register_minigui_ctrlclass (void) 
 * \brief Register the mDolphin control class to MiniGUI.
 *
 * \return TRUE if you register successfully, otherwise FALSE.
 *
 * \sa mdolphin_unregister_minigui_ctrlclass
 */
static inline BOOL mdolphin_register_minigui_ctrlclass (void)
{
    return mdRegisterMDolphinControl();
}

static inline BOOL RegisterMDolphinControl (void)
{
    return mdRegisterMDolphinControl();
}

/**
 * \fn void mdolphin_unregister_minigui_ctrlclass (void) 
 * \brief Unregister mDolphin control class from MiniGUI.
 *
 * \return TRUE if there is not any mDolphin control instance, otherwise FALSE.
 *
 * \sa mdolphin_register_minigui_ctrlclass
 */
static inline BOOL mdolphin_unregister_minigui_ctrlclass (void)
{
    return mdUnregisterMDolphinControl();
}

static inline void UnregisterMDolphinControl (void)
{
    mdUnregisterMDolphinControl();
    return;
}

/**
 * \fn unsigned mdolphin_set_cache_capacity (unsigned totalBytes)
 * \brief Set cache total capacity.
 *
 * \Param totalBytes The cache total capacity in bytes.
 * \Return Return the old cache capacity in bytes.
 *
 * \sa mdolphin_get_cache_capacity
 */
static inline unsigned mdolphin_set_cache_capacity (unsigned totalBytes)
{
    return  mdSetCacheCapacity (totalBytes);
}

/**
 * \fn BOOL mdolphin_get_cache_capacity (unsigned* totalBytes)
 * \brief Get cache total capacity.
 *
 * \Param totalBytes The cache total capacity in bytes.
 * \Return Return the cache state, TRUE for enabled and FALSE on other wise.
 *
 * \sa mdolphin_set_cache_capacity 
 */
static inline BOOL mdolphin_get_cache_capacity (unsigned* totalBytes)
{
    if (!totalBytes)
        return FALSE;
    *totalBytes = mdGetCacheCapacity();
    return TRUE;
}

/**
 * \fn void mdolphin_enable_cache(BOOL enabled)
 * \brief Set cache state. Disabling the cache will remove all resources from the cache. 
 * They may still live on if they are referenced by some Web page though.
 *
 * \Param enabled TRUE for enable cache, FALSE on other wise.
 *
 */
static inline void mdolphin_enable_cache (BOOL enabled)
{
    mdEnableCache(enabled);
}

/**
 * \fn void mdolphin_reset_cache (void)
 * \brief Remove the current cache in the memory or in the disk.
 *
 */
static inline void mdolphin_reset_cache (void)
{
    mdClearCache();
}

/**
 * \fn BOOL mdolphin_set_caPath (const char * path) 
 * \brief Specify a certificate directory holding alternate certificates to verify with.
 *
 * \param path The directory holding multiple CA certificates.
 *
 * \return TRUE if you set certificate path successfully, otherwise FALSE.
 */
static inline BOOL mdolphin_set_caPath (const char * path)
{
    //Not supported.
    return FALSE;
}



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
    MEM_CACHE,
    /** 
     * Disk cache type.
     */
    DISK_CACHE,
} CACHE_TYPE;

/**
 * \fn BOOL mdolphin_set_cache_type (CACHE_TYPE type, const char* path, size_t size)
 * \brief Set the cache type. It can only be used in initialization. Not implemented now.
 * 
 * \param type The cache type:
 *      - MEM_CACHE\n
 *        Use memory cache system.
 *      - DISK_CACHE\n
 *        Use disk cache system.
 * \param path The path in which disk cache will be located.
 * \param size The size of cache in bytes.
 *
 * If it is disk cache and the size is 0, the disk cache capacity will be 50 multiplier of 
 * mem cache size.
 */
static inline BOOL mdolphin_set_cache_type (MDECache type, const char* path, size_t size)
{
    return  mdSetCacheType ( type, path, size);
}

/** @} end of cache */

/**
 * \fn BOOL mdolphin_is_cookie_enabled(void);
 * \brief Judge if cookie is enabled.
 *
 * \sa mdolphin_enable_cookie
 * \return TRUE if cookie is enabled, otherwise FALSE.
 */
static inline BOOL mdolphin_is_cookie_enabled (void)
{
    return mdIsCookieEnabled();
}

/**
 * \fn BOOL mdolphin_enable_cookie (BOOL enabled)
 * \brief Set cookie to be enabled.
 *
 * \Param enabled TRUE to enable cookie, FALSE on other wise.
 */
static inline void mdolphin_enable_cookie (BOOL enabled)
{
    return mdEnableCookie(enabled);
}

/**
 * \fn BOOL mdolphin_reset_cookie (void);
 * \brief Reset the cookie.
 *
 */
static inline void mdolphin_reset_cookie (void)
{
    return mdClearCookie();
}

/**
 * \fn BOOL mdolphin_set_callback_info (HWND hwnd, const CB_INFO *cb)
 * \brief Set the callback functions of the browser. 
 *        In order to meet particular requirements or customization, 
 *        users need to define some callback functions.
 *
 * \param hwnd The handle of mDolphin control.
 * \param cb The callback functions structure.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa CB_INFO
 */
static inline BOOL mdolphin_set_callback_info (HWND hwnd, const CB_INFO *cb)
{
    return mdSetCallbackInfo(hwnd, cb);
}

/**
 * \fn void mdolphin_get_caret_rect (HWND hwnd, RECT *caretRect)
 * \brief Get the caret rect information of edit box.
 *
 * \Param hwnd The hwnd of mDolphin control.
 * \Param caretRect The pointer to rectangle of the caret.
 */
static inline void mdolphin_get_caret_rect (HWND hwnd, RECT *caretRect)
{
    if (!hwnd  || !caretRect)
        return;

    *caretRect = mdGetCaretRect(hwnd);
    return;
}

/**
 * \fn BOOL mdolphin_save_current_page (HWND hWnd, BOOL bHtmlOnly); 
 * \brief save current page to disc.
 *
 * \param hWnd The hwnd of mDolphin control.
 * \param bHtmlOnly Indicaates if only download main html:
 *        * TURE -  html file;
 *        * FALSE - complete web page.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_save_current_page
 */
static inline BOOL mdolphin_save_current_page (HWND hwnd, BOOL bHtmlOnly,const char* savedName)
{
	return mdSaveAsCurrentPage(hwnd,bHtmlOnly,savedName);
}

/**
 * \fn BOOL mdolphin_del_localpage (const char *fullpath); 
 * \brief delete  localpage saved by mdolphin_save_current_page.
 *
 * \param fullpath - The path of webpage to be deleted.
 *
 * \return TRUE on success, FALSE on error.
 *
 * \sa mdolphin_del_localpage  
 */
static inline BOOL mdolphin_del_localpage(const char *fullpath)
{
	return mdDeleteSavedPage(fullpath);
}

/**
 * \fn BOOL mdolphin_cleanup (void)
 * \brief Cleanup the internal resources used by mDolphin.
 *
 * \return TRUE if success, otherwise FALSE.
 *
 * \note This function should be called after mdolphin_unregister_minigui_ctrlclass.
 */
static inline BOOL mdolphin_cleanup (void)
{
    return FALSE;
}
#endif

#endif
