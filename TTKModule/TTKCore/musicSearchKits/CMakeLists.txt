cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_SEARCH_KITS_HEADERS
  ${MUSIC_CORE_LOCALSEARCH_DIR}/musicsearchinterface.h
  ${MUSIC_CORE_LOCALSEARCH_DIR}/musicsongsearchrecordconfigmanager.h
)

set_property(GLOBAL PROPERTY MUSIC_CORE_SEARCH_KITS_SOURCES
  ${MUSIC_CORE_LOCALSEARCH_DIR}/musicsongsearchrecordconfigmanager.cpp
)
