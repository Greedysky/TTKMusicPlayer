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
    $$PWD/core/musicabstracttablewidget.h \
    $$PWD/core/musicabstractmovedialog.h \
    $$PWD/core/musicabstractmovewidget.h \
    $$PWD/core/musicabstractmoveresizewidget.h \
    $$PWD/core/musicitemdelegate.h \
    $$PWD/core/musicmarqueewidget.h \
    $$PWD/core/musicpreviewlabel.h \
    $$PWD/core/musicmessagebox.h \
    $$PWD/core/musicprogresswidget.h \
    $$PWD/core/musicclickedlabel.h \
    $$PWD/core/musicclickedslider.h \
    $$PWD/core/musicmovinglabelslider.h \
    $$PWD/core/musictoastlabel.h \
    $$PWD/core/musicroundanimationlabel.h \
    $$PWD/core/musiccodearea.h \
    $$PWD/core/musicgrabwidget.h \
    $$PWD/core/musicfloatabstractwidget.h \
    $$PWD/core/musictimersliderwidget.h \
    $$PWD/core/musiccutsliderwidget.h \
    $$PWD/core/musicanimationstackedwidget.h \
    $$PWD/core/musicuploadfilewidget.h \
    $$PWD/core/musictoolmenuwidget.h \
    $$PWD/musicdownloadstatuslabel.h \
    $$PWD/musicquerytablewidget.h \
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
    $$PWD/musicbarragewidget.h \
    $$PWD/musicglobalhotkeyedit.h \
    $$PWD/musicsimilarfoundwidget.h \
    $$PWD/musicplaybackmodewidget.h \
    $$PWD/musicvolumepopwidget.h \
    $$PWD/musicmorefunctionspopwidget.h \
    $$PWD/musicplayedlistwidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/core/musicabstracttablewidget.cpp \
    $$PWD/core/musicabstractmovedialog.cpp \
    $$PWD/core/musicabstractmovewidget.cpp \
    $$PWD/core/musicabstractmoveresizewidget.cpp \
    $$PWD/core/musicitemdelegate.cpp \
    $$PWD/core/musicmarqueewidget.cpp \
    $$PWD/core/musicpreviewlabel.cpp \
    $$PWD/core/musicmessagebox.cpp \
    $$PWD/core/musicprogresswidget.cpp \
    $$PWD/core/musicclickedlabel.cpp \
    $$PWD/core/musicclickedslider.cpp \
    $$PWD/core/musicmovinglabelslider.cpp \
    $$PWD/core/musictoastlabel.cpp \
    $$PWD/core/musicroundanimationlabel.cpp \
    $$PWD/core/musiccodearea.cpp \
    $$PWD/core/musicgrabwidget.cpp \
    $$PWD/core/musicfloatabstractwidget.cpp \
    $$PWD/core/musictimersliderwidget.cpp \
    $$PWD/core/musiccutsliderwidget.cpp \
    $$PWD/core/musicanimationstackedwidget.cpp \
    $$PWD/core/musicuploadfilewidget.cpp \
    $$PWD/core/musictoolmenuwidget.cpp \
    $$PWD/musicdownloadstatuslabel.cpp \
    $$PWD/musicquerytablewidget.cpp \
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
    $$PWD/musicbarragewidget.cpp \
    $$PWD/musicglobalhotkeyedit.cpp \
    $$PWD/musicsimilarfoundwidget.cpp \
    $$PWD/musicplaybackmodewidget.cpp \
    $$PWD/musicvolumepopwidget.cpp \
    $$PWD/musicmorefunctionspopwidget.cpp \
    $$PWD/musicplayedlistwidget.cpp

}
