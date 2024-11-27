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

project(TTKWatcher)

set(HEADER_FILES
  qdevicewatcher.h
  qdevicewatcher_p.h
)

set(SOURCE_FILES
  qdevicewatcher.cpp
)

if(WIN32)
  list(APPEND SOURCE_FILES qdevicewatcher_win32.cpp ${PROJECT_NAME}.rc)
elseif(APPLE)
  list(APPEND SOURCE_FILES qdevicewatcher_mac.cpp)
elseif(WINCE)
  list(APPEND SOURCE_FILES qdevicewatcher_wince.cpp)
else()
  list(APPEND SOURCE_FILES qdevicewatcher_linux.cpp)
endif()

if(WIN32)
  list(APPEND QT_LINK_LIBS user32)
endif()

if(TTK_QT_VERSION VERSION_EQUAL "4")
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})

  list(APPEND QT_LINK_LIBS ${QT_QTCORE_LIBRARY})
elseif(TTK_QT_VERSION VERSION_EQUAL "5")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})

  list(APPEND QT_LINK_LIBS Qt5::Core)
elseif(TTK_QT_VERSION VERSION_EQUAL "6")
  qt6_wrap_cpp(MOC_FILES ${HEADER_FILES})

  list(APPEND QT_LINK_LIBS Qt6::Core)
endif()

if(TTK_BUILD_SHARED)
  add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})
else()
  add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})
endif()
target_link_libraries(${PROJECT_NAME} ${QT_LINK_LIBS})
