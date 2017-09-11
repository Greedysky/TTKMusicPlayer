# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musicdesktopwallpaperthread.h \
    $$PWD/musictimerautoobject.h \
    $$PWD/musiclocalsongsmanagerthread.h \
    $$PWD/musicaudiorecordercore.h \
    $$PWD/musicnetworktestthread.h \
    $$PWD/musicnetworkspeedtestthread.h \
    $$PWD/musicsongchecktoolscore.h \
    $$PWD/musicsongchecktoolsunit.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicdesktopwallpaperthread.cpp \
    $$PWD/musictimerautoobject.cpp \
    $$PWD/musiclocalsongsmanagerthread.cpp \
    $$PWD/musicaudiorecordercore.cpp \
    $$PWD/musicnetworktestthread.cpp \
    $$PWD/musicnetworkspeedtestthread.cpp \
    $$PWD/musicsongchecktoolscore.cpp

}
