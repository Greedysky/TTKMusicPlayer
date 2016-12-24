cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME MusicExtras)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)


set(QT_WEBKIT_MODULE_NAME "libQt5WebKit")
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
    message(STATUS "Message Found QT_WEBKIT_MODULE component, build in KuGou by webkit :" ${QT_WEBKIT_MODULE})
  else()
    message(STATUS "Message Not found QT_WEBKIT_MODULE component, build in KuGou by normal")
  endif()
endmacro(_TO_FOUND_WEBKIT)


if(TTK_QT_VERSION VERSION_GREATER "4")
  if(WIN32)
    if(MSVC)
      _TO_FOUND_WEBKIT(${QT_WEBKIT_MODULE_NAME}.lib)
    else()
      _TO_FOUND_WEBKIT(${QT_WEBKIT_MODULE_NAME}.a)
    endif()
  else(UNIX)
    _TO_FOUND_WEBKIT(${QT_WEBKIT_MODULE_NAME}.so)
  endif()
else()
  set(minimum_required_qt_version "4.8")
  find_package(Qt4 REQUIRED)
  add_definitions(-DMUSIC_WEBKIT)
  message(STATUS "Message Found QT_WEBKIT_MODULE component, build in KuGou by webkit")
  
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


set(MUSIC_HEADERS
    musicextrasglobaldefine.h
    qhz2py/pinyinresource.h
    qhz2py/chinesehelper.h
    qhz2py/pinyinhelper.h
    qshortcut/qxtglobal.h
    qshortcut/qxtglobalshortcut.h
    qshortcut/qxtglobalshortcut_p.h
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
    qshortcut/qxtglobalshortcut.cpp
    qhz2py/pinyinresource.cpp
    qhz2py/chinesehelper.cpp
    qhz2py/pinyinhelper.cpp
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
    list(APPEND MUSIC_SOURCES qshortcut/qxtglobalshortcut_win.cpp)
else(UNIX)
    list(APPEND MUSIC_SOURCES qshortcut/qxtglobalshortcut_x11.cpp)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES}  ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  list(APPEND QT5_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebKit Qt5::WebKitWidgets MusicUi -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT5_LIBS -luser32)
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
  list(APPEND QT4_LIBS ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTWEBKIT_LIBRARY} MusicUi -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT4_LIBS -luser32)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
