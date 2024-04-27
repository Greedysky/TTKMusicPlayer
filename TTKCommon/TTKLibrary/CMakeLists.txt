# ***************************************************************************
# * This file is part of the TTK Library Module project
# * Copyright (C) 2015 - 2024 Greedysky Studio
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

cmake_minimum_required(VERSION 3.0.0)

project(TTKLibrary)

set(HEADER_FILES
  ttkabstractbufferinterface.h
  ttkabstractmovedialog.h
  ttkabstractmoveresizewidget.h
  ttkabstractmovewidget.h
  ttkabstractnetwork.h
  ttkabstractresizeinterface.h
  ttkabstracttablewidget.h
  ttkabstractthread.h
  ttkabstractxml.h
  ttkany.h
  ttkclickedgroup.h
  ttkclickedlabel.h
  ttkclickedslider.h
  ttkcommandline.h
  ttkconcurrent.h
  ttkconcurrentqueue.h
  ttkcryptographichash.h
  ttkdefer.h
  ttkdesktopwrapper.h
  ttkdispatchmanager.h
  ttkfileassociation.h
  ttkfileinterface.h
  ttkglobalhelper.h
  ttkitemdelegate.h
  ttklibrary.h
  ttklibraryversion.h
  ttklogoutput.h
  ttkplatformsystem.h
  ttksemaphoreloop.h
  ttksuperenum.h
  ttktabbutton.h
  ttkthemelinelabel.h
  ttktime.h
  ttktoastlabel.h
  ttkunsortedmap.h
)

set(SOURCE_FILES
  ttkabstractmovedialog.cpp
  ttkabstractmoveresizewidget.cpp
  ttkabstractmovewidget.cpp
  ttkabstractnetwork.cpp
  ttkabstracttablewidget.cpp
  ttkabstractthread.cpp
  ttkabstractxml.cpp
  ttkany.cpp
  ttkclickedgroup.cpp
  ttkclickedlabel.cpp
  ttkclickedslider.cpp
  ttkcommandline.cpp
  ttkcryptographichash.cpp
  ttkdesktopwrapper.cpp
  ttkdispatchmanager.cpp
  ttkfileassociation.cpp
  ttkglobalhelper.cpp
  ttkitemdelegate.cpp
  ttklogoutput.cpp
  ttkplatformsystem.cpp
  ttksemaphoreloop.cpp
  ttksuperenum.cpp
  ttktabbutton.cpp
  ttkthemelinelabel.cpp
  ttktime.cpp
  ttktoastlabel.cpp
)
  
SET(QRC_FILES
  ${PROJECT_NAME}.qrc
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
endif()

if(TTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_add_resources(RCC_FILES ${QRC_FILES})

  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${MOC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES}  ${MOC_FILES} ${HEADER_FILES})
  endif()
  
  set(QT5_LIBS Qt5::Core Qt5::Xml Qt5::Network Qt5::Gui Qt5::Widgets)
  target_link_libraries(${PROJECT_NAME} ${QT5_LIBS})
else()
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_add_resources(RCC_FILES ${QRC_FILES})
  
  if(TTK_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${MOC_FILES} ${RCC_FILES} ${HEADER_FILES})
  else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${MOC_FILES} ${RCC_FILES} ${HEADER_FILES})
  endif()
  
  set(QT4_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTGUI_LIBRARY})
  target_link_libraries(${PROJECT_NAME} ${QT4_LIBS})
endif()
