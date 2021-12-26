cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_SEARCH_KITS_HEADERS
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsearchedit.h
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsongsearchdialog.h
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsongsearchedit.h
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsongsearchinterioredit.h
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsongsearchpopwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_WIDGET_SEARCH_KITS_SOURCES
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsearchedit.cpp
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsongsearchdialog.cpp
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsongsearchedit.cpp
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsongsearchinterioredit.cpp
    ${MUSIC_WIDGET_LOCALSEARCH_DIR}/musicsongsearchpopwidget.cpp
  )
  
