cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKService)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_APP_SOURCES
  musicservicemain.cpp
)

if(WIN32)
    list(APPEND MUSIC_APP_SOURCES ${TARGET_NAME}.rc)
endif()

add_executable(${TARGET_NAME} ${MUSIC_APP_SOURCES})
add_dependencies(${TARGET_NAME} TTKCore)

if(TTK_QT_VERSION VERSION_GREATER "4")
  target_link_libraries(${TARGET_NAME} Qt5::Core TTKCore TTKDumper)
else()
  target_link_libraries(${TARGET_NAME} ${QT_QTCORE_LIBRARY} TTKCore TTKDumper)
endif()
