cmake_minimum_required(VERSION 2.8.11)

project(MusicCore)

set(MUSIC_Core_HEADERS
    ${MUSIC_Core_DIR}/*.h
    ${MUSIC_Network_DIR}/*.h
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchrecordobject.h
    ${MUSIC_Toolswidget_DIR}/musicmydownloadrecordobject.h
    ${MUSIC_Toolswidget_DIR}/musicwebradiodatebase.h
    ${MUSIC_Usermanager_DIR}/musicuserconfigmanager.h
    ${MUSIC_Usermanager_DIR}/musicusermodel.h
  )

set(MUSIC_Core_SOURCES
    ${MUSIC_Core_DIR}/*.cpp
    ${MUSIC_Network_DIR}/*.cpp
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchrecordobject.cpp
    ${MUSIC_Toolswidget_DIR}/musicmydownloadrecordobject.cpp
    ${MUSIC_Toolswidget_DIR}/musicwebradiodatebase.cpp
    ${MUSIC_Usermanager_DIR}/musicuserconfigmanager.cpp
    ${MUSIC_Usermanager_DIR}/musicusermodel.cpp
  )

qt5_wrap_cpp(MUSIC_Core_SOURCES_MOC ${MUSIC_Core_HEADERS})

FIND_LIBRARY(LIBZPLAY_LIBRARY
  NAMES libzplay
  PATHS ${MUSIC_DIR}/MPlugins
 )

add_library(MusicCore SHARED ${MUSIC_Core_HEADERS} ${MUSIC_Core_SOURCES} ${MUSIC_Core_SOURCES_MOC} ${LIBZPLAY_LIBRARY})

qt5_use_modules(MusicCore Core Network Sql Gui Xml)