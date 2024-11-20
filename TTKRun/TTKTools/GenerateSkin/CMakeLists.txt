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

project(GenerateSkin)

set(HEADER_FILES
  toolskin.h
)

set(SOURCE_FILES
  toolskin.cpp
  main.cpp
)

set(UI_FILES
  toolskin.ui
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

set(QT_LINK_LIBS TTKCore)

if(TTK_QT_VERSION VERSION_EQUAL "4")
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_wrap_ui(UIC_FILES ${UI_FILES})

  list(APPEND QT_LINK_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
elseif(TTK_QT_VERSION VERSION_EQUAL "5")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_wrap_ui(UIC_FILES ${UI_FILES})

  list(APPEND QT_LINK_LIBS Qt5::Core Qt5::Gui Qt5::Widgets)
elseif(TTK_QT_VERSION VERSION_EQUAL "6")
  qt6_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt6_wrap_ui(UIC_FILES ${UI_FILES})

  list(APPEND QT_LINK_LIBS Qt6::Core Qt6::Gui Qt6::Widgets)
endif()

add_executable(${PROJECT_NAME} ${UIC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${QT_LINK_LIBS})
