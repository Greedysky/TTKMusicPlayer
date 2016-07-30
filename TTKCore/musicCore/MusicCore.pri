#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2014 - 2016 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD \
               $$PWD/network

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/network/musicnetworkabstract.h \
    $$PWD/network/musicdownloadthreadabstract.h \
    $$PWD/network/musicdata2downloadthread.h \
    $$PWD/network/musicdatadownloadthread.h \
    $$PWD/network/musictextdownloadthread.h \
    $$PWD/network/musicsourcedownloadthread.h \
    $$PWD/network/musicnetworkthread.h \
    $$PWD/network/musicnetworkproxy.h \
    $$PWD/network/musicnetworkoperator.h \
    $$PWD/network/musicbackgrounddownload.h \
    $$PWD/network/musicdownloadqueuecache.h \
    $$PWD/network/musicdownloadquerythreadabstract.h \
    $$PWD/network/musicdownloadquerysinglethread.h \
    $$PWD/network/musicdownloadquerymultiplethread.h \
    $$PWD/network/musicradiochannelthread.h \
    $$PWD/network/musicradioplaylistthread.h \
    $$PWD/network/musicradiosongsthread.h \
    $$PWD/network/musicradiothreadabstract.h \
    $$PWD/network/musictranslationthread.h \
    $$PWD/network/musicvoicettsthread.h \
    $$PWD/musiclogger.h \
    $$PWD/musicobject.h \
    $$PWD/musicuiobject.h \
    $$PWD/musicttkuiobject.h \
    $$PWD/musicutils.h \
    $$PWD/musicnumberdefine.h \
    $$PWD/musicglobaldefine.h \
    $$PWD/musicglobalhotkey.h \
    $$PWD/musicversion.h \
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
    $$PWD/musiccryptographichash.h \
    $$PWD/musicbackgroundmanager.h \
    $$PWD/musicsystemenvironment.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/network/musicnetworkabstract.cpp \
    $$PWD/network/musicdownloadthreadabstract.cpp \
    $$PWD/network/musicdata2downloadthread.cpp \
    $$PWD/network/musicdatadownloadthread.cpp \
    $$PWD/network/musictextdownloadthread.cpp \
    $$PWD/network/musicsourcedownloadthread.cpp \
    $$PWD/network/musicnetworkthread.cpp \
    $$PWD/network/musicnetworkproxy.cpp \
    $$PWD/network/musicnetworkoperator.cpp \
    $$PWD/network/musicbackgrounddownload.cpp \
    $$PWD/network/musicdownloadqueuecache.cpp \
    $$PWD/network/musicdownloadquerythreadabstract.cpp \
    $$PWD/network/musicdownloadquerysinglethread.cpp \
    $$PWD/network/musicdownloadquerymultiplethread.cpp \
    $$PWD/network/musicradiochannelthread.cpp \
    $$PWD/network/musicradioplaylistthread.cpp \
    $$PWD/network/musicradiosongsthread.cpp \
    $$PWD/network/musicradiothreadabstract.cpp \
    $$PWD/network/musictranslationthread.cpp \
    $$PWD/network/musicvoicettsthread.cpp \
    $$PWD/musicutils.cpp \
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
    $$PWD/musiccryptographichash.cpp \
    $$PWD/musicbackgroundmanager.cpp \
    $$PWD/musicsystemenvironment.cpp

}
