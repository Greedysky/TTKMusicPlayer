# ***************************************************************************
# * This file is part of the TTK Library Module project
# * Copyright (C) 2015 - 2026 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU Lesser General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU Lesser General Public License for more details.
#
# * You should have received a copy of the GNU Lesser General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
# ***************************************************************************

cmake_minimum_required(VERSION 3.18.0)

project(config)

# check default module
try_compile(COMPILE_SUCCEEDED
  ${PROJECT_BINARY_DIR}/test_default
  ${CMAKE_CURRENT_SOURCE_DIR}/config.tests/default/default.cpp
)

if(COMPILE_SUCCEEDED)
  message(STATUS "-- default module test is always true")
else()
  message(STATUS "-- default module test failed, check the platform compiler")
endif()

# check strlcat module
try_compile(COMPILE_SUCCEEDED
  ${PROJECT_BINARY_DIR}/test_strlcat
  ${CMAKE_CURRENT_SOURCE_DIR}/config.tests/strlcat/strlcat.cpp
)

if(COMPILE_SUCCEEDED)
  set(HAVE_STRLCAT 1)
  message(STATUS "-- string.h has strlcat function module")
else()
  set(HAVE_STRLCAT 0)
  message(STATUS "-- string.h hasn't strlcat function module, use compat module")
endif()

# check strlcpy module
try_compile(COMPILE_SUCCEEDED
  ${PROJECT_BINARY_DIR}/test_strlcpy
  ${CMAKE_CURRENT_SOURCE_DIR}/config.tests/strlcpy/strlcpy.cpp
)

if(COMPILE_SUCCEEDED)
  set(HAVE_STRLCPY 1)
  message(STATUS "-- string.h has strlcpy function module")
else()
  set(HAVE_STRLCPY 0)
  message(STATUS "-- string.h hasn't strlcpy function module, use compat module")
endif()

# configure ttk config file
configure_file(ttkconfig.h.cmake ${TTK_GENERATE_DIR}/ttkconfig.h @ONLY)
