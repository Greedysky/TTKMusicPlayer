cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKService)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
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
  list(APPEND QT5_LIBS Qt5::Core TTKCore)
  if(WIN32)
    list(APPEND QT5_LIBS TTKDumper)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT5_LIBS})
else()
  list(APPEND QT4_LIBS ${QT_QTCORE_LIBRARY} TTKCore)
  if(WIN32)
    list(APPEND QT4_LIBS TTKDumper)
  endif()
  target_link_libraries(${TARGET_NAME} ${QT4_LIBS})
endif()
