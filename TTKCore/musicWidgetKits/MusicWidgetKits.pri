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
    $$PWD/musicsettingwidget.h \
    $$PWD/musicbackgroundskindialog.h \
    $$PWD/musicsystemtraymenu.h \
    $$PWD/musicequalizerdialog.h \
    $$PWD/musicwindowextras.h \
    $$PWD/musicsongslistiteminfowidget.h \
    $$PWD/musicsongsearchonlinewidget.h \
    $$PWD/musicsongstoolitemrenamedwidget.h \
    $$PWD/musicsongslistplaywidget.h \
    $$PWD/musicsongslistfunctionwidget.h \
    $$PWD/musicsongslisttablewidget.h \
    $$PWD/musicsongslistplayedtablewidget.h \
    $$PWD/musicqualitychoicewidget.h \
    $$PWD/musicbackgroundlistwidget.h \
    $$PWD/musicbackgroundpalettewidget.h \
    $$PWD/musicbackgroundpopwidget.h \
    $$PWD/musicenhancedwidget.h \
    $$PWD/musicdownloadwidget.h \
    $$PWD/musicdownloadmgmtwidget.h \
    $$PWD/musicdownloadstatusobject.h \
    $$PWD/musicsongsharingwidget.h \
    $$PWD/musicsonglistsharingwidget.h \
    $$PWD/musicsourceupdatewidget.h \
    $$PWD/musicglobalhotkeyedit.h \
    $$PWD/musicsimilarfoundwidget.h \
    $$PWD/musicplaybackmodewidget.h \
    $$PWD/musicvolumepopwidget.h \
    $$PWD/musicmorefunctionspopwidget.h \
    $$PWD/musicplayedlistwidget.h \
    $$PWD/musicalbumfoundwidget.h \
    $$PWD/musicartistfoundwidget.h \
    $$PWD/musicsoundeffectswidget.h \
    $$PWD/musicmessageaboutdialog.h \
    $$PWD/musicplaylistfoundwidget.h \
    $$PWD/musicplaylistfoundinfowidget.h \
    $$PWD/musicplaylistfoundtablewidget.h \
    $$PWD/musicplaylistfoundcategorywidget.h \
    $$PWD/musicsonglistenhancelosslesswidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicsettingwidget.cpp \
    $$PWD/musicbackgroundskindialog.cpp \
    $$PWD/musicsystemtraymenu.cpp \
    $$PWD/musicequalizerdialog.cpp \
    $$PWD/musicwindowextras.cpp \
    $$PWD/musicsongslistiteminfowidget.cpp \
    $$PWD/musicsongsearchonlinewidget.cpp \
    $$PWD/musicsongstoolitemrenamedwidget.cpp \
    $$PWD/musicsongslistplaywidget.cpp \
    $$PWD/musicsongslistfunctionwidget.cpp \
    $$PWD/musicsongslisttablewidget.cpp \
    $$PWD/musicsongslistplayedtablewidget.cpp \
    $$PWD/musicqualitychoicewidget.cpp \
    $$PWD/musicbackgroundlistwidget.cpp \
    $$PWD/musicbackgroundpalettewidget.cpp \
    $$PWD/musicbackgroundpopwidget.cpp \
    $$PWD/musicenhancedwidget.cpp \
    $$PWD/musicdownloadwidget.cpp \
    $$PWD/musicdownloadmgmtwidget.cpp \
    $$PWD/musicdownloadstatusobject.cpp \
    $$PWD/musicsongsharingwidget.cpp \
    $$PWD/musicsonglistsharingwidget.cpp \
    $$PWD/musicsourceupdatewidget.cpp \
    $$PWD/musicglobalhotkeyedit.cpp \
    $$PWD/musicsimilarfoundwidget.cpp \
    $$PWD/musicplaybackmodewidget.cpp \
    $$PWD/musicvolumepopwidget.cpp \
    $$PWD/musicmorefunctionspopwidget.cpp \
    $$PWD/musicplayedlistwidget.cpp \
    $$PWD/musicalbumfoundwidget.cpp \
    $$PWD/musicartistfoundwidget.cpp \
    $$PWD/musicsoundeffectswidget.cpp \
    $$PWD/musicmessageaboutdialog.cpp \
    $$PWD/musicplaylistfoundwidget.cpp \
    $$PWD/musicplaylistfoundinfowidget.cpp \
    $$PWD/musicplaylistfoundtablewidget.cpp \
    $$PWD/musicplaylistfoundcategorywidget.cpp \
    $$PWD/musicsonglistenhancelosslesswidget.cpp

}
