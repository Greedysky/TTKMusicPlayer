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

INCLUDEPATH += $$PWD \
               $$PWD/core

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musiclocalsongsearchedit.h \
    $$PWD/musiclocalsongsearchinlineedit.h \
    $$PWD/musiclocalsongsearchpopwidget.h \
    $$PWD/musiclocalsongsearchdialog.h \
    $$PWD/core/musiclocalsongsearchrecordconfigmanager.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musiclocalsongsearchedit.cpp \
    $$PWD/musiclocalsongsearchinlineedit.cpp \
    $$PWD/musiclocalsongsearchpopwidget.cpp \
    $$PWD/musiclocalsongsearchdialog.cpp \
    $$PWD/core/musiclocalsongsearchrecordconfigmanager.cpp
}
