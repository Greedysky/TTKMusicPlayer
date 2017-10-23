cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_VIDEO_KITS_HEADERS
    ${MUSIC_CORE_VIDEOKITS_DIR}/musicbarragerecordconfigmanager.h
    ${MUSIC_CORE_VIDEOKITS_DIR}/musicbarragewidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_CORE_VIDEO_KITS_SOURCES
    ${MUSIC_CORE_VIDEOKITS_DIR}/musicbarragerecordconfigmanager.cpp
    ${MUSIC_CORE_VIDEOKITS_DIR}/musicbarragewidget.cpp
  )
  