cmake_minimum_required(VERSION 2.8.11)

project(TTKDumper)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
  cmake_policy(SET CMP0005 OLD)
  cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
  ttkdumper.h
  miniprocess.h
)

set(MUSIC_SOURCES
  ttkdumper.cpp
  miniprocess.cpp
)
  
if(WIN32)
  list(APPEND MUSIC_SOURCES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${PROJECT_NAME} STATIC ${MUSIC_SOURCES}  ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  
  set(QT5_LIBS Qt5::Core)
  if(WIN32)
    list(APPEND QT5_LIBS -lpsapi)
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT5_LIBS})
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${PROJECT_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  
  set(QT4_LIBS ${QT_QTCORE_LIBRARY})
  if(WIN32)
    list(APPEND QT4_LIBS -lpsapi)
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT4_LIBS})
endif()
