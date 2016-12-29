# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD \
               $$PWD/utils

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/utils/musiccoreutils.h \
    $$PWD/utils/musicwidgetutils.h \
    $$PWD/utils/musicnumberutils.h \
    $$PWD/utils/musicstringutils.h \
    $$PWD/musiclogger.h \
    $$PWD/musicobject.h \
    $$PWD/musicdatabaseobject.h \
    $$PWD/musicformats.h \
    $$PWD/musicnumberdefine.h \
    $$PWD/musicglobaldefine.h \
    $$PWD/musicglobalhotkey.h \
    $$PWD/musictime.h \
    $$PWD/musicxmlconfigmanager.h \
    $$PWD/musicplayer.h \
    $$PWD/musicplaylist.h \
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
    $$PWD/musicsystemenvironment.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/utils/musiccoreutils.cpp \
    $$PWD/utils/musicwidgetutils.cpp \
    $$PWD/utils/musicnumberutils.cpp \
    $$PWD/utils/musicstringutils.cpp \
    $$PWD/musicformats.cpp \
    $$PWD/musicglobalhotkey.cpp \
    $$PWD/musictime.cpp \
    $$PWD/musicplayer.cpp \
    $$PWD/musicplaylist.cpp \
    $$PWD/musicabstractxml.cpp \
    $$PWD/musicxmlconfigmanager.cpp \
    $$PWD/musicconnectionpool.cpp \
    $$PWD/musicregeditmanager.cpp \
    $$PWD/musiccoremplayer.cpp \
    $$PWD/musicsong.cpp \
    $$PWD/musicsongtag.cpp \
    $$PWD/musiccryptographichash.cpp \
    $$PWD/musicbackgroundmanager.cpp \
    $$PWD/musicsystemenvironment.cpp

}
