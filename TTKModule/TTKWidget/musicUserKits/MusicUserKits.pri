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
    $$PWD/musicuserdialog.h \
    $$PWD/musicuserlineedit.h \
    $$PWD/musicusermanagerdialog.h \
    $$PWD/musicuserwindow.h \
    $$PWD/musicuserrecordwidget.h
    
}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicuserdialog.cpp \
    $$PWD/musicuserlineedit.cpp \
    $$PWD/musicusermanagerdialog.cpp \
    $$PWD/musicuserwindow.cpp \
    $$PWD/musicuserrecordwidget.cpp

}
