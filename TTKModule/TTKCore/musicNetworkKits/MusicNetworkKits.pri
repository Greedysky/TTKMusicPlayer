# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2022 Greedysky Studio
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
# ***************************************************************************

INCLUDEPATH += \
    $$PWD \
    $$PWD/core \
    $$PWD/image \
    $$PWD/tools \
    $$PWD/radio/fm \
    $$PWD/radio/dj \
    $$PWD/radio/mv \
    $$PWD/music/wy \
    $$PWD/music/kg \
    $$PWD/music/kw \
    $$PWD/music/bd \
    $$PWD/music/tx

HEADERS += \
    $$PWD/core/musicabstractqueryrequest.h \
    $$PWD/core/musicabstractnetwork.h \
    $$PWD/core/musicabstractdownloadrequest.h \
    $$PWD/core/musicdownloadimagerequest.h \
    $$PWD/core/musicpagequeryrequest.h \
    $$PWD/core/musicqueryalbumrequest.h \
    $$PWD/core/musicqueryartistrequest.h \
    $$PWD/core/musicqueryartistlistrequest.h \
    $$PWD/core/musicqueryplaylistrequest.h \
    $$PWD/core/musicquerytoplistrequest.h \
    $$PWD/core/musicquerymovierequest.h \
    $$PWD/core/musicsongsuggestrequest.h \
    $$PWD/core/musictranslationrequest.h \
    $$PWD/core/musiccommentsrequest.h \
    $$PWD/core/musicdiscoverlistrequest.h \
    $$PWD/image/musicdownloadbackgroundrequest.h \
    $$PWD/image/musicbpdownloadimagerequest.h \
    $$PWD/image/musicdownloadskin.h \
    $$PWD/image/musicabstractdownloadskinrequest.h \
    $$PWD/image/musicdownloadbingskinrequest.h \
    $$PWD/image/musicdownloadthunderskinrequest.h \
    $$PWD/image/musicdownloadbirdskinrequest.h \
    $$PWD/tools/musicnetworkthread.h \
    $$PWD/tools/musicnetworkproxy.h \
    $$PWD/tools/musicnetworkoperator.h \
    $$PWD/tools/musicdownloaddatarequest.h \
    $$PWD/tools/musicdownloadtextrequest.h \
    $$PWD/tools/musicdownloadtagdatarequest.h \
    $$PWD/tools/musicdownloadsourcerequest.h \
    $$PWD/tools/musicdownloadqueuerequest.h \
    $$PWD/tools/musicidentifysongsrequest.h \
    $$PWD/tools/musicsourceupdaterequest.h \
    $$PWD/tools/musicdownloadcounterpvrequest.h \
    $$PWD/tools/musicdownloadmanager.h \
    $$PWD/tools/musicqueryrecommendrequest.h \
    $$PWD/radio/fm/musicfmradiochannelrequest.h \
    $$PWD/radio/fm/musicfmradiodownloadtextrequest.h \
    $$PWD/radio/fm/musicfmradiosongsrequest.h \
    $$PWD/radio/fm/musicabstractfmradiorequest.h \
    $$PWD/radio/dj/musicdjradiocategoryrequest.h \
    $$PWD/radio/dj/musicdjradioprogramrequest.h \
    $$PWD/radio/dj/musicdjradioprogramcategoryrequest.h \
    $$PWD/radio/dj/musicabstractdjradiorequest.h \
    $$PWD/radio/mv/musicmvradiocategoryrequest.h \
    $$PWD/radio/mv/musicmvradioprogramrequest.h \
    $$PWD/radio/mv/musicabstractmvradiorequest.h \
    $$PWD/music/wy/musicwyqueryinterface.h \
    $$PWD/music/wy/musicwyqueryrequest.h \
    $$PWD/music/wy/musicwyqueryalbumrequest.h \
    $$PWD/music/wy/musicwyqueryartistrequest.h \
    $$PWD/music/wy/musicwyqueryplaylistrequest.h \
    $$PWD/music/wy/musicwyquerytoplistrequest.h \
    $$PWD/music/wy/musicwyquerymovierequest.h \
    $$PWD/music/wy/musicwyqueryartistlistrequest.h \
    $$PWD/music/wy/musicwydownloadtextrequest.h \
    $$PWD/music/wy/musicwycommentsrequest.h \
    $$PWD/music/wy/musicwysongsuggestrequest.h \
    $$PWD/music/wy/musicwytranslationrequest.h \
    $$PWD/music/wy/musicwydiscoverlistrequest.h \
    $$PWD/music/kg/musickgqueryinterface.h \
    $$PWD/music/kg/musickgqueryrequest.h \
    $$PWD/music/kg/musickgqueryalbumrequest.h \
    $$PWD/music/kg/musickgqueryartistrequest.h \
    $$PWD/music/kg/musickgqueryplaylistrequest.h \
    $$PWD/music/kg/musickgquerytoplistrequest.h \
    $$PWD/music/kg/musickgquerymovierequest.h \
    $$PWD/music/kg/musickgqueryartistlistrequest.h \
    $$PWD/music/kg/musickgdiscoverlistrequest.h \
    $$PWD/music/kg/musickgcommentsrequest.h \
    $$PWD/music/kg/musickgsongsuggestrequest.h \
    $$PWD/music/kw/musickwqueryinterface.h \
    $$PWD/music/kw/musickwqueryrequest.h \
    $$PWD/music/kw/musickwqueryalbumrequest.h \
    $$PWD/music/kw/musickwqueryartistrequest.h \
    $$PWD/music/kw/musickwqueryplaylistrequest.h \
    $$PWD/music/kw/musickwquerytoplistrequest.h \
    $$PWD/music/kw/musickwquerymovierequest.h \
    $$PWD/music/kw/musickwqueryartistlistrequest.h \
    $$PWD/music/kw/musickwdownloadtextrequest.h \
    $$PWD/music/kw/musickwdownloadimagerequest.h \
    $$PWD/music/kw/musickwdiscoverlistrequest.h \
    $$PWD/music/kw/musickwcommentsrequest.h \
    $$PWD/music/kw/musickwsongsuggestrequest.h \
    $$PWD/music/bd/musicbdqueryinterface.h \
    $$PWD/music/bd/musicbdquerylearnrequest.h \
    $$PWD/music/bd/musicbdtranslationrequest.h \
    $$PWD/music/tx/musictxqueryinterface.h \
    $$PWD/music/tx/musictxdownloadimagerequest.h \
    $$PWD/musicnetworkdefines.h \
    $$PWD/musicdownloadqueryfactory.h

SOURCES += \
    $$PWD/core/musicabstractqueryrequest.cpp \
    $$PWD/core/musicabstractnetwork.cpp \
    $$PWD/core/musicabstractdownloadrequest.cpp \
    $$PWD/core/musicdownloadimagerequest.cpp \
    $$PWD/core/musicpagequeryrequest.cpp \
    $$PWD/core/musicqueryalbumrequest.cpp \
    $$PWD/core/musicqueryartistrequest.cpp \
    $$PWD/core/musicqueryartistlistrequest.cpp \
    $$PWD/core/musicqueryplaylistrequest.cpp \
    $$PWD/core/musicquerytoplistrequest.cpp \
    $$PWD/core/musicquerymovierequest.cpp \
    $$PWD/core/musicsongsuggestrequest.cpp \
    $$PWD/core/musictranslationrequest.cpp \
    $$PWD/core/musiccommentsrequest.cpp \
    $$PWD/core/musicdiscoverlistrequest.cpp \
    $$PWD/image/musicdownloadbackgroundrequest.cpp \
    $$PWD/image/musicbpdownloadimagerequest.cpp \
    $$PWD/image/musicabstractdownloadskinrequest.cpp \
    $$PWD/image/musicdownloadbingskinrequest.cpp \
    $$PWD/image/musicdownloadthunderskinrequest.cpp \
    $$PWD/image/musicdownloadbirdskinrequest.cpp \
    $$PWD/tools/musicnetworkthread.cpp \
    $$PWD/tools/musicnetworkproxy.cpp \
    $$PWD/tools/musicnetworkoperator.cpp \
    $$PWD/tools/musicdownloaddatarequest.cpp \
    $$PWD/tools/musicdownloadtextrequest.cpp \
    $$PWD/tools/musicdownloadtagdatarequest.cpp \
    $$PWD/tools/musicdownloadsourcerequest.cpp \
    $$PWD/tools/musicdownloadqueuerequest.cpp \
    $$PWD/tools/musicidentifysongsrequest.cpp \
    $$PWD/tools/musicsourceupdaterequest.cpp \
    $$PWD/tools/musicdownloadcounterpvrequest.cpp \
    $$PWD/tools/musicdownloadmanager.cpp \
    $$PWD/tools/musicqueryrecommendrequest.cpp \
    $$PWD/radio/fm/musicfmradiochannelrequest.cpp \
    $$PWD/radio/fm/musicfmradiodownloadtextrequest.cpp \
    $$PWD/radio/fm/musicfmradiosongsrequest.cpp \
    $$PWD/radio/fm/musicabstractfmradiorequest.cpp \
    $$PWD/radio/dj/musicdjradiocategoryrequest.cpp \
    $$PWD/radio/dj/musicdjradioprogramrequest.cpp \
    $$PWD/radio/dj/musicdjradioprogramcategoryrequest.cpp \
    $$PWD/radio/dj/musicabstractdjradiorequest.cpp \
    $$PWD/radio/mv/musicmvradiocategoryrequest.cpp \
    $$PWD/radio/mv/musicmvradioprogramrequest.cpp \
    $$PWD/radio/mv/musicabstractmvradiorequest.cpp \
    $$PWD/music/wy/musicwyqueryinterface.cpp \
    $$PWD/music/wy/musicwyqueryrequest.cpp \
    $$PWD/music/wy/musicwyqueryalbumrequest.cpp \
    $$PWD/music/wy/musicwyqueryartistrequest.cpp \
    $$PWD/music/wy/musicwyqueryplaylistrequest.cpp \
    $$PWD/music/wy/musicwyquerytoplistrequest.cpp \
    $$PWD/music/wy/musicwyquerymovierequest.cpp \
    $$PWD/music/wy/musicwyqueryartistlistrequest.cpp \
    $$PWD/music/wy/musicwydownloadtextrequest.cpp \
    $$PWD/music/wy/musicwycommentsrequest.cpp \
    $$PWD/music/wy/musicwysongsuggestrequest.cpp \
    $$PWD/music/wy/musicwytranslationrequest.cpp \
    $$PWD/music/wy/musicwydiscoverlistrequest.cpp \
    $$PWD/music/kg/musickgqueryinterface.cpp \
    $$PWD/music/kg/musickgqueryrequest.cpp \
    $$PWD/music/kg/musickgqueryalbumrequest.cpp \
    $$PWD/music/kg/musickgqueryartistrequest.cpp \
    $$PWD/music/kg/musickgqueryplaylistrequest.cpp \
    $$PWD/music/kg/musickgquerytoplistrequest.cpp \
    $$PWD/music/kg/musickgquerymovierequest.cpp \
    $$PWD/music/kg/musickgqueryartistlistrequest.cpp \
    $$PWD/music/kg/musickgdiscoverlistrequest.cpp \
    $$PWD/music/kg/musickgcommentsrequest.cpp \
    $$PWD/music/kg/musickgsongsuggestrequest.cpp \
    $$PWD/music/kw/musickwqueryinterface.cpp \
    $$PWD/music/kw/musickwqueryrequest.cpp \
    $$PWD/music/kw/musickwqueryalbumrequest.cpp \
    $$PWD/music/kw/musickwqueryartistrequest.cpp \
    $$PWD/music/kw/musickwqueryplaylistrequest.cpp \
    $$PWD/music/kw/musickwquerytoplistrequest.cpp \
    $$PWD/music/kw/musickwquerymovierequest.cpp \
    $$PWD/music/kw/musickwqueryartistlistrequest.cpp \
    $$PWD/music/kw/musickwdownloadtextrequest.cpp \
    $$PWD/music/kw/musickwdownloadimagerequest.cpp \
    $$PWD/music/kw/musickwdiscoverlistrequest.cpp \
    $$PWD/music/kw/musickwcommentsrequest.cpp \
    $$PWD/music/kw/musickwsongsuggestrequest.cpp \
    $$PWD/music/bd/musicbdqueryinterface.cpp \
    $$PWD/music/bd/musicbdquerylearnrequest.cpp \
    $$PWD/music/bd/musicbdtranslationrequest.cpp \
    $$PWD/music/tx/musictxqueryinterface.cpp \
    $$PWD/music/tx/musictxdownloadimagerequest.cpp \
    $$PWD/musicdownloadqueryfactory.cpp
