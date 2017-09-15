# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = app

contains(CONFIG, MUSIC_BUILD_LIB){
    CONFIG -= MUSIC_BUILD_LIB
}

CONFIG += MUSIC_NO_MSVC_LINK_NEED
win32{
    msvc{
        CONFIG -= MUSIC_NO_MSVC_LINK_NEED
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
    LIBS += -L./lib/$$TTKMusicPlayer -lTTKCore -lTTKDumper
}


INCLUDEPATH += ../TTKModule

!contains(CONFIG, MUSIC_NO_MSVC_LINK_NEED){
HEADERS  += \
    ../TTKModule/musicapplication.h \
    ../TTKModule/musicleftareawidget.h \
    ../TTKModule/musictopareawidget.h \
    ../TTKModule/musicrightareawidget.h \
    ../TTKModule/musicbottomareawidget.h \
    ../TTKModule/musicapplicationobject.h

}

SOURCES += \
    musicservicemain.cpp

win32{
    RC_FILE = TTKService.rc
}
