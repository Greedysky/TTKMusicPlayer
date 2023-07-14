cmake_minimum_required(VERSION 2.8.11)

project(TTKMusicPlayer)

set(LIBRARY_OUTPUT_PATH ${MUSIC_INSTALL_DIR})
set(EXECUTABLE_OUTPUT_PATH ${MUSIC_INSTALL_DIR})

include_directories(${MUSIC_COMMON_DIR})

set(HEADER_FILES
  ttkrunobject.h
)

set(SOURCE_FILES
  ttkrunobject.cpp
  ttkrunmain.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES TTKApp.rc)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
