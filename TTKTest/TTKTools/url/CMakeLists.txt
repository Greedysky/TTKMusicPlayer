cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME url)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
    toolsurl.h
  )

set(MUSIC_SOURCES
    toolsurl.cpp
    mainurl.cpp
  )
  
set(MUSIC_UIS
    toolsurl.ui
  )
  
if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT5_WRAP_UI(MUSIC_UIS_H ${MUSIC_UIS})
  
  add_executable(${TARGET_NAME} ${MUSIC_UIS_H} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  target_link_libraries(${TARGET_NAME} Qt5::Core Qt5::Gui Qt5::Widgets TTKCore)
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT4_WRAP_UI(MUSIC_UIS_H ${MUSIC_UIS})
  
  add_executable(${TARGET_NAME} ${MUSIC_UIS_H} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  target_link_libraries(${TARGET_NAME} ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} TTKCore)
endif()