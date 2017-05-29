#!/bin/sh

PREFIX=/opt/rel302/target
export CFLAGS=-I$PREFIX/include
export CPPFLAGS=$CFLAGS
export CXXFLAGS=$CFLAGS
export LDFLAGS=-L$PREFIX/lib
export PKG_CONFIG=/usr/bin/pkg-config
export PKG_CINFIG_PATH=$PREFIX/lib/pkgconfig

export GLIB_CFLAGS="`pkg-config --cflags glib-2.0`"
export GLIB_LIBS="`pkg-config --libs glib-2.0`"

export GSTREAMER_LIBS="`pkg-config --libs gstreamer-0.10 gstreamer-app-0.10 gstreamer-base-0.10 gstreamer-interfaces-0.10 gstreamer-pbutils-0.10 gstreamer-plugins-base-0.10 gstreamer-video-0.10`"
export GSTREAMER_CFLAGS="`pkg-config --cflags gstreamer-0.10 gstreamer-app-0.10 gstreamer-base-0.10 gstreamer-interfaces-0.10 gstreamer-pbutils-0.10 gstreamer-plugins-base-0.10 gstreamer-video-0.10`"

./configure --enable-video

#logging setting
#MD_WEBKIT_LOG="PlatformLeaks"
#
#AUTOGEN_CONFIGURE_ARGS="\
#    --disable-tools \
#    --with-unicode-backend=minigui \
#    --disable-schemeextension \
#    --disable-jsnativebinding \
#    --disable-plugin \
#    --disable-database \
#    --disable-video \
#    --disable-icon-database \
#    --disable-dom-storage \
#    --disable-offline-web-applications \
#    --disable-channel-messaging \
#    --disable-eventsource \
#    --disable-ruby \
#    --disable-svg \
#    --disable-shared-workers \
#    --disable-workers \
#    --disable-svg-animation \
#    --disable-filters \
#    --disable-svg-fonts \
#    --disable-svg-foreign-object \
#    --disable-svg-as-image \
#    --disable-svg-use \
#    --disable-web-sockets   \
#    --disable-javascript-debugger"
#
##If you want to use default building path "./WebKitBuild", please
##don't specify the following environment variable.
#WEBKITOUTPUTDIR=~/WebKitBuild
#export MD_WEBIT_LOG AUTOGEN_CONFIGURE_ARGS WEBKITOUTPUTDIR 
#
#srcdir=`dirname $0`
#$srcdir/WebKitTools/Scripts/build-webkit --minigui --makeargs=-j3 #--debug 
