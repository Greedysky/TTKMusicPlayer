cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKMusicPlayer)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin) 

set(MUSIC_HEADERS
    musicserviceglobaldefine.h
    musicserviceapplication.h
    musicserviceobject.h
  )

set(MUSIC_SOURCES
    musicserviceapplication.cpp
    musicserviceobject.cpp
    musicservicemain.cpp
  )
  

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  
  add_executable(${TARGET_NAME} TTKService.rc ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  list(APPEND QT5_LIBS Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets)
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})

  add_executable(${TARGET_NAME} TTKService.rc ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  list(APPEND QT4_LIBS ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY})
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
