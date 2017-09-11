cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_USER_KITS_HEADERS
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicusermanagerdialog.h
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicuserdialog.h
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicuserlineedit.h
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicuserwindow.h
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicuserrecordwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_WIDGET_USER_KITS_SOURCES
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicuserrecordwidget.cpp
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicusermanagerdialog.cpp
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicuserdialog.cpp
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicuserlineedit.cpp
    ${MUSIC_WIDGET_USERMANAGER_DIR}/musicuserwindow.cpp
  )
  