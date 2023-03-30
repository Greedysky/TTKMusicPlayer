cmake_minimum_required(VERSION 2.8.11)

project(TTKZip)

add_definitions(-DZLIB_DLL)
add_definitions(-DZLIB_INTERNAL)

set(HEADER_FILES
  zip.h
  unzip.h
  mztools.h
  ioapi.h
  crypt.h
)

set(SOURCE_FILES
  zip.c
  unzip.c
  mztools.c
  ioapi.c
)

if(WIN32)
  list(APPEND HEADER_FILES iowin32.h)
  list(APPEND SOURCE_FILES iowin32.c ${PROJECT_NAME}.rc)
endif()

if(TTK_BUILD_SHARED)
  add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
else()
  add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
endif()
target_link_libraries(${PROJECT_NAME} zlib)
