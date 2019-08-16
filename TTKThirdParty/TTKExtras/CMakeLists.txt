cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKExtras)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(QT_WEBKIT_MODULE_NAME "libQt5WebKit")
set(QT_WEBENGINE_MODULE_NAME "libQt5WebEngine")
set(USE_QT_WEBKIT_MODULE "0")


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
    add_definitions(-DMUSIC_WEBKIT)
    set(USE_QT_WEBKIT_MODULE "1")
    message(STATUS "Message Found QT_WEBKIT_MODULE component, build in KuGou by Qt webkit :" ${QT_WEBKIT_MODULE})
  else()
    message(STATUS "Message Not found QT_WEBKIT_MODULE component, build in KuGou by no webkit")
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
    add_definitions(-DMUSIC_WEBENGINE)
    set(USE_QT_WEBKIT_MODULE "2")
    message(STATUS "Message Found QT_WEBENGINE_MODULE component, build in KuGou by Qt webengine :" ${QT_WEBENGINE_MODULE})
  else()
    message(STATUS "Message Not found QT_WEBENGINE_MODULE component, build in KuGou by no webengine")
  endif()
endmacro(_TO_FOUND_WEBENGINE)


if(WIN32)
  if(TTK_QT_VERSION VERSION_GREATER "4")
    find_package(Qt5AxContainer REQUIRED)
  else()
    set(QT_USE_QAXCONTAINER ON)
  endif()
  set(USE_QT_WEBKIT_MODULE "3")
  message(STATUS "Webview build in KuGou by QAxContainer")
else()
  if(TTK_QT_VERSION VERSION_GREATER "4")
    if(WIN32)
      if(MSVC)
        _TO_FOUND_WEBKIT(${QT_WEBKIT_MODULE_NAME}.lib)
      else()
        _TO_FOUND_WEBKIT(${QT_WEBKIT_MODULE_NAME}.a)
      endif()
    else(UNIX)
      _TO_FOUND_WEBKIT(${QT_WEBKIT_MODULE_NAME}.so)
      string(COMPARE EQUAL "${USE_QT_WEBKIT_MODULE}" "0" QT_RESULT)
      if(${QT_RESULT})
        _TO_FOUND_WEBENGINE(${QT_WEBENGINE_MODULE_NAME}.so)
      endif()
    endif()
  else()
    set(minimum_required_qt_version "4.8")
    find_package(Qt4 REQUIRED)
    add_definitions(-DMUSIC_WEBKIT)
    set(USE_QT_WEBKIT_MODULE "1")
    message(STATUS "Message Found QT_WEBKIT_MODULE component, build in KuGou by Qt webkit")
    
    if(QT4_FOUND)
      if("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" VERSION_LESS "${minimum_required_qt_version}")
        message(FATAL_ERROR "Message error: TTK requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
      endif()

      set(QT_USE_QTWEBKIT ON)
      include(${QT_USE_FILE})

      if(WIN32)
        get_filename_component(${QT_LIBRARY_DIR} ${QT_QMAKE_EXECUTABLE} PATH)
      endif()
    else()
      message(FATAL_ERROR "Message error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
    endif()
  endif()
endif()

set(MUSIC_HEADERS
    ../musicextrasglobaldefine.h
    qalg/aes.h
    qalg/aes_locl.h
    qalg/base64.h
    qalg/cbc128.h
    qalg/qdeswrap.h
    qalg/qaeswrap.h
    qdlna/dlnaclient.h
    qdlna/dlnafinder.h
    qdlna/dlnahelper.h
    qdlna/dlnaservice.h
    qdlna/dlnaxml.h
    qhttpserver/http_parser.h
    qhttpserver/qhttpconnection.h
    qhttpserver/qhttprequest.h
    qhttpserver/qhttpresponse.h
    qhttpserver/qhttpserver.h
    qhttpserver/qhttpserverfwd.h
    qimage/gaussianblur.h
    qimage/qimagewrap.h
    qshortcut/qglobalshortcut.h
    qshortcut/qglobalshortcut_p.h
    qkugou/kugouurl.h
    qkugou/kugouwindow.h
    qkugou/kugouuiobject.h
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
    qrencode/split.h
    qrencode/qrcodewidget.h
    qiniu/qnconf.h
    qiniu/qniohelper.h
    qiniu/qndataitem.h
    qiniu/qnmac.h
    qiniu/qnputextra.h
    qiniu/qnputpolicy.h
    qiniu/qnputret.h
    qiniu/qnutils.h
    qiniu/qnsimplelistdata.h
    qiniu/qnsimpleuploaddata.h
    qiniu/qnsimpledeletedata.h
    qjson/parser.h
    qjson/parserrunnable.h
    qjson/qobjecthelper.h
    qjson/serializer.h
    qjson/serializerrunnable.h
    
  )

set(MUSIC_SOURCES
    qalg/aes_cbc.cpp
    qalg/aes_core.cpp
    qalg/aes_ecb.cpp
    qalg/base64.cpp
    qalg/cbc128.cpp
    qalg/qdeswrap.cpp
    qalg/qaeswrap.cpp
    qdlna/dlnaclient.cpp
    qdlna/dlnafinder.cpp
    qdlna/dlnahelper.cpp
    qdlna/dlnaservice.cpp
    qdlna/dlnaxml.cpp
    qhttpserver/http_parser.c
    qhttpserver/qhttpconnection.cpp
    qhttpserver/qhttprequest.cpp
    qhttpserver/qhttpresponse.cpp
    qhttpserver/qhttpserver.cpp
    qshortcut/qglobalshortcut.cpp
    qimage/gaussianblur.cpp
    qimage/qimagewrap.cpp
    qkugou/kugouurl.cpp
    qkugou/kugouwindow.cpp
    qrencode/bitstream.c
    qrencode/mask.c
    qrencode/mmask.c
    qrencode/mqrspec.c
    qrencode/qrencode.c
    qrencode/qrinput.c
    qrencode/qrspec.c
    qrencode/rscode.c
    qrencode/split.c
    qrencode/qrcodewidget.cpp
    qiniu/qnconf.cpp
    qiniu/qniohelper.cpp
    qiniu/qnmac.cpp
    qiniu/qnputextra.cpp
    qiniu/qnputpolicy.cpp
    qiniu/qnputret.cpp
    qiniu/qnutils.cpp
    qiniu/qnsimplelistdata.cpp
    qiniu/qnsimpleuploaddata.cpp
    qiniu/qnsimpledeletedata.cpp
    qjson/parser.cpp
    qjson/qobjecthelper.cpp
    qjson/json_scanner.cpp
    qjson/json_parser.cc
    qjson/parserrunnable.cpp
    qjson/serializer.cpp
    qjson/serializerrunnable.cpp
    
  )

if(WIN32)
    list(APPEND MUSIC_SOURCES qshortcut/qglobalshortcut_win.cpp ${TARGET_NAME}.rc)
else(UNIX)
    list(APPEND MUSIC_SOURCES qshortcut/qglobalshortcut_x11.cpp)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES}  ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  
  set(QT5_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network TTKUi)
  if(WIN32)
    string(COMPARE EQUAL "${USE_QT_WEBKIT_MODULE}" "3" QT_RESULT)
    if(${QT_RESULT})
      list(APPEND QT5_LIBS Qt5::AxContainer -luser32)
    endif()
  else()
    string(COMPARE EQUAL "${USE_QT_WEBKIT_MODULE}" "1" QT_RESULT)
    if(${QT_RESULT})
      list(APPEND QT5_LIBS Qt5::WebKit Qt5::WebKitWidgets)
    endif()

    string(COMPARE EQUAL "${USE_QT_WEBKIT_MODULE}" "2" QT_RESULT)
    if(${QT_RESULT})
      list(APPEND QT5_LIBS Qt5::WebEngineWidgets)
    endif()

    list(APPEND QT5_LIBS Qt5::X11Extras)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
  
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  
  set(QT4_LIBS ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} TTKUi)
  if(WIN32)
    string(COMPARE EQUAL "${USE_QT_WEBKIT_MODULE}" "3" QT_RESULT)
    if(${QT_RESULT})
      list(INSERT QT4_LIBS 0 ${QT_QAXCONTAINER_LIBRARY} -luser32)
    endif()
  else()
    list(APPEND QT4_LIBS ${QT_QTWEBKIT_LIBRARY})
  endif()
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
  
endif()
