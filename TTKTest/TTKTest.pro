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

QT       += core testlib gui
equals(QT_MAJOR_VERSION, 5){
QT       += widgets
}

TARGET = TTKTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS  += \
    musicautotest.h

SOURCES  += \
    musicapplicationmain.cpp

