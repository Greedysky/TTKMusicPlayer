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
   
INCLUDEPATH += $$PWD

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musicvideocontrol.h \
    $$PWD/musicvideoview.h \
    $$PWD/musicvideotablewidget.h \
    $$PWD/musicvideoplaywidget.h \
    $$PWD/musicvideofloatwidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicvideocontrol.cpp \
    $$PWD/musicvideoview.cpp \
    $$PWD/musicvideotablewidget.cpp \
    $$PWD/musicvideoplaywidget.cpp \
    $$PWD/musicvideofloatwidget.cpp

}
