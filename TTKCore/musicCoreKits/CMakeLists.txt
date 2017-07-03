cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_CORE_KITS_HEADERS
    ${MUSIC_CORE_DIR}/utils/musiccoreutils.h
    ${MUSIC_CORE_DIR}/utils/musiccoreutils.h
    ${MUSIC_CORE_DIR}/utils/musicwidgetutils.h
    ${MUSIC_CORE_DIR}/utils/musicnumberutils.h
    ${MUSIC_CORE_DIR}/utils/musicstringutils.h
    ${MUSIC_CORE_DIR}/utils/musicalgorithmutils.h
    ${MUSIC_CORE_DIR}/musicobject.h
    ${MUSIC_CORE_DIR}/musicformats.h
    ${MUSIC_CORE_DIR}/musicglobaldefine.h
    ${MUSIC_CORE_DIR}/musicglobalhotkey.h
    ${MUSIC_CORE_DIR}/musictime.h
    ${MUSIC_CORE_DIR}/musicxmlconfigmanager.h
    ${MUSIC_CORE_DIR}/musicplayer.h
    ${MUSIC_CORE_DIR}/musicplaylist.h
    ${MUSIC_CORE_DIR}/musicabstractxml.h
    ${MUSIC_CORE_DIR}/musicbackgroundmanager.h
    ${MUSIC_CORE_DIR}/musicsettingmanager.h
    ${MUSIC_CORE_DIR}/musicconnectionpool.h
    ${MUSIC_CORE_DIR}/musicregeditmanager.h
    ${MUSIC_CORE_DIR}/musicsingleton.h
    ${MUSIC_CORE_DIR}/musiccoremplayer.h
    ${MUSIC_CORE_DIR}/musicsong.h
    ${MUSIC_CORE_DIR}/musicsongtag.h
    ${MUSIC_CORE_DIR}/musiccryptographichash.h
    ${MUSIC_CORE_DIR}/musicsemaphoreloop.h
    ${MUSIC_CORE_DIR}/musiclicensecore.h
    ${MUSIC_CORE_DIR}/musiccategoryconfigmanager.h
  )

set_property(GLOBAL PROPERTY MUSIC_CORE_KITS_SOURCES
    ${MUSIC_CORE_DIR}/utils/musiccoreutils.cpp
    ${MUSIC_CORE_DIR}/utils/musiccoreutils.cpp
    ${MUSIC_CORE_DIR}/utils/musicwidgetutils.cpp
    ${MUSIC_CORE_DIR}/utils/musicnumberutils.cpp
    ${MUSIC_CORE_DIR}/utils/musicstringutils.cpp
    ${MUSIC_CORE_DIR}/utils/musicalgorithmutils.cpp
    ${MUSIC_CORE_DIR}/musicglobalhotkey.cpp
    ${MUSIC_CORE_DIR}/musicformats.cpp
    ${MUSIC_CORE_DIR}/musictime.cpp
    ${MUSIC_CORE_DIR}/musicplayer.cpp
    ${MUSIC_CORE_DIR}/musicplaylist.cpp
    ${MUSIC_CORE_DIR}/musicabstractxml.cpp
    ${MUSIC_CORE_DIR}/musicxmlconfigmanager.cpp
    ${MUSIC_CORE_DIR}/musicbackgroundmanager.cpp
    ${MUSIC_CORE_DIR}/musicconnectionpool.cpp
    ${MUSIC_CORE_DIR}/musicregeditmanager.cpp
    ${MUSIC_CORE_DIR}/musiccoremplayer.cpp
    ${MUSIC_CORE_DIR}/musicsong.cpp
    ${MUSIC_CORE_DIR}/musicsongtag.cpp
    ${MUSIC_CORE_DIR}/musiccryptographichash.cpp
    ${MUSIC_CORE_DIR}/musicsemaphoreloop.cpp
    ${MUSIC_CORE_DIR}/musiclicensecore.cpp
    ${MUSIC_CORE_DIR}/musiccategoryconfigmanager.cpp
  )
  