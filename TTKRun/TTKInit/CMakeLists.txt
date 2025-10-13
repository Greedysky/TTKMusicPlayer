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

project(TTKInit)

include_directories(${TTK_CONFIG_DIR})

set(SOURCE_FILES
  musicinitmain.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

set(QT_LINK_LIBS TTKConfig)

if(TTK_QT_VERSION VERSION_EQUAL "4")
  list(APPEND QT_LINK_LIBS ${QT_QTCORE_LIBRARY})
elseif(TTK_QT_VERSION VERSION_EQUAL "5")
  list(APPEND QT_LINK_LIBS Qt5::Core)
elseif(TTK_QT_VERSION VERSION_EQUAL "6")
  list(APPEND QT_LINK_LIBS Qt6::Core)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LINK_LIBS})
