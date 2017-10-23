cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_LRC_KITS_HEADERS
    ${MUSIC_CORE_LRCMANAGER_DIR}/musiclrcanalysis.h
    ${MUSIC_CORE_LRCMANAGER_DIR}/musiclrcdefines.h
    ${MUSIC_CORE_LRCMANAGER_DIR}/musiclrcfromkrc.h
  )

set_property(GLOBAL PROPERTY MUSIC_CORE_LRC_KITS_SOURCES
    ${MUSIC_CORE_LRCMANAGER_DIR}/musiclrcanalysis.cpp
    ${MUSIC_CORE_LRCMANAGER_DIR}/musiclrcdefines.cpp
    ${MUSIC_CORE_LRCMANAGER_DIR}/musiclrcfromkrc.cpp
  )
