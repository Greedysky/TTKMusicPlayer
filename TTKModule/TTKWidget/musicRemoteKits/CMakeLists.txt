cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_REMOTE_KITS_HEADERS
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidget.h
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsquare.h
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforrectangle.h
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetfordiamond.h
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforcircle.h
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsimplestyle.h
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforcomplexstyle.h
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforstrip.h
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforripples.h
  )

set_property(GLOBAL PROPERTY MUSIC_WIDGET_REMOTE_KITS_SOURCES
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidget.cpp
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsquare.cpp
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforrectangle.cpp
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetfordiamond.cpp
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforcircle.cpp
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsimplestyle.cpp
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforcomplexstyle.cpp
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforstrip.cpp
    ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforripples.cpp
  )
  