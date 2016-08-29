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
    $$PWD/core/musicdesktopwallpaperthread.h \
    $$PWD/core/musictimerautoobject.h \
    $$PWD/core/musiclocalsongsmanagerthread.h \
    $$PWD/core/musicsongtag.h \
    $$PWD/core/musicnetworktestthread.h \
    $$PWD/core/musicmobiledevicesthread.h \
    $$PWD/core/musicaudiorecordercore.h \
    $$PWD/musicmobiledeviceswidget.h \
    $$PWD/musicaudiorecorderwidget.h \
    $$PWD/musictimerwidget.h \
    $$PWD/musiclocalsongsmanagerwidget.h \
    $$PWD/musictransformwidget.h \
    $$PWD/musicfileinformationwidget.h \
    $$PWD/musicdesktopwallpaperwidget.h \
    $$PWD/musiclocalsongstablewidget.h \
    $$PWD/musicnetworktestwidget.h \
    $$PWD/musicnetworksuspensionwidget.h \
    $$PWD/musicvolumegainwidget.h \
    $$PWD/musicsoundtouchwidget.h \
    $$PWD/musicspeedmeterwidget.h \
    $$PWD/musicsongringtonemakerwidget.h \
    $$PWD/musicidentifysongswidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/core/musicdesktopwallpaperthread.cpp \
    $$PWD/core/musictimerautoobject.cpp \
    $$PWD/core/musiclocalsongsmanagerthread.cpp \
    $$PWD/core/musicsongtag.cpp \
    $$PWD/core/musicnetworktestthread.cpp \
    $$PWD/core/musicmobiledevicesthread.cpp \
    $$PWD/core/musicaudiorecordercore.cpp \
    $$PWD/musicmobiledeviceswidget.cpp \
    $$PWD/musicaudiorecorderwidget.cpp \
    $$PWD/musictimerwidget.cpp \
    $$PWD/musiclocalsongsmanagerwidget.cpp \
    $$PWD/musictransformwidget.cpp \
    $$PWD/musicfileinformationwidget.cpp \
    $$PWD/musicdesktopwallpaperwidget.cpp \
    $$PWD/musiclocalsongstablewidget.cpp \
    $$PWD/musicnetworktestwidget.cpp \
    $$PWD/musicnetworksuspensionwidget.cpp \
    $$PWD/musicvolumegainwidget.cpp \
    $$PWD/musicsoundtouchwidget.cpp \
    $$PWD/musicspeedmeterwidget.cpp \
    $$PWD/musicsongringtonemakerwidget.cpp \
    $$PWD/musicidentifysongswidget.cpp

}
