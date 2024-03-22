# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2024 Greedysky Studio
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

cmake_minimum_required(VERSION 3.0.0)

project(TTKMusicPlayer)

set(LIBRARY_OUTPUT_PATH ${MUSIC_BUILD_DIR})
set(EXECUTABLE_OUTPUT_PATH ${MUSIC_BUILD_DIR})

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
