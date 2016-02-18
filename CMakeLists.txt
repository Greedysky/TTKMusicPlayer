cmake_minimum_required(VERSION 2.8.11)

add_definitions(-DMUSIC_LIBRARY)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC OFF)
# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -fopenmp -Wall")
# Build Type
set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to Release")  
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug)


set(TTK_USE_MULTIPLE_QUERY "false" CACHE STRING "Use multiple query")
set_property(CACHE TTK_USE_MULTIPLE_QUERY PROPERTY STRINGS false true)
if(TTK_USE_MULTIPLE_QUERY VERSION_EQUAL "true")
  message(STATUS "Expected value for TTK use multiple query")
  add_definitions(-DUSE_MULTIPLE_QUERY)
else()
  message(STATUS "Unexpected value for TTK use multiple query")
endif()

set(MUSIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MUSIC_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/TTKCore")
set(MUSIC_CORE_DIR "${MUSIC_LIB_DIR}/musicCore")
set(MUSIC_WIDGET_DIR "${MUSIC_LIB_DIR}/musicWidget")
set(MUSIC_UI_DIR "${MUSIC_LIB_DIR}/musicUi")
set(MUSIC_LOCALSEARCH_DIR "${MUSIC_LIB_DIR}/musicLocalsearch")
set(MUSIC_LRCMANAGER_DIR "${MUSIC_LIB_DIR}/musicLrcmanager")
set(MUSIC_REMOTEWIDGET_DIR "${MUSIC_LIB_DIR}/musicRemotewidget")
set(MUSIC_TOOLSETSWIDGET_DIR "${MUSIC_LIB_DIR}/musicToolsetswidget")
set(MUSIC_TOOLSWIDGET_DIR "${MUSIC_LIB_DIR}/musicToolswidget")
set(MUSIC_USERMANAGER_DIR "${MUSIC_LIB_DIR}/musicUsermanager")
set(MUSIC_VIDEOKITS_DIR "${MUSIC_LIB_DIR}/musicVideokits")

set(MUSIC_SCRIPT_DIR "${MUSIC_DIR}/TTKScript")
set(MUSIC_QRC_DIR "${MUSIC_DIR}/TTKQrc")
set(MUSIC_LANGUAGE_DIR "${MUSIC_DIR}/TTKLanguage")
set(MUSIC_RESOURCE_DIR "${MUSIC_DIR}/TTKResource")


include_directories(${MUSIC_DIR})
include_directories(${MUSIC_LIB_DIR})
include_directories(${MUSIC_CORE_DIR})
include_directories(${MUSIC_CORE_DIR}/network)
include_directories(${MUSIC_LOCALSEARCH_DIR})
include_directories(${MUSIC_LOCALSEARCH_DIR}/core)
include_directories(${MUSIC_LRCMANAGER_DIR})
include_directories(${MUSIC_REMOTEWIDGET_DIR})
include_directories(${MUSIC_TOOLSETSWIDGET_DIR})
include_directories(${MUSIC_TOOLSETSWIDGET_DIR}/core)
include_directories(${MUSIC_TOOLSWIDGET_DIR})
include_directories(${MUSIC_TOOLSWIDGET_DIR}/core)
include_directories(${MUSIC_USERMANAGER_DIR})
include_directories(${MUSIC_USERMANAGER_DIR}/core)
include_directories(${MUSIC_VIDEOKITS_DIR}) 
include_directories(${MUSIC_WIDGET_DIR})
include_directories(${MUSIC_WIDGET_DIR}/core)


set(LIBRARY_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin) 

set(TTK_QT_VERSION "0" CACHE STRING "Expected Qt version")
set_property(CACHE TTK_QT_VERSION PROPERTY STRINGS 4 5)

if(NOT (TTK_QT_VERSION VERSION_EQUAL "4" OR TTK_QT_VERSION VERSION_EQUAL "5"))
  message(FATAL_ERROR "Expected value for TTK_QT_VERSION is either '4' or '5'")
endif()

set(CMAKE_PREFIX_PATH ${QT_INSTALL_DIR} CACHE PATH "Qt install path")

find_program(QT_QMAKE_EXECUTABLE qmake ${QT_INSTALL_BINS})
if(QT_QMAKE_EXECUTABLE)
  message(STATUS "Found qmake executable: " ${QT_QMAKE_EXECUTABLE})
else()
  message(FATAL_ERROR "Could NOT find qmake executable")
endif()


if(TTK_QT_VERSION VERSION_GREATER "4")
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Sql REQUIRED)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Multimedia REQUIRED)
  find_package(Qt5MultimediaWidgets REQUIRED)
  find_package(Qt5Network REQUIRED)
  find_package(Qt5Xml REQUIRED)
  if(WIN32)
    find_package(Qt5WinExtras REQUIRED)
  endif()
else()
  set(minimum_required_qt_version "4.6")
  find_package(Qt4 REQUIRED)
  
  if(QT4_FOUND)
    if("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" VERSION_LESS "${minimum_required_qt_version}")
      message(FATAL_ERROR "error: TTK requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
    endif()

    set(QT_USE_QTCORE ON)
    set(QT_USE_QTGUI ON)
    if(WIN32)
      set(QT_USE_QTMULTIMEDIA ON)
    endif()
    set(QT_USE_QTXML ON)
    set(QT_USE_QTNETWORK ON)
    set(QT_USE_QTSQL ON)
    set(QT_USE_QTSCRIPT ON)
    set(QT_USE_QTXML ON)
    include(${QT_USE_FILE})

    if(WIN32)
      get_filename_component(${QT_LIBRARY_DIR} ${QT_QMAKE_EXECUTABLE} PATH)
    endif()
  else()
    message(FATAL_ERROR "error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
  endif()

else()
  message(FATAL_ERROR "error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
endif()


find_program(QT_LRELEASE_EXECUTABLE lrelease ${QT_INSTALL_BINS})
if(QT_LRELEASE_EXECUTABLE)
  message(STATUS "Found lrelease executable: " ${QT_LRELEASE_EXECUTABLE})
  if(WIN32)
    execute_process(COMMAND ${MUSIC_SCRIPT_DIR}/win.bat ${QT_LRELEASE_EXECUTABLE} ${EXECUTABLE_OUTPUT_PATH})
  else(UNIX)
    execute_process(COMMAND ${MUSIC_SCRIPT_DIR}/linux.sh ${QT_LRELEASE_EXECUTABLE} ${MUSIC_DIR} ${EXECUTABLE_OUTPUT_PATH})
  endif()
else()
  message(FATAL_ERROR "Could NOT find lrelease executable")
endif()


find_library(QMMP_LIBRARY
  NAMES qmmp1
  PATHS ${LIBRARY_OUTPUT_PATH}
)

# qmmp include
if(TTK_QT_VERSION VERSION_GREATER "4")
  include_directories(${MUSIC_DIR}/TTKExtra/Qt5)
else()
  include_directories(${MUSIC_DIR}/TTKExtra/Qt4)
  if(UNIX)
    include_directories(/usr/include/QtMultimediaKit)
    include_directories(/usr/include/QtMobility)
  endif()
endif()


add_subdirectory(TTKCore)
add_subdirectory(TTKApp)
