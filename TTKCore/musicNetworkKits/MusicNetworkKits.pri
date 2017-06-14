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
               $$PWD/wy \
               $$PWD/xm \
               $$PWD/kg \
               $$PWD/kw \
               $$PWD/bd \
               $$PWD/qq \
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
    $$PWD/wy/musicdownloadwyinterface.h \
    $$PWD/wy/musicdownloadquerywythread.h \
    $$PWD/wy/musicwytextdownloadthread.h \
    $$PWD/wy/musicwydiscoverlistthread.h \
    $$PWD/wy/musicdownloadquerywyalbumthread.h \
    $$PWD/wy/musicdownloadquerywyartistthread.h \
    $$PWD/wy/musicdownloadquerywyplaylistthread.h \
    $$PWD/wy/musicwycommentsthread.h \
    $$PWD/xm/musicdownloadxminterface.h \
    $$PWD/xm/musicdownloadqueryxmthread.h \
    $$PWD/xm/musicdownloadqueryxmalbumthread.h \
    $$PWD/xm/musicdownloadqueryxmartistthread.h \
    $$PWD/xm/musicxmtextdownloadthread.h \
    $$PWD/xm/musicdownloadqueryxmplaylistthread.h \
    $$PWD/xm/musicxmdiscoverlistthread.h \
    $$PWD/xm/musicxmcommentsthread.h \
    $$PWD/kg/musicdownloadquerykgthread.h \
    $$PWD/kg/musicdownloadquerykgalbumthread.h \
    $$PWD/kg/musicdownloadquerykgartistthread.h \
    $$PWD/kg/musicdownloadkginterface.h \
    $$PWD/kg/musicdownloadquerykgplaylistthread.h \
    $$PWD/kg/musickgdiscoverlistthread.h \
    $$PWD/kg/musickgcommentsthread.h \
    $$PWD/kw/musicdownloadquerykwthread.h \
    $$PWD/kw/musicdownloadquerykwalbumthread.h \
    $$PWD/kw/musicdownloadquerykwartistthread.h \
    $$PWD/kw/musicdownloadkwinterface.h \
    $$PWD/kw/musickwtextdownloadthread.h \
    $$PWD/kw/musicdownloadquerykwplaylistthread.h \
    $$PWD/kw/musickwdiscoverlistthread.h \
    $$PWD/kw/musickwcommentsthread.h \
    $$PWD/bd/musicdownloadquerybdthread.h \
    $$PWD/bd/musicdownloadquerybdalbumthread.h \
    $$PWD/bd/musicdownloadquerybdartistthread.h \
    $$PWD/bd/musicdownloadbdinterface.h \
    $$PWD/bd/musicdownloadquerybdplaylistthread.h \
    $$PWD/bd/musicdownloadquerybdlearnthread.h \
    $$PWD/bd/musicbddiscoverlistthread.h \
    $$PWD/bd/musicbdcommentsthread.h \
    $$PWD/qq/musicdownloadqueryqqthread.h \
    $$PWD/qq/musicdownloadqueryqqalbumthread.h \
    $$PWD/qq/musicdownloadqueryqqartistthread.h \
    $$PWD/qq/musicdownloadqqinterface.h \
    $$PWD/qq/musicqqtextdownloadthread.h \
    $$PWD/qq/musicdownloadqueryqqplaylistthread.h \
    $$PWD/qq/musicqqdiscoverlistthread.h \
    $$PWD/qq/musicqqcommentsthread.h \
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
    $$PWD/musicidentifysongsthread.h \
    $$PWD/musicsourceupdatethread.h \
    $$PWD/musiccounterpvdownloadthread.h \
    $$PWD/musicdownloaddiscoverlistthread.h \
    $$PWD/musicdownloadpagingthread.h \
    $$PWD/musicdownloadcommentsthread.h

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
    $$PWD/wy/musicdownloadwyinterface.cpp \
    $$PWD/wy/musicdownloadquerywythread.cpp \
    $$PWD/wy/musicwytextdownloadthread.cpp \
    $$PWD/wy/musicwydiscoverlistthread.cpp \
    $$PWD/wy/musicdownloadquerywyalbumthread.cpp \
    $$PWD/wy/musicdownloadquerywyartistthread.cpp \
    $$PWD/wy/musicdownloadquerywyplaylistthread.cpp \
    $$PWD/wy/musicwycommentsthread.cpp \
    $$PWD/xm/musicdownloadxminterface.cpp \
    $$PWD/xm/musicdownloadqueryxmthread.cpp \
    $$PWD/xm/musicdownloadqueryxmalbumthread.cpp \
    $$PWD/xm/musicdownloadqueryxmartistthread.cpp \
    $$PWD/xm/musicxmtextdownloadthread.cpp \
    $$PWD/xm/musicdownloadqueryxmplaylistthread.cpp \
    $$PWD/xm/musicxmdiscoverlistthread.cpp \
    $$PWD/xm/musicxmcommentsthread.cpp \
    $$PWD/kg/musicdownloadquerykgthread.cpp \
    $$PWD/kg/musicdownloadquerykgalbumthread.cpp \
    $$PWD/kg/musicdownloadquerykgartistthread.cpp \
    $$PWD/kg/musicdownloadkginterface.cpp \
    $$PWD/kg/musicdownloadquerykgplaylistthread.cpp \
    $$PWD/kg/musickgdiscoverlistthread.cpp \
    $$PWD/kg/musickgcommentsthread.cpp \
    $$PWD/kw/musicdownloadquerykwthread.cpp \
    $$PWD/kw/musicdownloadquerykwalbumthread.cpp \
    $$PWD/kw/musicdownloadquerykwartistthread.cpp \
    $$PWD/kw/musicdownloadkwinterface.cpp \
    $$PWD/kw/musickwtextdownloadthread.cpp \
    $$PWD/kw/musicdownloadquerykwplaylistthread.cpp \
    $$PWD/kw/musickwdiscoverlistthread.cpp \
    $$PWD/kw/musickwcommentsthread.cpp \
    $$PWD/bd/musicdownloadquerybdthread.cpp \
    $$PWD/bd/musicdownloadquerybdalbumthread.cpp \
    $$PWD/bd/musicdownloadquerybdartistthread.cpp \
    $$PWD/bd/musicdownloadbdinterface.cpp \
    $$PWD/bd/musicdownloadquerybdplaylistthread.cpp \
    $$PWD/bd/musicdownloadquerybdlearnthread.cpp \
    $$PWD/bd/musicbddiscoverlistthread.cpp \
    $$PWD/bd/musicbdcommentsthread.cpp \
    $$PWD/qq/musicdownloadqueryqqthread.cpp \
    $$PWD/qq/musicdownloadqueryqqalbumthread.cpp \
    $$PWD/qq/musicdownloadqueryqqartistthread.cpp \
    $$PWD/qq/musicdownloadqqinterface.cpp \
    $$PWD/qq/musicqqtextdownloadthread.cpp \
    $$PWD/qq/musicdownloadqueryqqplaylistthread.cpp \
    $$PWD/qq/musicqqdiscoverlistthread.cpp \
    $$PWD/qq/musicqqcommentsthread.cpp \
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
    $$PWD/musicidentifysongsthread.cpp \
    $$PWD/musicsourceupdatethread.cpp \
    $$PWD/musiccounterpvdownloadthread.cpp \
    $$PWD/musicdownloaddiscoverlistthread.cpp \
    $$PWD/musicdownloadpagingthread.cpp \
    $$PWD/musicdownloadcommentsthread.cpp

}
