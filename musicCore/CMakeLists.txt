cmake_minimum_required(VERSION 2.8.11)

project(MusicCore)

set(MUSIC_Core_HEADERS
    ${MUSIC_Core_DIR}/*.h
    ${MUSIC_Core_DIR}/network/*.h
    ${MUSIC_Localsearch_DIR}/core/*.h
    ${MUSIC_Toolswidget_DIR}/core/*.h
    ${MUSIC_Toolsetswidget_DIR}/core/*.h
    ${MUSIC_Usermanager_DIR}/core/*.h
  )

set(MUSIC_Core_SOURCES
    ${MUSIC_Core_DIR}/*.cpp
    ${MUSIC_Core_DIR}/network/*.cpp
    ${MUSIC_Localsearch_DIR}/core/*.cpp
    ${MUSIC_Toolswidget_DIR}/core/*.cpp
    ${MUSIC_Toolsetswidget_DIR}/core/*.cpp
    ${MUSIC_Usermanager_DIR}/core/*.cpp
  )

qt5_wrap_cpp(MUSIC_Core_SOURCES_MOC ${MUSIC_Core_HEADERS})

FIND_LIBRARY(LIBZPLAY_LIBRARY
  NAMES libzplay
  PATHS ${MUSIC_DIR}/MPlugins
 )

add_library(MusicCore SHARED ${MUSIC_Core_HEADERS} ${MUSIC_Core_SOURCES} ${MUSIC_Core_SOURCES_MOC} ${LIBZPLAY_LIBRARY})

qt5_use_modules(MusicCore Core Network Sql Gui Xml)