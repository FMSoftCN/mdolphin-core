#!/bin/sh

. build.env

aclocal
autoconf
autoheader
automake --add-missing

./configure --prefix=${PREFIX}  \
            --build=i386-linux  \
            --host=${PLATFORM}-linux \
            --target=${PLATFORM}-linux
