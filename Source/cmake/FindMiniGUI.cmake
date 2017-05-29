# - Try to find MiniGUI
# Once done, this will define
#
#  MiniGUI_FOUND - system has MiniGUI
#  MiniGUI_INCLUDE_DIRS - the MiniGUI include directories
#  MiniGUI_LIBRARIES - link these to use MiniGUI

include(LibFindMacros)

# Dependencies
libfind_package(MiniGUI Freetype)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(MiniGUI_PKGCONF minigui)

# Include dir
find_path(MiniGUI_INCLUDE_DIR
  NAMES minigui/minigui.h
  PATHS ${MiniGUI_PKGCONF_INCLUDE_DIRS}
)

SET (MINIGUI_NAMES minigui minigui_ths minigui_procs minigui_sa)
# Finally the library itself
find_library(MiniGUI_LIBRARY
  NAMES ${MINIGUI_NAMES}
  PATHS ${MiniGUI_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(MiniGUI_PROCESS_INCLUDES MiniGUI_INCLUDE_DIR Freetype_INCLUDE_DIRS)
set(MiniGUI_PROCESS_LIBS MiniGUI_LIBRARY Freetype_LIBRARIES)
libfind_process(MiniGUI)

