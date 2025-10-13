# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2025 Greedysky Studio
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

project(TTKExtras)

set(QT_WEBKIT_MODULE_NAME "libQt5WebKit")
set(QT_WEBENGINE_MODULE_NAME "libQt5WebEngine")
set(QT_WEBKIT_MODULE_TYPE "0")

macro(_TO_FOUND_WEBKIT module)
  find_library(QT_WEBKIT_MODULE
    NAMES ${module}
    PATHS ${QT_INSTALL_LIBS}
  )

  string(REGEX MATCH "${QT_WEBKIT_MODULE_NAME}" QT_WEBKIT_NAME_FILTER ${QT_WEBKIT_MODULE})
  string(COMPARE EQUAL "${QT_WEBKIT_NAME_FILTER}" "${QT_WEBKIT_MODULE_NAME}" QT_RESULT)

  if(${QT_RESULT})
    find_package(Qt5WebKit REQUIRED)
    find_package(Qt5WebKitWidgets REQUIRED)
    add_definitions(-DTTK_WEBKIT)
    set(QT_WEBKIT_MODULE_TYPE "2")
    message(STATUS "Found Qt web component, build in KuGou module by Qt webkit :" ${QT_WEBKIT_MODULE})
  else()
    message(STATUS "Not found Qt webkit")
  endif()
endmacro(_TO_FOUND_WEBKIT)


macro(_TO_FOUND_WEBENGINE module)
  find_library(QT_WEBENGINE_MODULE
    NAMES ${module}
    PATHS ${QT_INSTALL_LIBS}
  )

  string(REGEX MATCH "${QT_WEBENGINE_MODULE_NAME}" QT_WEBENGINE_NAME_FILTER ${QT_WEBENGINE_MODULE})
  string(COMPARE EQUAL "${QT_WEBENGINE_NAME_FILTER}" "${QT_WEBENGINE_MODULE_NAME}" QT_RESULT)

  if(${QT_RESULT})
    find_package(Qt5WebEngineWidgets REQUIRED)
    add_definitions(-DTTK_WEBENGINE)
    set(QT_WEBKIT_MODULE_TYPE "3")
    message(STATUS "Found Qt web component, build in KuGou module by Qt webengine :" ${QT_WEBENGINE_MODULE})
  else()
    message(STATUS "Not found Qt webengine")
  endif()
endmacro(_TO_FOUND_WEBENGINE)


if(WIN32)
  add_definitions(-DTTK_MINIBLINK)
  set(QT_WEBKIT_MODULE_TYPE "1")
  message(STATUS "Found Qt web component, build in KuGou module by Qt miniblink")
else()
  if(TTK_QT_VERSION VERSION_GREATER "4")
    if(APPLE)
      _TO_FOUND_WEBKIT(${QT_WEBKIT_MODULE_NAME}.dylib)
      if(QT_WEBKIT_MODULE_TYPE STREQUAL "0")
        _TO_FOUND_WEBENGINE(${QT_WEBENGINE_MODULE_NAME}.dylib)
      endif()
    else()
      _TO_FOUND_WEBKIT(${QT_WEBKIT_MODULE_NAME}.so)
      if(QT_WEBKIT_MODULE_TYPE STREQUAL "0")
        _TO_FOUND_WEBENGINE(${QT_WEBENGINE_MODULE_NAME}.so)
      endif()
    endif()
  else()
    set(minimum_required_qt_version "4.8")
    find_package(Qt4 REQUIRED)

    if(QT4_FOUND)
      if("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" VERSION_LESS "${minimum_required_qt_version}")
        message(FATAL_ERROR "Fatal error: TTK requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
      endif()

      set(QT_USE_QTWEBKIT ON)
      include(${QT_USE_FILE})

      add_definitions(-DTTK_WEBKIT)
      set(QT_WEBKIT_MODULE_TYPE "1")
      message(STATUS "Found Qt web component, build in KuGou module by Qt webkit")
    else()
      message(FATAL_ERROR "Fatal error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
    endif()
  endif()
endif()

set(HEADER_FILES
  qalgorithm/aes.h
  qalgorithm/aes_locl.h
  qalgorithm/cbc128.h
  qalgorithm/deswrapper.h
  qalgorithm/aeswrapper.h
  qalgorithm/imagewrapper.h
  qdlna/qdlna.h
  qdlna/qdlnaclient.h
  qdlna/qdlnafinder.h
  qdlna/qdlnaservice.h
  qdlna/qdlnaxml.h
  qdlna/qdlnafileserver.h
  qhttpserver/http_parser.h
  qhttpserver/qhttpconnection.h
  qhttpserver/qhttprequest.h
  qhttpserver/qhttpresponse.h
  qhttpserver/qhttpserver.h
  qhttpserver/qhttpserverfwd.h
  qshortcut/qglobalshortcut.h
  qshortcut/qglobalshortcut_p.h
  qkugou/qkugouurl.h
  qkugou/qkugouwindow.h
  qkugou/qkugouuiobject.h
  qrencode/bitstream.h
  qrencode/config.h
  qrencode/mask.h
  qrencode/mmask.h
  qrencode/mqrspec.h
  qrencode/qrencode_inner.h
  qrencode/qrencode.h
  qrencode/qrinput.h
  qrencode/qrspec.h
  qrencode/rscode.h
  qrencode/rsecc.h
  qrencode/split.h
  qrencode/qrcodewidget.h
  qsync/qsyncconfig.h
  qsync/qsyncdataitem.h
  qsync/qsyncutils.h
  qsync/qsyncdatainterface.h
  qsync/qsynclistdata.h
  qsync/qsyncdeletedata.h
  qsync/qsyncuploaddata.h
  qsync/qsyncdownloaddata.h
  qjson/json.h
  qjson/QJsonArray.h
  qjson/QJsonDocument.h
  qjson/QJsonObject.h
  qjson/QJsonParseError.h
  qjson/QJsonValue.h
  qjson/QJsonValueRef.h
  qjson/QJsonParser.h
  qjson/QJsonRoot.h
)

set(SOURCE_FILES
  qalgorithm/aes_cbc.cpp
  qalgorithm/aes_core.cpp
  qalgorithm/aes_ecb.cpp
  qalgorithm/cbc128.cpp
  qalgorithm/deswrapper.cpp
  qalgorithm/aeswrapper.cpp
  qalgorithm/imagewrapper.cpp
  qdlna/qdlna.cpp
  qdlna/qdlnaclient.cpp
  qdlna/qdlnafinder.cpp
  qdlna/qdlnaxml.cpp
  qdlna/qdlnafileserver.cpp
  qhttpserver/http_parser.c
  qhttpserver/qhttpconnection.cpp
  qhttpserver/qhttprequest.cpp
  qhttpserver/qhttpresponse.cpp
  qhttpserver/qhttpserver.cpp
  qshortcut/qglobalshortcut.cpp
  qkugou/qkugouurl.cpp
  qkugou/qkugouwindow.cpp
  qrencode/bitstream.c
  qrencode/mask.c
  qrencode/mmask.c
  qrencode/mqrspec.c
  qrencode/qrencode.c
  qrencode/qrinput.c
  qrencode/qrspec.c
  qrencode/rscode.c
  qrencode/rsecc.c
  qrencode/split.c
  qrencode/qrcodewidget.cpp
  qsync/qsyncconfig.cpp
  qsync/qsyncutils.cpp
  qsync/qsyncdatainterface.cpp
  qsync/qsynclistdata.cpp
  qsync/qsyncdeletedata.cpp
  qsync/qsyncuploaddata.cpp
  qsync/qsyncdownloaddata.cpp
  qjson/QJsonArray.cpp
  qjson/QJsonDocument.cpp
  qjson/QJsonObject.cpp
  qjson/QJsonParseError.cpp
  qjson/QJsonValue.cpp
  qjson/QJsonValueRef.cpp
  qjson/QJsonParser.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES qshortcut/qglobalshortcut_win.cpp ${PROJECT_NAME}.rc)
  list(APPEND HEADER_FILES qkugou/miniblink/miniblink.h)
  list(APPEND SOURCE_FILES qkugou/miniblink/miniblink.cpp)
elseif(APPLE)
  list(APPEND SOURCE_FILES qshortcut/qglobalshortcut_mac.cpp)
else()
  list(APPEND SOURCE_FILES qshortcut/qglobalshortcut_x11.cpp)
endif()

set(QT_LINK_LIBS TTKLibrary TTKUi)

# qglobalshortcut_mac.cpp needs this.
# Notice, it cannot work on recent macOS versions,
# so either additional implementation is needed, using
# modern API, or at least a dummy fallback.
if(APPLE)
  list(APPEND QT_LINK_LIBS "-framework Carbon")
endif()

if(TTK_QT_VERSION VERSION_EQUAL "4")
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})

  if(WIN32)
    list(APPEND QT_LINK_LIBS user32)
  else()
    list(APPEND QT_LINK_LIBS ${QT_QTWEBKIT_LIBRARY})
  endif()

  list(APPEND QT_LINK_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTXML_LIBRARY})
elseif(TTK_QT_VERSION VERSION_EQUAL "5")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})

  if(WIN32)
    list(APPEND QT_LINK_LIBS user32)
  else()
    if(QT_WEBKIT_MODULE_TYPE STREQUAL "2")
      list(APPEND QT_LINK_LIBS Qt5::WebKit Qt5::WebKitWidgets)
    elseif(QT_WEBKIT_MODULE_TYPE STREQUAL "3")
      list(APPEND QT_LINK_LIBS Qt5::WebEngineWidgets)
    else()
      message(STATUS "Not found Qt web component, build in KuGou module by none web component")
    endif()

    if(NOT APPLE)
      list(APPEND QT_LINK_LIBS Qt5::X11Extras)
    endif()
  endif()

  list(APPEND QT_LINK_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Xml)
elseif(TTK_QT_VERSION VERSION_EQUAL "6")
  qt6_wrap_cpp(MOC_FILES ${HEADER_FILES})

  if(WIN32)
    list(APPEND QT_LINK_LIBS user32)
  else()
    if(QT_WEBKIT_MODULE_TYPE STREQUAL "2")
      list(APPEND QT_LINK_LIBS Qt6::WebKit Qt6::WebKitWidgets)
    elseif(QT_WEBKIT_MODULE_TYPE STREQUAL "3")
      list(APPEND QT_LINK_LIBS Qt6::WebEngineWidgets)
    else()
      message(STATUS "Not found Qt web component, build in KuGou module by none web component")
    endif()
  endif()

  list(APPEND QT_LINK_LIBS Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network Qt6::Xml)
endif()

if(TTK_BUILD_SHARED)
  add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})
else()
  add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})
endif()
target_link_libraries(${PROJECT_NAME} ${QT_LINK_LIBS})
