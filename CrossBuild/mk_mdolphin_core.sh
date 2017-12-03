#!/bin/bash

# Please run this script in the parent directory of mdolphin-core source tree.
# And make sure the correct directory name of the source tree.
MD_CORE_DIR=mdolphin-core-3.0.5

# Define the cross tool chain info in this file
. cross-toolchain-info

# Configure options
# Note that the following enabled options only valid for uclibc, just for samples
MDOLPHIN_CORE_CONFIGURE="--enable-no_nptl --enable-uclibc_mathpatch --enable-force_double_align"

cd ${MD_CORE_DIR}

# Configure and make mdolphin-core
CC=${CROSS_TOOL_TARGET}gcc \
CXX=${CROSS_TOOL_TARGET}g++ \
LD=${CROSS_TOOL_TARGET}ld \
AS=${CROSS_TOOL_TARGET}as \
AR=${CROSS_TOOL_TARGET}ar \
RANLIB=${CROSS_TOOL_TARGET}ranlib \
CPPFLAGS=${CROSS_CPPFLAGS} \
CFLAGS=$CROSS_CFLAGS \
CXXFLAGS=${CROSS_CFLAGS} \
LDFLAGS=$CROSS_LDFLAGS \
./configure --prefix=${INST_PREFIX_ADD} $CROSS_CONFIG_OPTS ${MDOLPHIN_CORE_CONFIGURE}

make -j$NR_MAKE_JOBS && make install && echo done

cd -

