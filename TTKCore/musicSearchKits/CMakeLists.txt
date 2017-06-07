cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_SEARCH_KITS_HEADERS
    ${MUSIC_LOCALSEARCH_DIR}/core/musiclocalsongsearchrecordconfigmanager.h
    ${MUSIC_LOCALSEARCH_DIR}/musiclocalsongsearchdialog.h
    ${MUSIC_LOCALSEARCH_DIR}/musiclocalsongsearchedit.h
    ${MUSIC_LOCALSEARCH_DIR}/musiclocalsongsearchinlineedit.h
    ${MUSIC_LOCALSEARCH_DIR}/musiclocalsongsearchpopwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_SEARCH_KITS_SOURCES
    ${MUSIC_LOCALSEARCH_DIR}/core/musiclocalsongsearchrecordconfigmanager.cpp
    ${MUSIC_LOCALSEARCH_DIR}/musiclocalsongsearchdialog.cpp
    ${MUSIC_LOCALSEARCH_DIR}/musiclocalsongsearchedit.cpp
    ${MUSIC_LOCALSEARCH_DIR}/musiclocalsongsearchinlineedit.cpp
    ${MUSIC_LOCALSEARCH_DIR}/musiclocalsongsearchpopwidget.cpp
  )
  