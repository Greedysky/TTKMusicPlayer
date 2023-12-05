# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2023 Greedysky Studio
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
  toolsskin.h
)

set(SOURCE_FILES
  toolsskin.cpp
  main.cpp
)
  
set(UI_FILES
  toolsskin.ui
)
  
if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_wrap_ui(UIC_FILES ${UI_FILES})
  
  add_executable(${PROJECT_NAME} ${UIC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets TTKCore)
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_wrap_ui(UIC_FILES ${UI_FILES})
  
  add_executable(${PROJECT_NAME} ${UIC_FILES} ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  target_link_libraries(${PROJECT_NAME} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} TTKCore)
endif()
