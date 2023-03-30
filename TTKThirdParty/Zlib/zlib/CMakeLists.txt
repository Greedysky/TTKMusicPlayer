cmake_minimum_required(VERSION 2.8.11)

project(zlib)

add_definitions(-DZLIB_DLL)

set(HEADER_FILES
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

set(SOURCE_FILES
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
  
if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_BUILD_SHARED)
  add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
else()
  add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
endif()
