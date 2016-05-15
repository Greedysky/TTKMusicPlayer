cmake_minimum_required(VERSION 2.8.11)

project(MusicExtras)
set(TARGET_NAME MusicExtras)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
    qxtglobal.h
    qxtglobalshortcut.h
    qxtglobalshortcut_p.h
  )

set(MUSIC_SOURCES
    qxtglobalshortcut.cpp
  )

if(WIN32)
    list(APPEND MUSIC_SOURCES qxtglobalshortcut_win.cpp)
else(UNIX)
    list(APPEND MUSIC_SOURCES qxtglobalshortcut_x11.cpp)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  list(APPEND QT5_LIBS Qt5::Core Qt5::Gui Qt5::Widgets -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT5_LIBS -luser32)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  list(APPEND QT4_LIBS ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT4_LIBS -luser32)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
