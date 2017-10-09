# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

include(../../../TTKVersion.pri)

QT       += core gui
equals(QT_MAJOR_VERSION, 5){
QT       += widgets
}

win32:msvc{
    CONFIG +=c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
}

TARGET = toolsurl
TEMPLATE = app
DEFINES += MUSIC_LIBRARY

INCLUDEPATH += $$PWD/../../../ \
               $$PWD/../../../TTKModule/TTKCore/musicCoreKits \
               $$PWD/../../../TTKModule/TTKCore/musicUtilsKits

win32{
    LIBS += -L../../../bin/$$TTKMusicPlayer -lTTKCore
}

SOURCES += mainurl.cpp\
           toolsurl.cpp

HEADERS  += toolsurl.h

FORMS   += toolsurl.ui
