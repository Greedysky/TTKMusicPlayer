cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_UTILS_KITS_HEADERS
    ${MUSIC_UTILS_DIR}/musiccoreutils.h
    ${MUSIC_UTILS_DIR}/musicwidgetutils.h
    ${MUSIC_UTILS_DIR}/musicnumberutils.h
    ${MUSIC_UTILS_DIR}/musicstringutils.h
    ${MUSIC_UTILS_DIR}/musicalgorithmutils.h
  )

set_property(GLOBAL PROPERTY MUSIC_UTILS_KITS_SOURCES
    ${MUSIC_UTILS_DIR}/musiccoreutils.cpp
    ${MUSIC_UTILS_DIR}/musicwidgetutils.cpp
    ${MUSIC_UTILS_DIR}/musicnumberutils.cpp
    ${MUSIC_UTILS_DIR}/musicstringutils.cpp
    ${MUSIC_UTILS_DIR}/musicalgorithmutils.cpp
  )
  