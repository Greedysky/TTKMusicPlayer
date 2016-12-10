# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

unix:!mac{
    equals(QT_MAJOR_VERSION, 5){
        QT  += x11extras
    }
}

INCLUDEPATH += $$PWD

HEADERS  += \
    $$PWD/qxtglobal.h \
    $$PWD/qxtglobalshortcut.h \
    $$PWD/qxtglobalshortcut_p.h


win32:SOURCES += $$PWD/qxtglobalshortcut_win.cpp
unix:SOURCES += $$PWD/qxtglobalshortcut_x11.cpp
mac:SOURCES += $$PWD/qxtglobalshortcut_mac.cpp

SOURCES += \
    $$PWD/qxtglobalshortcut.cpp

