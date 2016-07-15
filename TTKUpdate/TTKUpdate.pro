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
equals(QT_MAJOR_VERSION, 5){
QT       += widgets
}

TEMPLATE = app
win32{
  TARGET = ../../bin/TTKUpdate
}
unix{
  TARGET = ../lib/TTKUpdate
}


UI_DIR = ./.build/ui/
MOC_DIR = ./.build/moc/
OBJECTS_DIR = ./.build/obj
RCC_DIR = ./.build/rcc

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += \
        ../TTKCore/musicCore \
        ../

HEADERS += \
    ../TTKCore/musicCore/musicuiobject.h \
    ../musicglobal.h \
    musicupdatemainwindow.h


SOURCES += \
    musicapplicationmain.cpp \
    musicupdatemainwindow.cpp


win32{
    RC_FILE = TTKUpdate.rc
}

RESOURCES += \
    ../TTKQrc/MusicUpdate.qrc

include(musicCore/MusicCore.pri)
include(musicWidget/MusicWidget.pri)
include(musicZip/MusicZip.pri)
include(musicUi/MusicUi.pri)
