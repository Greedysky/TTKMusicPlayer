# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

QT       += core
unix:VERSION += 1.0.0.0

win32:TARGET = ../../../bin/TTKInit
unix:TARGET = ../../lib/TTKInit

TEMPLATE = app

win32:msvc{
    CONFIG +=c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
}

INCLUDEPATH += \
    ../ \
    ../../ \
    ../../TTKModule/TTKCore/musicCoreKits

SOURCES += \
    musicinitmain.cpp \
    musicinitobject.cpp

HEADERS += \
    ../musicrunglobaldefine.h \
    musicinitobject.h

RESOURCES += \
    ../../TTKQrc/MusicApp.qrc

win32{
    RC_FILE = TTKInit.rc
}
