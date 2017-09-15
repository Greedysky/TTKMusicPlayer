# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD

!contains(CONFIG, MUSIC_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musiclrcmanager.h \
    $$PWD/musiclrcmanagerfordesktop.h \
    $$PWD/musiclrcmanagerforinline.h \
    $$PWD/musiclrccontainer.h \
    $$PWD/musiclrccontainerfordesktop.h \
    $$PWD/musiclrccontainerforinline.h \
    $$PWD/musiclrcsearchwidget.h \
    $$PWD/musiclrcsearchtablewidget.h \
    $$PWD/musiclrcartphotouploadwidget.h \
    $$PWD/musiclrcfloatwidget.h \
    $$PWD/musiclrcfloatsettingwidget.h \
    $$PWD/musiclrcfloatphotowidget.h \
    $$PWD/musiclrcfloatplaywidget.h \
    $$PWD/musiclrcmakerwidget.h \
    $$PWD/musiclrcerrorwidget.h \
    $$PWD/musiclrclocallinkwidget.h \
    $$PWD/musiclrccolorwidget.h \
    $$PWD/musiclrccommentswidget.h \
    $$PWD/musiclrctranslatedwidget.h \
    $$PWD/musiclrccontainerforwallpaper.h \
    $$PWD/musiclrcposterwidget.h

}

contains(CONFIG, MUSIC_BUILD_LIB){
SOURCES += \
    $$PWD/musiclrcmanager.cpp \
    $$PWD/musiclrcmanagerfordesktop.cpp \
    $$PWD/musiclrcmanagerforinline.cpp \
    $$PWD/musiclrccontainer.cpp \
    $$PWD/musiclrccontainerfordesktop.cpp \
    $$PWD/musiclrccontainerforinline.cpp \
    $$PWD/musiclrcsearchwidget.cpp \
    $$PWD/musiclrcsearchtablewidget.cpp \
    $$PWD/musiclrcartphotouploadwidget.cpp \
    $$PWD/musiclrcfloatwidget.cpp \
    $$PWD/musiclrcfloatsettingwidget.cpp \
    $$PWD/musiclrcfloatphotowidget.cpp \
    $$PWD/musiclrcfloatplaywidget.cpp \
    $$PWD/musiclrcmakerwidget.cpp \
    $$PWD/musiclrcerrorwidget.cpp \
    $$PWD/musiclrclocallinkwidget.cpp \
    $$PWD/musiclrccolorwidget.cpp \
    $$PWD/musiclrccommentswidget.cpp \
    $$PWD/musiclrctranslatedwidget.cpp \
    $$PWD/musiclrccontainerforwallpaper.cpp \
    $$PWD/musiclrcposterwidget.cpp

}
