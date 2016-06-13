cmake_minimum_required(VERSION 2.8.11)

project(MusicExtras)
set(TARGET_NAME MusicExtras)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)


if(TTK_QT_VERSION VERSION_GREATER "4")
  find_package(Qt5WebKit REQUIRED)
  find_package(Qt5WebKitWidgets REQUIRED)
else()
  set(minimum_required_qt_version "4.6")
  find_package(Qt4 REQUIRED)
  
  if(QT4_FOUND)
    if("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" VERSION_LESS "${minimum_required_qt_version}")
      message(FATAL_ERROR "error: TTK requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
    endif()

    set(QT_USE_QTWEBKIT ON)
    include(${QT_USE_FILE})

    if(WIN32)
      get_filename_component(${QT_LIBRARY_DIR} ${QT_QMAKE_EXECUTABLE} PATH)
    endif()
  else()
    message(FATAL_ERROR "error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
  endif()
endif()


set(MUSIC_HEADERS
    musicextrasglobaldefine.h.h
    shortcut/qxtglobal.h
    shortcut/qxtglobalshortcut.h
    shortcut/qxtglobalshortcut_p.h
    kugou/kugouurl.h
    kugou/kugouwindow.h
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
  )

set(MUSIC_SOURCES
    shortcut/qxtglobalshortcut.cpp
    kugou/kugouurl.cpp
    kugou/kugouwindow.cpp
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
  )

if(WIN32)
    list(APPEND MUSIC_SOURCES shortcut/qxtglobalshortcut_win.cpp)
else(UNIX)
    list(APPEND MUSIC_SOURCES shortcut/qxtglobalshortcut_x11.cpp)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  if(TTK_BUILD_SHARED STREQUAL "true")
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  list(APPEND QT5_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WebKit Qt5::WebKitWidgets -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT5_LIBS -luser32)
    list(APPEND QT5_LIBS Qt5::X11Extras)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  if(TTK_BUILD_SHARED STREQUAL "true")
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  list(APPEND QT4_LIBS ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTWEBKIT_LIBRARY} -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT4_LIBS -luser32)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
