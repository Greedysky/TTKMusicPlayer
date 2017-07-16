cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKWatcher)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
    ../musicextrasglobaldefine.h
    qdevicewatcher.h
    qdevicewatcher_p.h
    
  )

set(MUSIC_SOURCES
    qdevicewatcher.cpp
    
  )

if(WIN32)
    list(APPEND MUSIC_SOURCES qdevicewatcher_win32.cpp Devicewatcher.rc)
elseif(APPLE)
    list(APPEND MUSIC_SOURCES qdevicewatcher_mac.cpp)
elseif(WINCE)
    list(APPEND MUSIC_SOURCES qdevicewatcher_wince.cpp)
else(UNIX)
    list(APPEND MUSIC_SOURCES qdevicewatcher_linux.cpp)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES}  ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  list(APPEND QT5_LIBS Qt5::Core -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT5_LIBS -luser32)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  if(TTK_BUILD_SHARED)
    add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  else()
    add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  endif()
  list(APPEND QT4_LIBS ${QT_QTCORE_LIBRARY} -luser32)
  if(UNIX)
    list(REMOVE_ITEM QT4_LIBS -luser32)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
