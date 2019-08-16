cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_LRC_KITS_HEADERS
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanager.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerfordesktop.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerforinline.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainer.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerfordesktop.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforinline.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforwallpaper.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcsearchwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcsearchtablewidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcartphotouploadwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatsettingwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatphotowidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatplaywidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmakerwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcerrorwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrclocallinkwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccommentswidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrctranslatedwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccolorwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcposterwidget.h
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcdownloadbatchwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_WIDGET_LRC_KITS_SOURCES
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanager.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerfordesktop.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerforinline.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforwallpaper.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainer.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerfordesktop.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforinline.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcsearchwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcsearchtablewidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcartphotouploadwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatsettingwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatphotowidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatplaywidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmakerwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcerrorwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrclocallinkwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccommentswidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrctranslatedwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccolorwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcposterwidget.cpp
    ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcdownloadbatchwidget.cpp
  )
