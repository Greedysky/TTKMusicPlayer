# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

QT       += core gui testlib
equals(QT_MAJOR_VERSION, 5){
QT       += widgets
}

win32:msvc{
    CONFIG +=c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
}

INCLUDEPATH += $$PWD/../

TARGET = TTKTest

TEMPLATE = app

HEADERS  += \
    musicautotest.h \
    tst_codelines.h

SOURCES  += \
    musicapplicationmain.cpp \
    tst_codelines.cpp

