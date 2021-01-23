cmake_minimum_required(VERSION 2.8.11)

add_definitions(-DTTK_LIBRARY)
add_definitions(-DQMMP_LIBRARY)

set(TTK_MAJOR_VERSION "2.13.4")
set(TTK_VERSION ${TTK_MAJOR_VERSION}.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC OFF)
# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread -fopenmp -Wall")
# Build Type
set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to Release")  
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug)

option(TTK_BUILD_SHARED "TTK BUILD SHARED LIBS" ON)
if(TTK_BUILD_SHARED)
  message(STATUS "Message TTK build by shared link")
else()
  message(STATUS "Message TTK build by static link")
endif()

set(MUSIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MUSIC_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/TTKModule")
set(MUSIC_LIB_CORE_DIR "${MUSIC_LIB_DIR}/TTKCore")
set(MUSIC_LIB_WIDGET_DIR "${MUSIC_LIB_DIR}/TTKWidget")

include_directories(
    ${MUSIC_DIR}
    ${MUSIC_LIB_DIR}
)


set(MUSIC_CORE_DIR "${MUSIC_LIB_CORE_DIR}/musicCoreKits")
set(MUSIC_CORE_LRCMANAGER_DIR "${MUSIC_LIB_CORE_DIR}/musicLrcKits")
set(MUSIC_CORE_NETWORK_DIR "${MUSIC_LIB_CORE_DIR}/musicNetworkKits")
set(MUSIC_CORE_PLAYLIST_DIR "${MUSIC_LIB_CORE_DIR}/musicPlaylistKits")
set(MUSIC_CORE_LOCALSEARCH_DIR "${MUSIC_LIB_CORE_DIR}/musicSearchKits")
set(MUSIC_CORE_TOOLSWIDGET_DIR "${MUSIC_LIB_CORE_DIR}/musicToolsKits")
set(MUSIC_CORE_TOOLSETSWIDGET_DIR "${MUSIC_LIB_CORE_DIR}/musicToolsSetsKits")
set(MUSIC_CORE_USERMANAGER_DIR "${MUSIC_LIB_CORE_DIR}/musicUserKits")
set(MUSIC_CORE_UTILS_DIR "${MUSIC_LIB_CORE_DIR}/musicUtilsKits")
set(MUSIC_CORE_VIDEOKITS_DIR "${MUSIC_LIB_CORE_DIR}/musicVideoKits")

include_directories(
    ${MUSIC_CORE_DIR}
    ${MUSIC_CORE_LRCMANAGER_DIR}
    ${MUSIC_CORE_NETWORK_DIR}
    ${MUSIC_CORE_NETWORK_DIR}/common
    ${MUSIC_CORE_NETWORK_DIR}/music/bd
    ${MUSIC_CORE_NETWORK_DIR}/music/kg
    ${MUSIC_CORE_NETWORK_DIR}/music/kw
    ${MUSIC_CORE_NETWORK_DIR}/music/mg
    ${MUSIC_CORE_NETWORK_DIR}/music/qq
    ${MUSIC_CORE_NETWORK_DIR}/music/wy
    ${MUSIC_CORE_NETWORK_DIR}/music/xm
    ${MUSIC_CORE_NETWORK_DIR}/radio/dj
    ${MUSIC_CORE_NETWORK_DIR}/radio/mv
    ${MUSIC_CORE_NETWORK_DIR}/radio/fm
    ${MUSIC_CORE_PLAYLIST_DIR}
    ${MUSIC_CORE_LOCALSEARCH_DIR}
    ${MUSIC_CORE_TOOLSWIDGET_DIR}
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}
    ${MUSIC_CORE_USERMANAGER_DIR}
    ${MUSIC_CORE_UTILS_DIR}
    ${MUSIC_CORE_VIDEOKITS_DIR}
)


set(MUSIC_WIDGET_DIR "${MUSIC_LIB_WIDGET_DIR}/musicWidgetKits")
set(MUSIC_WIDGET_CORE_DIR "${MUSIC_LIB_WIDGET_DIR}/musicCoreKits")
set(MUSIC_WIDGET_LRCMANAGER_DIR "${MUSIC_LIB_WIDGET_DIR}/musicLrcKits")
set(MUSIC_WIDGET_REMOTEWIDGET_DIR "${MUSIC_LIB_WIDGET_DIR}/musicRemoteKits")
set(MUSIC_WIDGET_LOCALSEARCH_DIR "${MUSIC_LIB_WIDGET_DIR}/musicSearchKits")
set(MUSIC_WIDGET_TOOLSWIDGET_DIR "${MUSIC_LIB_WIDGET_DIR}/musicToolsKits")
set(MUSIC_WIDGET_TOOLSETSWIDGET_DIR "${MUSIC_LIB_WIDGET_DIR}/musicToolsSetsKits")
set(MUSIC_WIDGET_UI_DIR "${MUSIC_LIB_WIDGET_DIR}/musicUiKits")
set(MUSIC_WIDGET_USERMANAGER_DIR "${MUSIC_LIB_WIDGET_DIR}/musicUserKits")
set(MUSIC_WIDGET_VIDEOKITS_DIR "${MUSIC_LIB_WIDGET_DIR}/musicVideoKits")

include_directories(
    ${MUSIC_WIDGET_DIR}
    ${MUSIC_WIDGET_CORE_DIR}
    ${MUSIC_WIDGET_LRCMANAGER_DIR}
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}
    ${MUSIC_WIDGET_UI_DIR}
    ${MUSIC_WIDGET_USERMANAGER_DIR}
    ${MUSIC_WIDGET_VIDEOKITS_DIR}
)

set(MUSIC_CONFIG_DIR "${MUSIC_DIR}/TTKConfig")
set(MUSIC_UI_DIR "${MUSIC_DIR}/TTKUi")
set(MUSIC_THIRDPARTY_DIR "${MUSIC_DIR}/TTKThirdParty")
set(MUSIC_TTKSERVICE_DIR "${MUSIC_DIR}/TTKService")

set(MUSIC_SCRIPT_DIR "${MUSIC_DIR}/TTKScript")
set(MUSIC_LANGUAGE_DIR "${MUSIC_DIR}/TTKLanguage")
set(MUSIC_RESOURCE_DIR "${MUSIC_DIR}/TTKResource")

include_directories(
    ${MUSIC_THIRDPARTY_DIR}
    ${MUSIC_THIRDPARTY_DIR}/Zlib
    ${MUSIC_THIRDPARTY_DIR}/TTKWatcher
    ${MUSIC_THIRDPARTY_DIR}/TTKDumper
    ${MUSIC_THIRDPARTY_DIR}/TTKExtras
)

set(MUSIC_INSTALL_DIR "${CMAKE_CACHEFILE_DIR}/bin")  
set(LIBRARY_OUTPUT_PATH ${MUSIC_INSTALL_DIR}/${TTK_VERSION})
set(EXECUTABLE_OUTPUT_PATH ${MUSIC_INSTALL_DIR}/${TTK_VERSION}) 

set(OPENSSL_DIR "" CACHE PATH "Openssl install path")  

set(TTK_QT_VERSION "0" CACHE STRING "Expected Qt version")
set_property(CACHE TTK_QT_VERSION PROPERTY STRINGS 4 5)

if(NOT (TTK_QT_VERSION STREQUAL "4" OR TTK_QT_VERSION STREQUAL "5"))
  message(FATAL_ERROR "Message Expected value for TTK_QT_VERSION is either '4' or '5'")
endif()

set(CMAKE_PREFIX_PATH ${QT_INSTALL_DIR} CACHE PATH "Qt install path")

find_program(QT_QMAKE_EXECUTABLE qmake ${QT_INSTALL_BINS})
if(QT_QMAKE_EXECUTABLE)
  message(STATUS "Message Found qmake executable: " ${QT_QMAKE_EXECUTABLE})
else()
  message(FATAL_ERROR "Message Could NOT find qmake executable")
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
  find_package(Qt5OpenGL REQUIRED)
  if(WIN32)
    find_package(Qt5WinExtras REQUIRED)
  else(UNIX)
    find_package(Qt5X11Extras REQUIRED)
  endif()
else()
  set(minimum_required_qt_version "4.8")
  find_package(Qt4 REQUIRED)
  
  if(QT4_FOUND)
    if("${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}" VERSION_LESS "${minimum_required_qt_version}")
      message(FATAL_ERROR "Message error: TTK requires Qt >= ${minimum_required_qt_version} -- you cannot use Qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")
    endif()

    set(QT_USE_QTCORE ON)
    set(QT_USE_QTGUI ON)
    if(WIN32)
      set(QT_USE_QTMULTIMEDIA ON)
    endif()
    set(QT_USE_QTXML ON)
    set(QT_USE_QTNETWORK ON)
    set(QT_USE_QTSQL ON)
    set(QT_USE_QTXML ON)
    set(QT_USE_QTOPENGL ON)
    include(${QT_USE_FILE})

    if(WIN32)
      get_filename_component(${QT_LIBRARY_DIR} ${QT_QMAKE_EXECUTABLE} PATH)
    endif()
  else()
    message(FATAL_ERROR "Message error: Qt4 was not found on your system. You probably need to set the QT_QMAKE_EXECUTABLE variable")
  endif()
endif()


find_program(QT_LRELEASE_EXECUTABLE lrelease ${QT_INSTALL_BINS})
if(QT_LRELEASE_EXECUTABLE)
  message(STATUS "Message Found lrelease executable: " ${QT_LRELEASE_EXECUTABLE})
  if(WIN32)
    execute_process(COMMAND ${MUSIC_SCRIPT_DIR}/ts_win.bat ${QT_LRELEASE_EXECUTABLE} ${EXECUTABLE_OUTPUT_PATH})
  else(UNIX)
    execute_process(COMMAND ${MUSIC_SCRIPT_DIR}/ts_linux.sh ${QT_LRELEASE_EXECUTABLE} ${MUSIC_DIR} ${EXECUTABLE_OUTPUT_PATH})
  endif()
else()
  message(FATAL_ERROR "Message Could NOT find lrelease executable")
endif()

# copy run resource
if(UNIX)
  execute_process(COMMAND ${MUSIC_SCRIPT_DIR}/resource.sh ${MUSIC_RESOURCE_DIR} ${EXECUTABLE_OUTPUT_PATH})
endif()

find_library(TTK_QMMP_LIBRARY
  NAMES TTKqmmp
  PATHS ${LIBRARY_OUTPUT_PATH}
)

# qmmp include
include_directories(${MUSIC_DIR}/TTKExtra)
if(NOT (TTK_QT_VERSION VERSION_GREATER "4"))
  if(UNIX)
    include_directories(
        /usr/include/QtMultimediaKit
        /usr/include/QtMobility
    )
  endif()
endif()

add_subdirectory(TTKConfig)
add_subdirectory(TTKUi)
add_subdirectory(TTKThirdParty)
add_subdirectory(TTKModule)
add_subdirectory(TTKService)
add_subdirectory(TTKRun)

install(FILES "${MUSIC_SCRIPT_DIR}/deploy/ttkmusicplayer.appdata.xml" DESTINATION "${MUSIC_INSTALL_DIR}/metainfo")
