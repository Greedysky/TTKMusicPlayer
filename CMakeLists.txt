cmake_minimum_required(VERSION 2.8.11)

ADD_DEFINITIONS(-DMUSIC_LIBRARY)

set(MUSIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MUSIC_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/TTKMusicPlayer")
set(MUSIC_Core_DIR "${MUSIC_LIB_DIR}/musicCore")
set(MUSIC_Widget_DIR "${MUSIC_LIB_DIR}/musicWidget")
set(MUSIC_Ui_DIR "${MUSIC_LIB_DIR}/musicUi")
set(MUSIC_Language_DIR "${MUSIC_DIR}/TTKLanguage")
set(MUSIC_Localsearch_DIR "${MUSIC_LIB_DIR}/musicLocalsearch")
set(MUSIC_Lrcmanager_DIR "${MUSIC_LIB_DIR}/musicLrcmanager")
set(MUSIC_QRC_DIR "${MUSIC_LIB_DIR}/musicQrc")
set(MUSIC_Remotewidget_DIR "${MUSIC_LIB_DIR}/musicRemotewidget")
set(MUSIC_Resource_DIR "${MUSIC_DIR}/TTKResource")
set(MUSIC_Toolsetswidget_DIR "${MUSIC_LIB_DIR}/musicToolsetswidget")
set(MUSIC_Toolswidget_DIR "${MUSIC_LIB_DIR}/musicToolswidget")
set(MUSIC_Usermanager_DIR "${MUSIC_LIB_DIR}/musicUsermanager")
set(MUSIC_Videokits_DIR "${MUSIC_LIB_DIR}/musicVideokits")

include_directories(${MUSIC_DIR})
include_directories(${MUSIC_LIB_DIR})
include_directories(${MUSIC_Core_DIR})
include_directories(${MUSIC_Core_DIR}/network)
include_directories(${MUSIC_Localsearch_DIR})
include_directories(${MUSIC_Localsearch_DIR}/core)
include_directories(${MUSIC_Lrcmanager_DIR})
include_directories(${MUSIC_Remotewidget_DIR})
include_directories(${MUSIC_Toolsetswidget_DIR})
include_directories(${MUSIC_Toolsetswidget_DIR}/core)
include_directories(${MUSIC_Toolswidget_DIR})
include_directories(${MUSIC_Toolswidget_DIR}/core)
include_directories(${MUSIC_Usermanager_DIR})
include_directories(${MUSIC_Usermanager_DIR}/core)
include_directories(${MUSIC_Videokits_DIR}) 
include_directories(${MUSIC_Widget_DIR})
include_directories(${MUSIC_Widget_DIR}/core)
# qmmp include
include_directories(${MUSIC_DIR}/TTKExtra/Qt5)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin) 

FIND_PROGRAM(QT_QMAKE_EXECUTABLE qmake ${QT_INSTALL_BINS})
IF(QT_QMAKE_EXECUTABLE)
  MESSAGE(STATUS "Found qmake executable: " ${QT_QMAKE_EXECUTABLE})
ELSE(QT_QMAKE_EXECUTABLE)
  MESSAGE(FATAL_ERROR "Could NOT find qmake executable")
ENDIF(QT_QMAKE_EXECUTABLE)

FIND_PROGRAM(QT_LRELEASE_EXECUTABLE lrelease ${QT_INSTALL_BINS})
IF(QT_LRELEASE_EXECUTABLE)
  MESSAGE(STATUS "Found lrelease executable: " ${QT_LRELEASE_EXECUTABLE})
ELSE(QT_LRELEASE_EXECUTABLE)
  MESSAGE(FATAL_ERROR "Could NOT find lrelease executable")
ENDIF(QT_LRELEASE_EXECUTABLE)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Sql)
find_package(Qt5Widgets)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)
find_package(Qt5Network)
find_package(Qt5Xml)
find_package(Qt5WinExtras)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC OFF)
# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -fopenmp -Wall")
set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to Release")  

FIND_LIBRARY(QMMP_LIBRARY
  NAMES qmmp1
  PATHS ${LIBRARY_OUTPUT_PATH}
)

add_subdirectory(TTKMusicPlayer)
add_subdirectory(TTKApp)