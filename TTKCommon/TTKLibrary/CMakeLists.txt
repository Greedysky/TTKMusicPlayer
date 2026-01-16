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

project(TTKLibrary)

set(TTK_CTL_MAJOR_VERSION 1)
set(TTK_CTL_MINOR_VERSION 4)
set(TTK_CTL_PATCH_VERSION 5)
set(TTK_CTL_VERSION "${TTK_CTL_MAJOR_VERSION}.${TTK_CTL_MINOR_VERSION}.${TTK_CTL_PATCH_VERSION}.0")

set(HEADER_FILES
  ${TTK_COMMON_LIB_DIR}/ttkabstractbufferinterface.h
  ${TTK_COMMON_LIB_DIR}/ttkabstractmovedialog.h
  ${TTK_COMMON_LIB_DIR}/ttkabstractmoveresizewidget.h
  ${TTK_COMMON_LIB_DIR}/ttkabstractmovewidget.h
  ${TTK_COMMON_LIB_DIR}/ttkabstractnetwork.h
  ${TTK_COMMON_LIB_DIR}/ttkabstractresizeinterface.h
  ${TTK_COMMON_LIB_DIR}/ttkabstracttablewidget.h
  ${TTK_COMMON_LIB_DIR}/ttkabstractthread.h
  ${TTK_COMMON_LIB_DIR}/ttkabstractxml.h
  ${TTK_COMMON_LIB_DIR}/ttkany.h
  ${TTK_COMMON_LIB_DIR}/ttkclickedgroup.h
  ${TTK_COMMON_LIB_DIR}/ttkclickedlabel.h
  ${TTK_COMMON_LIB_DIR}/ttkclickedslider.h
  ${TTK_COMMON_LIB_DIR}/ttkcommandline.h
  ${TTK_COMMON_LIB_DIR}/ttkconcurrent.h
  ${TTK_COMMON_LIB_DIR}/ttkconcurrentdata.h
  ${TTK_COMMON_LIB_DIR}/ttkconcurrentqueue.h
  ${TTK_COMMON_LIB_DIR}/ttkcryptographichash.h
  ${TTK_COMMON_LIB_DIR}/ttkdefer.h
  ${TTK_COMMON_LIB_DIR}/ttkdesktopscreen.h
  ${TTK_COMMON_LIB_DIR}/ttkdispatchmanager.h
  ${TTK_COMMON_LIB_DIR}/ttkdumper.h
  ${TTK_COMMON_LIB_DIR}/ttkeventloop.h
  ${TTK_COMMON_LIB_DIR}/ttkfileassociation.h
  ${TTK_COMMON_LIB_DIR}/ttkfileinterface.h
  ${TTK_COMMON_LIB_DIR}/ttkfunctor.h
  ${TTK_COMMON_LIB_DIR}/ttkglobalinterface.h
  ${TTK_COMMON_LIB_DIR}/ttkinitialization.h
  ${TTK_COMMON_LIB_DIR}/ttkitemdelegate.h
  ${TTK_COMMON_LIB_DIR}/ttklexicalcast.h
  ${TTK_COMMON_LIB_DIR}/ttklibrary.h
  ${TTK_COMMON_LIB_DIR}/ttklibraryversion.h
  ${TTK_COMMON_LIB_DIR}/ttklogoutput.h
  ${TTK_COMMON_LIB_DIR}/ttkoptional.h
  ${TTK_COMMON_LIB_DIR}/ttkplatformsystem.h
  ${TTK_COMMON_LIB_DIR}/ttkprocess.h
  ${TTK_COMMON_LIB_DIR}/ttkregularexpression.h
  ${TTK_COMMON_LIB_DIR}/ttksmartptr.h
  ${TTK_COMMON_LIB_DIR}/ttkspinlock.h
  ${TTK_COMMON_LIB_DIR}/ttkstringliterals.h
  ${TTK_COMMON_LIB_DIR}/ttkstringview.h
  ${TTK_COMMON_LIB_DIR}/ttksuperenum.h
  ${TTK_COMMON_LIB_DIR}/ttktabbutton.h
  ${TTK_COMMON_LIB_DIR}/ttkthemelinelabel.h
  ${TTK_COMMON_LIB_DIR}/ttktime.h
  ${TTK_COMMON_LIB_DIR}/ttktoastlabel.h
  ${TTK_COMMON_LIB_DIR}/ttktree.h
  ${TTK_COMMON_LIB_DIR}/ttkunsortedmap.h
  ${TTK_COMMON_LIB_DIR}/ttkvariant.h
)

set_property(GLOBAL PROPERTY TTK_LIBRARY_KITS_HEADER_FILES ${HEADER_FILES})

set(SOURCE_FILES
  ${TTK_COMMON_LIB_DIR}/ttkabstractmovedialog.cpp
  ${TTK_COMMON_LIB_DIR}/ttkabstractmoveresizewidget.cpp
  ${TTK_COMMON_LIB_DIR}/ttkabstractmovewidget.cpp
  ${TTK_COMMON_LIB_DIR}/ttkabstractnetwork.cpp
  ${TTK_COMMON_LIB_DIR}/ttkabstracttablewidget.cpp
  ${TTK_COMMON_LIB_DIR}/ttkabstractthread.cpp
  ${TTK_COMMON_LIB_DIR}/ttkabstractxml.cpp
  ${TTK_COMMON_LIB_DIR}/ttkany.cpp
  ${TTK_COMMON_LIB_DIR}/ttkclickedgroup.cpp
  ${TTK_COMMON_LIB_DIR}/ttkclickedlabel.cpp
  ${TTK_COMMON_LIB_DIR}/ttkclickedslider.cpp
  ${TTK_COMMON_LIB_DIR}/ttkcommandline.cpp
  ${TTK_COMMON_LIB_DIR}/ttkcryptographichash.cpp
  ${TTK_COMMON_LIB_DIR}/ttkdesktopscreen.cpp
  ${TTK_COMMON_LIB_DIR}/ttkdispatchmanager.cpp
  ${TTK_COMMON_LIB_DIR}/ttkdumper.cpp
  ${TTK_COMMON_LIB_DIR}/ttkeventloop.cpp
  ${TTK_COMMON_LIB_DIR}/ttkfileassociation.cpp
  ${TTK_COMMON_LIB_DIR}/ttkglobalinterface.cpp
  ${TTK_COMMON_LIB_DIR}/ttkinitialization.cpp
  ${TTK_COMMON_LIB_DIR}/ttkitemdelegate.cpp
  ${TTK_COMMON_LIB_DIR}/ttklogoutput.cpp
  ${TTK_COMMON_LIB_DIR}/ttkplatformsystem.cpp
  ${TTK_COMMON_LIB_DIR}/ttkprocess.cpp
  ${TTK_COMMON_LIB_DIR}/ttkregularexpression.cpp
  ${TTK_COMMON_LIB_DIR}/ttksuperenum.cpp
  ${TTK_COMMON_LIB_DIR}/ttktabbutton.cpp
  ${TTK_COMMON_LIB_DIR}/ttkthemelinelabel.cpp
  ${TTK_COMMON_LIB_DIR}/ttktime.cpp
  ${TTK_COMMON_LIB_DIR}/ttktoastlabel.cpp
)

SET(QRC_FILES
  ${PROJECT_NAME}.qrc
)

if(WIN32)
  list(APPEND SOURCE_FILES ${PROJECT_NAME}.rc)
  list(APPEND QT_LINK_LIBS gdi32 psapi)
endif()

if(TTK_QT_VERSION VERSION_EQUAL "4")
  qt4_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt4_add_resources(RCC_FILES ${QRC_FILES})

  list(APPEND QT_LINK_LIBS ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTGUI_LIBRARY})
elseif(TTK_QT_VERSION VERSION_EQUAL "5")
  qt5_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt5_add_resources(RCC_FILES ${QRC_FILES})

  list(APPEND QT_LINK_LIBS Qt5::Core Qt5::Xml Qt5::Network Qt5::Gui Qt5::Widgets)
elseif(TTK_QT_VERSION VERSION_EQUAL "6")
  qt6_wrap_cpp(MOC_FILES ${HEADER_FILES})
  qt6_add_resources(RCC_FILES ${QRC_FILES})

  list(APPEND QT_LINK_LIBS Qt6::Core Qt6::Xml Qt6::Network Qt6::Gui Qt6::Widgets)
endif()

if(TTK_BUILD_SHARED)
  add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})
else()
  add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${MOC_FILES})
endif()
target_link_libraries(${PROJECT_NAME} ${QT_LINK_LIBS})

# install CTL module
if(TTK_INSTALL_CTL)
  set(TTK_CTL_PATH "${TTK_INSTALL_DIR}/${PROJECT_NAME}/lib")
  if(MSVC)
    set(TTK_CTL_MODULE "lib${PROJECT_NAME}.lib")
  elseif(WIN32)
    set(TTK_CTL_MODULE "lib${PROJECT_NAME}.a")
  elseif(APPLE)
    set(TTK_CTL_MODULE "lib${PROJECT_NAME}.dylib")
  else()
    set(TTK_CTL_MODULE "lib${PROJECT_NAME}.so")
  endif()

  file(GLOB_RECURSE INSTALL_HEADERS "${TTK_COMMON_BASE_DIR}/*.h" "${TTK_COMMON_LIB_DIR}/*.h")
  install(FILES ${INSTALL_HEADERS} DESTINATION "${TTK_INSTALL_DIR}/${PROJECT_NAME}/include")
  install(FILES ${LIBRARY_OUTPUT_PATH}/${TTK_CTL_MODULE} DESTINATION ${TTK_CTL_PATH})
  # configure ttk config file
  install(FILES ${TTK_GENERATE_DIR}/${PROJECT_NAME}.pc DESTINATION ${TTK_CTL_PATH}/pkgconfig)
  install(FILES ${TTK_GENERATE_DIR}/${PROJECT_NAME}Config.cmake DESTINATION ${TTK_CTL_PATH}/cmake/${PROJECT_NAME})
endif()

# configure CTL files
configure_file(cmake/${PROJECT_NAME}.pc.in ${TTK_GENERATE_DIR}/${PROJECT_NAME}.pc @ONLY)
configure_file(cmake/${PROJECT_NAME}Config.cmake.in ${TTK_GENERATE_DIR}/${PROJECT_NAME}Config.cmake @ONLY)
