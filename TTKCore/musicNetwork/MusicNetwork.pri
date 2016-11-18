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
               $$PWD/tt \
               $$PWD/wy \
               $$PWD/vip \

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/tt/musicttdatadownloadthread.h \
    $$PWD/tt/musictttextdownloadthread.h \
    $$PWD/tt/musicdownloadqueryttthread.h \
    $$PWD/tt/musicdownloadqueryalbumttthread.h \
    $$PWD/wy/musicdownloadquerywythread.h \
    $$PWD/wy/musicwytextdownloadthread.h \
    $$PWD/wy/musicdownloadqueryalbumwythread.h \
    $$PWD/wy/musicwycommentsthread.h \
    $$PWD/vip/musicdownloadquerymultiplevipthread.h \
    $$PWD/vip/musicdownloadqueryalbumvipthread.h \
    $$PWD/vip/musicdownloadqueryartistvipthread.h \
    $$PWD/musicnetworkabstract.h \
    $$PWD/musicdownloadthreadabstract.h \
    $$PWD/musicdatadownloadthread.h \
    $$PWD/musictextdownloadthread.h \
    $$PWD/musicsourcedownloadthread.h \
    $$PWD/musicnetworkthread.h \
    $$PWD/musicnetworkproxy.h \
    $$PWD/musicnetworkoperator.h \
    $$PWD/musicnetworktestthread.h \
    $$PWD/musicbackgrounddownload.h \
    $$PWD/musicdownloadqueuecache.h \
    $$PWD/musicdownloadqueryfactory.h \
    $$PWD/musicdownloadquerythreadabstract.h \
    $$PWD/musicdownloadquerymultiplethread.h \
    $$PWD/musicdownloadqueryalbumthread.h \
    $$PWD/musicdownloadqueryartistthread.h \
    $$PWD/musicradiochannelthread.h \
    $$PWD/musicradioplaylistthread.h \
    $$PWD/musicradiosongsthread.h \
    $$PWD/musicradiothreadabstract.h \
    $$PWD/musictranslationthread.h \
    $$PWD/musicvoicettsthread.h \
    $$PWD/musicidentifysongsthread.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/tt/musicttdatadownloadthread.cpp \
    $$PWD/tt/musictttextdownloadthread.cpp \
    $$PWD/tt/musicdownloadqueryttthread.cpp \
    $$PWD/tt/musicdownloadqueryalbumttthread.cpp \
    $$PWD/wy/musicdownloadquerywythread.cpp \
    $$PWD/wy/musicwytextdownloadthread.cpp \
    $$PWD/wy/musicdownloadqueryalbumwythread.cpp \
    $$PWD/wy/musicwycommentsthread.cpp \
    $$PWD/vip/musicdownloadquerymultiplevipthread.cpp \
    $$PWD/vip/musicdownloadqueryalbumvipthread.cpp \
    $$PWD/vip/musicdownloadqueryartistvipthread.cpp \
    $$PWD/musicnetworkabstract.cpp \
    $$PWD/musicdownloadthreadabstract.cpp \
    $$PWD/musicdatadownloadthread.cpp \
    $$PWD/musictextdownloadthread.cpp \
    $$PWD/musicsourcedownloadthread.cpp \
    $$PWD/musicnetworkthread.cpp \
    $$PWD/musicnetworkproxy.cpp \
    $$PWD/musicnetworkoperator.cpp \
    $$PWD/musicnetworktestthread.cpp \
    $$PWD/musicbackgrounddownload.cpp \
    $$PWD/musicdownloadqueuecache.cpp \
    $$PWD/musicdownloadqueryfactory.cpp \
    $$PWD/musicdownloadquerythreadabstract.cpp \
    $$PWD/musicdownloadquerymultiplethread.cpp \
    $$PWD/musicdownloadqueryalbumthread.cpp \
    $$PWD/musicdownloadqueryartistthread.cpp \
    $$PWD/musicradiochannelthread.cpp \
    $$PWD/musicradioplaylistthread.cpp \
    $$PWD/musicradiosongsthread.cpp \
    $$PWD/musicradiothreadabstract.cpp \
    $$PWD/musictranslationthread.cpp \
    $$PWD/musicvoicettsthread.cpp \
    $$PWD/musicidentifysongsthread.cpp

}
