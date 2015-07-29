cmake_minimum_required(VERSION 2.8.11)

project(MusicCore)

set(MUSIC_Core_HEADERS
    musicbgthemedownload.h
    musicbgthememanager.h
    musicdata2downloadthread.h
    musicdatadownloadthread.h
    musicdesktopwallpaperthread.h
    musicdownloadquerythread.h
    musicdownloadthreadabstract.h
    musicequalizer.h
    musicfileinformation.h
    musiclibexportglobal.h
    musiclibzplay.h
    musiclocalsongsmanagerthread.h
    musiclogger.h
    musicobject.h
    musicplayer.h
    musicplaylist.h
    musicsingletone.h
    musicsonginformation.h
    musictextdownloadthread.h
    musictimerautoobject.h
    musicversion.h
    musicxmlabstract.h
    musicxmlconfigmanager.h
    ${MUSIC_Localsearch_DIR}/musiclocalsongsearchrecordobject.h
    ${MUSIC_Toolswidget_DIR}/musicmydownloadrecordobject.h
    ${MUSIC_Toolswidget_DIR}/musicwebradiodatebase.h
    ${MUSIC_Usermanager_DIR}/musicuserconfigmanager.h
    ${MUSIC_Usermanager_DIR}/musicusermodel.h
  )

set(MUSIC_Core_SOURCES
    musicbgthemedownload.cpp
    musicbgthememanager.cpp
    musicdata2downloadthread.cpp
    musicdatadownloadthread.cpp
    musicdesktopwallpaperthread.cpp
    musicdownloadquerythread.cpp
    musicdownloadthreadabstract.cpp
    musicequalizer.cpp
    musicfileinformation.cpp
    musiclocalsongsmanagerthread.cpp
    musicplayer.cpp
    musicplaylist.cpp
    musictextdownloadthread.cpp
    musictimerautoobject.cpp
    musicxmlabstract.cpp
    musicxmlconfigmanager.cpp
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