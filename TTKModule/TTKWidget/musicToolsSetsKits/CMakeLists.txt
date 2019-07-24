cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_TOOLSETS_KITS_HEADERS
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musiclocalsongstablewidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicnetworkconnectiontestwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicvolumegainwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsoundtouchwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicmobiledeviceswidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicaudiorecorderwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musictimerwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musiclocalsongsmanagerwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musictransformwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicfileinformationwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongringtonemakerwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicidentifysongswidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolswidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolstablewidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsoundkmicrowidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsoundkmicrosettingpopwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsoundkmicrosearchwidget.h
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_WIDGET_TOOLSETS_KITS_SOURCES
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicmobiledeviceswidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicaudiorecorderwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musictimerwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musiclocalsongsmanagerwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musictransformwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicfileinformationwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musiclocalsongstablewidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicnetworkconnectiontestwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicvolumegainwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsoundtouchwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongringtonemakerwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicidentifysongswidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolswidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolstablewidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsoundkmicrowidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsoundkmicrosettingpopwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsoundkmicrosearchwidget.cpp
    ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumwidget.cpp
  )
  
