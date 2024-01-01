# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2024 Greedysky Studio
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
    $$PWD/image/background \
    $$PWD/image/skin \
    $$PWD/tools \
    $$PWD/radio/fm \
    $$PWD/radio/dj \
    $$PWD/radio/mv \
    $$PWD/music/core \
    $$PWD/music/kg \
    $$PWD/music/kw \
    $$PWD/music/wy \
    $$PWD/translation

HEADERS += \
    $$PWD/core/musicabstractqueryrequest.h \
    $$PWD/core/musicabstractnetwork.h \
    $$PWD/core/musicabstractdownloadrequest.h \
    $$PWD/core/musicpagequeryrequest.h \
    $$PWD/image/background/musicabstractdownloadimagerequest.h \
    $$PWD/image/background/musicdownloadbackgroundrequest.h \
    $$PWD/image/background/musicbpdownloadimagerequest.h \
    $$PWD/image/background/musickgdownloadimagerequest.h \
    $$PWD/image/background/musickwdownloadimagerequest.h \
    $$PWD/image/background/musictxdownloadimagerequest.h \
    $$PWD/image/skin/musicabstractdownloadskinrequest.h \
    $$PWD/image/skin/musicdownloadbingskinrequest.h \
    $$PWD/image/skin/musicdownloadbirdskinrequest.h \
    $$PWD/image/skin/musicdownloadthunderskinrequest.h \
    $$PWD/tools/musicnetworkthread.h \
    $$PWD/tools/musicnetworkproxy.h \
    $$PWD/tools/musicnetworkoperator.h \
    $$PWD/tools/musiccoversourcerequest.h \
    $$PWD/tools/musicdatasourcerequest.h \
    $$PWD/tools/musicdownloaddatarequest.h \
    $$PWD/tools/musicdownloadtextrequest.h \
    $$PWD/tools/musicdownloadmetadatarequest.h \
    $$PWD/tools/musicdownloadqueuerequest.h \
    $$PWD/tools/musicidentifysongrequest.h \
    $$PWD/tools/musicsourceupdaterequest.h \
    $$PWD/tools/musiccounterpvrequest.h \
    $$PWD/tools/musicdownloadmanager.h \
    $$PWD/tools/musicsongrecommendrequest.h \
    $$PWD/tools/musicsongsuggestrequest.h \
    $$PWD/radio/fm/musicfmradiosongrequest.h \
    $$PWD/radio/dj/musicabstractdjradiorequest.h \
    $$PWD/radio/dj/musicdjradiocategoryrequest.h \
    $$PWD/radio/dj/musicdjradioprogramrequest.h \
    $$PWD/radio/dj/musicdjradioprogramcategoryrequest.h \
    $$PWD/radio/mv/musicabstractmvradiorequest.h \
    $$PWD/radio/mv/musicmvradiocategoryrequest.h \
    $$PWD/radio/mv/musicmvradioprogramrequest.h \
    $$PWD/music/core/musicqueryalbumrequest.h \
    $$PWD/music/core/musicqueryartistrequest.h \
    $$PWD/music/core/musicqueryartistlistrequest.h \
    $$PWD/music/core/musicqueryplaylistrequest.h \
    $$PWD/music/core/musicquerytoplistrequest.h \
    $$PWD/music/core/musicquerymovierequest.h \
    $$PWD/music/core/musiccommentsrequest.h \
    $$PWD/music/core/musicdiscoverlistrequest.h \
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
    $$PWD/music/kw/musickwqueryinterface.h \
    $$PWD/music/kw/musickwqueryrequest.h \
    $$PWD/music/kw/musickwqueryalbumrequest.h \
    $$PWD/music/kw/musickwqueryartistrequest.h \
    $$PWD/music/kw/musickwqueryplaylistrequest.h \
    $$PWD/music/kw/musickwquerytoplistrequest.h \
    $$PWD/music/kw/musickwquerymovierequest.h \
    $$PWD/music/kw/musickwqueryartistlistrequest.h \
    $$PWD/music/kw/musickwdownloadtextrequest.h \
    $$PWD/music/kw/musickwdiscoverlistrequest.h \
    $$PWD/music/kw/musickwcommentsrequest.h \
    $$PWD/translation/musicabstracttranslationrequest.h \
    $$PWD/translation/musictranslationrequest.h \
    $$PWD/translation/musicbdtranslationrequest.h \
    $$PWD/translation/musicbingtranslationrequest.h \
    $$PWD/translation/musicwytranslationrequest.h \
    $$PWD/translation/musicydtranslationrequest.h \
    $$PWD/musicnetworkdefines.h \
    $$PWD/musicdownloadqueryfactory.h

SOURCES += \
    $$PWD/core/musicabstractqueryrequest.cpp \
    $$PWD/core/musicabstractnetwork.cpp \
    $$PWD/core/musicabstractdownloadrequest.cpp \
    $$PWD/core/musicpagequeryrequest.cpp \
    $$PWD/image/background/musicabstractdownloadimagerequest.cpp \
    $$PWD/image/background/musicdownloadbackgroundrequest.cpp \
    $$PWD/image/background/musicbpdownloadimagerequest.cpp \
    $$PWD/image/background/musickgdownloadimagerequest.cpp \
    $$PWD/image/background/musickwdownloadimagerequest.cpp \
    $$PWD/image/background/musictxdownloadimagerequest.cpp \
    $$PWD/image/skin/musicabstractdownloadskinrequest.cpp \
    $$PWD/image/skin/musicdownloadbingskinrequest.cpp \
    $$PWD/image/skin/musicdownloadbirdskinrequest.cpp \
    $$PWD/image/skin/musicdownloadthunderskinrequest.cpp \
    $$PWD/tools/musicnetworkthread.cpp \
    $$PWD/tools/musicnetworkproxy.cpp \
    $$PWD/tools/musicnetworkoperator.cpp \
    $$PWD/tools/musicdatasourcerequest.cpp \
    $$PWD/tools/musicdownloaddatarequest.cpp \
    $$PWD/tools/musicdownloadtextrequest.cpp \
    $$PWD/tools/musicdownloadmetadatarequest.cpp \
    $$PWD/tools/musicdownloadqueuerequest.cpp \
    $$PWD/tools/musicidentifysongrequest.cpp \
    $$PWD/tools/musicsourceupdaterequest.cpp \
    $$PWD/tools/musiccounterpvrequest.cpp \
    $$PWD/tools/musicdownloadmanager.cpp \
    $$PWD/tools/musicsongrecommendrequest.cpp \
    $$PWD/tools/musicsongsuggestrequest.cpp \
    $$PWD/radio/fm/musicfmradiosongrequest.cpp \
    $$PWD/radio/dj/musicabstractdjradiorequest.cpp \
    $$PWD/radio/dj/musicdjradiocategoryrequest.cpp \
    $$PWD/radio/dj/musicdjradioprogramrequest.cpp \
    $$PWD/radio/dj/musicdjradioprogramcategoryrequest.cpp \
    $$PWD/radio/mv/musicabstractmvradiorequest.cpp \
    $$PWD/radio/mv/musicmvradiocategoryrequest.cpp \
    $$PWD/radio/mv/musicmvradioprogramrequest.cpp \
    $$PWD/music/core/musicqueryalbumrequest.cpp \
    $$PWD/music/core/musicqueryartistrequest.cpp \
    $$PWD/music/core/musicqueryartistlistrequest.cpp \
    $$PWD/music/core/musicqueryplaylistrequest.cpp \
    $$PWD/music/core/musicquerytoplistrequest.cpp \
    $$PWD/music/core/musicquerymovierequest.cpp \
    $$PWD/music/core/musiccommentsrequest.cpp \
    $$PWD/music/core/musicdiscoverlistrequest.cpp \
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
    $$PWD/music/kw/musickwqueryinterface.cpp \
    $$PWD/music/kw/musickwqueryrequest.cpp \
    $$PWD/music/kw/musickwqueryalbumrequest.cpp \
    $$PWD/music/kw/musickwqueryartistrequest.cpp \
    $$PWD/music/kw/musickwqueryplaylistrequest.cpp \
    $$PWD/music/kw/musickwquerytoplistrequest.cpp \
    $$PWD/music/kw/musickwquerymovierequest.cpp \
    $$PWD/music/kw/musickwqueryartistlistrequest.cpp \
    $$PWD/music/kw/musickwdownloadtextrequest.cpp \
    $$PWD/music/kw/musickwdiscoverlistrequest.cpp \
    $$PWD/music/kw/musickwcommentsrequest.cpp \
    $$PWD/translation/musicabstracttranslationrequest.cpp \
    $$PWD/translation/musictranslationrequest.cpp \
    $$PWD/translation/musicbdtranslationrequest.cpp \
    $$PWD/translation/musicbingtranslationrequest.cpp \
    $$PWD/translation/musicwytranslationrequest.cpp \
    $$PWD/translation/musicydtranslationrequest.cpp \
    $$PWD/musicdownloadqueryfactory.cpp
