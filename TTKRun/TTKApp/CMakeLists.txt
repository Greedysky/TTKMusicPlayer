cmake_minimum_required(VERSION 2.8.11)

project(TTKMusicPlayer)

set(LIBRARY_OUTPUT_PATH ${MUSIC_INSTALL_DIR})
set(EXECUTABLE_OUTPUT_PATH ${MUSIC_INSTALL_DIR})

include_directories(${MUSIC_COMMON_DIR})
include_directories(${MUSIC_COMMON_RUN_DIR})

set(HEADER_FILES
  ${MUSIC_COMMON_RUN_DIR}/ttkrunobject.h
)

set(SOURCE_FILES
  ${MUSIC_COMMON_RUN_DIR}/ttkrunobject.cpp
  ttkrunmain.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES TTKApp.rc)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
