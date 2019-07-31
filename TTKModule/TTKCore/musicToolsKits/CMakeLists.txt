cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_TOOLS_KITS_HEADERS
    ${MUSIC_CORE_TOOLSWIDGET_DIR}/musicdownloadrecordconfigmanager.h
    ${MUSIC_CORE_TOOLSWIDGET_DIR}/musicclouddataitem.h
    ${MUSIC_CORE_TOOLSWIDGET_DIR}/musicdeviceinfocore.h
  )

set_property(GLOBAL PROPERTY MUSIC_CORE_TOOLS_KITS_SOURCES
    ${MUSIC_CORE_TOOLSWIDGET_DIR}/musicdownloadrecordconfigmanager.cpp
    ${MUSIC_CORE_TOOLSWIDGET_DIR}/musicdeviceinfocore.cpp
  )
  
