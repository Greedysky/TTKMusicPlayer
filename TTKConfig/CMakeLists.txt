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

project(TTKConfig)

set(HEADER_FILES
  musicconfigdefine.h
  musicconfigobject.h
)

set(SOURCE_FILES
  musicconfigobject.cpp
)

SET(QRC_FILES
  ${MUSIC_UI_DIR}/MusicApp.qrc
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_add_resources(RCC_FILES ${QRC_FILES})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  endif()

  target_link_libraries(${PROJECT_NAME} Qt5::Core)
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_add_resources(RCC_FILES ${QRC_FILES})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${RCC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  endif()

  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY})
endif()
