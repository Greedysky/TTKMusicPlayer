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
    $$PWD/musicdownloadstatuslabel.h \
    $$PWD/musicsettingwidget.h \
    $$PWD/musicbackgroundskindialog.h \
    $$PWD/musicsystemtraymenu.h \
    $$PWD/musicequalizerdialog.h \
    $$PWD/musicwindowextras.h \
    $$PWD/musicsongslistwidget.h \
    $$PWD/musicsongslistiteminfowidget.h \
    $$PWD/musicsongsearchonlinewidget.h \
    $$PWD/musicsongstoolitemrenamedwidget.h \
    $$PWD/musicsongslistplaywidget.h \
    $$PWD/musicsongslistfunctionwidget.h \
    $$PWD/musicqualitychoicewidget.h \
    $$PWD/musicbackgroundremotewidget.h \
    $$PWD/musicbackgroundlistwidget.h \
    $$PWD/musicbackgroundpalettewidget.h \
    $$PWD/musicbackgroundpopwidget.h \
    $$PWD/musicenhancedwidget.h \
    $$PWD/musicdownloadwidget.h \
    $$PWD/musicdownloadmgmtwidget.h \
    $$PWD/musicsongsharingwidget.h \
    $$PWD/musicsourceupdatewidget.h \
    $$PWD/musicglobalhotkeyedit.h \
    $$PWD/musicsimilarfoundwidget.h \
    $$PWD/musicplaybackmodewidget.h \
    $$PWD/musicvolumepopwidget.h \
    $$PWD/musicmorefunctionspopwidget.h \
    $$PWD/musicplayedlistwidget.h \
    $$PWD/musicalbumfoundwidget.h \
    $$PWD/musicartistfoundwidget.h \
    $$PWD/musicsoundeffectswidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicdownloadstatuslabel.cpp \
    $$PWD/musicsettingwidget.cpp \
    $$PWD/musicbackgroundskindialog.cpp \
    $$PWD/musicsystemtraymenu.cpp \
    $$PWD/musicequalizerdialog.cpp \
    $$PWD/musicwindowextras.cpp \
    $$PWD/musicsongslistwidget.cpp \
    $$PWD/musicsongslistiteminfowidget.cpp \
    $$PWD/musicsongsearchonlinewidget.cpp \
    $$PWD/musicsongstoolitemrenamedwidget.cpp \
    $$PWD/musicsongslistplaywidget.cpp \
    $$PWD/musicsongslistfunctionwidget.cpp \
    $$PWD/musicqualitychoicewidget.cpp \
    $$PWD/musicbackgroundremotewidget.cpp \
    $$PWD/musicbackgroundlistwidget.cpp \
    $$PWD/musicbackgroundpalettewidget.cpp \
    $$PWD/musicbackgroundpopwidget.cpp \
    $$PWD/musicenhancedwidget.cpp \
    $$PWD/musicdownloadwidget.cpp \
    $$PWD/musicdownloadmgmtwidget.cpp \
    $$PWD/musicsongsharingwidget.cpp \
    $$PWD/musicsourceupdatewidget.cpp \
    $$PWD/musicglobalhotkeyedit.cpp \
    $$PWD/musicsimilarfoundwidget.cpp \
    $$PWD/musicplaybackmodewidget.cpp \
    $$PWD/musicvolumepopwidget.cpp \
    $$PWD/musicmorefunctionspopwidget.cpp \
    $$PWD/musicplayedlistwidget.cpp \
    $$PWD/musicalbumfoundwidget.cpp \
    $$PWD/musicartistfoundwidget.cpp \
    $$PWD/musicsoundeffectswidget.cpp

}
