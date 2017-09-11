cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_TOOLS_KITS_HEADERS
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicsongstoolboxwidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicsongssummariziedwidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musictoolsetswidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicdownloadrecordwidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnectmobilewidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransferwidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransfertablewidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmusicradiolistview.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmusicradiowidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudfilemanagerdialog.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudsharedsongwidget.h
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudtablewidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_WIDGET_TOOLS_KITS_SOURCES
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnectmobilewidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransferwidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransfertablewidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmusicradiolistview.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmusicradiowidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicsongstoolboxwidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicsongssummariziedwidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musictoolsetswidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicdownloadrecordwidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudfilemanagerdialog.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudsharedsongwidget.cpp
    ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudtablewidget.cpp
  )
  