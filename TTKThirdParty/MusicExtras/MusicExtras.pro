# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../TTKVersion.pri)
unix:VERSION += $$TTKMusicPlayer

win32{
    TARGET = ../../../bin/$$TTKMusicPlayer/TTKExtras
    msvc{
        LIBS += -luser32
    }
}
unix:TARGET = ../../lib/$$TTKMusicPlayer/TTKExtras
TEMPLATE = lib

win32:msvc{
    CONFIG +=c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
}

win32{
    LIBS += -L../../bin/$$TTKMusicPlayer -lTTKUi
}
unix:!mac{
    LIBS += -L../../lib/$$TTKMusicPlayer -lTTKUi
}

include(../TTKExtrasDefine.pri)

include(qaes/QAes.pri)
include(qhz2py/QHz2Py.pri)
include(qiniu/QiNiu.pri)
include(qjson/QJson.pri)
include(qkugou/QKuGou.pri)
include(qrencode/QRencode.pri)
include(qshortcut/QShortCut.pri)

win32{
    RC_FILE = MusicExtras.rc
}
