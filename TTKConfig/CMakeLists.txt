cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKConfig)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
    musicconfigglobaldefine.h
    musicconfigdefine.h
    musicconfigobject.h
  )

set(MUSIC_SOURCES
    musicconfigobject.cpp
  )

SET(MUSIC_QRC
    ${MUSIC_UI_DIR}/MusicApp.qrc
)

if(WIN32)
    list(APPEND MUSIC_SOURCES ${TARGET_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT5_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC})
  
  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_RCC_SRCS} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_RCC_SRCS} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  target_link_libraries(${TARGET_NAME} Qt5::Core)
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT4_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC})
  
  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_RCC_SRCS} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_RCC_SRCS} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  target_link_libraries(${TARGET_NAME} ${QT_QTCORE_LIBRARY})
endif()

