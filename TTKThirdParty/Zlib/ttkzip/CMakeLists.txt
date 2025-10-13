# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2025 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
#
# * You should have received a copy of the GNU General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
# ***************************************************************************

cmake_minimum_required(VERSION 3.18.0)

project(ttkzip)

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
