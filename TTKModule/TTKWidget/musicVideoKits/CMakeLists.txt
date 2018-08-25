cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_VIDEO_KITS_HEADERS
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideocontrolwidget.h
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideoview.h
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideosearchtablewidget.h
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideoplaywidget.h
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideofloatwidget.h
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideoqualitypopwidget.h
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideobarragestylepopwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_WIDGET_VIDEO_KITS_SOURCES
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideocontrolwidget.cpp
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideoview.cpp
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideosearchtablewidget.cpp
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideoplaywidget.cpp
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideofloatwidget.cpp
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideoqualitypopwidget.cpp
    ${MUSIC_WIDGET_VIDEOKITS_DIR}/musicvideobarragestylepopwidget.cpp
  )
  