cmake_minimum_required(VERSION 2.8.11)

set_property(GLOBAL PROPERTY MUSIC_LRC_KITS_HEADERS
    ${MUSIC_LRCMANAGER_DIR}/musiclrcanalysis.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcdefines.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcmanager.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcmanagerfordesktop.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcmanagerforinline.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrccontainer.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrccontainerfordesktop.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrccontainerforinline.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrccontainerforwallpaper.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcsearchwidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcsearchtablewidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcartphotouploadwidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfloatwidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfloatsettingwidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfloatphotowidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfloatplaywidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcmakerwidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcerrorwidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrclocallinkwidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfromkrc.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrccommentswidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrctranslatedwidget.h
    ${MUSIC_LRCMANAGER_DIR}/musiclrccolorwidget.h
  )

set_property(GLOBAL PROPERTY MUSIC_LRC_KITS_SOURCES
    ${MUSIC_LRCMANAGER_DIR}/musiclrcanalysis.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcdefines.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcmanager.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcmanagerfordesktop.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcmanagerforinline.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrccontainerforwallpaper.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrccontainer.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrccontainerfordesktop.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrccontainerforinline.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcsearchwidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcsearchtablewidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcartphotouploadwidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfloatwidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfloatsettingwidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfloatphotowidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfloatplaywidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcmakerwidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcerrorwidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrclocallinkwidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrcfromkrc.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrccommentswidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrctranslatedwidget.cpp
    ${MUSIC_LRCMANAGER_DIR}/musiclrccolorwidget.cpp
  )
  