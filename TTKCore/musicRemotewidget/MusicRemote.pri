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
    $$PWD/musicremotewidget.h \
    $$PWD/musicremotewidgetforsquare.h \
    $$PWD/musicremotewidgetforrectangle.h \
    $$PWD/musicremotewidgetfordiamond.h \
    $$PWD/musicremotewidgetforcircle.h \
    $$PWD/musicremotewidgetforsimplestyle.h \
    $$PWD/musicremotewidgetforcomplexstyle.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicremotewidget.cpp \
    $$PWD/musicremotewidgetforsquare.cpp \
    $$PWD/musicremotewidgetforrectangle.cpp \
    $$PWD/musicremotewidgetfordiamond.cpp \
    $$PWD/musicremotewidgetforcircle.cpp \
    $$PWD/musicremotewidgetforsimplestyle.cpp \
    $$PWD/musicremotewidgetforcomplexstyle.cpp

}
