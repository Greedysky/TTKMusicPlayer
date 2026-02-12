# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2026 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
#
# * You should have received a copy of the GNU General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
# ***************************************************************************

cmake_minimum_required(VERSION 3.18.0)

project(TTKMusicPlayer)

set(TTK_MAJOR_VERSION 4)
set(TTK_MINOR_VERSION 3)
set(TTK_PATCH_VERSION 0)
set(TTK_VERSION "${TTK_MAJOR_VERSION}.${TTK_MINOR_VERSION}.${TTK_PATCH_VERSION}.0")

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Turn off automatic invocation of the UI
set(CMAKE_AUTOUIC OFF)
# Turn off automatic invocation of the MOC
set(CMAKE_AUTOMOC OFF)
# Turn off automatic invocation of the RCC
set(CMAKE_AUTORCC OFF)

# Build Type
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug)

# skip msvc compiler
if(NOT MSVC)
  # Add a compiler flag
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall")
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -pedantic")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    add_definitions(-DQT_NO_DEBUG)
  endif()

  # clang: error: unsupported option '-fopenmp'
  if(NOT ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
  endif()

  if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
    # Fix "command line is too long" on Windows
    string(APPEND CMAKE_RC_FLAGS " --use-temp-file")
  endif()
endif()

option(TTK_BUILD_TOOLS "TTK build tools" OFF)
message(STATUS "TTK build tools option is " ${TTK_BUILD_TOOLS})

option(TTK_INSTALL_CTL "TTK install common tools library" OFF)
message(STATUS "TTK install common tools library is " ${TTK_INSTALL_CTL})

option(TTK_BUILD_SHARED "TTK build shared library" ON)
if(TTK_BUILD_SHARED)
  message(STATUS "TTK build by shared link")
else()
  message(STATUS "TTK build by static link")
endif()

add_definitions(-DQT_THREAD)
add_definitions(-DTTK_LIBRARY)
add_definitions(-DQMMP_LIBRARY)

set(TTK_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TTK_COMMON_DIR "${TTK_BASE_DIR}/TTKCommon")
set(TTK_COMMON_BASE_DIR "${TTK_COMMON_DIR}/base")
set(TTK_COMMON_COMPAT_DIR "${TTK_COMMON_DIR}/compat")
set(TTK_COMMON_APP_DIR "${TTK_COMMON_DIR}/TTKApplication")
set(TTK_COMMON_LIB_DIR "${TTK_COMMON_DIR}/TTKLibrary")
set(TTK_COMMON_RUN_DIR "${TTK_COMMON_DIR}/TTKRun")
set(TTK_MODULE_DIR "${TTK_BASE_DIR}/TTKModule")
set(TTK_MODULE_CORE_DIR "${TTK_MODULE_DIR}/TTKCore")
set(TTK_MODULE_WIDGET_DIR "${TTK_MODULE_DIR}/TTKWidget")

include_directories(
  ${TTK_BASE_DIR}
  ${TTK_COMMON_DIR}
  ${TTK_COMMON_LIB_DIR}
  ${TTK_COMMON_BASE_DIR}
  ${TTK_MODULE_DIR}
)


set(TTK_CORE_DIR "${TTK_MODULE_CORE_DIR}/musicCoreKits")
set(TTK_CORE_LRCMANAGER_DIR "${TTK_MODULE_CORE_DIR}/musicLrcKits")
set(TTK_CORE_NETWORK_DIR "${TTK_MODULE_CORE_DIR}/musicNetworkKits")
set(TTK_CORE_PLAYLIST_DIR "${TTK_MODULE_CORE_DIR}/musicPlaylistKits")
set(TTK_CORE_LOCALSEARCH_DIR "${TTK_MODULE_CORE_DIR}/musicSearchKits")
set(TTK_CORE_TOOLSWIDGET_DIR "${TTK_MODULE_CORE_DIR}/musicToolsKits")
set(TTK_CORE_TOOLSETSWIDGET_DIR "${TTK_MODULE_CORE_DIR}/musicToolsSetsKits")
set(TTK_CORE_UTILS_DIR "${TTK_MODULE_CORE_DIR}/musicUtilsKits")
set(TTK_CORE_VIDEOKITS_DIR "${TTK_MODULE_CORE_DIR}/musicVideoKits")

include_directories(
  ${TTK_CORE_DIR}
  ${TTK_CORE_LRCMANAGER_DIR}
  ${TTK_CORE_NETWORK_DIR}
  ${TTK_CORE_NETWORK_DIR}/core
  ${TTK_CORE_NETWORK_DIR}/image/background
  ${TTK_CORE_NETWORK_DIR}/image/skin
  ${TTK_CORE_NETWORK_DIR}/music
  ${TTK_CORE_NETWORK_DIR}/music/core
  ${TTK_CORE_NETWORK_DIR}/music/kg
  ${TTK_CORE_NETWORK_DIR}/music/kw
  ${TTK_CORE_NETWORK_DIR}/music/wy
  ${TTK_CORE_NETWORK_DIR}/music/zed
  ${TTK_CORE_NETWORK_DIR}/radio/dj
  ${TTK_CORE_NETWORK_DIR}/radio/fm
  ${TTK_CORE_NETWORK_DIR}/radio/mv
  ${TTK_CORE_NETWORK_DIR}/tools
  ${TTK_CORE_NETWORK_DIR}/translation
  ${TTK_CORE_PLAYLIST_DIR}
  ${TTK_CORE_LOCALSEARCH_DIR}
  ${TTK_CORE_TOOLSWIDGET_DIR}
  ${TTK_CORE_TOOLSETSWIDGET_DIR}
  ${TTK_CORE_UTILS_DIR}
  ${TTK_CORE_VIDEOKITS_DIR}
)


set(TTK_WIDGET_DIR "${TTK_MODULE_WIDGET_DIR}/musicWidgetKits")
set(TTK_WIDGET_CORE_DIR "${TTK_MODULE_WIDGET_DIR}/musicCoreKits")
set(TTK_WIDGET_LRCMANAGER_DIR "${TTK_MODULE_WIDGET_DIR}/musicLrcKits")
set(TTK_WIDGET_REMOTEWIDGET_DIR "${TTK_MODULE_WIDGET_DIR}/musicRemoteKits")
set(TTK_WIDGET_LOCALSEARCH_DIR "${TTK_MODULE_WIDGET_DIR}/musicSearchKits")
set(TTK_WIDGET_TOOLSWIDGET_DIR "${TTK_MODULE_WIDGET_DIR}/musicToolsKits")
set(TTK_WIDGET_TOOLSETSWIDGET_DIR "${TTK_MODULE_WIDGET_DIR}/musicToolsSetsKits")
set(TTK_WIDGET_UI_DIR "${TTK_MODULE_WIDGET_DIR}/musicUiKits")
set(TTK_WIDGET_VIDEOKITS_DIR "${TTK_MODULE_WIDGET_DIR}/musicVideoKits")

include_directories(
  ${TTK_WIDGET_DIR}
  ${TTK_WIDGET_CORE_DIR}
  ${TTK_WIDGET_LRCMANAGER_DIR}
  ${TTK_WIDGET_REMOTEWIDGET_DIR}
  ${TTK_WIDGET_LOCALSEARCH_DIR}
  ${TTK_WIDGET_TOOLSWIDGET_DIR}
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}
  ${TTK_WIDGET_UI_DIR}
  ${TTK_WIDGET_VIDEOKITS_DIR}
)


set(TTK_CONFIG_DIR "${TTK_BASE_DIR}/TTKConfig")
set(TTK_UI_DIR "${TTK_BASE_DIR}/TTKUi")
set(TTK_THIRDPARTY_DIR "${TTK_BASE_DIR}/TTKThirdParty")
set(TTK_TTKSERVICE_DIR "${TTK_BASE_DIR}/TTKService")
set(TTK_UTILS_DIR "${TTK_BASE_DIR}/TTKUtils")
set(TTK_LANGUAGE_DIR "${TTK_BASE_DIR}/TTKLanguage")
set(TTK_RESOURCE_DIR "${TTK_BASE_DIR}/TTKResource")

include_directories(
  ${TTK_THIRDPARTY_DIR}
  ${TTK_THIRDPARTY_DIR}/Zlib
  ${TTK_THIRDPARTY_DIR}/TTKWatcher
  ${TTK_THIRDPARTY_DIR}/TTKExtras
)


set(TTK_BUILD_DIR "${CMAKE_BINARY_DIR}/bin")
set(TTK_GENERATE_DIR "${CMAKE_BINARY_DIR}/Generate")

include_directories(${TTK_GENERATE_DIR})

set(LIBRARY_OUTPUT_PATH ${TTK_BUILD_DIR}/${TTK_VERSION})
set(EXECUTABLE_OUTPUT_PATH ${TTK_BUILD_DIR}/${TTK_VERSION})
set(TTK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}")
set(TTK_INSTALL_BIN_DIR "${TTK_INSTALL_DIR}/${TTK_VERSION}")


set(TTK_QT_VERSION "0" CACHE STRING "Select Qt version")
set_property(CACHE TTK_QT_VERSION PROPERTY STRINGS 4 5 6)

if(NOT (TTK_QT_VERSION STREQUAL "4" OR TTK_QT_VERSION STREQUAL "5" OR TTK_QT_VERSION STREQUAL "6"))
  message(FATAL_ERROR "Expected value for TTK_QT_VERSION is invalid")
endif()

set(OPENSSL_DIR "" CACHE PATH "Openssl install path")
set(CMAKE_PREFIX_PATH ${QT_INSTALL_DIR} CACHE PATH "Qt install path")


find_program(QT_QMAKE_EXECUTABLE qmake)
if(QT_QMAKE_EXECUTABLE)
  message(STATUS "Found qmake executable: " ${QT_QMAKE_EXECUTABLE})
else()
  find_program(QT_QMAKE_EXECUTABLE qmake-qt${TTK_QT_VERSION})
  if(QT_QMAKE_EXECUTABLE)
    message(STATUS "Found qmake executable: " ${QT_QMAKE_EXECUTABLE})
  else()
    message(FATAL_ERROR "Could not find qmake executable")
  endif()
endif()

if(TTK_QT_VERSION VERSION_EQUAL "4")
  set(minimum_required_qt_version "4.8")
  find_package(Qt4 REQUIRED)

  if(QT4_FOUND)
    if("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" VERSION_LESS "${minimum_required_qt_version}")
      message(FATAL_ERROR "Fatal error: TTK requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
    endif()

    set(QT_USE_QTCORE ON)
    set(QT_USE_QTGUI ON)
    set(QT_USE_QTNETWORK ON)
    set(QT_USE_QTXML ON)
    set(QT_USE_QTOPENGL ON)

    if(WIN32)
      set(QT_USE_QTMULTIMEDIA ON)
    elseif(APPLE)
      set(QT_USE_QTDBUS ON)
      set(QT_USE_QTMULTIMEDIA ON)
    else()
      set(QT_USE_QTDBUS ON)
    endif()

    include(${QT_USE_FILE})

#    if(APPLE)
#      set(QT_QTCORE_LIBRARY Qt4::QtCore)
#      set(QT_QTGUI_LIBRARY Qt4::QtDBus)
#      set(QT_QTNETWORK_LIBRARY Qt4::QtNetwork)
#      set(QT_QTXML_LIBRARY Qt4::QtXml)
#      set(QT_QTDBUS_LIBRARY Qt4::QtDBus)
#      set(QT_QTMULTIMEDIA_LIBRARY Qt4::QtMultimedia)
#    endif()
  else()
    message(FATAL_ERROR "Fatal error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
  endif()
elseif(TTK_QT_VERSION VERSION_EQUAL "5")
  find_package(Qt5 COMPONENTS Core Gui Widgets Multimedia Network Xml OpenGL REQUIRED)

  if(WIN32)
    find_package(Qt5WinExtras REQUIRED)
  elseif(APPLE)
    find_package(Qt5DBus REQUIRED)
  else()
    find_package(Qt5DBus REQUIRED)
    find_package(Qt5X11Extras REQUIRED)
  endif()
elseif(TTK_QT_VERSION VERSION_EQUAL "6")
  find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia Network Xml OpenGL REQUIRED)
  # Add a compiler flag
  if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  endif()

  if(UNIX)
    find_package(Qt6 COMPONENTS DBus REQUIRED)
  endif()
endif()

function(query_qt_env VAR RESULT)
  execute_process(COMMAND ${QT_QMAKE_EXECUTABLE} -query ${VAR} RESULT_VARIABLE result OUTPUT_VARIABLE output)
  if(NOT result)
    STRING(STRIP "${output}" output)
    file(TO_CMAKE_PATH "${output}" output)
    set(${RESULT} ${output} PARENT_SCOPE)
  endif()
endfunction()

if(TTK_QT_VERSION VERSION_GREATER "4")
  query_qt_env(QT_INSTALL_BINS QT_BINARY_DIR)
  query_qt_env(QT_INSTALL_LIBS QT_LIBRARY_DIR)
  query_qt_env(QT_INSTALL_HEADERS QT_INCLUDE_DIR)
  query_qt_env(QT_INSTALL_PLUGINS QT_PLUGINS_DIR)

  add_definitions(-DQT_DEPRECATED_WARNINGS)
  add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050F00)
endif()

set(QT_INSTALL_LIBS ${QT_LIBRARY_DIR})
set(QT_INSTALL_BINS ${QT_BINARY_DIR})
get_filename_component(QT_INSTALL_DIR "${QT_INSTALL_LIBS}/../.." ABSOLUTE)

message(STATUS "Found QT_BINARY_DIR: " ${QT_BINARY_DIR})
message(STATUS "Found QT_LIBRARY_DIR: " ${QT_LIBRARY_DIR})
message(STATUS "Found QT_INCLUDE_DIR: " ${QT_INCLUDE_DIR})
message(STATUS "Found QT_PLUGINS_DIR: " ${QT_PLUGINS_DIR})
message(STATUS "Found QT_INSTALL_DIR: " ${QT_INSTALL_DIR})
message(STATUS "Found QT_INSTALL_LIBS: " ${QT_INSTALL_LIBS})
message(STATUS "Found QT_INSTALL_BINS: " ${QT_INSTALL_BINS})

find_program(QT_LRELEASE_EXECUTABLE lrelease ${QT_INSTALL_BINS})
if(QT_LRELEASE_EXECUTABLE)
  message(STATUS "Found lrelease executable: " ${QT_LRELEASE_EXECUTABLE})
else()
  find_program(QT_LRELEASE_EXECUTABLE lrelease-qt${TTK_QT_VERSION} ${QT_INSTALL_BINS})
  if(QT_LRELEASE_EXECUTABLE)
    message(STATUS "Found lrelease executable: " ${QT_LRELEASE_EXECUTABLE})
  else()
    message(FATAL_ERROR "Could not find lrelease executable")
  endif()
endif()


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "Set up ccache module")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()


find_library(TTK_QMMP_LIBRARY
  NAMES TTKqmmp
  PATHS ${LIBRARY_OUTPUT_PATH}
)

if(TTK_QMMP_LIBRARY)
  message(STATUS "Found TTKqmmp library: " ${TTK_QMMP_LIBRARY})
else()
  message(FATAL_ERROR "Could not find TTKqmmp library")
endif()

# qmmp include
include_directories(${TTK_THIRDPARTY_DIR}/TTKqmmp)
if(TTK_QT_VERSION VERSION_EQUAL "4" AND UNIX AND NOT APPLE)
  include_directories(
    /usr/include/QtMultimediaKit
    /usr/include/QtMobility)
endif()


add_subdirectory(TTKCommon)
add_subdirectory(TTKLanguage)
add_subdirectory(TTKConfig)
add_subdirectory(TTKUi)
add_subdirectory(TTKThirdParty)
add_subdirectory(TTKModule)
add_subdirectory(TTKService)
add_subdirectory(TTKRun)


install(DIRECTORY "${TTK_BUILD_DIR}/" DESTINATION "${TTK_INSTALL_DIR}" USE_SOURCE_PERMISSIONS)
install(FILES "${TTK_BASE_DIR}/ChangeLog"
              "${TTK_BASE_DIR}/LICENSE"
              "${TTK_BASE_DIR}/LICENSE.LGPLv3"
              "${TTK_BASE_DIR}/README.md"
              "${TTK_BASE_DIR}/README_cn.md"
              "${TTK_BASE_DIR}/README_tc.md"
              "${TTK_BASE_DIR}/.github/FUNDING.yml"
              "${TTK_UTILS_DIR}/resource/LICENSE.QT-LICENSE-AGREEMENT"
        DESTINATION "${TTK_INSTALL_DIR}/doc")
install(FILES "${TTK_UTILS_DIR}/resource/README.txt" DESTINATION "${TTK_INSTALL_DIR}")
# copy resource
if(WIN32)
  install(FILES "${TTK_UTILS_DIR}/resource/qicon.dll" DESTINATION "${TTK_INSTALL_DIR}")
else()
  install(DIRECTORY "${TTK_UTILS_DIR}/deploy" DESTINATION "${TTK_INSTALL_DIR}")
  install(FILES "${TTK_UTILS_DIR}/install.sh" DESTINATION "${TTK_INSTALL_DIR}")
  install(FILES "${TTK_UTILS_DIR}/uninstall.sh" DESTINATION "${TTK_INSTALL_DIR}")
  install(FILES "${TTK_UTILS_DIR}/ttk_runtime" DESTINATION "${TTK_INSTALL_DIR}")
  install(DIRECTORY "${TTK_UTILS_DIR}/deploy/share/metainfo" DESTINATION "${TTK_INSTALL_DIR}/share")
endif()
# copy run resource
if(UNIX)
  install(CODE "execute_process(COMMAND \"${TTK_UTILS_DIR}/resource.sh\" \"${TTK_RESOURCE_DIR}\" \"${TTK_INSTALL_BIN_DIR}\")")
endif()
# update and release translations
if(WIN32)
  install(CODE "execute_process(COMMAND \"${TTK_UTILS_DIR}/ts_win.bat\" \"${QT_LRELEASE_EXECUTABLE}\" \"${TTK_INSTALL_BIN_DIR}\" \"${TTK_BASE_DIR}\")")
else()
  install(CODE "execute_process(COMMAND \"${TTK_UTILS_DIR}/ts_unix.py\" \"${QT_LRELEASE_EXECUTABLE}\" \"${TTK_INSTALL_BIN_DIR}\" \"${TTK_BASE_DIR}\")")
endif()
