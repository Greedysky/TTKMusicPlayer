# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2017 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
#
# * You should have received a copy of the GNU General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
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
               $$PWD/yyt \
               $$PWD/ztl
               
!contains(CONFIG, MUSIC_NO_MSVC_LINK_NEED){
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
    $$PWD/wy/musicdownloadquerywytoplistthread.h \
    $$PWD/wy/musicwycommentsthread.h \
    $$PWD/xm/musicdownloadxminterface.h \
    $$PWD/xm/musicdownloadqueryxmthread.h \
    $$PWD/xm/musicdownloadqueryxmalbumthread.h \
    $$PWD/xm/musicdownloadqueryxmartistthread.h \
    $$PWD/xm/musicxmtextdownloadthread.h \
    $$PWD/xm/musicdownloadqueryxmplaylistthread.h \
    $$PWD/xm/musicdownloadqueryxmtoplistthread.h \
    $$PWD/xm/musicxmdiscoverlistthread.h \
    $$PWD/xm/musicxmcommentsthread.h \
    $$PWD/kg/musicdownloadquerykgthread.h \
    $$PWD/kg/musicdownloadquerykgalbumthread.h \
    $$PWD/kg/musicdownloadquerykgartistthread.h \
    $$PWD/kg/musicdownloadkginterface.h \
    $$PWD/kg/musicdownloadquerykgplaylistthread.h \
    $$PWD/kg/musicdownloadquerykgtoplistthread.h \
    $$PWD/kg/musickgdiscoverlistthread.h \
    $$PWD/kg/musickgcommentsthread.h \
    $$PWD/kw/musicdownloadquerykwthread.h \
    $$PWD/kw/musicdownloadquerykwalbumthread.h \
    $$PWD/kw/musicdownloadquerykwartistthread.h \
    $$PWD/kw/musicdownloadkwinterface.h \
    $$PWD/kw/musickwtextdownloadthread.h \
    $$PWD/kw/musicdownloadquerykwplaylistthread.h \
    $$PWD/kw/musicdownloadquerykwtoplistthread.h \
    $$PWD/kw/musickwdiscoverlistthread.h \
    $$PWD/kw/musickwcommentsthread.h \
    $$PWD/kw/musickwbackgroundthread.h \
    $$PWD/bd/musicdownloadquerybdthread.h \
    $$PWD/bd/musicdownloadquerybdalbumthread.h \
    $$PWD/bd/musicdownloadquerybdartistthread.h \
    $$PWD/bd/musicdownloadbdinterface.h \
    $$PWD/bd/musicdownloadquerybdplaylistthread.h \
    $$PWD/bd/musicdownloadquerybdlearnthread.h \
    $$PWD/bd/musicdownloadquerybdtoplistthread.h \
    $$PWD/bd/musicbddiscoverlistthread.h \
    $$PWD/bd/musicbdcommentsthread.h \
    $$PWD/qq/musicdownloadqueryqqthread.h \
    $$PWD/qq/musicdownloadqueryqqalbumthread.h \
    $$PWD/qq/musicdownloadqueryqqartistthread.h \
    $$PWD/qq/musicdownloadqqinterface.h \
    $$PWD/qq/musicqqtextdownloadthread.h \
    $$PWD/qq/musicdownloadqueryqqplaylistthread.h \
    $$PWD/qq/musicdownloadqueryqqtoplistthread.h \
    $$PWD/qq/musicqqdiscoverlistthread.h \
    $$PWD/qq/musicqqcommentsthread.h \
    $$PWD/qq/musicqqbackgroundthread.h \
    $$PWD/yyt/musicdownloadqueryyytthread.h \
    $$PWD/ztl/musicdownloadsourcethread.h \
    $$PWD/ztl/musicnetworkthread.h \
    $$PWD/ztl/musicnetworkproxy.h \
    $$PWD/ztl/musicnetworkoperator.h \
    $$PWD/ztl/musicdownloadbackgroundthread.h \
    $$PWD/ztl/musicdownloadqueuecache.h \
    $$PWD/ztl/musictranslationthread.h \
    $$PWD/ztl/musicvoicettsthread.h \
    $$PWD/ztl/musicidentifysongsthread.h \
    $$PWD/ztl/musicsourceupdatethread.h \
    $$PWD/ztl/musicdownloadcounterpvthread.h \
    $$PWD/ztl/musicdownloadbackgroundremotethread.h \
    $$PWD/musicnetworkabstract.h \
    $$PWD/musicdownloadthreadabstract.h \
    $$PWD/musicdownloadqueryfactory.h \
    $$PWD/musicdownloadquerythreadabstract.h \
    $$PWD/musicdownloadcommentsthread.h \
    $$PWD/musicdownloaddiscoverlistthread.h \
    $$PWD/musicdownloadpagingthread.h \
    $$PWD/musicdownloadqueryalbumthread.h \
    $$PWD/musicdownloadqueryartistthread.h \
    $$PWD/musicdownloadqueryplaylistthread.h \
    $$PWD/musicdownloadquerytoplistthread.h

}

contains(CONFIG, MUSIC_BUILD_LIB){
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
    $$PWD/wy/musicdownloadquerywytoplistthread.cpp \
    $$PWD/wy/musicwycommentsthread.cpp \
    $$PWD/xm/musicdownloadxminterface.cpp \
    $$PWD/xm/musicdownloadqueryxmthread.cpp \
    $$PWD/xm/musicdownloadqueryxmalbumthread.cpp \
    $$PWD/xm/musicdownloadqueryxmartistthread.cpp \
    $$PWD/xm/musicxmtextdownloadthread.cpp \
    $$PWD/xm/musicdownloadqueryxmplaylistthread.cpp \
    $$PWD/xm/musicdownloadqueryxmtoplistthread.cpp \
    $$PWD/xm/musicxmdiscoverlistthread.cpp \
    $$PWD/xm/musicxmcommentsthread.cpp \
    $$PWD/kg/musicdownloadquerykgthread.cpp \
    $$PWD/kg/musicdownloadquerykgalbumthread.cpp \
    $$PWD/kg/musicdownloadquerykgartistthread.cpp \
    $$PWD/kg/musicdownloadkginterface.cpp \
    $$PWD/kg/musicdownloadquerykgplaylistthread.cpp \
    $$PWD/kg/musicdownloadquerykgtoplistthread.cpp \
    $$PWD/kg/musickgdiscoverlistthread.cpp \
    $$PWD/kg/musickgcommentsthread.cpp \
    $$PWD/kw/musicdownloadquerykwthread.cpp \
    $$PWD/kw/musicdownloadquerykwalbumthread.cpp \
    $$PWD/kw/musicdownloadquerykwartistthread.cpp \
    $$PWD/kw/musicdownloadkwinterface.cpp \
    $$PWD/kw/musickwtextdownloadthread.cpp \
    $$PWD/kw/musicdownloadquerykwplaylistthread.cpp \
    $$PWD/kw/musicdownloadquerykwtoplistthread.cpp \
    $$PWD/kw/musickwdiscoverlistthread.cpp \
    $$PWD/kw/musickwcommentsthread.cpp \
    $$PWD/kw/musickwbackgroundthread.cpp \
    $$PWD/bd/musicdownloadquerybdthread.cpp \
    $$PWD/bd/musicdownloadquerybdalbumthread.cpp \
    $$PWD/bd/musicdownloadquerybdartistthread.cpp \
    $$PWD/bd/musicdownloadbdinterface.cpp \
    $$PWD/bd/musicdownloadquerybdplaylistthread.cpp \
    $$PWD/bd/musicdownloadquerybdlearnthread.cpp \
    $$PWD/bd/musicdownloadquerybdtoplistthread.cpp \
    $$PWD/bd/musicbddiscoverlistthread.cpp \
    $$PWD/bd/musicbdcommentsthread.cpp \
    $$PWD/qq/musicdownloadqueryqqthread.cpp \
    $$PWD/qq/musicdownloadqueryqqalbumthread.cpp \
    $$PWD/qq/musicdownloadqueryqqartistthread.cpp \
    $$PWD/qq/musicdownloadqqinterface.cpp \
    $$PWD/qq/musicqqtextdownloadthread.cpp \
    $$PWD/qq/musicdownloadqueryqqplaylistthread.cpp \
    $$PWD/qq/musicdownloadqueryqqtoplistthread.cpp \
    $$PWD/qq/musicqqdiscoverlistthread.cpp \
    $$PWD/qq/musicqqcommentsthread.cpp \
    $$PWD/qq/musicqqbackgroundthread.cpp \
    $$PWD/yyt/musicdownloadqueryyytthread.cpp \
    $$PWD/ztl/musicdownloadsourcethread.cpp \
    $$PWD/ztl/musicnetworkthread.cpp \
    $$PWD/ztl/musicnetworkproxy.cpp \
    $$PWD/ztl/musicnetworkoperator.cpp \
    $$PWD/ztl/musicdownloadbackgroundthread.cpp \
    $$PWD/ztl/musicdownloadqueuecache.cpp \
    $$PWD/ztl/musictranslationthread.cpp \
    $$PWD/ztl/musicvoicettsthread.cpp \
    $$PWD/ztl/musicidentifysongsthread.cpp \
    $$PWD/ztl/musicsourceupdatethread.cpp \
    $$PWD/ztl/musicdownloadcounterpvthread.cpp \
    $$PWD/ztl/musicdownloadbackgroundremotethread.cpp \
    $$PWD/musicnetworkabstract.cpp \
    $$PWD/musicdownloadthreadabstract.cpp \
    $$PWD/musicdownloadqueryfactory.cpp \
    $$PWD/musicdownloadquerythreadabstract.cpp \
    $$PWD/musicdownloadcommentsthread.cpp \
    $$PWD/musicdownloaddiscoverlistthread.cpp \
    $$PWD/musicdownloadpagingthread.cpp \
    $$PWD/musicdownloadqueryalbumthread.cpp \
    $$PWD/musicdownloadqueryartistthread.cpp \
    $$PWD/musicdownloadqueryplaylistthread.cpp \
    $$PWD/musicdownloadquerytoplistthread.cpp

}
