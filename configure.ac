AC_PREREQ(2.59)

m4_define([mdolphin_major_version], [3])
m4_define([mdolphin_minor_version], [0])
m4_define([mdolphin_micro_version], [4])

AC_INIT([libmdolphin],[mdolphin_major_version.mdolphin_minor_version.mdolphin_micro_version],[http://bugs.webkit.org/])

AC_CONFIG_MACRO_DIR([Source/autotools])
AC_CONFIG_AUX_DIR([Source/autotools])
AC_SUBST(ACLOCAL_AMFLAGS, "-I Source/autotools")

AC_CONFIG_HEADERS([mdconfig.h])
AC_CANONICAL_HOST

mDolphin_MAJOR_VERSION=mdolphin_major_version
mDolphin_MINOR_VERSION=mdolphin_minor_version
mDolphin_MICRO_VERSION=mdolphin_micro_version
mDolphin_INTERFACE_AGE=0
mDolphin_BINARY_AGE=0
mDolphin_VERSION=$mDolphin_MAJOR_VERSION.$mDolphin_MINOR_VERSION.$mDolphin_MICRO_VERSION

AC_SUBST(mDolphin_MAJOR_VERSION)
    AC_SUBST(mDolphin_MINOR_VERSION) AC_SUBST(mDolphin_MICRO_VERSION) AC_SUBST(mDolphin_INTERFACE_AGE) AC_SUBST(mDolphin_BINARY_AGE) AC_SUBST(mDolphin_VERSION) AC_DEFINE_UNQUOTED(MDOLPHIN_MAJOR_VERSION, $mDolphin_MAJOR_VERSION, [Major version of mDolphin]) AC_DEFINE_UNQUOTED(MDOLPHIN_MINOR_VERSION, $mDolphin_MINOR_VERSION,
                        [Minor version of mDolphin])
AC_DEFINE_UNQUOTED(MDOLPHIN_MICRO_VERSION, $mDolphin_MICRO_VERSION,
                        [Micro version of mDolphin])
AC_DEFINE_UNQUOTED(MDOLPHIN_INTERFACE_AGE, $mDolphin_INTERFACE_AGE,
                        [Interface age of mDolphin])
AC_DEFINE_UNQUOTED(MDOLPHIN_BINARY_AGE, $mDolphin_BINARY_AGE,
                        [Binary age of mDolphin])


# This is the version we'll be using as part of our User-Agent string
WEBKIT_MAJOR_VERSION=534;
WEBKIT_MINOR_VERSION=26;
WEBKIT_MICRO_VERSION=0;

AC_SUBST(WEBKIT_MAJOR_VERSION)
AC_SUBST(WEBKIT_MINOR_VERSION)
AC_SUBST(WEBKIT_MICRO_VERSION)

AC_DEFINE_UNQUOTED(WEBKIT_MAJOR_VERSION, $WEBKIT_MAJOR_VERSION,
                        [Major version of WebKit])
AC_DEFINE_UNQUOTED(WEBKIT_MINOR_VERSION,  $WEBKIT_MINOR_VERSION,
                        [Minor version of WebKit])
AC_DEFINE_UNQUOTED(WEBKIT_MICRO_VERSION, $WEBKIT_MICRO_VERSION,
                        [Micro version of WebKit])

AC_CONFIG_SRCDIR([Source/WebCore/config.h])

dnl # Libtool library version, not to confuse with API version
dnl # see http://www.gnu.org/software/libtool/manual/html_node/Libtool-versioning.html
LT_RELEASE=$mDolphin_MAJOR_VERSION.$mDolphin_MINOR_VERSION
LT_CURRENT=`expr $mDolphin_MICRO_VERSION - $mDolphin_INTERFACE_AGE`
LT_REVISION=$mDolphin_INTERFACE_AGE
LT_AGE=`expr $mDolphin_BINARY_AGE - $mDolphin_INTERFACE_AGE`

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

LIBWEBKITMD_VERSION=$LT_CURRENT:$LT_REVISION:$LT_AGE
AC_SUBST([LIBWEBKITMD_VERSION])

AM_INIT_AUTOMAKE([foreign subdir-objects tar-ustar])

# Use AM_SILENT_RULES if present
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# host checking - inspired by the GTK+ configure.in
# TODO: move these to webkit.m4?
AC_MSG_CHECKING([for native Win32])
case "$host" in
     *-*-mingw*)
       os_win32=yes
       ;;
     *)
       os_win32=no
       ;;
esac
AC_MSG_RESULT([$os_win32])

case "$host" in
     *-*-linux*)
       os_linux=yes
       ;;
     *-*-freebsd*)
       os_freebsd=yes
       ;;
     *-*-darwin*)
       os_darwin=yes
       ;;
esac

case "$host_os" in
     gnu* | linux* | k*bsd*-gnu)
       os_gnu=yes
       ;;
     *)
       os_gnu=no
       ;;
esac

# initialize webkit options
WEBKIT_INIT
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
# use dolt to speedup the build
DOLT

#AC_PATH_PROG(FLEX, flex)
#if test -z "$FLEX"; then
#   AC_MSG_ERROR([You need the 'flex' lexer generator to compile WebKit])
#else
#    FLEX_VERSION=`$FLEX --version | sed 's,.*\ \([0-9]*\.[0-9]*\.[0-9]*\)$,\1,'`
#    AX_COMPARE_VERSION([2.5.33],[gt],[$FLEX_VERSION],
#        AC_MSG_WARN([You need at least version 2.5.33 of the 'flex' lexer generator to compile WebKit correctly]))
#fi

#AC_PATH_PROG(GPERF, gperf)
#if test -z "$GPERF"; then
#   AC_MSG_ERROR([You need the 'gperf' hash function generator to compile WebKit])
#fi

# pthread (not needed on Windows)
if test "$os_win32" = "no"; then
AC_CHECK_HEADERS([pthread.h],
                 AC_DEFINE([HAVE_PTHREAD_H],[1],[Define if pthread exists]),
                 AC_MSG_ERROR([pthread support is required to build WebKit]))
fi

# check for libjpeg the way Gtk does it.
AC_CHECK_LIB(jpeg, jpeg_destroy_decompress,
                   jpeg_ok=yes, jpeg_ok=no
                   AC_MSG_ERROR([JPEG library (libjpeg) not found]))
if test "$jpeg_ok" = yes; then
   AC_MSG_CHECKING([for jpeglib])
   AC_TRY_CPP(
[#include <stdio.h>
#undef PACKAGE
#undef VERSION
#undef HAVE_STDLIB_H
#include <jpeglib.h>],
         jpeg_ok=yes,
         jpeg_ok=no)
   AC_MSG_RESULT($jpeg_ok)
   if test "$jpeg_ok" = yes; then
      JPEG_LIBS="-ljpeg"
      # should we check for progressive JPEG like GTK+ as well?
   else
      AC_MSG_ERROR([JPEG library (libjpeg) not found])
   fi
fi
AC_SUBST([JPEG_LIBS])

# Check for libpng the way Gtk+ does it
for l in libpng libpng14 libpng12; do
  AC_MSG_CHECKING(for $l)
  if $PKG_CONFIG --exists $l ; then
    AC_MSG_RESULT(yes)
    PNG_LIBS=`$PKG_CONFIG --libs $l`
    png_ok=yes
    break
  else
    AC_MSG_RESULT(no)
    png_ok=no
  fi
done
if test "$png_ok" != yes; then
  AC_CHECK_LIB(png, png_read_info,
    [AC_CHECK_HEADER(png.h,
      png_ok=yes,
      png_ok=no)],
    AC_MSG_ERROR([PNG library (libpng) not found]), -lz -lm)
  if test "$png_ok" = yes; then
    AC_MSG_CHECKING([for png_structp in png.h])
    AC_TRY_COMPILE([#include <png.h>],
      [png_structp pp; png_infop info; png_colorp cmap; png_create_read_struct;],
      png_ok=yes,
      png_ok=no)
    AC_MSG_RESULT($png_ok)
    if test "$png_ok" = yes; then
      PNG_LIBS='-lpng -lz'
    else
      AC_MSG_ERROR([PNG library (libpng) not found])
    fi
  else
    AC_MSG_ERROR([PNG library (libpng) not found])
  fi
fi
AC_SUBST([PNG_LIBS])


if test "$os_win32" = "yes"; then
  WINMM_LIBS=-lwinmm
  SHLWAPI_LIBS=-lshlwapi
  OLE32_LIBS=-lole32
fi
AC_SUBST([WINMM_LIBS])
AC_SUBST([SHLWAPI_LIBS])
AC_SUBST([OLE32_LIBS])


AC_MSG_CHECKING([whether to enable spellcheck support])
AC_ARG_ENABLE([spellcheck],
  [AS_HELP_STRING([--enable-spellcheck],[enable support for spellcheck])],
  [],[enable_spellcheck="no"])
AC_MSG_RESULT([$enable_spellcheck])

if test "$enable_spellcheck" = "yes"; then
PKG_CHECK_MODULES(ENCHANT, enchant >= $ENCHANT_REQUIRED_VERSION, [], [enable_spellcheck="no"])
AC_SUBST(ENCHANT_CFLAGS)
AC_SUBST(ENCHANT_LIBS)
fi

# check whether to build with debugging enabled
AC_MSG_CHECKING([whether to do a debug build])
AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug],
                             [turn on debugging [default=no]]),
              [],[enable_debug="no"])
AC_MSG_RESULT([$enable_debug])

# check whether to enable optimized builds
AC_MSG_CHECKING([whether to enable optimized builds])
AC_ARG_ENABLE(optimizations,
              AC_HELP_STRING([--enable-optimizations],
                             [turn on optimize builds (GCC only)
                             [default=yes]]),
              [enable_optimizations=$enableval],
              [if test "$enable_debug" = "yes"; then enable_optimizations="no"; else enable_optimizations="yes"; fi])
AC_MSG_RESULT([$enable_optimizations])

# check whether to enable 3D transforms support
AC_MSG_CHECKING([whether to enable support for 3D Transforms])
AC_ARG_ENABLE(3d_transforms,
              AC_HELP_STRING([--enable-3d-transforms],
                             [enable support for 3D transforms [default=no]]),
              [],[enable_3d_transforms="no"])
AC_MSG_RESULT([$enable_3d_transforms])

# check whether to enable WebGL support
AC_MSG_CHECKING([whether to enable WebGL support])
AC_ARG_ENABLE(webgl,
              AC_HELP_STRING([--enable-webgl],
                             [enable support for WebGL (experimental) [default=no]]),
              [], [enable_webgl="no"])
AC_MSG_RESULT([$enable_webgl])

if test "$enable_webgl" = "yes"; then
    AC_CHECK_HEADERS([GL/gl.h], [], AC_MSG_ERROR([OpenGL header not found]))
    AC_CHECK_HEADERS([GL/glx.h], [], AC_MSG_ERROR([GLX header not found]))
    OPENGL_LIBS=-lGL
fi
AC_SUBST([OPENGL_LIBS])

# check whether to enable channel messaging support
AC_MSG_CHECKING([whether to enable HTML5 Channel Messaging support])
AC_ARG_ENABLE(channel_messaging,
              AC_HELP_STRING([--enable-channel-messaging],
                             [enable HTML5 channel messaging support [default=no]]),
              [],[enable_channel_messaging="no"])
AC_MSG_RESULT([$enable_channel_messaging])

# check whether to enable notifications
AC_MSG_CHECKING([whether to enable notifications])
AC_ARG_ENABLE(notifications,
              AC_HELP_STRING([--enable-notifications],
                             [enable notifications [default=no]]),
              [],[enable_notifications="no"])
AC_MSG_RESULT([$enable_notifications])

# check whether to enable the meter tag
AC_MSG_CHECKING([whether to enable HTML5 meter tag])
AC_ARG_ENABLE(meter_tag,
              AC_HELP_STRING([--enable-meter-tag],
                             [enable HTML5 meter [default=no]]),
              [],[enable_meter_tag="no"])
AC_MSG_RESULT([$enable_meter_tag])

# check whether to enable the progress tag
AC_MSG_CHECKING([whether to enable HTML5 progress tag])
AC_ARG_ENABLE(progress_tag,
              AC_HELP_STRING([--enable-progress-tag],
                             [enable HTML5 progress [default=no]]),
              [],[enable_progress_tag="no"])
AC_MSG_RESULT([$enable_progress_tag])

# check whether to enable JavaScript debugger/profiler support
AC_MSG_CHECKING([whether to enable JavaScript debugger/profiler support])
AC_ARG_ENABLE(javascript_debugger,
              AC_HELP_STRING([--enable-javascript-debugger],
                             [enable JavaScript debugger/profiler support [default=no]]),
              [],[enable_javascript_debugger="no"])
AC_MSG_RESULT([$enable_javascript_debugger])

# check whether to build with datagrid support
#AC_MSG_CHECKING([whether to enable HTML5 datagrid support])
#AC_ARG_ENABLE(datagrid,
#              AC_HELP_STRING([--enable-datagrid],
#                             [enable HTML5 datagrid support [default=no]]),
#              [],[enable_datagrid="no"])
#AC_MSG_RESULT([$enable_datagrid])

# check whether to build with data transfer items support
AC_MSG_CHECKING([whether to enable HTML5 data transfer items support])
AC_ARG_ENABLE(data_transfer_items,
              AC_HELP_STRING([--enable-data-transfer-items],
                             [enable HTML5 data transfer items support [default=no]]),
              [],[enable_data_transfer_items="no"])
AC_MSG_RESULT([$enable_data_transfer_items])

# check whether to enable HTML5 Offline Web Applications support
AC_MSG_CHECKING([whether to enable HTML5 offline web applications support])
AC_ARG_ENABLE(offline_web_applications,
              AC_HELP_STRING([--enable-offline-web-applications],
                             [enable HTML5 offline web applications support [default=no]]),
              [],[enable_offline_web_applications="no"])
AC_MSG_RESULT([$enable_offline_web_applications])

# check whether to enable HTML5 client-side session and persitent storage support
AC_MSG_CHECKING([whether to enable HTML5 client-side session and persistent storage support])
AC_ARG_ENABLE(dom_storage,
              AC_HELP_STRING([--enable-dom-storage],
                             [enable HTML5 client-side session and persistent storage support [default=no]]),
              [],[enable_dom_storage="no"])
AC_MSG_RESULT([$enable_dom_storage])

# check whether to enable the indexed database API
AC_MSG_CHECKING([whether to enable the indexed database API])
AC_ARG_ENABLE(indexed_database,
              AC_HELP_STRING([--enable-indexed-database],
                             [enable the indexed database API [default=no]]),
              [],[enable_indexed_database="no"])
AC_MSG_RESULT([$enable_indexed_database])

# check whether to enable the speech input API
AC_MSG_CHECKING([whether to enable the speech input API])
AC_ARG_ENABLE(input_speech,
              AC_HELP_STRING([--enable-input-speech],
                             [enable the speech input API [default=no]]),
              [],[enable_input_speech="no"])
AC_MSG_RESULT([$enable_input_speech])

# check whether to build with database support
AC_MSG_CHECKING([whether to enable HTML5 client-side database storage support])
AC_ARG_ENABLE(database,
              AC_HELP_STRING([--enable-database],
                             [enable HTML5 client-side database storage support [default=no]]),
              [],[enable_database="no"])
AC_MSG_RESULT([$enable_database])

# check whether to build with server-sent events support
AC_MSG_CHECKING([whether to enable HTML5 server-sent events support])
AC_ARG_ENABLE(eventsource,
              AC_HELP_STRING([--enable-eventsource],
                             [enable HTML5 server-sent events support [default=no]]),
              [],[enable_eventsource="no"])
AC_MSG_RESULT([$enable_eventsource])

# check whether to build with icon database support
AC_MSG_CHECKING([whether to enable icon database support])
AC_ARG_ENABLE(icon_database,
              AC_HELP_STRING([--enable-icon-database],
                             [enable icon database [default=no]]),
              [],[enable_icon_database="no"])
AC_MSG_RESULT([$enable_icon_database])

# check whether to build with image resizer API support
AC_MSG_CHECKING([whether to enable image resizer API support])
AC_ARG_ENABLE(image_resizer,
              AC_HELP_STRING([--enable-image-resizer],
                             [enable image resizer [default=no]]),
              [],[enable_image_resizer="no"])
AC_MSG_RESULT([$enable_image_resizer])

# check whether to enable HTML5 datalist support
AC_MSG_CHECKING([whether to enable HTML5 datalist support])
AC_ARG_ENABLE(datalist,
              AC_HELP_STRING([--enable-datalist],
                             [enable HTML5 datalist support [default=no]]),
              [],[enable_datalist="no"])
AC_MSG_RESULT([$enable_datalist])

# check whether to enable HTML5 sandbox iframe support
AC_MSG_CHECKING([whether to enable HTML5 sandboxed iframe support])
AC_ARG_ENABLE(sandbox,
              AC_HELP_STRING([--enable-sandbox],
                             [enable HTML5 sandboxed iframe support [default=no]]),
              [],[enable_sandbox="no"])
AC_MSG_RESULT([$enable_sandbox])

# check whether to enable HTML5 audio/video support
AC_MSG_CHECKING([whether to enable HTML5 video support])
AC_ARG_ENABLE(video,
              AC_HELP_STRING([--enable-video],
                             [enable HTML5 video support [default=no]]),
              [],[enable_video="no"])
AC_MSG_RESULT([$enable_video])

# check whether to enable media statistics support
AC_MSG_CHECKING([whether to enable media statistics support])
AC_ARG_ENABLE(media_statistics,
              AC_HELP_STRING([--enable-media-statistics],
                             [enable support for media statistics [default=no]]),
              [], [enable_media_statistics="no"])
AC_MSG_RESULT([$enable_media_statistics])

# check whether to enable Javascript Fullscreen API support
AC_MSG_CHECKING([whether to enable Fullscreen API support])
AC_ARG_ENABLE(fullscreen_api,
              AC_HELP_STRING([--enable-fullscreen-api],
                             [enable the Fullscreen API support [default=no]]),
              [],[enable_fullscreen_api="no"])
AC_MSG_RESULT([$enable_fullscreen_api])

# check whether to enable XHTML-MP support
AC_MSG_CHECKING([whether to enable XHTML-MP support])
AC_ARG_ENABLE(xhtmlmp,
              AC_HELP_STRING([--enable-xhtmlmp],
                             [enable support for XHTML-MP [default=no]]),
              [],[enable_xhtmlmp="no"])
AC_MSG_RESULT([$enable_xhtmlmp])

# check whether to enable XPath support
AC_MSG_CHECKING([whether to enable XPath support])
AC_ARG_ENABLE(xpath,
              AC_HELP_STRING([--enable-xpath],
                             [enable support for XPath [default=yes]]),
              [],[enable_xpath="yes"])
AC_MSG_RESULT([$enable_xpath])

# check whether to enable XSLT support
AC_MSG_CHECKING([whether to enable XSLT support])
AC_ARG_ENABLE(xslt,
              AC_HELP_STRING([--enable-xslt],
                             [enable support for XSLT [default=yes]]),
              [],[enable_xslt="yes"])
AC_MSG_RESULT([$enable_xslt])

# check whether to enable geolocation support
AC_MSG_CHECKING([whether to enable geolocation support])
AC_ARG_ENABLE(geolocation,
              AC_HELP_STRING([--enable-geolocation],
                             [enable support for geolocation [default=no]]),
              [],[enable_geolocation="no"])
AC_MSG_RESULT([$enable_geolocation])

# check whether to enable client-based geolocation support
AC_MSG_CHECKING([whether to enable client-based geolocation support])
AC_ARG_ENABLE(client_based_geolocation,
              AC_HELP_STRING([--enable-client-based-geolocation],
                             [enable support for client-based geolocation [default=no]]),
              [],[enable_client_based_geolocation="no"])
AC_MSG_RESULT([$enable_client_based_geolocation])

# check whether to enable MathML support
AC_MSG_CHECKING([whether to enable MathML support])
AC_ARG_ENABLE(mathml,
              AC_HELP_STRING([--enable-mathml],
                             [enable support for MathML [default=no]]),
              [],[enable_mathml="no"])
AC_MSG_RESULT([$enable_mathml])

# check whether to enable SVG support
AC_MSG_CHECKING([whether to enable SVG support])
AC_ARG_ENABLE(svg,
              AC_HELP_STRING([--enable-svg],
                             [enable support for SVG [default=no]]),
              [],[enable_svg="no"])
AC_MSG_RESULT([$enable_svg])

# check whether to enable WML support
AC_MSG_CHECKING([whether to enable WML support])
AC_ARG_ENABLE(wml,
              AC_HELP_STRING([--enable-wml],
                             [enable support for WML [default=no]]),
              [],[enable_wml="no"])
AC_MSG_RESULT([$enable_wml])

# check whether to enable WCSS support
AC_MSG_CHECKING([whether to enable WCSS support])
AC_ARG_ENABLE(wcss,
              AC_HELP_STRING([--enable-wcss],
                             [enable support for WCSS [default=no]]),
              [],[enable_wcss="no"])
AC_MSG_RESULT([$enable_wcss])

# check whether to enable SharedWorkers support
AC_MSG_CHECKING([whether to enable SharedWorkers support])
AC_ARG_ENABLE(shared_workers,
              AC_HELP_STRING([--enable-shared-workers],
                             [enable support for SharedWorkers [default=no]]),
              [],[enable_shared_workers="no"])
AC_MSG_RESULT([$enable_shared_workers])

# check whether to enable Web Workers support
AC_MSG_CHECKING([whether to enable Web Workers support])
AC_ARG_ENABLE(workers,
              AC_HELP_STRING([--enable-workers],
                             [enable support for Web Workers [default=no]]),
              [],[enable_workers="no"])
AC_MSG_RESULT([$enable_workers])

# check whether to enable directory upload support
AC_MSG_CHECKING([whether to enable directory upload support])
AC_ARG_ENABLE(directory_upload,
              AC_HELP_STRING([--enable-directory-upload],
                             [enable support for directory upload [default=no]]),
              [], [enable_directory_upload="no"])
AC_MSG_RESULT([$enable_directory_upload])

# check whether to enable HTML5 FileSystem API support
AC_MSG_CHECKING([whether to enable HTML5 FileSystem API support])
AC_ARG_ENABLE(file_system,
              AC_HELP_STRING([--enable-file-system],
                             [enable support for HTML5 FileSystem API [default=no]]),
              [], [enable_file_system="no"])
AC_MSG_RESULT([$enable_file_system])

# check whether to enable Quota API support
AC_MSG_CHECKING([whether to enable Quota API support])
AC_ARG_ENABLE(quota,
              AC_HELP_STRING([--enable-quota],
                             [enable support for Quota API [default=no]]),
              [], [enable_quota="no"])
AC_MSG_RESULT([$enable_quota])

# turn off svg features if --disable-svg is requested
if test "$enable_svg" = "no"; then
   enable_svg_animation=no
   enable_svg_fonts=no
   enable_svg_foreign_object=no
   enable_svg_as_image=no
   enable_svg_use=no
fi

# check whether to enable support for SVG animation
AC_MSG_CHECKING([whether to enable support for SVG animation])
AC_ARG_ENABLE(svg_animation,
              AC_HELP_STRING([--enable-svg-animation],
                             [enable support for SVG animation (experimental) [default=no]]),
              [],[enable_svg_animation="no"])
AC_MSG_RESULT([$enable_svg_animation])

# check whether to enable support for filters
AC_MSG_CHECKING([whether to enable support for filters])
AC_ARG_ENABLE(filters,
              AC_HELP_STRING([--enable-filters],
                             [enable support for filters (experimental) [default=no]]),
              [],[enable_filters="no"])
AC_MSG_RESULT([$enable_filters])

# check whether to enable support for SVG fonts
AC_MSG_CHECKING([whether to enable support for SVG fonts])
AC_ARG_ENABLE(svg_fonts,
              AC_HELP_STRING([--enable-svg-fonts],
                             [enable support for SVG fonts (experimental) [default=no]]),
              [],[enable_svg_fonts="no"])
AC_MSG_RESULT([$enable_svg_fonts])

# check whether to enable foreign objects support for SVG
AC_MSG_CHECKING([whether to enable support for SVG foreign objects])
AC_ARG_ENABLE(svg_foreign_object,
              AC_HELP_STRING([--enable-svg-foreign-object],
                             [enable support for SVG foreign objects (experimental) [default=no]]),
              [],[enable_svg_foreign_object="no"])
AC_MSG_RESULT([$enable_svg_foreign_object])

# check whether to enable SVG As Image support
AC_MSG_CHECKING([whether to enable SVG as Image support])
AC_ARG_ENABLE(svg_as_image,
              AC_HELP_STRING([--enable-svg-as-image],
                             [enable SVG as Image support (experimental) [default=no]]),
              [],[enable_svg_as_image="no"])
AC_MSG_RESULT([$enable_svg_as_image])

# check whether to enable SVG USE element support
AC_MSG_CHECKING([whether to enable support for SVG use element])
AC_ARG_ENABLE(svg_use,
              AC_HELP_STRING([--enable-svg-use],
                             [enable SVG use element support (experimental) [default=no]]),
              [],[enable_svg_use="no"])
AC_MSG_RESULT([$enable_svg_use])

# check for SVG features, enabling SVG if necessary
if test "$enable_svg_animation" = "yes" || \
   test "$enable_svg_fonts" = "yes" || \
   test "$enable_svg_foreign_object" = "yes" || \
   test "$enable_svg_as_image" = "yes" || \
   test "$enable_svg_use" = "yes"; then
   svg_flags=yes
   if test "$enable_svg" = "no"; then
       AC_MSG_WARN([SVG feature(s) requested but SVG is disabled.. Enabling SVG support])
       enable_svg=yes
   fi
fi

# check whether to enable Web Socket support
AC_MSG_CHECKING([whether to enable Web Sockets support])
AC_ARG_ENABLE(web_sockets,
              AC_HELP_STRING([--enable-web-sockets],
                             [enable support for Web Sockets [default=no]]),
              [],[enable_web_sockets="no"])
AC_MSG_RESULT([$enable_web_sockets])

# check whether to enable Web Audio support
AC_MSG_CHECKING([whether to enable Web Audio support])
AC_ARG_ENABLE(web_audio,
              AC_HELP_STRING([--enable-web-audio],
                             [enable support for Web Audio [default=no]]),
              [],[enable_web_audio="no"])
AC_MSG_RESULT([$enable_web_audio])

# check whether to enable Web Timing support
AC_MSG_CHECKING([whether to enable Web Timing support])
AC_ARG_ENABLE(web_timing,
              AC_HELP_STRING([--enable-web-timing],
                             [enable support for Web Timing [default=no]]),
              [],[enable_web_timing="no"])
AC_MSG_RESULT([$enable_web_timing])

# check whether to enable Blob support
AC_MSG_CHECKING([whether to enable Blob support])
AC_ARG_ENABLE(blob,
              AC_HELP_STRING([--enable-blob],
                             [enable support for Blob [default=no]]),
              [],[enable_blob="no"])
AC_MSG_RESULT([$enable_blob])

# check whether to enable Fast Mobile Scrolling support
AC_MSG_CHECKING([whether to enable Fast Mobile Scrolling])
AC_ARG_ENABLE(fast_mobile_scrolling,
              AC_HELP_STRING([--enable-fast-mobile-scrolling],
                             [enable support for Fast Mobile Scrolling [default=no]]),
              [],[enable_fast_mobile_scrolling="no"])
AC_MSG_RESULT([$enable_fast_mobile_scrolling])

# check whether to enable code coverage
AC_MSG_CHECKING([whether to enable code coverage support])
AC_ARG_ENABLE(coverage,
              AC_HELP_STRING([--enable-coverage],
                             [enable code coverage support [default=no]]),
              [],[enable_coverage="no"])
AC_MSG_RESULT([$enable_coverage])

# check whether to enable FastMalloc
AC_MSG_CHECKING([whether to enable optimized memory allocator])
AC_ARG_ENABLE(fast_malloc,
              AC_HELP_STRING([--enable-fast-malloc],
                             [enable optimized memory allocator default=yes, default=no for debug builds]),
              [],[if test "$enable_debug" = "yes"; then enable_fast_malloc="no"; else enable_fast_malloc="yes"; fi])
AC_MSG_RESULT([$enable_fast_malloc])

AC_MSG_CHECKING([whether to enable JIT compilation])
AC_ARG_ENABLE([jit],
              AC_HELP_STRING([--enable-jit],
                             [Enable JIT compilation default=yes]),
              [],[enable_jit="yes"])
if test "$enable_jit" = "yes"; then
    case "$host_cpu" in
        arm*)
            AC_DEFINE([ENABLE_JIT], [1], [Define to enable JIT])
            AC_DEFINE([ENABLE_YARR], [1], [Define to enable YARR])
            AC_DEFINE([ENABLE_YARR_JIT], [1], [Define to enable YARR JIT])
        ;;
        i*86|x86_64)
            AC_DEFINE([ENABLE_JIT], [1], [Define to enable JIT])
            AC_DEFINE([ENABLE_YARR], [1], [Define to enable YARR])
            AC_DEFINE([ENABLE_YARR_JIT], [1], [Define to enable YARR JIT])
            AC_DEFINE([ENABLE_JIT_OPTIMIZE_CALL], [1], [Define to enable optimizing calls])
            AC_DEFINE([ENABLE_JIT_OPTIMIZE_PROPERTY_ACCESS], [1], [Define to enable optimized property access])
            AC_DEFINE([ENABLE_JIT_OPTIMIZE_ARITHMETIC], [1], [Define to enable optimized arithmetic])
            case "$host_cpu" in
                i*86)
                    AC_DEFINE([WTF_USE_JIT_STUB_ARGUMENT_VA_LIST], [1], [Use stub va_list])
                ;;
                x86_64)
                    AC_DEFINE([WTF_USE_JIT_STUB_ARGUMENT_REGISTER], [1], [Use stub register])
                    AC_DEFINE([WTF_USE_ALTERNATE_JSIMMEDIATE], [1], [Use alternate JSImmediate])
                ;;
            esac
        ;;
        *)
            enable_jit="no (CPU '$host_cpu' not supported)"
        ;;
    esac
else
        AC_DEFINE([ENABLE_JIT], [0], [Define to enable JIT])
fi
AC_MSG_RESULT([$enable_jit])

# Opcode stats
AC_MSG_CHECKING([whether to enable opcode stats])
AC_ARG_ENABLE([opcode-stats],
  [AS_HELP_STRING([--enable-opcode-stats], [Enable Opcode statistics (default: disabled)])],
  [], [enable_opcode_stats=no])
AC_MSG_RESULT([$enable_opcode_stats])

if test "$enable_opcode_stats" = "yes"; then
  if test "$enable_jit" = "yes"; then
    AC_MSG_ERROR([JIT must be disabled for Opcode stats to work.])
  fi
  AC_DEFINE([ENABLE_OPCODE_STATS], [1], [Define to enable Opcode statistics])
fi

# Link prefetch
AC_MSG_CHECKING([whether to enable link prefetch support])
AC_ARG_ENABLE([link-prefetch],
  [AS_HELP_STRING([--enable-link-prefetch], [Enable Link prefetch support (default: disabled)])],
  [],[enable_link_prefetch=no])
AC_MSG_RESULT([$enable_link_prefetch])

if test "$enable_link_prefetch" = "yes"; then 
  AC_DEFINE([ENABLE_LINK_PREFETCH], [1], [Define to enable link prefetch support])
fi

# determine the font backend
AC_MSG_CHECKING([the font backend to use])
AC_ARG_WITH(font_backend,
            AC_HELP_STRING([--with-font-backend=@<:@freetype/pango@:>@],
                           [Select font backend [default=freetype]]),
            [],[with_font_backend="freetype"])

case "$with_font_backend" in
     freetype|pango) ;;
     *) AC_MSG_ERROR([Invalid font backend: must be freetype or pango.]) ;;
esac
AC_MSG_RESULT([$with_font_backend])

# Add '-g' flag to gcc if it's debug build
if test "$enable_debug" = "yes"; then
   CXXFLAGS="$CXXFLAGS -g"
   CFLAGS="$CFLAGS -g"
else
   AC_DEFINE([NDEBUG], [1], [Define to disable debugging features])
fi

# Add the appropriate 'O' level for optimized builds
if test "$enable_optimizations" = "yes"; then
   CXXFLAGS="$CXXFLAGS -O2"
   CFLAGS="$CFLAGS -O2"
else
   CXXFLAGS="$CXXFLAGS -O0"
   CFLAGS="$CFLAGS -O0"
fi

# check if SQLite3 is available. Error out only if one of the
# features hard-depending on it is enabled while SQLite3 is
# unavailable.
if test "enable_database" = "yes"; then
    PKG_CHECK_MODULES([SQLITE3],
                  [sqlite3 >= $SQLITE_REQUIRED_VERSION],
                  [sqlite3_has_pkg_config=yes],
                  [sqlite3_has_pkg_config=no])
    if test "$sqlite3_has_pkg_config" = "no"; then 
        AC_SEARCH_LIBS([sqlite3_open16], [sqlite3],
                  [sqlite3_found=yes;SQLITE3_LIBS="$LIBS";SQLITE3_CFLAGS="-I $srcdir/WebKitLibraries/WebCoreSQLite3"],
                  [sqlite3_found=no])
    fi
fi
AC_SUBST([SQLITE3_CFLAGS])
AC_SUBST([SQLITE3_LIBS])

if (test "$sqlite3_found" = "no") && (test "$enable_icon_database" = "yes" || \ 
   test "$enable_database" = "yes" || \ 
   test "$enable_offline_web_applications" = "yes" || \ 
   test "$enable_dom_storage" = "yes"); then 
   AC_MSG_ERROR([SQLite3 is required for the Database related features])
fi

# xml is enabled as default
LIBXML_REQUIRED_VERSION=2.6
PKG_CHECK_MODULES(LIBXML, libxml-2.0 >= $LIBXML_REQUIRED_VERSION)
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

# check if libxslt is available
LIBXSLT_REQUIRED_VERSION=1.1.7
if test "$enable_xslt" = "yes"; then
   PKG_CHECK_MODULES([LIBXSLT],[libxslt >= $LIBXSLT_REQUIRED_VERSION])
       AC_SUBST([LIBXSLT_CFLAGS])
       AC_SUBST([LIBXSLT_LIBS])
fi

# check if geoclue is available
if test "$enable_geolocation" = "yes"; then
    PKG_CHECK_MODULES([GEOCLUE], [geoclue])
    AC_SUBST([GEOCLUE_CFLAGS])
    AC_SUBST([GEOCLUE_LIBS])
fi

# check for XRender under Linux/Unix. Some linkers require explicit
# linkage (like GNU Gold), so we cannot rely on GTK+ pulling XRender
#if test "$os_win32" = "no"; then
#   PKG_CHECK_MODULES([XRENDER], [xrender])
#   AC_SUBST([XRENDER_CFLAGS])
#   AC_SUBST([XRENDER_LIBS])
#fi
#
# check if gstreamer is available
GSTREAMER_REQUIRED_VERSION=0.10
GSTREAMER_PLUGINS_BASE_REQUIRED_VERSION=0.10.25
if test "$enable_video" = "yes"; then
   PKG_CHECK_MODULES([GSTREAMER],
                     [gstreamer-0.10 >= $GSTREAMER_REQUIRED_VERSION
                     gstreamer-app-0.10
                     gstreamer-base-0.10
                     gstreamer-interfaces-0.10
                     gstreamer-pbutils-0.10
                     gstreamer-plugins-base-0.10 >= $GSTREAMER_PLUGINS_BASE_REQUIRED_VERSION
                     gstreamer-video-0.10],
                     [have_gstreamer=yes])

   AC_SUBST([GSTREAMER_CFLAGS])
   AC_SUBST([GSTREAMER_LIBS])
fi

# check for code coverage support
if test "$enable_coverage" = "yes"; then
   COVERAGE_CFLAGS="-MD"
   COVERAGE_LDFLAGS="-ftest-coverage -fprofile-arcs"
   AC_SUBST([COVERAGE_CFLAGS])
   AC_SUBST([COVERAGE_LDFLAGS])
fi

# check for HTML features
if test "$enable_video" = "yes"; then
    html_flags=yes
fi

#dnl determine the Unicode backend
AC_MSG_CHECKING([which Unicode backend to use])
AC_ARG_WITH(unicode_backend,
            AC_HELP_STRING([--with-unicode-backend=@<:@icu/minigui@:>@],
                           [Select Unicode backend [default=minigui]]),
            [],[with_unicode_backend="minigui"])

case "$with_unicode_backend" in
     icu|minigui) ;;
     *) AC_MSG_ERROR([Invalid Unicode backend: must be icu or minigui.]) ;;
esac

AC_MSG_RESULT([$with_unicode_backend])

if test "$with_unicode_backend" = "icu"; then
    case "$host" in
            *-*-darwin*)
		UNICODE_CFLAGS="-I$srcdir/JavaScriptCore/icu -I$srcdir/WebCore/icu"
		UNICODE_LIBS="-licucore"
                ;;
            *-*-mingw*)
		UNICODE_CFLAGS=""
		UNICODE_LIBS="-licuin -licuuc"
                ;;
            *)
		AC_PATH_PROG(icu_config, icu-config, no)
		if test "$icu_config" = "no"; then
			AC_MSG_ERROR([Cannot find icu-config. The ICU library is needed.])
		fi

		# We don't use --cflags as this gives us a lot of things that we don't
		# necessarily want, like debugging and optimization flags
		# See man (1) icu-config for more info.
		UNICODE_CFLAGS=`$icu_config --cppflags`
		UNICODE_LIBS=`$icu_config --ldflags-libsonly`
                ;;
        esac
fi

if test "x$threads_version" = "xyes"; then
    CFLAGS="$CFLAGS -D_REENTRANT"
    APPLIBS="$APPLIBS -lminigui_ths -lpthread"
    MINIGUI_LIBS="-lminigui_ths"
else
    if test "x$standalone_version" = "xyes"; then
        APPLIBS="$APPLIBS -lminigui_sa"
        MINIGUI_LIBS="-lminigui_sa"
    else
        APPLIBS="$APPLIBS -lminigui_procs"
        MINIGUI_LIBS="-lminigui_procs"
    fi
    AC_CHECK_DECLS(_MGGAL_SHADOW, APPLIBS="$APPLIBS -lpthread", lib_pthread="no", [#include <minigui/common.h>])
    if test "x$lib_pthread" = "xno"; then
        AC_CHECK_DECLS(_MGGAL_MLSHADOW, APPLIBS="$APPLIBS -lpthread", no_pthread="no", [#include <minigui/common.h>])
    fi
fi
AC_SUBST([MINIGUI_LIBS])

# mDolphin compile options

# check backward api
AC_MSG_CHECKING([whether to enable compatible to 2.x API])
AC_ARG_ENABLE(backward_api,
              AC_HELP_STRING([--enable-backward-api],
                             [whether to enable compatible to 2.x API <default=yes>]),
                [], enable_backward_api="yes")
AC_MSG_RESULT([$enable_backward_api])

# check focus ring
AC_MSG_CHECKING([whether to enable focus ring on tv])
AC_ARG_ENABLE(focusring_tv,
              AC_HELP_STRING([--enable-focusring_tv],
                             [support focus ring of the TV style <default=no>]),
                [], enable_focusring_tv="no")
AC_MSG_RESULT([$enable_focusring_tv])

# check cairo minigui
AC_MSG_CHECKING([whether to enable use cairo minigui as graphic backend])
AC_ARG_ENABLE(cairo_mg,
            AC_HELP_STRING([--enable-cairo_mg],
                           [ use cairo minigui as graphic backend <default=no>]),
            [], enable_cairo_mg="no")
AC_MSG_RESULT([$enable_cairo_mg])

if test "$enable_cairo_mg" = "yes"; then
    CAIROMG_CFLAGS="`pkg-config --cflags cairo`"
    CAIROMG_LIBS="`pkg-config --libs cairo`"
fi
AC_SUBST([CAIROMG_CFLAGS])
AC_SUBST([CAIROMG_LIBS])

# check whether to enable Cairo text support
AC_MSG_CHECKING([whether to enable Cairo text support])
AC_ARG_ENABLE(cairo_text,
              AC_HELP_STRING([--enable-cairo-text],
                             [enable cairo drawing for all text [default=no]]),
              [],[enable_cairo_text="no"])
AC_MSG_RESULT([$enable_cairo_text])

# check canvas
AC_MSG_CHECKING([whether to enable use HTML5 canvas tag])
AC_ARG_ENABLE(canvas,
        AC_HELP_STRING([--enable-canvas],
                        [ use HTML5 canvas tag <default=no>]),
        [], enable_canvas="no")
AC_MSG_RESULT([$enable_canvas])

# check fast mode
AC_MSG_CHECKING([whether to enable fast mode])
AC_ARG_ENABLE(fastmode,
        AC_HELP_STRING([--enable-fastmode],
                        [ build enable fast mode <default=no>]),
        [], enable_fastmode="no")
AC_MSG_RESULT([$enable_fastmode])

# check cookie
AC_MSG_CHECKING([whether to enable cookie])
AC_ARG_ENABLE(cookie,
        AC_HELP_STRING([--enable-cookie],
                        [ build enable cookie <default=yes>]),
        [], enable_cookie="yes")
AC_MSG_RESULT([$enable_cookie])

# check file cookie
AC_MSG_CHECKING([whether to enable file cookie])
AC_ARG_ENABLE(cookies_files,
        AC_HELP_STRING([--enable-cookies_files],
                        [ build enable cookie files <default=no>]),
        [], enable_cookies_files="no")
AC_MSG_RESULT([$enable_cookies_files])

# check file system 
AC_MSG_CHECKING([whether to enable FTP dir])
AC_ARG_ENABLE(ftpdir,
        AC_HELP_STRING([--enable-ftpdir],
                        [build support list ftp dir <default=yes>]),
        [], enable_ftpdir="yes")
AC_MSG_RESULT([$enable_ftpdir])

# check file system 
AC_MSG_CHECKING([whether to enable file system])
AC_ARG_ENABLE(filesystem,
        AC_HELP_STRING([--enable-filesystem],
                        [ build enable file system <default=yes>]),
        [], enable_filesystem="yes")
AC_MSG_RESULT([$enable_filesystem])

# check no NPTL
AC_MSG_CHECKING([whether uclibc has no nptl threads lib])
AC_ARG_ENABLE(no_nptl,
        AC_HELP_STRING([--enable-no_nptl],
                        [ some uclibc has no nptl threads lib <default=no>]),
        [], enable_no_nptl="no")
AC_MSG_RESULT([$enable_no_nptl])

# check if cairo minigui is enabled 
AC_MSG_CHECKING([whether use cairo minigui as graphic backend])
if test {"$enable_canvas" = "yes" || test "$enable_svg" = "yes" || test "$enable_cairo_text" = "yes"}; then
   if test "$enable_cairo_mg" = "no"; then
        AC_MSG_ERROR([cairo minigui is not enabled]
                [** --enable-cairo_mg is required to enable canvas/svg or cairo text support **])
   fi
fi
AC_MSG_RESULT([$build_cairo_mg])

# check whether to enable plugin support
AC_MSG_CHECKING([whether to enable plugin support])
AC_ARG_ENABLE(plugin,
              AC_HELP_STRING([--enable-plugin],
                             [enable support for plugin [default=no]]),
              [],[enable_plugin="no"])
AC_MSG_RESULT([$enable_plugin])

# check whether to enable ssl
AC_MSG_CHECKING([whether to enable SSL support])
AC_ARG_ENABLE(ssl,
              AC_HELP_STRING([--enable-ssl], 
                  [enable SSL support, obsolete [default=no]]), 
              [],[enable_ssl="no"]) 
AC_MSG_RESULT([$enable_ssl])

# check whether to enable ssl file
AC_MSG_CHECKING([whether to enable SSL file support])
AC_ARG_ENABLE(ssl_files,
              AC_HELP_STRING([--enable-ssl_files],
                             [enable SSL files support, obsolete [default=no]]),
              [],[enable_ssl_files="no"])
AC_MSG_RESULT([$enable_ssl_files])

# check whether to enable view source code 
AC_MSG_CHECKING([whether to open the view source mode ])
AC_ARG_ENABLE(viewsource,
              AC_HELP_STRING([--enable-viewsource],
                             [whether to open the view source mode <default=no>]),
              [],[enable_viewsource="no"])
AC_MSG_RESULT([$enable_viewsource])

# check whether to enable tools for mdcore debug
AC_MSG_CHECKING([whether to enable tools for mdcore debug])
AC_ARG_ENABLE(tools,
              AC_HELP_STRING([--enable-tools],
                             [compile launcher and tools programs <default=no>]),
              [],[enable_tools="no"])
AC_MSG_RESULT([$enable_tools])

# check whether to enable spider tool
AC_MSG_CHECKING([whether to enable spider tool])
AC_ARG_ENABLE(spider,
              AC_HELP_STRING([--enable-spider],
                             [spider test tool support <default=no>]),
              [],[enable_spider="no"])
AC_MSG_RESULT([$enable_spider])

# check whether to enable 8 bytes align
AC_MSG_CHECKING([whether to enable 8 bytes align])
AC_ARG_ENABLE(force_double_align,
              AC_HELP_STRING([--enable-force_double_align],
                             [8 bytes align ,when create arena <default=no>]),
              [],[enable_force_double_align="no"])
AC_MSG_RESULT([$enable_force_double_align])

# check whether to build hight quality zooming
AC_MSG_CHECKING([whether to build hight quality zooming])
AC_ARG_ENABLE(highqualityzoom,
              AC_HELP_STRING([--enable-highqualityzoom],
                             [build hight quality zooming <default=no>]),
              [],[enable_highqualityzoom="no"])
AC_MSG_RESULT([$enable_highqualityzoom])

# check whether to build support JavaScript native binding
AC_MSG_CHECKING([whether to build support JavaScript native binding])
AC_ARG_ENABLE(jsnativebinding,
              AC_HELP_STRING([--enable-jsnativebinding],
                             [build support JavaScript native binding <default=yes>]),
              [],[enable_jsnativebinding="yes"])
AC_MSG_RESULT([$enable_jsnativebinding])

# check whether to build support disk cache
AC_MSG_CHECKING([whether to build support disk cache])
AC_ARG_ENABLE(diskcache,
              AC_HELP_STRING([--enable-diskcache],
                             [build support disk cache <default=yes>]),
              [],[enable_diskcache="yes"])
AC_MSG_RESULT([$enable_diskcache])

# check whether to build support loadsplash
AC_MSG_CHECKING([whether to build support loadsplash])
AC_ARG_ENABLE([loadsplash], 
              AC_HELP_STRING([--enable-loadsplash],
                             [build support loadsplash <default=no>]),
              [], [enable_loadsplash="no"])
AC_MSG_RESULT([$enable_loadsplash])


# check whether to build support watermark
AC_MSG_CHECKING([whether to build support watermark])
AC_ARG_ENABLE([watermark],
              AC_HELP_STRING([--enable-watermark], [build support watermark <default=no>]),
              [], [enable_watermark="no"])
AC_MSG_RESULT([$enable_watermark])

# check whether to build support scheme extension
AC_MSG_CHECKING([whether to build support scheme extension])
AC_ARG_ENABLE(schemeextension,
              AC_HELP_STRING([--enable-schemeextension],
                             [build support scheme extension <default=yes>]),
              [],[enable_schemeextension="yes"])
AC_MSG_RESULT([$enable_schemeextension])

# check whether to enable uclibc math patch 
# fixed some uclibc problems such as lround(84.000000) return 42.000000
AC_MSG_CHECKING([whether to enable uclibc math patch])
AC_ARG_ENABLE(uclibc_mathpatch,
              AC_HELP_STRING([--enable-uclibc_mathpatch],
                             [whether to enable uclibc math patch <default=no>]),
              [],[enable_uclibc_mathpatch="no"])
AC_MSG_RESULT([$enable_uclibc_mathpatch])

# OS conditionals
AM_CONDITIONAL([OS_WIN32],[test "$os_win32" = "yes"])
AM_CONDITIONAL([OS_UNIX],[test "$os_win32" = "no"])
AM_CONDITIONAL([OS_LINUX],[test "$os_linux" = "yes"])
AM_CONDITIONAL([OS_GNU],[test "$os_gnu" = "yes"])
AM_CONDITIONAL([OS_FREEBSD],[test "$os_freebsd" = "yes"])

# Unicode backend conditionals
AM_CONDITIONAL([USE_ICU_UNICODE], [test "$with_unicode_backend" = "icu"])
AM_CONDITIONAL([USE_GLIB_UNICODE], [test "$with_unicode_backend" = "glib"])
AM_CONDITIONAL([USE_MINIGUI_UNICODE], [test "$with_unicode_backend" = "minigui"])

# Font backend conditionals
AM_CONDITIONAL([USE_FREETYPE], [test "$with_font_backend" = "freetype"])
AM_CONDITIONAL([USE_PANGO], [test "$with_font_backend" = "pango"])

# GStreamer feature conditional
AM_CONDITIONAL([USE_GSTREAMER], [test "$have_gstreamer" = "yes"])

# WebKit feature conditionals
AM_CONDITIONAL([ENABLE_DEBUG],[test "$enable_debug" = "yes"])
AM_CONDITIONAL([ENABLE_3D_TRANSFORMS],[test "$enable_3d_transforms" = "yes"])
AM_CONDITIONAL([ENABLE_WEBGL],[test "$enable_webgl" = "yes"])
AM_CONDITIONAL([ENABLE_BLOB],[test "$enable_blob" = "yes"])
AM_CONDITIONAL([ENABLE_CAIRO_TEXT],[test "$enable_cairo_text" = "yes"])
AM_CONDITIONAL([ENABLE_METER_TAG],[test "$enable_meter_tag" = "yes"])
AM_CONDITIONAL([ENABLE_PROGRESS_TAG],[test "$enable_progress_tag" = "yes"])
AM_CONDITIONAL([ENABLE_CHANNEL_MESSAGING],[test "$enable_channel_messaging" = "yes"])
AM_CONDITIONAL([ENABLE_JAVASCRIPT_DEBUGGER],[test "$enable_javascript_debugger" = "yes"])
AM_CONDITIONAL([ENABLE_OFFLINE_WEB_APPLICATIONS],[test "$enable_offline_web_applications" = "yes"])
AM_CONDITIONAL([ENABLE_DIRECTORY_UPLOAD],[test "$enable_directory_upload" = "yes"])
#AM_CONDITIONAL([ENABLE_DATAGRID],[test "$enable_datagrid" = "yes"])
AM_CONDITIONAL([ENABLE_DATA_TRANSFER_ITEMS],[test "$enable_data_transfer_items" = "yes"])
AM_CONDITIONAL([ENABLE_DOM_STORAGE],[test "$enable_dom_storage" = "yes"])
AM_CONDITIONAL([ENABLE_DATABASE],[test "$enable_database" = "yes"])
AM_CONDITIONAL([ENABLE_DATALIST],[test "$enable_datalist" = "yes"])
AM_CONDITIONAL([ENABLE_EVENTSOURCE],[test "$enable_eventsource" = "yes"])
AM_CONDITIONAL([ENABLE_FAST_MOBILE_SCROLLING],[test "$enable_fast_mobile_scrolling" = "yes"])
AM_CONDITIONAL([ENABLE_FILE_SYSTEM],[test "$enable_file_system" = "yes"])
AM_CONDITIONAL([ENABLE_QUOTA],[test "$enable_quota" = "yes"])
AM_CONDITIONAL([ENABLE_ICONDATABASE],[test "$enable_icon_database" = "yes"])
AM_CONDITIONAL([ENABLE_IMAGE_RESIZER],[test "$enable_image_resizer" = "yes"])
AM_CONDITIONAL([ENABLE_INDEXED_DATABASE],[test "$enable_indexed_database" = "yes"])
AM_CONDITIONAL([ENABLE_INPUT_SPEECH],[test "$enable_input_speech" = "yes"])
AM_CONDITIONAL([ENABLE_XHTMLMP],[test "$enable_xhtmlmp" = "yes"])
AM_CONDITIONAL([ENABLE_XPATH],[test "$enable_xpath" = "yes"])
AM_CONDITIONAL([ENABLE_XSLT],[test "$enable_xslt" = "yes"])
AM_CONDITIONAL([ENABLE_FILTERS],[test "$enable_filters" = "yes"])
AM_CONDITIONAL([ENABLE_GEOLOCATION], [test "$enable_geolocation" = "yes"])
AM_CONDITIONAL([ENABLE_CLIENT_BASED_GEOLOCATION], [test "$enable_client_based_geolocation" = "yes"])
AM_CONDITIONAL([ENABLE_MATHML], [test "$enable_mathml" = "yes"])
AM_CONDITIONAL([ENABLE_VIDEO],[test "$enable_video" = "yes"])
AM_CONDITIONAL([ENABLE_MEDIA_STATISTICS],[test "$enable_media_statistics" = "yes"])
AM_CONDITIONAL([ENABLE_FULLSCREEN_API],[test "$enable_fullscreen_api" = "yes"])
AM_CONDITIONAL([ENABLE_NOTIFICATIONS],[test "$enable_notifications" = "yes"])
AM_CONDITIONAL([ENABLE_ORIENTATION_EVENTS],[test "$enable_orientation_events" = "yes"])
AM_CONDITIONAL([ENABLE_SVG],[test "$enable_svg" = "yes"])
AM_CONDITIONAL([ENABLE_SVG_ANIMATION],[test "$enable_svg_animation" = "yes"])
AM_CONDITIONAL([ENABLE_SVG_FONTS],[test "$enable_svg_fonts" = "yes"])
AM_CONDITIONAL([ENABLE_SVG_FOREIGN_OBJECT],[test "$enable_svg_foreign_object" = "yes"])
AM_CONDITIONAL([ENABLE_SVG_AS_IMAGE],[test "$enable_svg_as_image" = "yes"])
AM_CONDITIONAL([ENABLE_SVG_USE],[test "$enable_svg_use" = "yes"])
AM_CONDITIONAL([ENABLE_COVERAGE],[test "$enable_coverage" = "yes"])
AM_CONDITIONAL([ENABLE_FAST_MALLOC],[test "$enable_fast_malloc" = "yes"])
AM_CONDITIONAL([ENABLE_WML],[test "$enable_wml" = "yes"])
AM_CONDITIONAL([ENABLE_WCSS],[test "$enable_wcss" = "yes"])
AM_CONDITIONAL([ENABLE_WORKERS],[test "$enable_workers" = "yes"])
AM_CONDITIONAL([ENABLE_SHARED_WORKERS],[test "$enable_shared_workers" = "yes"])
AM_CONDITIONAL([SVG_FLAGS],[test "$svg_flags" = "yes"])
AM_CONDITIONAL([HTML_FLAGS],[test "$html_flags" = "yes"])
AM_CONDITIONAL([ENABLE_WEB_SOCKETS],[test "$enable_web_sockets" = "yes"])
AM_CONDITIONAL([ENABLE_WEB_AUDIO],[test "$enable_web_audio" = "yes"])
AM_CONDITIONAL([ENABLE_WEB_TIMING],[test "$enable_web_timing" = "yes"])
AM_CONDITIONAL([ENABLE_OPCODE_STATS],[test "$enable_opcode_stats" = "yes"])
AM_CONDITIONAL([ENABLE_SPELLCHECK],[test "$enable_spellcheck" = "yes"])

# mDolphin conditionals
AM_CONDITIONAL([ENABLE_BACKWARD_API],[test "$enable_backward_api" = "yes"])
AM_CONDITIONAL([ENABLE_FOCUSRING_TV],[test "$enable_focusring_tv" = "yes"])
AM_CONDITIONAL([ENABLE_CAIRO_MG],[test "$enable_cairo_mg" = "yes"])
AM_CONDITIONAL([ENABLE_CANVAS],[test "$enable_canvas" = "yes"])
AM_CONDITIONAL([ENABLE_FASTMODE],[test "$enable_fastmode" = "yes"])
AM_CONDITIONAL([ENABLE_COOKIE],[test "$enable_cookie" = "yes"])
AM_CONDITIONAL([ENABLE_FILECOOKIE],[test "$enable_cookies_files" = "yes"])
AM_CONDITIONAL([ENABLE_FTPDIR],[test "$enable_ftpdir" = "yes"])
AM_CONDITIONAL([ENABLE_FILESYSTEM],[test "$enable_filesystem" = "yes"])
AM_CONDITIONAL([ENABLE_NO_NPTL],[test "$enable_no_nptl" = "yes"])
AM_CONDITIONAL([ENABLE_PLUGIN],[test "$enable_plugin" = "yes"])
AM_CONDITIONAL([ENABLE_SSL],[test "$enable_ssl" = "yes"])
AM_CONDITIONAL([ENABLE_SSL_FILES],[test "$enable_ssl_files" = "yes"])
AM_CONDITIONAL([ENABLE_VIEWSOURCE],[test "$enable_viewsource" = "yes"])
AM_CONDITIONAL([ENABLE_SPIDER],[test "$enable_spider" = "yes"])
AM_CONDITIONAL([ENABLE_FORCE_DOUBLE_ALIGN],[test "$enable_force_double_align" = "yes"])
AM_CONDITIONAL([ENABLE_HIGHQUALITYZOOM],[test "$enable_highqualityzoom" = "yes"])
AM_CONDITIONAL([ENABLE_JSNATIVEBINDING],[test "$enable_jsnativebinding" = "yes"])
AM_CONDITIONAL([ENABLE_DISK_CACHE],[test "$enable_diskcache" = "yes"])
AM_CONDITIONAL([_MD_ENABLE_LOADSPLASH],[test "$enable_loadsplash" = "yes"])
AM_CONDITIONAL([_MD_ENABLE_WATERMARK],[test "$enable_watermark" = "yes"])
AM_CONDITIONAL([USE_LICENSE], [test "$enable_loadsplash" = "yes" -o "$enable_watermark" = "yes"])
AM_CONDITIONAL([ENABLE_SCHEMEEXTENSION],[test "$enable_schemeextension" = "yes"])
AM_CONDITIONAL([ENABLE_UCLIBC_MATHPATCH],[test "$enable_uclibc_mathpatch" = "yes"])

# WebKit user define macores
if test "x$enable_3d_transforms" = "xyes"; then 
    AC_DEFINE(ENABLE_3D_TRANSFORMS, 1, [Define if enable support for 3D Transforms])
fi

if test "x$enable_webgl" = "xyes"; then 
    AC_DEFINE(ENABLE_WEBGL, 1, [Define if enable WebGL support])
fi

if test "x$enable_blob" = "xyes"; then 
    AC_DEFINE(ENABLE_BLOB, 1, [Define if enable Blob support])
fi

if test "x$enable_cairo_text" = "xyes"; then 
    AC_DEFINE(ENABLE_CAIRO_TEXT, 1, [Define if enable cairo text support])
fi

if test "x$enable_meter_tag" = "xyes"; then 
    AC_DEFINE(ENABLE_METER_TAG, 1, [Define if enable HTML5 meter tag])
fi

if test "x$enable_progress_tag" = "xyes"; then 
    AC_DEFINE(ENABLE_PROGRESS_TAG, 1, [Define if enable HTML5 progress tag])
fi

if test "x$enable_channel_messaging" = "xyes"; then 
    AC_DEFINE(ENABLE_CHANNEL_MESSAGING, 1, [Define if enable HTML5 Channel Messaging support])
fi

if test "x$enable_javascript_debugger" = "xyes"; then 
    AC_DEFINE(ENABLE_JAVASCRIPT_DEBUGGER, 1, [Define if enable JavaScript debugger/profiler support])
fi

if test "x$enable_offline_web_applications" = "xyes"; then 
    AC_DEFINE(ENABLE_OFFLINE_WEB_APPLICATIONS, 1, [Define if enable HTML5 offline web applications support])
fi

if test "x$enable_directory_upload" = "xyes"; then 
    AC_DEFINE(ENABLE_DIRECTORY_UPLOAD, 1, [Define if enable directory upload support])
fi

#if test "x$enable_datagrid" = "xyes"; then 
#    AC_DEFINE(ENABLE_DATAGRID, 1, [Define if enable HTML5 datagrid support])
#fi

if test "x$enable_data_transfer_items" = "xyes"; then 
    AC_DEFINE(ENABLE_DATA_TRANSFER_ITEMS, 1, [Define if enable HTML5 data transfer items support])
fi

if test "x$enable_dom_storage" = "xyes"; then 
    AC_DEFINE(ENABLE_DOM_STORAGE, 1, [Define if enable HTML5 client-side session and persistent storage support])
fi

if test "x$enable_database" = "xyes"; then 
    AC_DEFINE(ENABLE_DATABASE, 1, [Define if enable HTML5 client-side database storage support])
fi

if test "x$enable_datalist" = "xyes"; then 
    AC_DEFINE(ENABLE_DATALIST, 1, [Define if enable HTML5 datalist support])
fi

if test "x$enable_eventsource" = "xyes"; then 
    AC_DEFINE(ENABLE_EVENTSOURCE, 1, [Define if enable HTML5 server-sent events support])
fi

if test "x$enable_fast_mobile_scrolling" = "xyes"; then 
    AC_DEFINE(ENABLE_FAST_MOBILE_SCROLLING, 1, [Define if enable Fast Mobile Scrolling])
fi

if test "x$enable_file_system" = "xyes"; then 
    AC_DEFINE(ENABLE_FILE_SYSTEM, 1, [Define if enable HTML5 FileSystem API support])
fi

if test "x$enable_quota" = "xyes"; then 
    AC_DEFINE(ENABLE_QUOTA, 1, [Define if enable Quota API support])
fi

if test "x$enable_icon_database" = "xyes"; then 
    AC_DEFINE(ENABLE_ICONDATABASE, 1, [Define if enable icon database support])
fi

if test "x$enable_image_resizer" = "xyes"; then 
    AC_DEFINE(ENABLE_IMAGE_RESIZER, 1, [Define if enable image resizer API support])
fi

if test "x$enable_indexed_database" = "xyes"; then 
    AC_DEFINE(ENABLE_INDEXED_DATABASE, 1, [Define if enable the indexed database API])
fi

if test "x$enable_input_speech" = "xyes"; then 
    AC_DEFINE(ENABLE_INPUT_SPEECH, 1, [Define if enable the speech input API])
fi

if test "x$enable_xhtmlmp" = "xyes"; then 
    AC_DEFINE(ENABLE_XHTMLMP, 1, [Define if enable XHTML-MP support])
fi

if test "x$enable_xpath" = "xyes"; then 
    AC_DEFINE(ENABLE_XPATH, 1, [Define if enable XPath support])
fi

if test "x$enable_xslt" = "xyes"; then 
    AC_DEFINE(ENABLE_XSLT, 1, [Define if enable XSLT support])
fi

if test "x$enable_filters" = "xyes"; then 
    AC_DEFINE(ENABLE_FILTERS, 1, [Define if enable support for filters])
fi

if test "x$enable_geolocation" = "xyes"; then 
    AC_DEFINE(ENABLE_GEOLOCATION, 1, [Define if enable geolocation support])
fi

if test "x$enable_client_based_geolocation" = "xyes"; then 
    AC_DEFINE(ENABLE_CLIENT_BASED_GEOLOCATION, 1, [Define if enable client-based geolocation support])
fi

if test "x$enable_mathml" = "xyes"; then 
    AC_DEFINE(ENABLE_MATHML, 1, [Define if enable MathML support])
fi

if test "x$enable_video" = "xyes"; then 
    AC_DEFINE(ENABLE_VIDEO, 1, [Define if enable HTML5 audio/video support])
fi

if test "x$enable_media_statistics" = "xyes"; then 
    AC_DEFINE(ENABLE_MEDIA_STATISTICS, 1, [Define if enable media statistics support])
fi

if test "x$enable_fullscreen_api" = "xyes"; then 
    AC_DEFINE(ENABLE_FULLSCREEN_API, 1, [Define if enable Javascript Fullscreen API support])
fi

if test "x$enable_notifications" = "xyes"; then 
    AC_DEFINE(ENABLE_NOTIFICATIONS, 1, [Define if enable notifications])
fi

if test "x$enable_orientation_events" = "xyes"; then 
    AC_DEFINE(ENABLE_ORIENTATION_EVENTS, 1, [Define if enable orientation events])
fi

if test "x$enable_svg" = "xyes"; then 
    AC_DEFINE(ENABLE_SVG, 1, [Define if enable SVG support])
fi

if test "x$enable_svg_animation" = "xyes"; then 
    AC_DEFINE(ENABLE_SVG_ANIMATION, 1, [Define if enable support for SVG animation])
fi

if test "x$enable_svg_fonts" = "xyes"; then 
    AC_DEFINE(ENABLE_SVG_FONTS, 1, [Define if enable support for SVG fonts])
fi

if test "x$enable_svg_foreign_object" = "xyes"; then 
    AC_DEFINE(ENABLE_SVG_FOREIGN_OBJECT, 1, [Define if enable foreign objects support for SVG])
fi

if test "x$enable_svg_as_image" = "xyes"; then 
    AC_DEFINE(ENABLE_SVG_AS_IMAGE, 1, [Define if enable SVG As Image support])
fi

if test "x$enable_svg_use" = "xyes"; then 
    AC_DEFINE(ENABLE_SVG_USE, 1, [Define if enable SVG USE element support])
fi

if test "x$enable_coverage" = "xyes"; then 
    AC_DEFINE(ENABLE_COVERAGE, 1, [Define if enable code coverage support])
fi

if test "x$enable_fast_malloc" = "xyes"; then 
    AC_DEFINE(ENABLE_FAST_MALLOC, 1, [enable optimized memory allocator])
fi

if test "x$enable_wml" = "xyes"; then 
    AC_DEFINE(ENABLE_WML, 1, [Define if enable WML support])
fi

if test "x$enable_wcss" = "xyes"; then 
    AC_DEFINE(ENABLE_WCSS, 1, [Define if enable WCSS support])
fi

if test "x$enable_workers" = "xyes"; then 
    AC_DEFINE(ENABLE_WORKERS, 1, [Define if enable Web Workers support])
fi

if test "x$enable_shared_workers" = "xyes"; then 
    AC_DEFINE(ENABLE_SHARED_WORKERS, 1, [Define if enable SharedWorkers support])
fi

if test "x$enable_web_sockets" = "xyes"; then 
    AC_DEFINE(ENABLE_WEB_SOCKETS, 1, [Define if enable Web Socket support])
fi

if test "x$enable_web_audio" = "xyes"; then 
    AC_DEFINE(ENABLE_WEB_AUDIO, 1, [Define if enable Web Audio support])
fi

if test "x$enable_web_timing" = "xyes"; then 
    AC_DEFINE(ENABLE_WEB_TIMING, 1, [Define if enable Web Timing support])
fi

if test "x$$enable_spellcheck" = "xyes"; then 
    AC_DEFINE(ENABLE_SPELLCHECK, 1, [Define if enable spellcheck support])
fi

# mDolphin user defined macros
if test "x$enable_backward_api" = "xyes"; then
    AC_DEFINE(ENABLE_BACKWARD_API, 1, [Define if backward compatibility for mdolphin V2.x API.])
fi

if test "x$enable_focusring_tv" = "xyes"; then
    AC_DEFINE(ENABLE_FOCUSRING_TV, 1, [Define if focus ring of the TV style is supported.])
fi

if test "x$enable_cairo_mg" = "xyes"; then
    AC_DEFINE(ENABLE_CAIRO_MG, 1, [Define if using cairo minigui as graphic backend.])
fi

if test "x$enable_canvas" = "xyes"; then
    AC_DEFINE(ENABLE_CANVAS, 1, [Define if canvas is supported.])
fi

if test "x$enable_fastmode" = "xyes"; then
    AC_DEFINE(ENABLE_FASTMODE, 1, [Define if enable fast mode.])
fi

if test "x$enable_cookie" = "xyes"; then
    AC_DEFINE(ENABLE_COOKIE, 1, [Define if cookie is supported.])
    if test "x$enable_cookies_files" = "xyes"; then
        AC_DEFINE(ENABLE_FILECOOKIE, 1, [Define if cookies support file.])
    fi
fi

if test "x$enable_ftpdir" = "xyes"; then
    AC_DEFINE(ENABLE_FTPDIR, 1, [Define if ftp list is supported.])
fi

if test "x$enable_filesystem" = "xyes"; then
    AC_DEFINE(ENABLE_FILESYSTEM, 1, [Define if FILESYSTEM is supported.])
fi

if test "x$enable_no_nptl" = "xyes"; then
    AC_DEFINE(ENABLE_NO_NPTL, 1, [Define if some uclibc has no nptl threads .])
fi

if test "x$enable_plugin" = "xyes"; then 
    AC_DEFINE(ENABLE_PLUGIN, 1, [Define if plugin is supported.])
fi

if test "x$enable_ssl" = "xyes"; then 
    AC_DEFINE(ENABLE_SSL, 1, [Define if enable SSL files support.])
fi

if test "x$enable_ssl_files" = "xyes"; then 
    AC_DEFINE(ENABLE_SSLFILE, 1, [Define if enable SSL files support.])
fi

if test "x$enable_viewsource" = "xyes"; then
    AC_DEFINE(ENABLE_VIEWSOURCE, 1, [Define if open the view source mode.])
fi

if test "x$enable_spider" = "xyes"; then
    AC_DEFINE(ENABLE_SPIDER, 1, [Define if support spider test tool.])
fi

if test "x$enable_force_double_align" = "xyes"; then
    AC_DEFINE(ENABLE_FORCE_DOUBLE_ALIGN, 1, [Define if 8 bytes alignment when create arena.])
fi

if test "x$enable_highqualityzoom" = "xyes"; then
    AC_DEFINE(ENABLE_HIGHQUALITYZOOM, 1, [Define if demo version is supported.])
fi

if test "x$enable_jsnativebinding" = "xyes"; then
    AC_DEFINE(ENABLE_JSNATIVEBINDING, 1, [Define if JSNATIVEBINDING is supported.])
fi
if test "x$enable_diskcache" = "xyes"; then
    AC_DEFINE(ENABLE_DISK_CACHE, 1, [Define if DISK_CACHE is supported.])
fi

if test "x$enable_loadsplash" = "xyes"; then
    AC_DEFINE(_MD_ENABLE_LOADSPLASH, 1, [Define if loadsplash is supported])
fi

if test "x$enable_watermark" = "xyes"; then
    AC_DEFINE(_MD_ENABLE_WATERMARK, 1, [Define if watermark is supported])
fi

if test "x$enable_schemeextension" = "xyes"; then
    AC_DEFINE(ENABLE_SCHEMEEXTENSION, 1, [Define if SCHEMEEXTENSION is supported.])
fi

if test "x$enable_uclibc_mathpatch" = "xyes"; then
    AC_DEFINE(ENABLE_UCLIBC_MATHPATCH, 1, [Define if fixed some uclibc problem such as lround(84.000000) return 42.000000 ])
fi

AC_CONFIG_FILES([
GNUmakefile
])
 
AC_OUTPUT

echo "
WebKit was configured with the following options:

Build configuration:
 Enable debugging (slow)                                  : $enable_debug
 Enable GCC build optimization                            : $enable_optimizations
 Code coverage support                                    : $enable_coverage
 Unicode backend                                          : $with_unicode_backend
 Font backend                                             : $with_font_backend
 Optimized memory allocator                               : $enable_fast_malloc
Features:
 3D Transforms                                            : $enable_3d_transforms
 WebGL                                                    : $enable_webgl
 Blob support                                             : $enable_blob
 Directory upload                                         : $enable_directory_upload
 Fast Mobile Scrolling                                    : $enable_fast_mobile_scrolling
 JIT compilation                                          : $enable_jit
 Filters support                                          : $enable_filters
 Geolocation support                                      : $enable_geolocation
 Client-based geolocation support                         : $enable_client_based_geolocation
 JavaScript debugger/profiler support                     : $enable_javascript_debugger
 MathML support                                           : $enable_mathml
 Media statistics                                         : $enable_media_statistics
 HTML5 offline web applications support                   : $enable_offline_web_applications
 HTML5 channel messaging support                          : $enable_channel_messaging
 HTML5 meter element support                              : $enable_meter_tag
 HTML5 progress element support                           : $enable_progress_tag
 HTML5 client-side session and persistent storage support : $enable_dom_storage
 HTML5 client-side database storage support               : $enable_database
 HTML5 data transfer items support                        : $enable_data_transfer_items
 HTML5 FileSystem API support                             : $enable_file_system
 Quota API support                                        : $enable_quota
 HTML5 sandboxed iframe support                           : $enable_sandbox
 HTML5 server-sent events support                         : $enable_eventsource
 HTML5 video element support                              : $enable_video
 Fullscreen API support                                   : $enable_fullscreen_api
 Icon database support                                    : $enable_icon_database
 Image resizer support                                    : $enable_image_resizer
 Link prefetch support                                    : $enable_link_prefetch
 Opcode stats                                             : $enable_opcode_stats
 SharedWorkers support                                    : $enable_shared_workers
 Speech input support                                     : $enable_input_speech
 SVG support                                              : $enable_svg
 SVG animation support                                    : $enable_svg_animation
 SVG fonts support                                        : $enable_svg_fonts
 SVG foreign object support                               : $enable_svg_foreign_object
 SVG as image support                                     : $enable_svg_as_image
 SVG use element support                                  : $enable_svg_use
 WML support                                              : $enable_wml
 WCSS support                                             : $enable_wcss
 Web Audio support                                        : $enable_web_audio
 Web Sockets support                                      : $enable_web_sockets
 Web Timing support                                       : $enable_web_timing
 Web Workers support                                      : $enable_workers
 XHTML-MP support                                         : $enable_xhtmlmp
 XPATH support                                            : $enable_xpath
 XSLT support                                             : $enable_xslt
 Spellcheck support                                       : $enable_spellcheck

MG configuration:
 compatible with 2.x api                                  : $enable_backward_api
 focus ring support                                       : $enable_focusring_tv
 cairo mg support                                         : $enable_cairo_mg
 cairo text support                                       : $enable_cairo_text
 canvas support                                           : $enable_canvas
 fast mode support                                        : $enable_fastmode
 cookie support                                           : $enable_cookie
 cookie file support                                      : $enable_cookies_files
 ftp dir support                                          : $enable_ftpdir
 file system support                                      : $enable_filesystem
 no NPTL support                                          : $enable_no_nptl
 plugin support                                           : $enable_plugin
 SSL support                                              : $enable_ssl
 SSL file support                                         : $enable_ssl_files
 view source support                                      : $enable_viewsource
 spider support                                           : $enable_spider
 force double align support                               : $enable_force_double_align
 high qutlity zoom support                                : $enable_highqualityzoom
 javascript native binding support                        : $enable_jsnativebinding
 disk cache support                                       : $enable_diskcache
 loadsplash support                                       : $enable_loadsplash
 watermark support                                        : $enable_watermark
 scheme extension support                                 : $enable_schemeextension
 uclibs math patch support                                : $enable_uclibc_mathpatch
"
if test "$with_unicode_backend" = "glib"; then
   echo "     >> WARNING: the glib-based unicode backend is slow and incomplete <<"
   echo
   echo
fi
