# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2019 Greedysky Studio
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

INCLUDEPATH += $$PWD

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musicremotewidget.h \
    $$PWD/musicremotewidgetforsquare.h \
    $$PWD/musicremotewidgetforrectangle.h \
    $$PWD/musicremotewidgetfordiamond.h \
    $$PWD/musicremotewidgetforcircle.h \
    $$PWD/musicremotewidgetforsimplestyle.h \
    $$PWD/musicremotewidgetforcomplexstyle.h \
    $$PWD/musicremotewidgetforstrip.h \
    $$PWD/musicremotewidgetforripples.h \
    $$PWD/musicremotewidgetforrayswave.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicremotewidget.cpp \
    $$PWD/musicremotewidgetforsquare.cpp \
    $$PWD/musicremotewidgetforrectangle.cpp \
    $$PWD/musicremotewidgetfordiamond.cpp \
    $$PWD/musicremotewidgetforcircle.cpp \
    $$PWD/musicremotewidgetforsimplestyle.cpp \
    $$PWD/musicremotewidgetforcomplexstyle.cpp \
    $$PWD/musicremotewidgetforstrip.cpp \
    $$PWD/musicremotewidgetforripples.cpp \
    $$PWD/musicremotewidgetforrayswave.cpp

}
