AC_PREREQ(2.59)

m4_define([mdolphin_major_version], [3])
m4_define([mdolphin_minor_version], [0])
m4_define([mdolphin_micro_version], [0])

AC_INIT([libmdolphin],[mdolphin_major_version.mdolphin_minor_version.mdolphin_micro_version],[http://bugs.webkit.org/])

AC_CONFIG_MACRO_DIR([autotools])
AC_CONFIG_AUX_DIR([autotools])
AC_SUBST(ACLOCAL_AMFLAGS, "-I autotools")

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


# Sourced from WebCore/Configurations/Version.xcconfig
WEBKIT_MAJOR_VERSION=533;
WEBKIT_MINOR_VERSION=9;
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

AC_CONFIG_SRCDIR([WebCore/config.h])

dnl libtool version
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

AC_PROG_CXX
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

AC_PATH_PROG(CURLCFG, curl-config, , $PATH)
if test "x$CURLCFG" = "x"; then
    AC_MSG_ERROR([Couldn't find   curl-config]
    [ ** you should install LIBCURL first!**])
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

#if test "$with_unicode_backend" = "glib"; then
#	PKG_CHECK_MODULES([UNICODE], [glib-2.0 pango >= 1.21.0])
#fi

AC_SUBST([UNICODE_CFLAGS])
AC_SUBST([UNICODE_LIBS])

#user define macores
build_cookies="yes"
build_focusring_tv="no"
build_cookies_files="no"
build_activation="no"
build_system_malloc="no"
build_xml="yes"
build_xslt="yes"
build_xpath="yes"
build_ssl="no"
build_ssl_files="yes"
build_png="yes"
build_jpeg="yes"
build_bmp="yes"
build_gif="yes"
build_ftpdir="yes"
build_filesystem="yes"
build_schemeextension="yes"
build_plugin="yes"
build_jsnativebinding="yes"
build_nativeerror="no"
build_fixedmath="no"
build_lowbanddisplay="no"
build_ajax="yes"
build_highqualityzoom="no"
build_demoversion="no"
build_bitblt_image="yes"
build_multiple_threads="no"
build_curl_thread="no"
build_canvas="no"
build_tools="no"
build_spider="no"
build_force_double_align="no"
build_uclibc_mathpatch="no"
build_no_nptl="no"
build_cairo_mg="no"
build_viewsource="no"
build_loadsplash="no"
build_watermark="no"
build_fastmode="no"
build_backward_api="yes"
build_enable_inspector="no"


# initialize webkit options
#WEBKIT_INIT
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
# use dolt to speedup the build
DOLT

AC_PATH_PROG(PERL, perl)
if test -z "$PERL"; then
   AC_MSG_ERROR([You need 'perl' to compile WebKit])
fi

AC_PATH_PROG(BISON, bison)
if test -z "$BISON"; then
   AC_MSG_ERROR([You need the 'bison' parser generator to compile WebKit])
fi

AC_PATH_PROG(MV, mv)
if test -z "$MV"; then
   AC_MSG_ERROR([You need 'mv' to compile WebKit])
fi

AC_PATH_PROG(FLEX, flex)
if test -z "$FLEX"; then
   AC_MSG_ERROR([You need the 'flex' lexer generator to compile WebKit])
else
   FLEX_VERSION=`$FLEX --version | sed 's,.*\ \([0-9]*\.[0-9]*\.[0-9]*\)$,\1,'`
   AX_COMPARE_VERSION([2.5.33],[gt],[$FLEX_VERSION],
      AC_MSG_WARN([You need at least version 2.5.33 of the 'flex' lexer generator to compile WebKit correctly]))
fi

AC_PATH_PROG(GPERF, gperf)
if test -z "$GPERF"; then
   AC_MSG_ERROR([You need the 'gperf' hash function generator to compile WebKit])
fi

# pthread (not needed on Windows)
if test "$os_win32" = "no"; then
AC_CHECK_HEADERS([pthread.h],
                 AC_DEFINE([HAVE_PTHREAD_H],[1],[Define if pthread exists]),
                 AC_MSG_ERROR([pthread support is required to build WebKit]))
fi

AC_ARG_ENABLE(cookies,
[  --enable-cookies         build cookie support <default=yes>],
build_cookies=$enableval)

AC_ARG_ENABLE(cookies_files,
[  --enable-cookies_files   build file support with cookies <default=no>],
build_cookies_files=$enableval)

AC_ARG_ENABLE(plugin,
[  --enable-plugin          build file support with plugin <default=yes>],
build_plugin=$enableval)

AC_ARG_ENABLE(focusring_tv,
[  --enable-focusring_tv    support focus ring of the TV style <default=no>],
build_focusring_tv=$enableval)

AC_ARG_ENABLE(activation,
[  --enable-activation      build activaton support <default=no>],
build_activation=$enableval)

AC_ARG_ENABLE(system_malloc,
[  --enable-system_malloc   build std libc malloc functions instead of FastMalloc <default=no>],
build_system_malloc=$enableval)

AC_ARG_ENABLE(xml,
[  --enable-xml             build XML support <default=yes>],
build_xml=$enableval)

AC_ARG_ENABLE(xslt,
[  --enable-xslt            build XSLT support <default=yes>],
build_xslt=$enableval)

AC_ARG_ENABLE(xpath,
[  --enable-xpath           build XPATH support <default=yes>],
build_xpath=$enableval)

AC_ARG_ENABLE(ssl,
[  --enable-ssl             build SSL support <default=no>],
build_ssl=$enableval)

AC_ARG_ENABLE(ssl_files,
[  --enable-ssl_files       build file support with ssl <default=yes>],
build_ssl_files=$enableval)

AC_ARG_ENABLE(png,
[  --enable-png             build support PNG image format <default=yes>],
build_png=$enableval)

AC_ARG_ENABLE(jpeg,
[  --enable-jpeg            build support JPEG image format <default=yes>],
build_jpeg=$enableval)

AC_ARG_ENABLE(bmp,
[  --enable-bmp             build support BMP image format <default=yes>],
build_bmp=$enableval)

AC_ARG_ENABLE(gif,
[  --enable-gif             build support GIF image format <default=yes>],
build_gif=$enableval)

AC_ARG_ENABLE(ftpdir,
[  --enable-ftpdir          build support list ftp dir <default=yes>],
build_ftpdir=$enableval)

AC_ARG_ENABLE(filesystem,
[  --enable-filesystem      build support file system <default=yes>],
build_filesystem=$enableval)

AC_ARG_ENABLE(schemeextension,
[  --enable-schemeextension build support scheme extension <default=yes>],
build_schemeextension=$enableval)

AC_ARG_ENABLE(jsnativebinding,
[  --enable-jsnativebinding build support JavaScript native binding <default=yes>],
build_jsnativebinding=$enableval)

AC_ARG_ENABLE(nativeerror,
[  --enable-nativeerror     build support native error <default=no>],
build_nativeerror=$enableval)

AC_ARG_ENABLE(fixedmath,
[  --enable-fixedmath       build support fixed math with transform <default=no>],
build_fixedmath=$enableval)

AC_ARG_ENABLE(lowbanddisplay,
[  --enable-lowbanddisplay  build support low band display with transform <default=no>],
build_lowbanddisplay=$enableval)

AC_ARG_ENABLE(ajax,
[  --enable-ajax            build support ajax <default=yes>],
build_ajax=$enableval)

AC_ARG_ENABLE(highqualityzoom,
[  --enable-highqualityzoom build hight quality zooming <default=no>],
build_highqualityzoom=$enableval)

AC_ARG_ENABLE(demoversion,
[  --enable-demoversion     build DEMO version<default=no>],
build_demoversion=$enableval)

AC_ARG_ENABLE(multiple_threads,
[  --enable-multiple_threads    build multiple threads version <default=yes>],
build_multiple_threads=$enableval)

AC_ARG_ENABLE(curl_thread,
[  --enable-curl_thread     use multiple curl threads for downloading <default=no>],
build_curl_thread=$enableval)

AC_ARG_ENABLE(canvas,
[  --enable-canvas    use HTML5 canvas tag <default=no>],
build_canvas=$enableval)

AC_ARG_ENABLE(tools,
[  --enable-tools compile launcher and tools programs<default=no>],
build_tools=$enableval)

AC_ARG_ENABLE(spider,
[  --enable-spider spider test tool support<default=no>],
build_spider=$enableval)

AC_ARG_ENABLE(bitblt_image,
[  --enable-bitblt_image    use MemDC and BitBlt to paint image <default=yes>],
build_bitblt_image=$enableval)

AC_ARG_ENABLE(force_double_align,
			  [  --enable-force_double_align    8 bytes align ,when create arena <default=no>],
build_force_double_align=$enableval)

AC_ARG_ENABLE(uclibc_mathpatch,
			  [  --enable-uclibc_mathpatch fixed some uclibc problems such as lround(84.000000) return 42.000000 <default=no>],
build_uclibc_mathpatch=$enableval)

AC_ARG_ENABLE(no_nptl,
			  [  --enable-no_nptl some uclibc has no nptl threads lib <default=no>],
build_no_nptl=$enableval)

AC_ARG_ENABLE(cairo_mg,
[  --enable-cairo_mg    use cairo minigui as graphic backend <default=no>],
build_cairo_mg=$enableval)

AC_ARG_ENABLE(viewsource,
              [  --enable-viewsource     whether to open the view source mode <default=no>],
build_viewsource=$enableval)

AC_ARG_ENABLE(loadsplash,
[  --enable-loadsplash      build enable load splash <default=no>],
build_loadsplash=$enableval)

AC_ARG_ENABLE(watermark,
[  --enable-watermark      build enable watermark <default=no>],
build_watermark=$enableval)

AC_ARG_ENABLE(fastmode,
[  --enable-fastmode    build enable fast mode <default=no>],
build_fastmode=$enableval)

AC_ARG_ENABLE(backward_api,
[  --enable-backward_api    build enable fast mode <default=no>],
build_backward_api=$enableval)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(errno.h fcntl.h float.h inttypes.h limits.h locale.h malloc.h stddef.h stdint.h stdlib.h string.h strings.h sys/param.h sys/time.h sys/timeb.h unistd.h pthread_np.h)

AC_CHECK_DECLS(_MGLIB_SUFFIX, build_minigui_v3="yes", foo=bar, [[#include <minigui/common.h>]] )

# Check if include ttf support.
if test "x$build_minigui_v3" = "xyes"; then
AC_CHECK_DECLS(_MGFONT_TTF, APPLIBS="$APPLIBS -lttf", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGFONT_FT2, APPLIBS="$APPLIBS -lfreetype", foo=bar, [#include <minigui/common.h>])

else
AC_CHECK_DECLS(_TTF_SUPPORT, APPLIBS="$APPLIBS -lttf", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_FT2_SUPPORT, APPLIBS="$APPLIBS -lfreetype", foo=bar, [#include <minigui/common.h>])
fi

#user define macores

if test "x$build_cookies" = "xyes"; then
    AC_DEFINE(ENABLE_COOKIES, 1, [Define if cookie is supported.])
    if test "x$build_cookies_files" = "xyes"; then
        AC_DEFINE(ENABLE_FILECOOKIE, 1, [Define if cookies support file.])
    fi
fi

if test "x$build_plugin" = "xyes"; then
    AC_DEFINE(ENABLE_PLUGIN, 1, [Define if plugin is supported.])
fi

if test "x$build_activation" = "xyes"; then
    if test "$USERID" = ""; then
        AC_MSG_ERROR([][** please configure USERID **])
    else
        AC_DEFINE_UNQUOTED(CUSTOMERID, "$USERID", [Define customer identification.])
    fi
   AC_DEFINE(SUPPORT_ACTIVATION, 1, [Define if activate mdolphin.])
fi

if test "x$build_focusring_tv" = "xyes"; then
    AC_DEFINE(ENABLE_FOCUSRING_TV, 1, [Define if focus ring of the TV style is supported.])
fi

if test "x$build_system_malloc" = "xyes"; then
    AC_DEFINE(USE_SYSTEM_MALLOC, 1, [Define if use std libc malloc functions.])
fi

if test "x$build_ssl" = "xyes"; then
    AC_CHECK_HEADERS(openssl/x509.h openssl/pkcs12.h, , AC_MSG_ERROR([][** Couldn't find libssl headers in your include path **]))
    AC_DEFINE(ENABLE_SSL, 1, [Define if SSL is supported.])
    APPLIBS="$APPLIBS -lssl -lcrypto"
    if test "x$build_ssl_files" = "xyes"; then
        AC_DEFINE(ENABLE_SSLFILE, 1, [Define if ssl support file.])
    fi
fi

if test "x$build_png" = "xyes"; then
    AC_CHECK_LIB(png, png_check_sig, APPLIBS="$APPLIBS -lpng -lz", AC_MSG_ERROR([Couldn't find libpng ** you should install this package first! **]), -lz -lm)
    AC_CHECK_HEADER(png.h, ,AC_MSG_ERROR([][** Couldn't find libpng headers in your include path **]))
    AC_DEFINE(PNG_SUPPORT, 1, [Define if PNG image is supported.])
fi

if test "x$build_jpeg" = "xyes"; then
    AC_CHECK_LIB(jpeg, jpeg_std_error, APPLIBS="$APPLIBS -ljpeg", AC_MSG_ERROR([Couldn't find libjpeg ** you should install this package first! **]), -lm)
    AC_CHECK_HEADER(jpeglib.h, ,AC_MSG_ERROR([][** Couldn't find libjpeg headers in your include path **]))
    AC_DEFINE(JPEG_SUPPORT, 1, [Define if JPEG image is supported.])
fi

if test "x$build_xml" = "xyes"; then
    AC_PATH_PROG(XMLCFG, xml2-config, , $PATH)
    if test "x$XMLCFG" = "x"; then
        AC_MSG_ERROR([Couldn't find   xml2-config]
            [ ** you should install LIBXML2 first!**])
    fi
    AC_DEFINE(ENABLE_XML, 1, [Define if XML is supported.])

    if test "x$build_xslt" = "xyes"; then
        AC_PATH_PROG(XSLTCFG, xslt-config, , $PATH)
        if test "x$XSLTCFG" = "x"; then
            AC_MSG_ERROR([Couldn't find   xslt-config]
                [ ** you should install LIBXSLT first!**])
        fi
        AC_DEFINE(ENABLE_XSLT, 1, [Define if XSLT is supported.])
    fi

    if test "x$build_xpath" = "xyes"; then
        AC_DEFINE(ENABLE_XPATH, 1, [Define if XPATH is supported.])
    fi
fi

if test "x$build_bmp" = "xyes"; then
    AC_DEFINE(BMP_SUPPORT, 1, [Define if BMP image is supported.])
fi

if test "x$build_gif" = "xyes"; then
    AC_DEFINE(GIF_SUPPORT, 1, [Define if GIF image is supported.])
fi

if test "x$build_ftpdir" = "xyes"; then
    AC_DEFINE(ENABLE_FTPDIR, 1, [Define if FTPDIR is supported.])
fi

if test "x$build_filesystem" = "xyes"; then
    AC_DEFINE(ENABLE_FILESYSTEM, 1, [Define if FILESYSTEM is supported.])
fi

if test "x$build_schemeextension" = "xyes"; then
    AC_DEFINE(ENABLE_SCHEMEEXTENSION, 1, [Define if SCHEMEEXTENSION is supported.])
fi

if test "x$build_jsnativebinding" = "xyes"; then
    AC_DEFINE(ENABLE_JSNATIVEBINDING, 1, [Define if JSNATIVEBINDING is supported.])
fi

if test "x$build_nativeerror" = "xyes"; then
    AC_DEFINE(ENABLE_NATIVEERROR, 1, [Define if NATIVEERROR is supported.])
fi

if test "x$build_fixedmath" = "xyes"; then
    AC_DEFINE(ENABLE_FIXED_MATH, 1, [Define if FIXED MATH is supported.])
fi

if test "x$build_lowbanddisplay" = "xyes"; then
    AC_DEFINE(WTF_USE_LOWBANDWIDTH_DISPLAY, 1, [Define if LOWBAND WIDTH DISPLAY is supported.])
fi

if test "x$build_ajax" = "xyes"; then
    AC_DEFINE(ENABLE_AJAX, 1, [Define if AJAX is supported.])
fi

if test "x$build_demoversion" = "xyes"; then
    AC_DEFINE(ENABLE_DEMOVERSION, 1, [Define if demo version is supported.])
fi

if test "x$build_multiple_threads" = "xyes"; then
    AC_DEFINE(USE_MULTIPLE_THREADS, 1, [Define if multiple threads enabled for KJS Engine.])
fi

if test "x$build_curl_thread" = "xyes"; then
    AC_DEFINE(ENABLE_CURL_THREAD, 1, [Define if CURL thread for downloading.])
fi

if test "x$build_bitblt_image" = "xyes"; then
    AC_DEFINE(ENABLE_BITBLT_IMAGE, 1, [Define if using MemDC and BitBlt  painting image supported.])
fi

if test "x$build_force_double_align" = "xyes"; then
    AC_DEFINE(ENABLE_FORCE_DOUBLE_ALIGN, 1, [Define if 8 bytes alignment when create arena.])
fi

if test "x$build_uclibc_mathpatch" = "xyes"; then
    AC_DEFINE(ENABLE_FLOATING_PATCH, 1, [Define if fixed some uclibc problem such as lround(84.000000) return 42.000000 ])
fi

if test "x$build_no_nptl" = "xyes"; then
    AC_DEFINE(ENABLE_NO_NPTL, 1, [Define if some uclibc has no nptl threads .])
fi

if test "x$build_highqualityzoom" = "xyes"; then
    AC_DEFINE(ENABLE_HIGHQUALITYZOOM, 1, [Define if demo version is supported.])
fi

if test "x$build_canvas" = "xyes"; then
    AC_DEFINE(ENABLE_CANVAS, 1, [Define if canvas is supported.])
fi

if test "x$build_cairo_mg" = "xyes"; then
    AC_DEFINE(ENABLE_CAIRO_MG, 1, [Define if using cairo minigui as graphic backend.])
fi

if test "x$build_viewsource" = "xyes"; then
        AC_DEFINE(ENABLE_VIEWSOURCE, 1, [Define if open the view source mode.])
fi

if test "x$build_fastmode" = "xyes"; then
        AC_DEFINE(ENABLE_FASTMODE, 1, [Define if enable fast mode.])
fi

# do not write to mdconfig.h
if test "x$build_loadsplash" = "xyes"; then
    CXXFLAGS="$CXXFLAGS -D_MD_ENABLE_LOADSPLASH=1"
fi
if test "x$build_watermark" = "xyes"; then
    CXXFLAGS="$CXXFLAGS -D_MD_ENABLE_WATERMARK=1"
fi
#

AC_ARG_WITH(canvassupport,
   [  --with-canvassupport=mgplus/minigui/none])

case "$with_canvassupport" in
    minigui)
    with_canvassupport="minigui"
    AC_DEFINE(MD_CANVAS_MINIGUI, 1, 
            [Define Canvas support  with mgplus ])
    ;;

    mgplus)
    with_canvassupport="mgplus"
    AC_DEFINE(MD_CANVAS_MGPLUS, 1, 
            [Define Canvas support  with mgplus ])
    ;;
    
    *)
    with_canvassupport="mgplus"
    AC_DEFINE(MD_CANVAS_MGPLUS, 1, 
            [Define Canvas support  with mgplus ])
    ;;
esac

AC_ARG_WITH(platformname,
   [  --with-platform=unknown])
case "$with_platform" in
  *)
    with_platform="unknown"
    AC_DEFINE(MD_PLATFORM_UNKNOWN, 1, 
            [Define for unknown platform])
    ;;
esac
 
if test "x$build_spider" = "xyes"; then
    AC_DEFINE(ENABLE_SPIDER, 1, [Define if support spider test tool.])
fi

# add curl to build option
CFLAGS="$CFLAGS `curl-config --cflags`"
CXXFLAGS="$CXXFLAGS `curl-config --cflags`"
APPLIBS="$APPLIBS `curl-config --libs`"

# add xml2 to build option
if test "x$build_xml" = "xyes"; then
CFLAGS="$CFLAGS `xml2-config --cflags`"
CXXFLAGS="$CXXFLAGS `xml2-config --cflags`"
APPLIBS="$APPLIBS `xml2-config --libs`"

# add xslt to build option
if test "x$build_xslt" = "xyes"; then
CFLAGS="$CFLAGS `xslt-config --cflags`"
CXXFLAGS="$CXXFLAGS `xslt-config --cflags`"
APPLIBS="$APPLIBS `xslt-config --libs`"
fi
fi

AM_CONDITIONAL(MINIGUI_V3, test "x$build_minigui_v3" = "xyes")
AM_CONDITIONAL(MD_BUILD_TOOLS, test "x$build_tools" = "xyes")

if test "x$build_minigui_v3" = "xyes"; then
    AC_DEFINE(MINIGUI_V3, 1, [Define if minigui is 3.x version.])
fi
AC_CHECK_DECLS(_MGRM_THREADS, threads_version="yes", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGRM_PROCESSES, procs_version="yes", foo=bar, [#include <minigui/common.h>])
AC_CHECK_DECLS(_MGRM_STANDALONE, standalone_version="yes", foo=bar, [#include <minigui/common.h>])

if test "x$build_minigui_v3" == "xyes" ; then
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
else
    APPLIBS="$APPLIBS -lminigui"
    if test "x$threads_version" = "xyes"; then
        CFLAGS="$CFLAGS -D_REENTRANT"
        APPLIBS="$APPLIBS -lpthread"
    else
        AC_CHECK_DECLS(_MGGAL_SHADOW, APPLIBS="$APPLIBS -lpthread", lib_pthread="no", [#include <minigui/common.h>])
        if test "x$lib_pthread" = "xno"; then
            AC_CHECK_DECLS(_MGGAL_MLSHADOW, APPLIBS="$APPLIBS -lpthread", no_pthread="no", [#include <minigui/common.h>])
        fi
    fi
fi
AC_SUBST([MINIGUI_LIBS])

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
AC_ARG_ENABLE(3D_transforms,
              AC_HELP_STRING([--enable-3D-transforms],
                             [enable support for 3D transforms [default=no]]),
              [],[enable_3D_transforms="no"])
AC_MSG_RESULT([$enable_3D_transforms])

# check whether to enable channel messaging support
AC_MSG_CHECKING([whether to enable HTML5 Channel Messaging support])
AC_ARG_ENABLE(channel_messaging,
              AC_HELP_STRING([--enable-channel-messaging],
                             [enable HTML5 channel messaging support [default=yes]]),
              [],[enable_channel_messaging="yes"])
AC_MSG_RESULT([$enable_channel_messaging])

# check whether to enable JavaScript debugger/profiler support
AC_MSG_CHECKING([whether to enable JavaScript debugger/profiler support])
AC_ARG_ENABLE(javascript_debugger,
              AC_HELP_STRING([--enable-javascript-debugger],
                             [enable JavaScript debugger/profiler support [default=yes]]),
              [],[enable_javascript_debugger="yes"])
AC_MSG_RESULT([$enable_javascript_debugger])

# check whether to enable HTML5 Offline Web Applications support
AC_MSG_CHECKING([whether to enable HTML5 offline web applications support])
AC_ARG_ENABLE(offline_web_applications,
              AC_HELP_STRING([--enable-offline-web-applications],
                             [enable HTML5 offline web applications support [default=yes]]),
              [],[enable_offline_web_applications="yes"])
AC_MSG_RESULT([$enable_offline_web_applications])

# check whether to enable HTML5 client-side session and persitent storage support
AC_MSG_CHECKING([whether to enable HTML5 client-side session and persistent storage support])
AC_ARG_ENABLE(dom_storage,
              AC_HELP_STRING([--enable-dom-storage],
                             [enable HTML5 client-side session and persistent storage support [default=yes]]),
              [],[enable_dom_storage="yes"])
AC_MSG_RESULT([$enable_dom_storage])

# check whether to enable the indexed database API
AC_MSG_CHECKING([whether to enable the indexed database API])
AC_ARG_ENABLE(indexed_database,
              AC_HELP_STRING([--enable-indexeddb],
                             [enable the indexed database API [default=no]]),
              [],[enable_indexed_database="no"])
AC_MSG_RESULT([$enable_indexed_database])

# check whether to build with database support
AC_MSG_CHECKING([whether to enable HTML5 client-side database storage support])
AC_ARG_ENABLE(database,
              AC_HELP_STRING([--enable-database],
                             [enable HTML5 client-side database storage support [default=yes]]),
              [],[enable_database="yes"])
AC_MSG_RESULT([$enable_database])

# check whether to build with server-sent events support
AC_MSG_CHECKING([whether to enable HTML5 server-sent events support])
AC_ARG_ENABLE(eventsource,
              AC_HELP_STRING([--enable-eventsource],
                             [enable HTML5 server-sent events support [default=yes]]),
              [],[enable_eventsource="yes"])
AC_MSG_RESULT([$enable_eventsource])

# check whether to build with icon database support
AC_MSG_CHECKING([whether to enable icon database support])
AC_ARG_ENABLE(icon_database,
              AC_HELP_STRING([--enable-icon-database],
                             [enable icon database [default=yes]]),
              [],[enable_icon_database="yes"])
AC_MSG_RESULT([$enable_icon_database])

# check whether to enable HTML5 datalist support
AC_MSG_CHECKING([whether to enable HTML5 datalist support])
AC_ARG_ENABLE(datalist,
              AC_HELP_STRING([--enable-datalist],
                             [enable HTML5 datalist support [default=yes]]),
              [],[enable_datalist="yes"])
AC_MSG_RESULT([$enable_datalist])

# check whether to enable HTML5 ruby support
AC_MSG_CHECKING([whether to enable HTML5 ruby support])
AC_ARG_ENABLE(ruby,
              AC_HELP_STRING([--enable-ruby],
                             [enable HTML5 ruby support [default=yes]]),
              [],[enable_ruby="yes"])
AC_MSG_RESULT([$enable_ruby])

# check whether to enable HTML5 sandbox iframe support
AC_MSG_CHECKING([whether to enable HTML5 sandboxed iframe support])
AC_ARG_ENABLE(ruby,
              AC_HELP_STRING([--enable-sandbox],
                             [enable HTML5 sandboxed iframe support [default=yes]]),
              [],[enable_sandbox="yes"])
AC_MSG_RESULT([$enable_sandbox])

# check whether to enable HTML5 audio/video support
AC_MSG_CHECKING([whether to enable HTML5 video support])
AC_ARG_ENABLE(video,
              AC_HELP_STRING([--enable-video],
                             [enable HTML5 video support [default=no]]),
              [],[enable_video="no"])
AC_MSG_RESULT([$enable_video])
if test "x$enable_video" = "xyes"; then
    AC_DEFINE(ENABLE_VIDEO, 1, [Define if video/audio tag support.])
fi

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
                             [enable support for SVG [default=yes]]),
              [],[enable_svg="yes"])
AC_MSG_RESULT([$enable_svg])

# check whether to enable WML support
AC_MSG_CHECKING([whether to enable WML support])
AC_ARG_ENABLE(wml,
              AC_HELP_STRING([--enable-wml],
                             [enable support for WML [default=no]]),
              [],[enable_wml="no"])
AC_MSG_RESULT([$enable_wml])
if test "x$enable_wml" = "xyes"; then
    AC_DEFINE(ENABLE_WML, 1, [Define if support wml.])
fi



# check whether to enable SharedWorkers support
AC_MSG_CHECKING([whether to enable SharedWorkers support])
AC_ARG_ENABLE(shared_workers,
              AC_HELP_STRING([--enable-shared-workers],
                             [enable support for SharedWorkers [default=yes]]),
              [],[enable_shared_workers="yes"])
AC_MSG_RESULT([$enable_shared_workers])

# check whether to enable Web Workers support
AC_MSG_CHECKING([whether to enable Web Workers support])
AC_ARG_ENABLE(workers,
              AC_HELP_STRING([--enable-workers],
                             [enable support for Web Workers [default=yes]]),
              [],[enable_workers="yes"])
AC_MSG_RESULT([$enable_workers])

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
                             [enable support for SVG animation (experimental) [default=yes]]),
              [],[enable_svg_animation="yes"])
AC_MSG_RESULT([$enable_svg_animation])

# check whether to enable support for filters
AC_MSG_CHECKING([whether to enable support for filters])
AC_ARG_ENABLE(filters,
              AC_HELP_STRING([--enable-filters],
                             [enable support for filters (experimental) [default=yes]]),
              [],[enable_filters="yes"])
AC_MSG_RESULT([$enable_filters])

# check whether to enable support for SVG fonts
AC_MSG_CHECKING([whether to enable support for SVG fonts])
AC_ARG_ENABLE(svg_fonts,
              AC_HELP_STRING([--enable-svg-fonts],
                             [enable support for SVG fonts (experimental) [default=yes]]),
              [],[enable_svg_fonts="yes"])
AC_MSG_RESULT([$enable_svg_fonts])

# check whether to enable foreign objects support for SVG
AC_MSG_CHECKING([whether to enable support for SVG foreign objects])
AC_ARG_ENABLE(svg_foreign_object,
              AC_HELP_STRING([--enable-svg-foreign-object],
                             [enable support for SVG foreign objects (experimental) [default=yes]]),
              [],[enable_svg_foreign_object="yes"])
AC_MSG_RESULT([$enable_svg_foreign_object])

# check whether to enable SVG As Image support
AC_MSG_CHECKING([whether to enable SVG as Image support])
AC_ARG_ENABLE(svg_as_image,
              AC_HELP_STRING([--enable-svg-as-image],
                             [enable SVG as Image support (experimental) [default=yes]]),
              [],[enable_svg_as_image="yes"])
AC_MSG_RESULT([$enable_svg_as_image])

# check whether to enable SVG USE element support
AC_MSG_CHECKING([whether to enable support for SVG use element])
AC_ARG_ENABLE(svg_use,
              AC_HELP_STRING([--enable-svg-use],
                             [enable SVG use element support (experimental) [default=yes]]),
              [],[enable_svg_use="yes"])
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
        i*86|x86_64|mipsel|arm)
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
fi
AC_MSG_RESULT([$enable_jit])

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


#compile library flags:__MDOLPHIN_LIB__
CFLAGS="$CFLAGS -D__MDOLPHIN_LIB__"

# check if sqlite 3 is available
if test "$enable_icon_database" = "yes" || \
   test "$enable_database" = "yes" || \
   test "$enable_offline_web_applications" = "yes" || \
   test "$enable_dom_storage" = "yes"; then
   PKG_CHECK_MODULES([SQLITE3],
                     [sqlite3 >= $SQLITE_REQUIRED_VERSION],
                     [sqlite3_has_pkg_config=yes],
                     [sqlite3_has_pkg_config=no])
   if test "$sqlite3_has_pkg_config" = "no"; then
      AC_SEARCH_LIBS([sqlite3_open16], [sqlite3],
                     [SQLITE3_LIBS="$LIBS";SQLITE3_CFLAGS="-I $srcdir/WebKitLibraries/WebCoreSQLite3"],
                     [AC_MSG_ERROR([SQLite3 is required to enable Database support])])
   fi
   AC_SUBST([SQLITE3_CFLAGS])
   AC_SUBST([SQLITE3_LIBS])
fi


# check if geoclue is available
if test "$enable_geolocation" = "yes"; then
    PKG_CHECK_MODULES([GEOCLUE], [geoclue])
    AC_SUBST([GEOCLUE_CFLAGS])
    AC_SUBST([GEOCLUE_LIBS])
fi

# check if gstreamer is available
if test "$enable_video" = "yes"; then
   PKG_CHECK_MODULES([GSTREAMER],
                     [gstreamer-0.10 >= $GSTREAMER_REQUIRED_VERSION
                     gstreamer-app-0.10
                     gstreamer-base-0.10
                     gstreamer-pbutils-0.10
                     gstreamer-plugins-base-0.10 >= $GSTREAMER_PLUGINS_BASE_REQUIRED_VERSION
                     gstreamer-video-0.10])
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

# check if cairo minigui is enabled 
AC_MSG_CHECKING([whether use cairo minigui as graphic backend])
if test "$enable_canvas" = "yes" || \
   test "$enable_svg" = "yes"; then
   if test "$build_cairo_mg" = "no"; then
        AC_MSG_ERROR([cairo minigui is not enabled]
                [** --enable-cairo_mg is required to enable canvas/svg support **])
   fi
fi
AC_MSG_RESULT([$build_cairo_mg])

if test "$build_cairo_mg" = "yes"; then
    CAIROMG_CFLAGS="`pkg-config --cflags cairo`"
    CAIROMG_LIBS="`pkg-config --libs cairo`"
fi
AC_SUBST([CAIROMG_CFLAGS])
AC_SUBST([CAIROMG_LIBS])

if test "$enable_video" = "yes"; then
CFLAGS="$CFLAGS `pkg-config --cflags glib-2.0`"
CXXFLAGS="$CXXFLAGS `pkg-config --cflags glib-2.0`"
GOBJECT_LIBS="`pkg-config --libs gio-2.0`"
fi

if test "$build_backward_api" = "yes"; then
    AC_DEFINE(ENABLE_BACKWARD_API, 1, [Define if backward compatibility for mdolphin V2.x API.])
fi

AC_SUBST([GOBJECT_LIBS])

#GTK_DOC_CHECK([1.10])

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

# WebKit feature conditionals
AM_CONDITIONAL([ENABLE_DEBUG],[test "$enable_debug" = "yes"])
AM_CONDITIONAL([ENABLE_3D_TRANSFORMS],[test "$enable_3D_transforms" = "yes"])
AM_CONDITIONAL([ENABLE_CHANNEL_MESSAGING],[test "$enable_channel_messaging" = "yes"])
AM_CONDITIONAL([ENABLE_JAVASCRIPT_DEBUGGER],[test "$enable_javascript_debugger" = "yes"])
AM_CONDITIONAL([ENABLE_OFFLINE_WEB_APPLICATIONS],[test "$enable_offline_web_applications" = "yes"])
AM_CONDITIONAL([ENABLE_DOM_STORAGE],[test "$enable_dom_storage" = "yes"])
AM_CONDITIONAL([ENABLE_DATABASE],[test "$enable_database" = "yes"])
AM_CONDITIONAL([ENABLE_DATALIST],[test "$enable_datalist" = "yes"])
AM_CONDITIONAL([ENABLE_EVENTSOURCE],[test "$enable_eventsource" = "yes"])
AM_CONDITIONAL([ENABLE_ICONDATABASE],[test "$enable_icon_database" = "yes"])
AM_CONDITIONAL([ENABLE_INDEXED_DATABASE],[test "$enable_indexed_database" = "yes"])
AM_CONDITIONAL([ENABLE_XHTMLMP],[test "$enable_xhtmlmp" = "yes"])
AM_CONDITIONAL([ENABLE_XPATH],[test "$enable_xpath" = "yes"])
AM_CONDITIONAL([ENABLE_XSLT],[test "$enable_xslt" = "yes"])
AM_CONDITIONAL([ENABLE_FILTERS],[test "$enable_filters" = "yes"])
AM_CONDITIONAL([ENABLE_GEOLOCATION], [test "$enable_geolocation" = "yes"])
AM_CONDITIONAL([ENABLE_MATHML], [test "$enable_mathml" = "yes"])
AM_CONDITIONAL([ENABLE_RUBY],[test "$enable_ruby" = "yes"])
AM_CONDITIONAL([ENABLE_SANDBOX],[test "$enable_sandbox" = "yes"])
AM_CONDITIONAL([ENABLE_VIDEO],[test "$enable_video" = "yes"])
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
AM_CONDITIONAL([ENABLE_WORKERS],[test "$enable_workers" = "yes"])
AM_CONDITIONAL([ENABLE_SHARED_WORKERS],[test "$enable_shared_workers" = "yes"])
AM_CONDITIONAL([SVG_FLAGS],[test "$svg_flags" = "yes"])
AM_CONDITIONAL([HTML_FLAGS],[test "$html_flags" = "yes"])
AM_CONDITIONAL([ENABLE_WEB_SOCKETS],[test "$enable_web_sockets" = "yes"])
AM_CONDITIONAL([ENABLE_PLUGIN],[test "$build_plugin" = "yes"])
AM_CONDITIONAL([ENABLE_INSPECTOR],[test "$build_enable_inspector" = "yes"])

AM_CONDITIONAL([USE_CAIRO_MG], [test "$build_cairo_mg" = "yes"])

AM_CONDITIONAL([USE_LICENSE], [test "$build_loadsplash" = "yes" -o \
                                    "$build_watermark" = "yes"])
# Gtk conditionals
AM_CONDITIONAL([ENABLE_INTROSPECTION],[test "$enable_introspection" = "yes"])


AC_CONFIG_FILES([
GNUmakefile
WebKit/mg/mdolphin.pc
WebKit/mg/control/GNUmakefile
WebKit/mg/WebCoreSupport/GNUmakefile
JavaScriptCore/GNUmakefile
WebCore/GNUmakefile
WebCore/accessibility/GNUmakefile
WebCore/bindings/GNUmakefile 
WebCore/bridge/GNUmakefile  
WebCore/css/GNUmakefile 
WebCore/dom/GNUmakefile 
WebCore/editing/GNUmakefile   
WebCore/history/GNUmakefile 
WebCore/html/GNUmakefile  
WebCore/inspector/GNUmakefile 
WebCore/loader/GNUmakefile 
WebCore/mathml/GNUmakefile 
WebCore/notifications/GNUmakefile 
WebCore/page/GNUmakefile 
WebCore/platform/GNUmakefile 
WebCore/plugins/GNUmakefile 
WebCore/rendering/GNUmakefile  
WebCore/storage/GNUmakefile 
WebCore/svg/GNUmakefile   
WebCore/websockets/GNUmakefile 
WebCore/wml/GNUmakefile 
WebCore/workers/GNUmakefile 
WebCore/xml/GNUmakefile
DerivedSources/GNUmakefile
]
)

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
 3D Transforms                                            : $enable_3D_transforms
 JIT compilation                                          : $enable_jit
 Filters support                                          : $enable_filters
 Geolocation support                                      : $enable_geolocation
 JavaScript debugger/profiler support                     : $enable_javascript_debugger
 MathML support                                           : $enable_mathml
 HTML5 offline web applications support                   : $enable_offline_web_applications
 HTML5 channel messaging support                          : $enable_channel_messaging
 HTML5 client-side session and persistent storage support : $enable_dom_storage
 HTML5 client-side database storage support               : $enable_database
 HTML5 ruby support                                       : $enable_ruby
 HTML5 sandboxed iframe support                           : $enable_sandbox
 HTML5 server-sent events support                         : $enable_eventsource
 HTML5 video element support                              : $enable_video
 Icon database support                                    : $enable_icon_database
 SharedWorkers support                                    : $enable_shared_workers
 SVG support                                              : $enable_svg
 SVG animation support                                    : $enable_svg_animation
 SVG fonts support                                        : $enable_svg_fonts
 SVG foreign object support                               : $enable_svg_foreign_object
 SVG as image support                                     : $enable_svg_as_image
 SVG use element support                                  : $enable_svg_use
 WML support                                              : $enable_wml
 Web Sockets support                                      : $enable_web_sockets
 Web Workers support                                      : $enable_workers
 XHTML-MP support                                         : $enable_xhtmlmp
 XPATH support                                            : $enable_xpath
 XSLT support                                             : $enable_xslt
"
