# ***************************************************************************
# * This file is part of the TTK Library Module project
# * Copyright (C) 2015 - 2025 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU Lesser General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU Lesser General Public License for more details.
#
# * You should have received a copy of the GNU Lesser General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
# ***************************************************************************

TEMPLATE = lib
CONFIG += console plugin lib

SOURCES += main.cpp

greaterThan(QT_MAJOR_VERSION, 4){ #Qt5
    load(configure)

    # check default module
    qtCompileTest(default){
        message("default module test is always true")
    }else{
        message("default module test failed, check the platform compiler")
    }

    # check strlcat module
    qtCompileTest(strlcat){
        HAVE_STRLCAT = 1
        message("string.h has strlcat function module")
    }else{
        HAVE_STRLCAT = 0
        message("string.h hasn't strlcat function module, use compat module")
    }

    # check strlcpy module
    qtCompileTest(strlcpy){
        HAVE_STRLCPY = 1
        message("string.h has strlcpy function module")
    }else{
        HAVE_STRLCPY = 0
        message("string.h hasn't strlcpy function module, use compat module")
    }

    CONFIG+=recheck
}else{
    HAVE_STRLCAT = 0
    HAVE_STRLCPY = 0
}

# configure ttkconfig file
ttkconfig.input = $$PWD/ttkconfig.h.qmake
ttkconfig.output = $$OUT_PWD/../../Generate/ttkconfig.h
QMAKE_SUBSTITUTES += ttkconfig
