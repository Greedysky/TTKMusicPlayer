cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_SEARCH_KITS_HEADERS
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musiclocalsongsearchdialog.h
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musiclocalsongsearchedit.h
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musiclocalsongsearchinlineedit.h
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musiclocalsongsearchpopwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_WIDGET_SEARCH_KITS_SOURCES
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musiclocalsongsearchdialog.cpp
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musiclocalsongsearchedit.cpp
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musiclocalsongsearchinlineedit.cpp
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musiclocalsongsearchpopwidget.cpp
  )
  