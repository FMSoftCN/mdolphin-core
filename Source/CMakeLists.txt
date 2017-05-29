CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(mdolphin)

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF ()
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

SET(JAVASCRIPTCORE_DIR "${CMAKE_SOURCE_DIR}/JavaScriptCore")
SET(WEBCORE_DIR "${CMAKE_SOURCE_DIR}/WebCore")
SET(WEBKIT_DIR "${CMAKE_SOURCE_DIR}/WebKit")
SET(TOOLS_DIR "${CMAKE_SOURCE_DIR}/../Tools")
SET(DERIVED_SOURCES_DIR "${CMAKE_SOURCE_DIR}/../DerivedSources")
SET(DERIVED_SOURCES_WEBCORE_DIR "${DERIVED_SOURCES_DIR}/WebCore")
SET(DERIVED_SOURCES_JAVASCRIPTCORE_DIR "${DERIVED_SOURCES_DIR}/JavaScriptCore")

INCLUDE(WebKitMacros)
INCLUDE(WebKitFS)
INCLUDE(WebKitHelpers)
INCLUDE(WebKitFeatures)

# -----------------------------------------------------------------------------
# Determine which port will be built
# -----------------------------------------------------------------------------
SET(ALL_PORTS Mg)
SET(PORT "Mg" CACHE STRING "choose which WebKit port to build (one of ${ALL_PORTS})")
#
#LIST(FIND ALL_PORTS ${PORT} RET)
#IF (${RET} EQUAL -1)
#    MESSAGE(FATAL_ERROR "Please choose which WebKit port to build (one of ${ALL_PORTS})")
#ENDIF ()

#SET(ENABLE_GENERATOR_SOURCES 1)
IF (ENABLE_GENERATOR_SOURCES)
# -----------------------------------------------------------------------------
# Find common packages (used by all ports)
# -----------------------------------------------------------------------------
FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(Gperf REQUIRED)
FIND_PACKAGE(Perl REQUIRED)
FIND_PACKAGE(PythonInterp REQUIRED)
ENDIF (ENABLE_GENERATOR_SOURCES)

# -----------------------------------------------------------------------------
# Determine the target processor
# -----------------------------------------------------------------------------
STRING(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} LOWERCASE_CMAKE_SYSTEM_PROCESSOR)
IF (LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
    SET(WTF_CPU_ARM 1)
ELSEIF (LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)")
    SET(WTF_CPU_X86_64 1)
ELSEIF (LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "(i[3-6]86|x86)")
    SET(WTF_CPU_X86 1)
ELSE ()
    MESSAGE(FATAL_ERROR "Unknown CPU '${CMAKE_SYSTEM_PROCESSOR}'")
ENDIF ()

# -----------------------------------------------------------------------------
# Determine the operating system
# -----------------------------------------------------------------------------
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(WTF_OS_UNIX 1)

    IF (APPLE)
        SET(WTF_OS_MAC_OS_X 1)
    ENDIF ()
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    SET(WTF_OS_WINDOWS 1)
ELSE ()
    MESSAGE(FATAL_ERROR "Unknown OS '${CMAKE_SYSTEM_NAME}'")
ENDIF ()

# -----------------------------------------------------------------------------
# Default target names (can be overrriden in Options${PORT}.cmake file)
# -----------------------------------------------------------------------------
SET(JSC_EXECUTABLE_NAME JSC)
SET(mDolphin_LIBRARY_NAME mdolphin)

# -----------------------------------------------------------------------------
# Port-specific options
# -----------------------------------------------------------------------------
INCLUDE(OptionsCommon)
INCLUDE(Options${PORT})

# -----------------------------------------------------------------------------
# Define packaging
# -----------------------------------------------------------------------------
INCLUDE(WebKitPackaging)

# -----------------------------------------------------------------------------
# Add module directories
# -----------------------------------------------------------------------------
WEBKIT_ADD_SUBDIRECTORY(JavaScriptCore)
WEBKIT_ADD_SUBDIRECTORY(WebCore)
WEBKIT_ADD_SUBDIRECTORY(WebKit)

# -----------------------------------------------------------------------------
# Add test browser if exists
# -----------------------------------------------------------------------------
IF (ENABLE_TOOLS)
INCLUDE_IF_EXISTS(${TOOLS_DIR}/CMakeLists${PORT}.txt)
ENDIF (ENABLE_TOOLS)

# -----------------------------------------------------------------------------
# Set compiler flags for all targets
# -----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(${mDolphin_INCLUDE_DIRECTORIES})

IF (BUILD_SHARED_LIB)
    ADD_LIBRARY(${mDolphin_LIBRARY_NAME} SHARED ${mDolphin_SOURCES})
    TARGET_LINK_LIBRARIES(${mDolphin_LIBRARY_NAME} ${mDolphin_LIBRARIES})

    SET_TARGET_PROPERTIES(${mDolphin_LIBRARY_NAME} PROPERTIES VERSION ${MDOLPHIN_VERSION} SOVERSION ${MDOLPHIN_MAJOR_VERSION})
    SET_TARGET_PROPERTIES(${mDolphin_LIBRARY_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

    INSTALL(TARGETS ${mDolphin_LIBRARY_NAME} DESTINATION lib)

    IF (mDolphin_LINK_FLAGS)
        ADD_TARGET_PROPERTIES(${mDolphin_LIBRARY_NAME} LINK_FLAGS "${mDolphin_LINK_FLAGS}")
    ENDIF (mDolphin_LINK_FLAGS)

    IF (VERSION_SCRIPT)
        ADD_TARGET_PROPERTIES(${mDolphin_LIBRARY_NAME} LINK_FLAGS "${VERSION_SCRIPT}")
    ENDIF ()
    WEBKIT_SET_EXTRA_COMPILER_FLAGS(${mDolphin_LIBRARY_NAME})
ENDIF (BUILD_SHARED_LIB)

IF (BUILD_STATIC_LIB)
    ADD_LIBRARY(${mDolphin_LIBRARY_NAME}_static STATIC ${mDolphin_SOURCES})
    SET_TARGET_PROPERTIES(${mDolphin_LIBRARY_NAME}_static PROPERTIES OUTPUT_NAME ${mDolphin_LIBRARY_NAME})
    SET_TARGET_PROPERTIES(${mDolphin_LIBRARY_NAME}_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

    INSTALL(TARGETS ${mDolphin_LIBRARY_NAME}_static DESTINATION lib)

    WEBKIT_SET_EXTRA_COMPILER_FLAGS(${mDolphin_LIBRARY_NAME}_static)
ENDIF (BUILD_STATIC_LIB)

# -----------------------------------------------------------------------------
# Create the configuration file
# -----------------------------------------------------------------------------
CONFIGURE_FILE(mdconfig.h.cmake ${CMAKE_BINARY_DIR}/mdconfig.h @ONLY)

set (prefix ${CMAKE_INSTALL_PREFIX})
set (exec_prefix "\${prefix}")
set (libdir "\${exec_prefix}/lib")
set (includedir "\${prefix}/include")
set (mdolphin_libs "${mDolphin_DEPLIBS_LIST}")
set (mdolphin_incs "${mDolphin_DEPHDRS_LIST}")

CONFIGURE_FILE("${WEBKIT_DIR}/mg/mdolphin.pc.in" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc" @ONLY)
INSTALL (FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc"
    DESTINATION lib/pkgconfig)
UNSET(prefix)
UNSET(exec_prefix)
UNSET(libdir)
UNSET(includedir)
UNSET(mdolphin_libs)

# -----------------------------------------------------------------------------
# Prints a summary of all enabled/disabled features
# -----------------------------------------------------------------------------
WEBKIT_PRINT_FEATURES()
