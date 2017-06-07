cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_VIDEO_KITS_HEADERS
    ${MUSIC_VIDEOKITS_DIR}/core/musicbarragerecordconfigmanager.h
    ${MUSIC_VIDEOKITS_DIR}/core/musicbarragewidget.h
    ${MUSIC_VIDEOKITS_DIR}/musicvideocontrolwidget.h
    ${MUSIC_VIDEOKITS_DIR}/musicvideoview.h
    ${MUSIC_VIDEOKITS_DIR}/musicvideotablewidget.h
    ${MUSIC_VIDEOKITS_DIR}/musicvideoplaywidget.h
    ${MUSIC_VIDEOKITS_DIR}/musicvideofloatwidget.h
    ${MUSIC_VIDEOKITS_DIR}/musicvideoqualitypopwidget.h
    ${MUSIC_VIDEOKITS_DIR}/musicvideobarragestylepopwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_VIDEO_KITS_SOURCES
    ${MUSIC_VIDEOKITS_DIR}/core/musicbarragerecordconfigmanager.cpp
    ${MUSIC_VIDEOKITS_DIR}/core/musicbarragewidget.cpp
    ${MUSIC_VIDEOKITS_DIR}/musicvideocontrolwidget.cpp
    ${MUSIC_VIDEOKITS_DIR}/musicvideoview.cpp
    ${MUSIC_VIDEOKITS_DIR}/musicvideotablewidget.cpp
    ${MUSIC_VIDEOKITS_DIR}/musicvideoplaywidget.cpp
    ${MUSIC_VIDEOKITS_DIR}/musicvideofloatwidget.cpp
    ${MUSIC_VIDEOKITS_DIR}/musicvideoqualitypopwidget.cpp
    ${MUSIC_VIDEOKITS_DIR}/musicvideobarragestylepopwidget.cpp
  )
  