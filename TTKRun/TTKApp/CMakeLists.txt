cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKMusicPlayer)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

if(UNIX)
add_definitions(-DCONFIG_OUT_BUILD)
set(LIBRARY_OUTPUT_PATH ${MUSIC_INSTALL_DIR})
set(EXECUTABLE_OUTPUT_PATH ${MUSIC_INSTALL_DIR})
endif()

include_directories(${MUSIC_CONFIG_DIR})
include_directories(${MUSIC_COMMON_APP_DIR})

set(TTKAPPLICATION_HEADERS
    ${MUSIC_COMMON_APP_DIR}/ttklocalpeer.h
    ${MUSIC_COMMON_APP_DIR}/ttkrunapplication.h
  )

set(TTKAPPLICATION_SOURCES
    ${MUSIC_COMMON_APP_DIR}/ttklocalpeer.cpp
    ${MUSIC_COMMON_APP_DIR}/ttkrunapplication.cpp
  )

set(MUSIC_HEADERS
    ttkrunobject.h
    ${TTKAPPLICATION_HEADERS}
  )

set(MUSIC_SOURCES
    ttkrunobject.cpp
    ttkrunmain.cpp
    ${TTKAPPLICATION_SOURCES}
  )

if(UNIX)
set(MUSIC_HEADERS
    ${MUSIC_CONFIG_DIR}/musicconfigobject.h
    ${MUSIC_CONFIG_DIR}/musicconfigdefine.h
    ${MUSIC_HEADERS}
  )

set(MUSIC_SOURCES
    ${MUSIC_CONFIG_DIR}/musicconfigobject.cpp
    ${MUSIC_SOURCES}
  )
endif()

SET(MUSIC_QRC
    ${MUSIC_UI_DIR}/MusicApp.qrc
)

if(WIN32)
    list(APPEND MUSIC_SOURCES TTKApp.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  QT5_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT5_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC})
  
  add_executable(${TARGET_NAME} ${MUSIC_RCC_SRCS} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  list(APPEND QT5_LIBS Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets)
  if(WIN32)
      list(APPEND QT5_LIBS TTKConfig)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  QT4_WRAP_CPP(MUSIC_MOC_H ${MUSIC_HEADERS})
  QT4_ADD_RESOURCES(MUSIC_RCC_SRCS ${MUSIC_QRC})
  
  add_executable(${TARGET_NAME} ${MUSIC_RCC_SRCS} ${MUSIC_SOURCES} ${MUSIC_MOC_H} ${MUSIC_HEADERS})
  list(APPEND QT4_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY})
  if(WIN32)
      list(APPEND QT4_LIBS TTKConfig)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
