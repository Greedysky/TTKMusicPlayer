# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = lib

include(../TTKVersion.pri)
unix:VERSION += $$TTKMusicPlayer

win32:TARGET = ../../bin/$$TTKMusicPlayer/TTKUi
unix:TARGET = ../lib/$$TTKMusicPlayer/TTKUi

RESOURCES += \
    MusicPlayer.qrc \
    MusicThirdParty.qrc
    
win32{
    RC_FILE = TTKQrc.rc
}
