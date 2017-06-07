cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_USER_KITS_HEADERS
    ${MUSIC_USERMANAGER_DIR}/core/musicusermodel.h
    ${MUSIC_USERMANAGER_DIR}/core/musicuserconfigmanager.h
    ${MUSIC_USERMANAGER_DIR}/musicusermanagerdialog.h
    ${MUSIC_USERMANAGER_DIR}/musicuserdialog.h
    ${MUSIC_USERMANAGER_DIR}/musicuserlineedit.h
    ${MUSIC_USERMANAGER_DIR}/musicuserwindow.h
    ${MUSIC_USERMANAGER_DIR}/musicuserrecordwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_USER_KITS_SOURCES
    ${MUSIC_USERMANAGER_DIR}/core/musicusermodel.cpp
    ${MUSIC_USERMANAGER_DIR}/core/musicuserconfigmanager.cpp
    ${MUSIC_USERMANAGER_DIR}/musicuserrecordwidget.cpp
    ${MUSIC_USERMANAGER_DIR}/musicusermanagerdialog.cpp
    ${MUSIC_USERMANAGER_DIR}/musicuserdialog.cpp
    ${MUSIC_USERMANAGER_DIR}/musicuserlineedit.cpp
    ${MUSIC_USERMANAGER_DIR}/musicuserwindow.cpp
  )
  