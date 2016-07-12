cmake_minimum_required(VERSION 2.8.11)

set(TARGET_NAME zlib)
project(${TARGET_NAME})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_THREAD)

set(MUSIC_HEADERS
    zconf.h
    zlib.h
    crc32.h
    deflate.h
    gzguts.h
    inffixed.h
    inffast.h
    inflate.h
    inftrees.h
    trees.h
    zutil.h
  )

set(MUSIC_SOURCES
    adler32.c
    compress.c
    crc32.c
    deflate.c
    gzclose.c
    gzlib.c
    gzread.c
    gzwrite.c
    inflate.c
    infback.c
    inftrees.c
    inffast.c
    trees.c
    uncompr.c
    zutil.c
  )

if(TTK_BUILD_SHARED)
  add_library(${TARGET_NAME} SHARED ${MUSIC_SOURCES} ${MUSIC_HEADERS})
else()
  add_library(${TARGET_NAME} STATIC ${MUSIC_SOURCES} ${MUSIC_HEADERS})
endif()