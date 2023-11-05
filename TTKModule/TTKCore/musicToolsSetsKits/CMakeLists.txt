cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_TOOLSETS_KITS_HEADERS
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicaudiorecordermodule.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicbackupmodule.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperthread.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musictimerautomodule.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongsmanagerthread.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsunit.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsthread.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicnetworktestthread.h
)

set_property(GLOBAL PROPERTY MUSIC_CORE_TOOLSETS_KITS_SOURCES
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicaudiorecordermodule.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicbackupmodule.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperthread.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musictimerautomodule.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongsmanagerthread.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsthread.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicnetworktestthread.cpp
)
