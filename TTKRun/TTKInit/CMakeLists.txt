cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKInit)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

include_directories(${MUSIC_CONFIG_DIR})

set(MUSIC_SOURCES
    musicinitmain.cpp
  )

if(WIN32)
    list(APPEND MUSIC_SOURCES ${TARGET_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  
  add_executable(${TARGET_NAME} ${MUSIC_SOURCES})
  target_link_libraries(${TARGET_NAME} Qt5::Core TTKConfig)
else()
  add_executable(${TARGET_NAME} ${MUSIC_SOURCES})
  target_link_libraries(${TARGET_NAME} ${QT_QTCORE_LIBRARY} TTKConfig)
endif()
