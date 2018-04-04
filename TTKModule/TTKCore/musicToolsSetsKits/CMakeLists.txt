cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_TOOLSETS_KITS_HEADERS
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicaudiorecordercore.h
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperthread.h
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musictimerautoobject.h
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musiclocalsongsmanagerthread.h
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicnetworkspeedtestthread.h
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsunit.h
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolscore.h
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicnetworktestthread.h
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicwebradioobject.h
  )

set_property(GLOBAL PROPERTY MUSIC_CORE_TOOLSETS_KITS_SOURCES
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicaudiorecordercore.cpp
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperthread.cpp
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musictimerautoobject.cpp
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musiclocalsongsmanagerthread.cpp
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicnetworkspeedtestthread.cpp
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolscore.cpp
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicnetworktestthread.cpp
    ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicwebradioobject.cpp
  )
  