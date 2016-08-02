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

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32{
    TARGET = ../../../bin/MusicExtras
    msvc{
        LIBS += -luser32
    }
}
unix:TARGET = ../../lib/MusicExtras
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11
unix:VERSION += 2.3.2.0

INCLUDEPATH += ../../
HEADERS  += \
    musicextrasglobaldefine.h \
    ../../musicglobal.h \
    ../../musicprivate.h

RESOURCES += \
    ../../TTKQrc/MusicThirdParty.qrc

include(hz2py/Hz2Py.pri)
include(kugou/KuGou.pri)
include(qiniu/QiNiu.pri)
include(qjson/QJson.pri)
include(qrencode/QRencode.pri)
include(shortcut/ShortCut.pri)
