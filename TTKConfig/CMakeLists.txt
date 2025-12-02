# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2026 Greedysky Studio
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

project(TTKConfig)

set(HEADER_FILES
  musicconfigdefine.h
  musicconfigmodule.h
)

set(SOURCE_FILES
  musicconfigmodule.cpp
)

SET(QRC_FILES
  ${TTK_UI_DIR}/TTKApp.qrc
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_EQUAL "4")
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_add_resources(RCC_FILES ${QRC_FILES})

  list(APPEND QT_LINK_LIBS ${QT_QTCORE_LIBRARY})
elseif(TTK_QT_VERSION VERSION_EQUAL "5")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_add_resources(RCC_FILES ${QRC_FILES})

  list(APPEND QT_LINK_LIBS Qt5::Core)
elseif(TTK_QT_VERSION VERSION_EQUAL "6")
  qt6_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt6_add_resources(RCC_FILES ${QRC_FILES})

  list(APPEND QT_LINK_LIBS Qt6::Core)
endif()

if(TTK_BUILD_SHARED)
  add_library(${PROJECT_NAME} SHARED ${RCC_FILES} ${HEADER_FILES} ${SOURCE_FILES} ${MOC_FILES})
else()
  add_library(${PROJECT_NAME} STATIC ${RCC_FILES} ${HEADER_FILES} ${SOURCE_FILES} ${MOC_FILES})
endif()
target_link_libraries(${PROJECT_NAME} ${QT_LINK_LIBS})
