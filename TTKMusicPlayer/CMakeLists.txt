cmake_minimum_required(VERSION 2.8.11)

project(MusicPlayer)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC OFF)

set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.5.0/5.5/msvc2013")

add_definitions(-MUSIC_LIBRARY)
set(MUSIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MUSIC_Core_DIR "${MUSIC_DIR}/musicCore")
set(MUSIC_Widget_DIR "${MUSIC_DIR}/musicWidget")
set(MUSIC_Ui_DIR "${MUSIC_DIR}/musicUi")
set(MUSIC_Language_DIR "${MUSIC_DIR}/musicLanguage")
set(MUSIC_Localsearch_DIR "${MUSIC_DIR}/musicLocalsearch")
set(MUSIC_Lrcmanager_DIR "${MUSIC_DIR}/musicLrcmanager")
set(MUSIC_QRC_DIR "${MUSIC_DIR}/musicQrc")
set(MUSIC_Remotewidget_DIR "${MUSIC_DIR}/musicRemotewidget")
set(MUSIC_Resource_DIR "${MUSIC_DIR}/musicResource")
set(MUSIC_Toolsetswidget_DIR "${MUSIC_DIR}/musicToolsetswidget")
set(MUSIC_Toolswidget_DIR "${MUSIC_DIR}/musicToolswidget")
set(MUSIC_Usermanager_DIR "${MUSIC_DIR}/musicUsermanager")
set(MUSIC_Videokits_DIR "${MUSIC_DIR}/musicVideokits")


include_directories(${MUSIC_DIR}) 
include_directories(${MUSIC_Core_DIR})
include_directories(${MUSIC_Widget_DIR})
include_directories(${MUSIC_Localsearch_DIR})
include_directories(${MUSIC_Lrcmanager_DIR})
include_directories(${MUSIC_Remotewidget_DIR})
include_directories(${MUSIC_Toolsetswidget_DIR})
include_directories(${MUSIC_Toolswidget_DIR}) 
include_directories(${MUSIC_Usermanager_DIR})
include_directories(${MUSIC_Videokits_DIR}) 


find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Sql)
find_package(Qt5Widgets)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)
find_package(Qt5Network)
find_package(Qt5Xml)
find_package(Qt5WinExtras)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(MUSIC_HEADERS
    ${MUSIC_DIR}/*.h
    ${MUSIC_Core_DIR}/*.h
    ${MUSIC_Core_DIR}/network/*.h
    ${MUSIC_Widget_DIR}/*.h
    ${MUSIC_Localsearch_DIR}/*.h
    ${MUSIC_Localsearch_DIR}/core/*.h
    ${MUSIC_Lrcmanager_DIR}/*.h
    ${MUSIC_Remotewidget_DIR}/*.h
    ${MUSIC_Toolsetswidget_DIR}/*.h
    ${MUSIC_Toolsetswidget_DIR}/core/*.h
    ${MUSIC_Toolswidget_DIR}/*.h
    ${MUSIC_Toolswidget_DIR}/core/*.h
    ${MUSIC_Usermanager_DIR}/*.h
    ${MUSIC_Usermanager_DIR}/core/*.h
    ${MUSIC_Videokits_DIR}/*.h
  )
  
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CACHEFILE_DIR}/bin)  

add_subdirectory(core)
add_subdirectory(ui)

add_executable(MusicPlayer musicapplicationmain.cpp )
add_dependencies(MusicPlayer MusicWidget MusicCore)
target_link_libraries(MusicPlayer MusicWidget MusicCore)

qt5_use_modules(MusicPlayer Core Network Sql Gui Widgets Multimedia MultimediaWidgets WinExtras)