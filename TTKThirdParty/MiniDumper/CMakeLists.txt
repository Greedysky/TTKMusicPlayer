cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKDumper)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
    ../musicextrasglobaldefine.h
    minidumper.h
    
  )

set(MUSIC_SOURCES
    minidumper.cpp
    MiniDumper.rc
  )

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES}  ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  target_link_libraries(${TARGET_NAME} Qt5::Core)
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  target_link_libraries(${TARGET_NAME} ${QT_QTCORE_LIBRARY})
endif()
