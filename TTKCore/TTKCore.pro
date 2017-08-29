# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = lib
CONFIG += TTK_BUILD_LIB

include(../TTKMusicPlayer.pri)
unix:VERSION += $$TTKMusicPlayer

win32:TARGET = ../../bin/$$TTKMusicPlayer/TTKCore
unix:TARGET = ../lib/$$TTKMusicPlayer/TTKCore

INCLUDEPATH += $$PWD

SOURCES += \
    musicapplication.cpp \
    musicleftareawidget.cpp \
    musictopareawidget.cpp \
    musicrightareawidget.cpp \
    musicbottomareawidget.cpp \
    musicapplicationobject.cpp


HEADERS  += \
    musicapplication.h \
    musicleftareawidget.h \
    musictopareawidget.h \
    musicrightareawidget.h \
    musicbottomareawidget.h \
    musicapplicationobject.h
    
win32{
    RC_FILE = TTKCore.rc
}
