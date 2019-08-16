cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_UTILS_KITS_HEADERS
    ${MUSIC_CORE_UTILS_DIR}/musiccoreutils.h
    ${MUSIC_CORE_UTILS_DIR}/musicwidgetutils.h
    ${MUSIC_CORE_UTILS_DIR}/musicnumberutils.h
    ${MUSIC_CORE_UTILS_DIR}/musicstringutils.h
    ${MUSIC_CORE_UTILS_DIR}/musicalgorithmutils.h
    ${MUSIC_CORE_UTILS_DIR}/musicurlutils.h
    ${MUSIC_CORE_UTILS_DIR}/musicqmmputils.h
    ${MUSIC_CORE_UTILS_DIR}/musiccodecutils.h
    ${MUSIC_CORE_UTILS_DIR}/musicfileutils.h
  )

set_property(GLOBAL PROPERTY MUSIC_CORE_UTILS_KITS_SOURCES
    ${MUSIC_CORE_UTILS_DIR}/musiccoreutils.cpp
    ${MUSIC_CORE_UTILS_DIR}/musicwidgetutils.cpp
    ${MUSIC_CORE_UTILS_DIR}/musicnumberutils.cpp
    ${MUSIC_CORE_UTILS_DIR}/musicstringutils.cpp
    ${MUSIC_CORE_UTILS_DIR}/musicalgorithmutils.cpp
    ${MUSIC_CORE_UTILS_DIR}/musicurlutils.cpp
    ${MUSIC_CORE_UTILS_DIR}/musicqmmputils.cpp
    ${MUSIC_CORE_UTILS_DIR}/musiccodecutils.cpp
    ${MUSIC_CORE_UTILS_DIR}/musicfileutils.cpp
  )
  
