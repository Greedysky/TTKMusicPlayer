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

INCLUDEPATH += $$PWD

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musicnetworkabstract.h \
    $$PWD/musicdownloadthreadabstract.h \
    $$PWD/musicdata2downloadthread.h \
    $$PWD/musicdatadownloadthread.h \
    $$PWD/musictextdownloadthread.h \
    $$PWD/musicsourcedownloadthread.h \
    $$PWD/musicnetworkthread.h \
    $$PWD/musicnetworkproxy.h \
    $$PWD/musicnetworkoperator.h \
    $$PWD/musicbackgrounddownload.h \
    $$PWD/musicdownloadqueuecache.h \
    $$PWD/musicdownloadqueryfactory.h \
    $$PWD/musicdownloadquerythreadabstract.h \
    $$PWD/musicdownloadquerymultiplethread.h \
    $$PWD/musicdownloadqueryttthread.h \
    $$PWD/musicdownloadquerywythread.h \
    $$PWD/musicradiochannelthread.h \
    $$PWD/musicradioplaylistthread.h \
    $$PWD/musicradiosongsthread.h \
    $$PWD/musicradiothreadabstract.h \
    $$PWD/musictranslationthread.h \
    $$PWD/musicvoicettsthread.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicnetworkabstract.cpp \
    $$PWD/musicdownloadthreadabstract.cpp \
    $$PWD/musicdata2downloadthread.cpp \
    $$PWD/musicdatadownloadthread.cpp \
    $$PWD/musictextdownloadthread.cpp \
    $$PWD/musicsourcedownloadthread.cpp \
    $$PWD/musicnetworkthread.cpp \
    $$PWD/musicnetworkproxy.cpp \
    $$PWD/musicnetworkoperator.cpp \
    $$PWD/musicbackgrounddownload.cpp \
    $$PWD/musicdownloadqueuecache.cpp \
    $$PWD/musicdownloadqueryfactory.cpp \
    $$PWD/musicdownloadquerythreadabstract.cpp \
    $$PWD/musicdownloadquerymultiplethread.cpp \
    $$PWD/musicdownloadqueryttthread.cpp \
    $$PWD/musicdownloadquerywythread.cpp \
    $$PWD/musicradiochannelthread.cpp \
    $$PWD/musicradioplaylistthread.cpp \
    $$PWD/musicradiosongsthread.cpp \
    $$PWD/musicradiothreadabstract.cpp \
    $$PWD/musictranslationthread.cpp \
    $$PWD/musicvoicettsthread.cpp
}
