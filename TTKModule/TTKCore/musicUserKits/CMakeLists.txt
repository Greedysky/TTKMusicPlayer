cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_USER_KITS_HEADERS
    ${MUSIC_CORE_USERMANAGER_DIR}/musicusermodel.h
    ${MUSIC_CORE_USERMANAGER_DIR}/musicuserconfigmanager.h
  )

set_property(GLOBAL PROPERTY MUSIC_CORE_USER_KITS_SOURCES
    ${MUSIC_CORE_USERMANAGER_DIR}/musicusermodel.cpp
    ${MUSIC_CORE_USERMANAGER_DIR}/musicuserconfigmanager.cpp
  )
  