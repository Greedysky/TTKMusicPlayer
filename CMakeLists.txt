# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2024 Greedysky Studio
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

cmake_minimum_required(VERSION 3.0.0)

project(TTKMusicPlayer)

set(TTK_MAJOR_VERSION 4)
set(TTK_MINOR_VERSION 1)
set(TTK_PATCH_VERSION 0)
set(TTK_VERSION "${TTK_MAJOR_VERSION}.${TTK_MINOR_VERSION}.${TTK_PATCH_VERSION}.0")

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC OFF)
# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread -fopenmp -Wall")
if(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif()

# Build Type
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug)

option(TTK_BUILD_SHARED "TTK BUILD SHARED LIBS" ON)
if(TTK_BUILD_SHARED)
  message(STATUS "Message TTK build by shared link")
else()
  message(STATUS "Message TTK build by static link")
endif()

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
  cmake_policy(SET CMP0005 OLD)
  cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

add_definitions(-DTTK_LIBRARY)
add_definitions(-DQMMP_LIBRARY)

set(MUSIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MUSIC_COMMON_DIR "${MUSIC_DIR}/TTKCommon")
set(MUSIC_COMMON_COMPAT_DIR "${MUSIC_COMMON_DIR}/compat")
set(MUSIC_COMMON_APP_DIR "${MUSIC_COMMON_DIR}/TTKApplication")
set(MUSIC_COMMON_DUMPER_DIR "${MUSIC_COMMON_DIR}/TTKDumper")
set(MUSIC_COMMON_LIB_DIR "${MUSIC_COMMON_DIR}/TTKLibrary")
set(MUSIC_COMMON_RUN_DIR "${MUSIC_COMMON_DIR}/TTKRun")
set(MUSIC_LIB_DIR "${MUSIC_DIR}/TTKModule")
set(MUSIC_LIB_CORE_DIR "${MUSIC_LIB_DIR}/TTKCore")
set(MUSIC_LIB_WIDGET_DIR "${MUSIC_LIB_DIR}/TTKWidget")

include_directories(
  ${MUSIC_DIR}
  ${MUSIC_COMMON_DIR}
  ${MUSIC_COMMON_LIB_DIR}
  ${MUSIC_LIB_DIR}
)


set(MUSIC_CORE_DIR "${MUSIC_LIB_CORE_DIR}/musicCoreKits")
set(MUSIC_CORE_LRCMANAGER_DIR "${MUSIC_LIB_CORE_DIR}/musicLrcKits")
set(MUSIC_CORE_NETWORK_DIR "${MUSIC_LIB_CORE_DIR}/musicNetworkKits")
set(MUSIC_CORE_PLAYLIST_DIR "${MUSIC_LIB_CORE_DIR}/musicPlaylistKits")
set(MUSIC_CORE_LOCALSEARCH_DIR "${MUSIC_LIB_CORE_DIR}/musicSearchKits")
set(MUSIC_CORE_TOOLSWIDGET_DIR "${MUSIC_LIB_CORE_DIR}/musicToolsKits")
set(MUSIC_CORE_TOOLSETSWIDGET_DIR "${MUSIC_LIB_CORE_DIR}/musicToolsSetsKits")
set(MUSIC_CORE_UTILS_DIR "${MUSIC_LIB_CORE_DIR}/musicUtilsKits")
set(MUSIC_CORE_VIDEOKITS_DIR "${MUSIC_LIB_CORE_DIR}/musicVideoKits")

include_directories(
  ${MUSIC_CORE_DIR}
  ${MUSIC_CORE_LRCMANAGER_DIR}
  ${MUSIC_CORE_NETWORK_DIR}
  ${MUSIC_CORE_NETWORK_DIR}/core
  ${MUSIC_CORE_NETWORK_DIR}/image/background
  ${MUSIC_CORE_NETWORK_DIR}/image/skin
  ${MUSIC_CORE_NETWORK_DIR}/music
  ${MUSIC_CORE_NETWORK_DIR}/music/core
  ${MUSIC_CORE_NETWORK_DIR}/music/kg
  ${MUSIC_CORE_NETWORK_DIR}/music/kw
  ${MUSIC_CORE_NETWORK_DIR}/music/wy
  ${MUSIC_CORE_NETWORK_DIR}/music/zed
  ${MUSIC_CORE_NETWORK_DIR}/radio/dj
  ${MUSIC_CORE_NETWORK_DIR}/radio/fm
  ${MUSIC_CORE_NETWORK_DIR}/radio/mv
  ${MUSIC_CORE_NETWORK_DIR}/tools
  ${MUSIC_CORE_NETWORK_DIR}/translation
  ${MUSIC_CORE_PLAYLIST_DIR}
  ${MUSIC_CORE_LOCALSEARCH_DIR}
  ${MUSIC_CORE_TOOLSWIDGET_DIR}
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}
  ${MUSIC_CORE_UTILS_DIR}
  ${MUSIC_CORE_VIDEOKITS_DIR}
)


set(MUSIC_WIDGET_DIR "${MUSIC_LIB_WIDGET_DIR}/musicWidgetKits")
set(MUSIC_WIDGET_CORE_DIR "${MUSIC_LIB_WIDGET_DIR}/musicCoreKits")
set(MUSIC_WIDGET_LRCMANAGER_DIR "${MUSIC_LIB_WIDGET_DIR}/musicLrcKits")
set(MUSIC_WIDGET_REMOTEWIDGET_DIR "${MUSIC_LIB_WIDGET_DIR}/musicRemoteKits")
set(MUSIC_WIDGET_LOCALSEARCH_DIR "${MUSIC_LIB_WIDGET_DIR}/musicSearchKits")
set(MUSIC_WIDGET_TOOLSWIDGET_DIR "${MUSIC_LIB_WIDGET_DIR}/musicToolsKits")
set(MUSIC_WIDGET_TOOLSETSWIDGET_DIR "${MUSIC_LIB_WIDGET_DIR}/musicToolsSetsKits")
set(MUSIC_WIDGET_UI_DIR "${MUSIC_LIB_WIDGET_DIR}/musicUiKits")
set(MUSIC_WIDGET_VIDEOKITS_DIR "${MUSIC_LIB_WIDGET_DIR}/musicVideoKits")

include_directories(
  ${MUSIC_WIDGET_DIR}
  ${MUSIC_WIDGET_CORE_DIR}
  ${MUSIC_WIDGET_LRCMANAGER_DIR}
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}
  ${MUSIC_WIDGET_LOCALSEARCH_DIR}
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}
  ${MUSIC_WIDGET_UI_DIR}
  ${MUSIC_WIDGET_VIDEOKITS_DIR}
)


set(MUSIC_CONFIG_DIR "${MUSIC_DIR}/TTKConfig")
set(MUSIC_UI_DIR "${MUSIC_DIR}/TTKUi")
set(MUSIC_THIRDPARTY_DIR "${MUSIC_DIR}/TTKThirdParty")
set(MUSIC_TTKSERVICE_DIR "${MUSIC_DIR}/TTKService")
set(MUSIC_UTILS_DIR "${MUSIC_DIR}/TTKUtils")
set(MUSIC_LANGUAGE_DIR "${MUSIC_DIR}/TTKLanguage")
set(MUSIC_RESOURCE_DIR "${MUSIC_DIR}/TTKResource")

include_directories(
  ${MUSIC_THIRDPARTY_DIR}
  ${MUSIC_THIRDPARTY_DIR}/Zlib
  ${MUSIC_THIRDPARTY_DIR}/TTKWatcher
  ${MUSIC_THIRDPARTY_DIR}/TTKExtras
)


set(MUSIC_BUILD_DIR "${CMAKE_CACHEFILE_DIR}/bin")
set(MUSIC_GENERATE_DIR "${CMAKE_CACHEFILE_DIR}/Generate")

include_directories(${MUSIC_GENERATE_DIR})

set(LIBRARY_OUTPUT_PATH ${MUSIC_BUILD_DIR}/${TTK_VERSION})
set(EXECUTABLE_OUTPUT_PATH ${MUSIC_BUILD_DIR}/${TTK_VERSION})
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH}/GLanguage)

set(OPENSSL_DIR "" CACHE PATH "Openssl install path")

set(TTK_QT_VERSION "0" CACHE STRING "Expected Qt version")
set_property(CACHE TTK_QT_VERSION PROPERTY STRINGS 4 5)

if(NOT (TTK_QT_VERSION STREQUAL "4" OR TTK_QT_VERSION STREQUAL "5"))
  message(FATAL_ERROR "Message Expected value for TTK_QT_VERSION is either '4' or '5'")
endif()

set(CMAKE_PREFIX_PATH ${QT_INSTALL_DIR} CACHE PATH "Qt install path")

find_program(QT_QMAKE_EXECUTABLE qmake ${QT_INSTALL_BINS})
if(QT_QMAKE_EXECUTABLE)
  message(STATUS "Message Found qmake executable: " ${QT_QMAKE_EXECUTABLE})
else()
  message(FATAL_ERROR "Message Could NOT find qmake executable")
endif()


if(TTK_QT_VERSION VERSION_GREATER "4")
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Multimedia REQUIRED)
  find_package(Qt5Network REQUIRED)
  find_package(Qt5Xml REQUIRED)
  find_package(Qt5OpenGL REQUIRED)

  if(WIN32)
    find_package(Qt5WinExtras REQUIRED)
  else(UNIX)
    find_package(Qt5X11Extras REQUIRED)
    find_package(Qt5DBus REQUIRED)
  endif()
else()
  set(minimum_required_qt_version "4.8")
  find_package(Qt4 REQUIRED)
  
  if(QT4_FOUND)
    if("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" VERSION_LESS "${minimum_required_qt_version}")
      message(FATAL_ERROR "Message error: TTK requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
    endif()

    set(QT_USE_QTCORE ON)
    set(QT_USE_QTGUI ON)
    set(QT_USE_QTNETWORK ON)
    set(QT_USE_QTXML ON)
    set(QT_USE_QTOPENGL ON)

    if(WIN32)
      set(QT_USE_QTMULTIMEDIA ON)
    else(UNIX)
      set(QT_USE_QTDBUS ON)
    endif()

    include(${QT_USE_FILE})

    if(WIN32)
      get_filename_component(${QT_LIBRARY_DIR} ${QT_QMAKE_EXECUTABLE} PATH)
    endif()
  else()
    message(FATAL_ERROR "Message error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
  endif()
endif()


find_program(QT_LRELEASE_EXECUTABLE lrelease ${QT_INSTALL_BINS})
if(QT_LRELEASE_EXECUTABLE)
  message(STATUS "Message Found lrelease executable: " ${QT_LRELEASE_EXECUTABLE})
else()
  message(FATAL_ERROR "Message Could NOT find lrelease executable")
endif()


if(UNIX)
  find_program(RENAME_EXECUTABLE rename /usr/bin)
  if(RENAME_EXECUTABLE)
    message(STATUS "Message Found rename executable: " ${RENAME_EXECUTABLE})
  else()
    message(FATAL_ERROR "Message Could NOT find rename executable")
  endif()
endif()


find_library(TTK_QMMP_LIBRARY
  NAMES TTKqmmp
  PATHS ${LIBRARY_OUTPUT_PATH}
)

# qmmp include
include_directories(${MUSIC_THIRDPARTY_DIR}/TTKqmmp)
if(NOT (TTK_QT_VERSION VERSION_GREATER "4"))
  if(UNIX)
    include_directories(
      /usr/include/QtMultimediaKit
      /usr/include/QtMobility
    )
  endif()
endif()


add_subdirectory(TTKCommon)
add_subdirectory(TTKConfig)
add_subdirectory(TTKUi)
add_subdirectory(TTKThirdParty)
add_subdirectory(TTKModule)
add_subdirectory(TTKService)
add_subdirectory(TTKRun)


set(MUSIC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}")
set(MUSIC_INSTALL_BIN_DIR "${MUSIC_INSTALL_DIR}/${TTK_VERSION}")

install(DIRECTORY "${MUSIC_BUILD_DIR}/" DESTINATION "${MUSIC_INSTALL_DIR}" USE_SOURCE_PERMISSIONS)
install(FILES "${MUSIC_DIR}/ChangeLog"
              "${MUSIC_DIR}/LICENSE"
              "${MUSIC_DIR}/LICENSE.LGPLv3"
              "${MUSIC_DIR}/README.md"
              "${MUSIC_DIR}/README_cn.md"
              "${MUSIC_DIR}/README_tc.md"
              "${MUSIC_DIR}/.github/FUNDING.yml"
              "${MUSIC_UTILS_DIR}/resource/LICENSE.QT-LICENSE-AGREEMENT"
        DESTINATION "${MUSIC_INSTALL_DIR}/doc")
install(FILES "${MUSIC_UTILS_DIR}/resource/README.txt" DESTINATION "${MUSIC_INSTALL_DIR}")
if(WIN32)
  install(FILES "${MUSIC_UTILS_DIR}/resource/qicon.dll" DESTINATION "${MUSIC_INSTALL_DIR}")
else(UNIX)
  install(DIRECTORY "${MUSIC_UTILS_DIR}/deploy" DESTINATION "${MUSIC_INSTALL_DIR}")
  install(FILES "${MUSIC_UTILS_DIR}/ttk_runtime" DESTINATION "${MUSIC_INSTALL_DIR}")
  install(FILES "${MUSIC_UTILS_DIR}/deploy/share/appdata/ttkmusicplayer.appdata.xml" DESTINATION "${MUSIC_INSTALL_DIR}/share/metainfo")
endif()
# copy run resource
if(UNIX)
  install(CODE "execute_process(COMMAND \"${MUSIC_UTILS_DIR}/resource.sh\" \"${MUSIC_RESOURCE_DIR}\" \"${MUSIC_INSTALL_BIN_DIR}\")")
endif()
# update and release translations
if(WIN32)
  install(CODE "execute_process(COMMAND \"${MUSIC_UTILS_DIR}/ts_win.bat\" \"${QT_LRELEASE_EXECUTABLE}\" \"${MUSIC_INSTALL_BIN_DIR}\" \"${MUSIC_DIR}\")")
else(UNIX)
  install(CODE "execute_process(COMMAND \"${MUSIC_UTILS_DIR}/ts_linux.sh\" \"${QT_LRELEASE_EXECUTABLE}\" \"${MUSIC_INSTALL_BIN_DIR}\" \"${MUSIC_DIR}\")")
endif()
