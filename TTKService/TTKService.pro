# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = app

contains(CONFIG, TTK_BUILD_LIB){
    CONFIG -= TTK_BUILD_LIB
}

CONFIG += TTK_NO_MSVC_LINK_NEED
win32{
    msvc{
        CONFIG -= TTK_NO_MSVC_LINK_NEED
    }
}
include(../TTKMusicPlayer.pri)
unix:VERSION += $$TTKMusicPlayer

win32{
    TARGET = ../../bin/$$TTKMusicPlayer/TTKService
    LIBS += -L../bin/$$TTKMusicPlayer -lTTKCore -lTTKDumper
}
unix{
    TARGET = ../lib/$$TTKMusicPlayer/TTKService
    LIBS += -L./lib/$$TTKMusicPlayer -lTTKCore
}


INCLUDEPATH += ../TTKCore

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    ../TTKCore/musicapplication.h \
    ../TTKCore/musicleftareawidget.h \
    ../TTKCore/musictopareawidget.h \
    ../TTKCore/musicrightareawidget.h \
    ../TTKCore/musicbottomareawidget.h \
    ../TTKCore/musicapplicationobject.h

}

SOURCES += \
    musicservicemain.cpp

win32{
    RC_FILE = TTKService.rc
}
