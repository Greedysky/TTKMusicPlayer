cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME TTKZip)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
    cmake_policy(SET CMP0028 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)
add_definitions(-DZLIB_DLL)
add_definitions(-DZLIB_INTERNAL)

set(MUSIC_HEADERS
    zip.h
    unzip.h
    mztools.h
    ioapi.h
    crypt.h
  )

set(MUSIC_SOURCES
    zip.c
    unzip.c
    mztools.c
    ioapi.c
  )

if(WIN32)
    list(APPEND MUSIC_HEADERS iowin32.h)
    list(APPEND MUSIC_SOURCES iowin32.c ${TARGET_NAME}.rc)
endif()

if(TTK_BUILD_SHARED)
  add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_HEADERS})
else()
  add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_HEADERS})
endif()
target_link_libraries(${TARGET_NAME} zlib)
