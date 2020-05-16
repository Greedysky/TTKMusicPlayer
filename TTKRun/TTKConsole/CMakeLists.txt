cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKConsole)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
    ../musicrunglobaldefine.h
    musicconsoleobject.h
  )

set(MUSIC_SOURCES
    musicconsoleobject.cpp
    musicconsolemain.cpp
  )

if(WIN32)
    list(APPEND MUSIC_SOURCES ${TARGET_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  add_executable(${TARGET_NAME} ${MUSIC_RCC_SRCS} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  target_link_libraries(${TARGET_NAME} Qt5::Core TTKCore )
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  add_executable(${TARGET_NAME} ${MUSIC_RCC_SRCS} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  target_link_libraries(${TARGET_NAME} ${QT_QTCORE_LIBRARY} TTKCore)
endif()
