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
               $$PWD/xm \
               $$PWD/kg \
               $$PWD/kw \
               $$PWD/bd \
               $$PWD/qq \
               $$PWD/vip \
               $$PWD/yyt

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/radio/musicradiochannelthread.h \
    $$PWD/radio/musicradioplaylistthread.h \
    $$PWD/radio/musicradiosongsthread.h \
    $$PWD/radio/musicradiothreadabstract.h \
    $$PWD/nr/musicdatadownloadthread.h \
    $$PWD/nr/musictextdownloadthread.h \
    $$PWD/nr/musicdatatagdownloadthread.h \
    $$PWD/tt/musicdownloadttinterface.h \
    $$PWD/tt/musicttdatadownloadthread.h \
    $$PWD/tt/musictttextdownloadthread.h \
    $$PWD/tt/musicdownloadqueryttthread.h \
    $$PWD/tt/musicdownloadqueryttalbumthread.h \
    $$PWD/tt/musicdownloadqueryttartistthread.h \
    $$PWD/tt/musicdownloadttinterface.h \
    $$PWD/wy/musicdownloadwyinterface.h \
    $$PWD/wy/musicdownloadquerywythread.h \
    $$PWD/wy/musicwytextdownloadthread.h \
    $$PWD/wy/musicwycommentsthread.h \
    $$PWD/wy/musicwydiscoverlistthread.h \
    $$PWD/wy/musicdownloadquerywyalbumthread.h \
    $$PWD/wy/musicdownloadquerywyartistthread.h \
    $$PWD/xm/musicdownloadxminterface.h \
    $$PWD/xm/musicdownloadqueryxmthread.h \
    $$PWD/xm/musicdownloadqueryxmalbumthread.h \
    $$PWD/xm/musicdownloadqueryxmartistthread.h \
    $$PWD/xm/musicxmtextdownloadthread.h \
    $$PWD/kg/musicdownloadquerykgthread.h \
    $$PWD/kg/musicdownloadquerykgalbumthread.h \
    $$PWD/kg/musicdownloadquerykgartistthread.h \
    $$PWD/kg/musicdownloadkginterface.h \
    $$PWD/kw/musicdownloadquerykwthread.h \
    $$PWD/kw/musicdownloadquerykwalbumthread.h \
    $$PWD/kw/musicdownloadquerykwartistthread.h \
    $$PWD/kw/musicdownloadkwinterface.h \
    $$PWD/kw/musickwtextdownloadthread.h \
    $$PWD/bd/musicdownloadquerybdthread.h \
    $$PWD/bd/musicdownloadquerybdalbumthread.h \
    $$PWD/bd/musicdownloadquerybdartistthread.h \
    $$PWD/bd/musicdownloadbdinterface.h \
    $$PWD/qq/musicdownloadqueryqqthread.h \
    $$PWD/qq/musicdownloadqueryqqalbumthread.h \
    $$PWD/qq/musicdownloadqueryqqartistthread.h \
    $$PWD/qq/musicdownloadqqinterface.h \
    $$PWD/qq/musicqqtextdownloadthread.h \
    $$PWD/yyt/musicdownloadqueryyytthread.h \
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
    $$PWD/nr/musicdatatagdownloadthread.cpp \
    $$PWD/tt/musicttdatadownloadthread.cpp \
    $$PWD/tt/musictttextdownloadthread.cpp \
    $$PWD/tt/musicdownloadqueryttthread.cpp \
    $$PWD/tt/musicdownloadqueryttalbumthread.cpp \
    $$PWD/tt/musicdownloadqueryttartistthread.cpp \
    $$PWD/tt/musicdownloadttinterface.cpp \
    $$PWD/wy/musicdownloadwyinterface.cpp \
    $$PWD/wy/musicdownloadquerywythread.cpp \
    $$PWD/wy/musicwytextdownloadthread.cpp \
    $$PWD/wy/musicwycommentsthread.cpp \
    $$PWD/wy/musicwydiscoverlistthread.cpp \
    $$PWD/wy/musicdownloadquerywyalbumthread.cpp \
    $$PWD/wy/musicdownloadquerywyartistthread.cpp \
    $$PWD/xm/musicdownloadxminterface.cpp \
    $$PWD/xm/musicdownloadqueryxmthread.cpp \
    $$PWD/xm/musicdownloadqueryxmalbumthread.cpp \
    $$PWD/xm/musicdownloadqueryxmartistthread.cpp \
    $$PWD/xm/musicxmtextdownloadthread.cpp \
    $$PWD/kg/musicdownloadquerykgthread.cpp \
    $$PWD/kg/musicdownloadquerykgalbumthread.cpp \
    $$PWD/kg/musicdownloadquerykgartistthread.cpp \
    $$PWD/kg/musicdownloadkginterface.cpp \
    $$PWD/kw/musicdownloadquerykwthread.cpp \
    $$PWD/kw/musicdownloadquerykwalbumthread.cpp \
    $$PWD/kw/musicdownloadquerykwartistthread.cpp \
    $$PWD/kw/musicdownloadkwinterface.cpp \
    $$PWD/kw/musickwtextdownloadthread.cpp \
    $$PWD/bd/musicdownloadquerybdthread.cpp \
    $$PWD/bd/musicdownloadquerybdalbumthread.cpp \
    $$PWD/bd/musicdownloadquerybdartistthread.cpp \
    $$PWD/bd/musicdownloadbdinterface.cpp \
    $$PWD/qq/musicdownloadqueryqqthread.cpp \
    $$PWD/qq/musicdownloadqueryqqalbumthread.cpp \
    $$PWD/qq/musicdownloadqueryqqartistthread.cpp \
    $$PWD/qq/musicdownloadqqinterface.cpp \
    $$PWD/qq/musicqqtextdownloadthread.cpp \
    $$PWD/yyt/musicdownloadqueryyytthread.cpp \
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
