SET(MDOLPHIN_MAJOR_VERSION 3)
SET(MDOLPHIN_MINOR_VERSION 0)
SET(MDOLPHIN_MICRO_VERSION 4)

SET(WEBKIT_MAJOR_VERSION 534)
SET(WEBKIT_MINOR_VERSION 32)
SET(WEBKIT_MICRO_VERSION 0)

SET(MDOLPHIN_VERSION ${MDOLPHIN_MAJOR_VERSION}.${MDOLPHIN_MINOR_VERSION}.${MDOLPHIN_MICRO_VERSION})

ADD_DEFINITIONS(-D__MDOLPHIN_LIB__=1)
ADD_DEFINITIONS(-DWTF_PLATFORM_MG=1)
ADD_DEFINITIONS(-DXP_MINIGUI)
SET(WTF_PLATFORM_MG 1)

# -----------------------------------------------------------------------------
# Determine which network backend will be used
# -----------------------------------------------------------------------------
SET(ALL_NETWORK_BACKENDS curl)
SET(NETWORK_BACKEND "curl" CACHE STRING "choose which network backend to use (one of ${ALL_NETWORK_BACKENDS})")
IF (${NETWORK_BACKEND} STREQUAL "curl")
    ADD_DEFINITIONS(-DWTF_USE_CURL=1)
    SET(WTF_USE_CURL 1)
ENDIF(${NETWORK_BACKEND} STREQUAL "curl")

# -----------------------------------------------------------------------------
# Determine which font backend will be used
# -----------------------------------------------------------------------------
SET(ALL_FONT_BACKENDS freetype pango)
SET(FONT_BACKEND "freetype" CACHE STRING "choose which network backend to use (one of ${ALL_FONT_BACKENDS})")

FIND_PACKAGE(MiniGUI REQUIRED)
FIND_PACKAGE(LibXml2 2.6 REQUIRED)
FIND_PACKAGE(LibXslt 1.1.7 REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(CURL REQUIRED)
# -----------------------------------------------------------------------------
# Prints dependent libraries path 
# -----------------------------------------------------------------------------
MESSAGE("")
MESSAGE("---------------------------------------------------------------------")
MESSAGE("Found MiniGUI Include Path: ${MiniGUI_INCLUDE_DIR}")
MESSAGE("Found LibXml2 Include Path: ${LIBXML2_INCLUDE_DIR}")
MESSAGE("Found LibXslt Include Path: ${LIBXSLT_INCLUDE_DIR}")
MESSAGE("Found Curl Include Path: ${CURL_INCLUDE_DIR}")
MESSAGE("Found JPEG Include Path: ${JPEG_INCLUDE_DIR}")
MESSAGE("Found PNG Include Path: ${PNG_PNG_INCLUDE_DIR}")

SET(WTF_USE_PTHREADS 1)
ADD_DEFINITIONS(-DWTF_USE_PTHREADS=1)

SET(JSC_EXECUTABLE_NAME jsc)
SET(mDolphin_LIBRARY_NAME mdolphin)

SET(DATA_DIR ${CMAKE_INSTALL_PREFIX}/share/${mDolphin_LIBRARY_NAME}-${MDOLPHIN_MAJOR_VERSION})
ADD_DEFINITIONS(-DDATA_DIR="${DATA_DIR}")

SET(VERSION_SCRIPT "-Wl,--version-script,${CMAKE_MODULE_PATH}/mdsymbols.filter")

WEBKIT_FEATURE(ENABLE_BLOB "Enable blob slice" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_CHANNEL_MESSAGING "Enable HTML5 channel messaging" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_CLIENT_BASED_GEOLOCATION "Enable client-based geolocation" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_CONTEXT_MENUS "Enable context menus" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_DATABASE "Enable database" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_DATALIST "Enable datalist" DEFAULT ON HTML)
WEBKIT_FEATURE(ENABLE_DIRECTORY_UPLOAD "Enable directory upload" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_DISK_CACHE "Enable disk cache" DEFAULT OFF)
#WEBKIT_FEATURE(ENABLE_DOM_STORAGE "Enable DOM storage" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_EVENTSOURCE "Enable event source" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FAST_MALLOC "Enable TCmalloc instead of system's allocator" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FAST_MOBILE_SCROLLING "Enable fast mobile scrolling" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FILE_SYSTEM "Enable HTML5 FileSystem API" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FILTERS "Enable SVG filters" DEFAULT OFF SVG)
WEBKIT_FEATURE(ENABLE_FTPDIR "Enable FTP directory support" DEFAULT ON)
#WEBKIT_FEATURE(ENABLE_FULLSCREEN_API "Enable Javascript Fullscreen API" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_GEOLOCATION "Enable geolocation" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_ICONDATABASE "Enable icon database" DEFAULT OFF)
#WEBKIT_FEATURE(ENABLE_IMAGE_RESIZER "Enable image resizer API" DEFAULT OFF)
#index database now is valid in v8 only.
#WEBKIT_FEATURE(ENABLE_INDEXED_DATABASE "Enable indexed database API" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_INPUT_SPEECH "Enable speech input API" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_INSPECTOR "Enable inspector" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_JAVASCRIPT_DEBUGGER "Enable JavaScript debugger" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_JIT "Enable JIT code" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_MATHML "Enable MathML" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_MEDIA_STATISTICS "Enable media statistics" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_METER_TAG "Enable HTML5 meter tag" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_NETSCAPE_PLUGIN_API "Enable Netscape plugin API" DEFAULT ON)
#WEBKIT_FEATURE(ENABLE_NOTIFICATIONS "Enable notifications" DEFAULT OFF)
#WEBKIT_FEATURE(ENABLE_OFFLINE_WEB_APPLICATIONS "Enable offline web applications" DEFAULT OFF)
#WEBKIT_FEATURE(ENABLE_OPCODE_STATS "Enable Opcode statistics" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_ORIENTATION_EVENTS "Enable orientation events" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_PROGRESS_TAG "Enable progress tag" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_SHARED_WORKERS "Enable SharedWorkers" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_SVG "Enable SVG" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_SVG_ANIMATION "Enable SVG animation" DEFAULT OFF SVG)
WEBKIT_FEATURE(ENABLE_SVG_AS_IMAGE "Enable SVG as image" DEFAULT OFF SVG)
WEBKIT_FEATURE(ENABLE_SVG_FONTS "Enable SVG fonts" DEFAULT OFF SVG)
WEBKIT_FEATURE(ENABLE_SVG_FOREIGN_OBJECT "Enable SVG foreign object" DEFAULT OFF SVG)
WEBKIT_FEATURE(ENABLE_SVG_USE "Enable SVG use" DEFAULT OFF SVG)
#WEBKIT_FEATURE(ENABLE_TOUCH_EVENTS "Enable Touch Events" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_VIDEO "Enable video" DEFAULT OFF HTML)
WEBKIT_FEATURE(ENABLE_WCSS "Enable WCSS" DEFAULT OFF)
#WEBKIT_FEATURE(ENABLE_WEBGL "Enable WebGL" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_WEB_SOCKETS "Enable Web Sockets" DEFAULT OFF)
#WEBKIT_FEATURE(ENABLE_WEB_AUDIO "Enable Web Audio" DEFAULT OFF)
#WEBKIT_FEATURE(ENABLE_WEB_TIMING "Enable Web Timing" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_WML "Enable WML" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_WORKERS "Enable workers" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_XHTMLMP "Enable XHTML-MP" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_XPATH "Enable XPath" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_XSLT "Enable XSLT" DEFAULT ON)

#mdolphin new features
WEBKIT_FEATURE(ENABLE_BACKWARD_API "Enable backward compatible API for mdolphin V2.x" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_CAIRO_TEXT "Enable cairo drawing for all text" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_CAIRO_MG "Enable cairo for graphics backend" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_CANVAS "Enable Canvas" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_COOKIE "Enable Cookie" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_DEBUG_MSG "Enable debug output information" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FASTMODE "Enable fast mode for callback" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FILECOOKIE "Enable file cookie" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FILESYSTEM "Enable file system" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_FLOATING_PATCH "Enable math patch for uclibc, such as lround." DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FOCUSRING_TV "Enable focus ring style for TV" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_FORCE_DOUBLE_ALIGN "Enable 8 bytes alignment when create arena" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_HIGHQUALITYZOOM "Enable " DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_JSNATIVEBINDING "Enable JS native binding" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_NO_NPTL "Enable it when there is no nptl thread in uclibc" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_PLUGIN "Enable Plugin" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_SSL "Enable SSL" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_SSLFILE "Enable SSL files" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_SCHEMEEXTENSION "Enable scheme extension" DEFAULT ON)
WEBKIT_FEATURE(ENABLE_VIEWSOURCE "Enable source view" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_SPIDER "Enable spider test tool" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_TOOLS "Enable browser and DumpRenderTree test tool" DEFAULT ON)

#for license limitation
WEBKIT_FEATURE(ENABLE_LOADSPLASH "Enable loading splash animation" DEFAULT OFF)
WEBKIT_FEATURE(ENABLE_WATERMARK "Enable water mask limitation" DEFAULT OFF)

IF (NOT ENABLE_DEBUG_MSG)
    SET (NDEBUG 1)
ENDIF ()

IF (ENABLE_LOADSPLASH)
    ADD_DEFINITIONS(-D_MD_ENABLE_LOADSPLASH)
ENDIF (ENABLE_LOADSPLASH)

IF (ENABLE_WATERMARK)
    ADD_DEFINITIONS(-D_MD_ENABLE_WATERMARK)
ENDIF (ENABLE_WATERMARK)

# -----------------------------------------------------------------------------
# Default library types
# -----------------------------------------------------------------------------
OPTION(BUILD_SHARED_LIB "build mdolphin as shared libraries" ON)
OPTION(BUILD_STATIC_LIB "build mdolphin as static libraries" OFF)

IF (BUILD_SHARED_LIB)
    SET(SHARED_CORE ON)
ENDIF (BUILD_SHARED_LIB)

IF (NOT BUILD_SHARED_LIB AND NOT BUILD_STATIC_LIB)
    SET(BUILD_STATIC_LIB ON)
    MESSAGE("Forcing to build mdolphin as static libraries.")
ENDIF (NOT BUILD_SHARED_LIB AND NOT BUILD_STATIC_LIB)

IF (NOT ENABLE_COOKIE AND ENABLE_FILECOOKIE)
    SET(ENABLE_FILECOOKIE OFF)
    MESSAGE("Forcing to close file cookie support. Firstly, you should open cookie support. ")
ENDIF (NOT ENABLE_COOKIE AND ENABLE_FILECOOKIE)

SET(UNICODE_BACKEND "minigui" CACHE STRING "UNICODE_BACKEND=minigui/icu <default=minigui>")

SET(CPACK_SOURCE_GENERATOR TBZ2)

# -----------------------------------------------------------------------------
# Define mDolphin_INCLUDE_DIRECTORIES
# -----------------------------------------------------------------------------
SET(mDolphin_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${WEBCORE_DIR}"
    "${WEBCORE_DIR}/accessibility"
    "${WEBCORE_DIR}/bindings"
    "${WEBCORE_DIR}/bindings/generic"
    "${WEBCORE_DIR}/bindings/js"
    "${WEBCORE_DIR}/bindings/js/specialization"
    "${WEBCORE_DIR}/bridge"
    "${WEBCORE_DIR}/bridge/c"
    "${WEBCORE_DIR}/bridge/jni/jsc"
    "${WEBCORE_DIR}/bridge/jsc"
    "${WEBCORE_DIR}/css"
    "${WEBCORE_DIR}/dom"
    "${WEBCORE_DIR}/dom/default"
    "${WEBCORE_DIR}/editing"
    "${WEBCORE_DIR}/fileapi"
    "${WEBCORE_DIR}/history"
    "${WEBCORE_DIR}/html"
    "${WEBCORE_DIR}/html/canvas"
    "${WEBCORE_DIR}/html/parser"
    "${WEBCORE_DIR}/html/shadow"
    "${WEBCORE_DIR}/inspector"
    "${WEBCORE_DIR}/loader"
    "${WEBCORE_DIR}/loader/appcache"
    "${WEBCORE_DIR}/loader/archive"
    "${WEBCORE_DIR}/loader/cache"
    "${WEBCORE_DIR}/loader/icon"
    "${WEBCORE_DIR}/mathml"
    "${WEBCORE_DIR}/notifications"
    "${WEBCORE_DIR}/page"
    "${WEBCORE_DIR}/page/animation"
    "${WEBCORE_DIR}/platform"
    "${WEBCORE_DIR}/platform/animation"
    "${WEBCORE_DIR}/platform/audio"
    "${WEBCORE_DIR}/platform/graphics"
    "${WEBCORE_DIR}/platform/graphics/filters"
    "${WEBCORE_DIR}/platform/graphics/transforms"
    "${WEBCORE_DIR}/platform/image-decoders"
    "${WEBCORE_DIR}/platform/image-decoders/bmp"
    "${WEBCORE_DIR}/platform/image-decoders/gif"
    "${WEBCORE_DIR}/platform/image-decoders/ico"
    "${WEBCORE_DIR}/platform/image-decoders/jpeg"
    "${WEBCORE_DIR}/platform/image-decoders/png"
    "${WEBCORE_DIR}/platform/image-decoders/webp"
    "${WEBCORE_DIR}/platform/mock"
    "${WEBCORE_DIR}/platform/network"
    "${WEBCORE_DIR}/platform/network/curl"
    "${WEBCORE_DIR}/platform/sql"
    "${WEBCORE_DIR}/platform/text"
    "${WEBCORE_DIR}/platform/text/transcoder"
    "${WEBCORE_DIR}/plugins"
    "${WEBCORE_DIR}/rendering"
    "${WEBCORE_DIR}/rendering/mathml"
    "${WEBCORE_DIR}/rendering/style"
    "${WEBCORE_DIR}/rendering/svg"
    "${WEBCORE_DIR}/storage"
    "${WEBCORE_DIR}/svg"
    "${WEBCORE_DIR}/svg/animation"
    "${WEBCORE_DIR}/svg/graphics"
    "${WEBCORE_DIR}/svg/graphics/filters"
    "${WEBCORE_DIR}/svg/properties"
    "${WEBCORE_DIR}/webaudio"
    "${WEBCORE_DIR}/websockets"
    "${WEBCORE_DIR}/wml"
    "${WEBCORE_DIR}/workers"
    "${WEBCORE_DIR}/xml"

    "${JAVASCRIPTCORE_DIR}/API"
    "${JAVASCRIPTCORE_DIR}/ForwardingHeaders"
    "${JAVASCRIPTCORE_DIR}/assembler"
    "${JAVASCRIPTCORE_DIR}/bytecode"
    "${JAVASCRIPTCORE_DIR}/bytecompiler"
    "${JAVASCRIPTCORE_DIR}/collector/handles"
    "${JAVASCRIPTCORE_DIR}/debugger"
    "${JAVASCRIPTCORE_DIR}/interpreter"
    "${JAVASCRIPTCORE_DIR}/jit"
    "${JAVASCRIPTCORE_DIR}/parser"
    "${JAVASCRIPTCORE_DIR}/pcre"
    "${JAVASCRIPTCORE_DIR}/profiler"
    "${JAVASCRIPTCORE_DIR}/yarr"
    "${JAVASCRIPTCORE_DIR}"
    "${JAVASCRIPTCORE_DIR}/runtime" 

    # for MiniGUI platform
    "${WEBKIT_DIR}"
    "${WEBCORE_DIR}/page/mg"
    "${WEBCORE_DIR}/platform/graphics/mg"
    "${WEBCORE_DIR}/platform/network/mg"
    "${WEBCORE_DIR}/platform/mg"
    "${WEBCORE_DIR}/platform/text/mg"
    "${JAVASCRIPTCORE_DIR}/wtf/text"
    "${WEBKIT_DIR}/mg/WebCoreSupport"
    "${WEBKIT_DIR}/mg/control"

    "${WTF_INCLUDE_DIRECTORIES}"
    "${CMAKE_SOURCE_DIR}"

    "${CURL_INCLUDE_DIR}"
    "${LIBXML2_INCLUDE_DIR}"
    "${LIBXSLT_INCLUDE_DIR}"
    "${MiniGUI_INCLUDE_DIR}"
)

# -----------------------------------------------------------------------------
# append mDolphin_LIBRARIES and mDolphin_INCLUDE_DIRECTORIES
# -----------------------------------------------------------------------------
IF (ENABLE_DISK_CACHE)
    LIST(APPEND mDolphin_INCLUDE_DIRECTORIES 
        "${WEBCORE_DIR}/loader/cache/disk_cache")
ENDIF (ENABLE_DISK_CACHE)

IF (${UNICODE_BACKEND} STREQUAL "icu")
    FIND_PACKAGE(ICU REQUIRED)
    MESSAGE("Found ICU Include Path: ${ICU_INCLUDE_DIRS}")

    SET(WTF_USE_ICU_UNICODE 1)
    ADD_DEFINITIONS(-DWTF_USE_ICU_UNICODE=1)
    LIST(APPEND mDolphin_INCLUDE_DIRECTORIES ${ICU_INCLUDE_DIRS})
    LIST(APPEND mDolphin_LIBRARIES 
        ${ICU_LIBRARIES}
        ${ICU_I18N_LIBRARIES}
        )
    MESSAGE("We are using icu as unicode backend.")

ELSEIF (${UNICODE_BACKEND} STREQUAL "minigui")
    SET(WTF_USE_MINIGUI_UNICODE 1)
    ADD_DEFINITIONS(-DWTF_USE_MINIGUI_UNICODE=1)

    LIST(APPEND mDolphin_INCLUDE_DIRECTORIES
        ${JAVASCRIPTCORE_DIR}/mg/
        ${JAVASCRIPTCORE_DIR}/wtf/unicode/mg/
    )
    MESSAGE("We are using MiniGUI as unicode backend.")
ELSE ()
    MESSAGE(FATAL_ERROR "You must provide an unicode backend.")
ENDIF(${UNICODE_BACKEND} STREQUAL "icu")

IF (ENABLE_SVG AND NOT ENABLE_FILTERS)
    SET(ENABLE_FILTERS ON)
    SET(ENABLE_FILTERS_VALUE 1)
    MESSAGE("Forcing open ENABLE_FILTERS.")
ENDIF (ENABLE_SVG AND NOT ENABLE_FILTERS)

IF (ENABLE_CANVAS OR ENABLE_SVG OR ENABLE_CAIRO_TEXT)
    SET(ENABLE_CAIRO_MG 1)
    SET(ENABLE_CAIRO_MG_VALUE 1)
    MESSAGE("Forcing open ENABLE_CAIRO_MG.")
ENDIF (ENABLE_CANVAS OR ENABLE_SVG OR ENABLE_CAIRO_TEXT)

IF (ENABLE_CAIRO_MG)
    FIND_PACKAGE(Cairo 1.6 REQUIRED)
    MESSAGE("Found Cairo Include Path: ${Cairo_INCLUDE_DIRS}")

    LIST(APPEND mDolphin_INCLUDE_DIRECTORIES ${Cairo_INCLUDE_DIRS})
    LIST(APPEND mDolphin_LIBRARIES ${Cairo_LIBRARIES})
ENDIF (ENABLE_CAIRO_MG)


IF (ENABLE_DATABASE)
    FIND_PACKAGE(Sqlite REQUIRED)
    MESSAGE("Found Sqlite Include Path: ${SQLITE_INCLUDE_DIR}")
    LIST(APPEND mDolphin_LIBRARIES ${SQLITE_LIBRARIES})
    LIST(APPEND mDolphin_INCLUDE_DIRECTORIES ${SQLITE_INCLUDE_DIR})
ENDIF (ENABLE_DATABASE)

IF (NOT ENABLE_FAST_MALLOC)
    ADD_DEFINITIONS(-DUSE_SYSTEM_MALLOC=1)
ENDIF (NOT ENABLE_FAST_MALLOC)

IF (FONT_BACKEND STREQUAL "freetype")
    FIND_PACKAGE(Freetype 9.0 REQUIRED)
    SET(WTF_USE_FREETYPE 1)
    ADD_DEFINITIONS(-DWTF_USE_FREETYPE=1)

ELSE ()
  FIND_PACKAGE(Pango REQUIRED)
  SET(WTF_USE_PANGO 1)
  ADD_DEFINITIONS(-DWTF_USE_PANGO=1)

  IF (NOT ENABLE_GLIB_SUPPORT)
    SET(ENABLE_GLIB_SUPPORT 1)
    MESSAGE("Forcing Glib support")
  ENDIF ()
ENDIF ()

IF (ENABLE_VIDEO)
  IF (NOT ENABLE_GLIB_SUPPORT)
    SET(ENABLE_GLIB_SUPPORT 1)
    MESSAGE("Forcing Glib support")
  ENDIF ()

    FIND_PACKAGE(GStreamer REQUIRED)
    FIND_PACKAGE(GStreamer-App REQUIRED)
    FIND_PACKAGE(GStreamer-Base REQUIRED)
    FIND_PACKAGE(GStreamer-Pbutils REQUIRED)
    FIND_PACKAGE(GStreamer-Interfaces REQUIRED)
    FIND_PACKAGE(GStreamer-Plugins-Base REQUIRED)
    FIND_PACKAGE(GStreamer-Video REQUIRED)

    MESSAGE("Found Gstreamer-App Include Path: ${GStreamer-App_INCLUDE_DIRS}")
    MESSAGE("Found Gstreamer-Pbutils Include Path: ${GStreamer-Pbutils_INCLUDE_DIRS}")
    MESSAGE("Found Gstreamer-Interfaces Include Path: ${GStreamer-Interfaces_INCLUDE_DIRS}")
    MESSAGE("Found Gstreamer-Video Include Path: ${GStreamer-Video_INCLUDE_DIRS}")

    MESSAGE("Found Gstreamer-App LIBRARIES: ${GStreamer-App_LIBRARIES}")
    MESSAGE("Found Gstreamer-Pbutils LIBRARIES: ${GStreamer-Pbutils_LIBRARIES}")
    MESSAGE("Found Gstreamer-Interfaces LIBRARIES: ${GStreamer-Interfaces_LIBRARIES}")
    MESSAGE("Found Gstreamer-Video LIBRARIES: ${GStreamer-Video_LIBRARIES}")

    SET(WTF_USE_GSTREAMER 1)
    ADD_DEFINITIONS(-DWTF_USE_GSTREAMER=1)

    LIST(APPEND mDolphin_INCLUDE_DIRECTORIES
        "${JAVASCRIPTCORE_DIR}/wtf/gobject/"
        "${WEBCORE_DIR}/platform/graphics/gstreamer/"
        ${GStreamer-App_INCLUDE_DIRS}
        ${GStreamer-Pbutils_INCLUDE_DIRS}
        ${GStreamer-Interfaces_INCLUDE_DIRS}
        ${GStreamer-Video_INCLUDE_DIRS}
    )
    LIST(APPEND mDolphin_LIBRARIES
        ${GStreamer-App_LIBRARIES}
        ${GStreamer-Pbutils_LIBRARIES}
        ${GStreamer-Interfaces_LIBRARIES}
        ${GStreamer-Video_LIBRARIES}
        ${Glib_LIBRARIES}
    )
ENDIF (ENABLE_VIDEO)

IF (ENABLE_GLIB_SUPPORT)
    FIND_PACKAGE(GIO REQUIRED)
    FIND_PACKAGE(Glib REQUIRED)
    FIND_PACKAGE(Gthread REQUIRED)
    ADD_DEFINITIONS(-DENABLE_GLIB_SUPPORT=1)

    MESSAGE("Found Glib Include Path: ${Glib_INCLUDE_DIRS}")
    MESSAGE("Found Glib Libraries: ${Glib_LIBRARIES}")

    MESSAGE("Found GIO Include Path: ${GIO_INCLUDE_DIRS}")
    MESSAGE("Found GIO Libraries: ${GIO_LIBRARIES}")
    LIST(APPEND mDolphin_INCLUDE_DIRECTORIES
        ${GIO_INCLUDE_DIRS}
        ${Glib_INCLUDE_DIRS}
    )
    LIST(APPEND mDolphin_LIBRARIES
        ${GIO_LIBRARIES}
        ${Glib_LIBRARIES}
    )
ENDIF()

# -----------------------------------------------------------------------------
# Define mDolphin_LIBRARIES
# -----------------------------------------------------------------------------
LIST(APPEND mDolphin_LIBRARIES
    ${LIBXML2_LIBRARIES}
    ${LIBXSLT_LIBRARIES}
    ${CURL_LIBRARIES}
    )

FOREACH (_file ${mDolphin_LIBRARIES})
    STRING (REGEX MATCH "[a-z|A-Z|0-9|_|-]+[.](a|so|lib|dll|LIB|DLL)[^/]*$" _match_lib "${_file}")
    IF (_match_lib)
        STRING (REGEX MATCH "[^lib][a-z|A-Z|0-9|_|-| ]*[^.so]" _match_name ${_match_lib})
    ELSE()
        SET (_match_name ${_file})
    ENDIF ()

    IF (_match_name)
        #LIST(APPEND mDolphin_DEPLIBS_LIST
        #    "-l${_match_name}"
        #    )
    ENDIF (_match_name)
ENDFOREACH (_file ${mDolphin_LIBRARIES})

LIST(APPEND mDolphin_LIBRARIES
    ${MiniGUI_LIBRARIES}
    ${Freetype_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
    pthread
    )

MESSAGE("---------------------------------------------------------------------")
MESSAGE("")
