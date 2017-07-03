# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = lib

include(../../TTKVersion.pri)

win32:TARGET = ../../../bin/$$TTKMusicPlayer/TTKWatcher
unix:TARGET = ../../lib/$$TTKMusicPlayer/TTKWatcher

CONFIG       += warn_off
unix:VERSION += 2.1.0

unix{
  macx{
    SOURCES += $$PWD/qdevicewatcher_mac.cpp
    LIBS += -framework DiskArbitration -framework Foundation
  }else{
    SOURCES += $$PWD/qdevicewatcher_linux.cpp
  }
}
win32{
  wince*: SOURCES += $$PWD/qdevicewatcher_wince.cpp
  else: SOURCES += $$PWD/qdevicewatcher_win32.cpp
  LIBS *= -luser32
}

SOURCES += \
    $$PWD/qdevicewatcher.cpp
    
HEADERS += \
    $$PWD/qdevicewatcher_p.h \
    $$PWD/qdevicewatcher.h

#load extra define
include(../TTKExtrasDefine.pri)

win32{
    RC_FILE = Devicewatcher.rc
}
