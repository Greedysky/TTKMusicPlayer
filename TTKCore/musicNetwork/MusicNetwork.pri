# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD \
               $$PWD/radio \
               $$PWD/nr \
               $$PWD/tt \
               $$PWD/wy \
               $$PWD/vip

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/radio/musicradiochannelthread.h \
    $$PWD/radio/musicradioplaylistthread.h \
    $$PWD/radio/musicradiosongsthread.h \
    $$PWD/radio/musicradiothreadabstract.h \
    $$PWD/nr/musicdatadownloadthread.h \
    $$PWD/nr/musictextdownloadthread.h \
    $$PWD/nr/musicdownloadqueryalbumthread.h \
    $$PWD/nr/musicdownloadqueryartistthread.h \
    $$PWD/nr/musicdownloadquerymultiplethread.h \
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
    $$PWD/musicsourcedownloadthread.h \
    $$PWD/musicnetworkthread.h \
    $$PWD/musicnetworkproxy.h \
    $$PWD/musicnetworkoperator.h \
    $$PWD/musicnetworktestthread.h \
    $$PWD/musicbackgrounddownload.h \
    $$PWD/musicdownloadqueuecache.h \
    $$PWD/musicdownloadqueryfactory.h \
    $$PWD/musicdownloadquerythreadabstract.h \
    $$PWD/musictranslationthread.h \
    $$PWD/musicvoicettsthread.h \
    $$PWD/musicidentifysongsthread.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/radio/musicradiochannelthread.cpp \
    $$PWD/radio/musicradioplaylistthread.cpp \
    $$PWD/radio/musicradiosongsthread.cpp \
    $$PWD/radio/musicradiothreadabstract.cpp \
    $$PWD/nr/musicdatadownloadthread.cpp \
    $$PWD/nr/musictextdownloadthread.cpp \
    $$PWD/nr/musicdownloadqueryalbumthread.cpp \
    $$PWD/nr/musicdownloadqueryartistthread.cpp \
    $$PWD/nr/musicdownloadquerymultiplethread.cpp \
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
    $$PWD/musicsourcedownloadthread.cpp \
    $$PWD/musicnetworkthread.cpp \
    $$PWD/musicnetworkproxy.cpp \
    $$PWD/musicnetworkoperator.cpp \
    $$PWD/musicnetworktestthread.cpp \
    $$PWD/musicbackgrounddownload.cpp \
    $$PWD/musicdownloadqueuecache.cpp \
    $$PWD/musicdownloadqueryfactory.cpp \
    $$PWD/musicdownloadquerythreadabstract.cpp \
    $$PWD/musictranslationthread.cpp \
    $$PWD/musicvoicettsthread.cpp \
    $$PWD/musicidentifysongsthread.cpp

}
