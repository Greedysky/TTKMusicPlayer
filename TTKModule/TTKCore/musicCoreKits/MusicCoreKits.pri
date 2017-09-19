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
    $$PWD/musiclogger.h \
    $$PWD/musicobject.h \
    $$PWD/musicdatabaseobject.h \
    $$PWD/musicformats.h \
    $$PWD/musicnumberdefine.h \
    $$PWD/musicglobaldefine.h \
    $$PWD/musicotherdefine.h \
    $$PWD/musictime.h \
    $$PWD/musicplayer.h \
    $$PWD/musicplaylist.h \
    $$PWD/musicplayedlist.h \
    $$PWD/musicabstractxml.h \
    $$PWD/musicsettingmanager.h \
    $$PWD/musicconnectionpool.h \
    $$PWD/musicregeditmanager.h \
    $$PWD/musicsingleton.h \
    $$PWD/musiccoremplayer.h \
    $$PWD/musicsong.h \
    $$PWD/musicsongtag.h \
    $$PWD/musiccryptographichash.h \
    $$PWD/musicbackgroundmanager.h \
    $$PWD/musicsystemenvironment.h \
    $$PWD/musicsemaphoreloop.h \
    $$PWD/musiccategoryconfigmanager.h \
    $$PWD/musiclicensecore.h \
    $$PWD/musicplaylistmanager.h \
    $$PWD/musichotkeymanager.h \
    $$PWD/musicruntimemanager.h \
    $$PWD/musicdispatchmanager.h \
    $$PWD/musicextractwrap.h \
    $$PWD/musicbackgroundconfigmanager.h \
    $$PWD/musicsysconfigmanager.h

}

contains(CONFIG, MUSIC_BUILD_LIB){
SOURCES += \
    $$PWD/musicformats.cpp \
    $$PWD/musictime.cpp \
    $$PWD/musicplayer.cpp \
    $$PWD/musicplaylist.cpp \
    $$PWD/musicplayedlist.cpp \
    $$PWD/musicabstractxml.cpp \
    $$PWD/musicconnectionpool.cpp \
    $$PWD/musicregeditmanager.cpp \
    $$PWD/musiccoremplayer.cpp \
    $$PWD/musicsong.cpp \
    $$PWD/musicsongtag.cpp \
    $$PWD/musiccryptographichash.cpp \
    $$PWD/musicbackgroundmanager.cpp \
    $$PWD/musicsystemenvironment.cpp \
    $$PWD/musicsemaphoreloop.cpp \
    $$PWD/musiccategoryconfigmanager.cpp \
    $$PWD/musiclicensecore.cpp \
    $$PWD/musicplaylistmanager.cpp \
    $$PWD/musichotkeymanager.cpp \
    $$PWD/musicruntimemanager.cpp \
    $$PWD/musicdispatchmanager.cpp \
    $$PWD/musicextractwrap.cpp \
    $$PWD/musicbackgroundconfigmanager.cpp \
    $$PWD/musicsysconfigmanager.cpp

}
