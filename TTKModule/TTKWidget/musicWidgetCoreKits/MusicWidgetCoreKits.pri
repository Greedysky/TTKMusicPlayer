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
    $$PWD/musicabstracttablewidget.h \
    $$PWD/musicabstractmovedialog.h \
    $$PWD/musicabstractmovewidget.h \
    $$PWD/musicabstractmoveresizewidget.h \
    $$PWD/musicsongslistabstracttablewidget.h \
    $$PWD/musicfillitemtablewidget.h \
    $$PWD/musicquerytablewidget.h \
    $$PWD/musicqueryfoundtablewidget.h \
    $$PWD/musicslowmovingtablewidget.h \
    $$PWD/musicitemdelegate.h \
    $$PWD/musicmarqueewidget.h \
    $$PWD/musicpreviewlabel.h \
    $$PWD/musicmessagebox.h \
    $$PWD/musicprogresswidget.h \
    $$PWD/musicclickedlabel.h \
    $$PWD/musicclickedslider.h \
    $$PWD/musicmovinglabelslider.h \
    $$PWD/musictoastlabel.h \
    $$PWD/musicroundanimationlabel.h \
    $$PWD/musicanimationstackedwidget.h \
    $$PWD/musictransitionanimationlabel.h \
    $$PWD/musicfloatabstractwidget.h \
    $$PWD/musicfoundabstractwidget.h \
    $$PWD/musictimersliderwidget.h \
    $$PWD/musiccutsliderwidget.h \
    $$PWD/musictoolmenuwidget.h \
    $$PWD/musicgiflabelwidget.h \
    $$PWD/musicemojilabelwidget.h \
    $$PWD/musicopenfilewidget.h \
    $$PWD/musicsplititemclickedlabel.h \
    $$PWD/musicphotomodlabel.h \
    $$PWD/musicphotograblabel.h \
    $$PWD/musicresizegrabitemwidget.h \
    $$PWD/musiccodeareawidget.h \
    $$PWD/musiclayoutanimationwidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicabstracttablewidget.cpp \
    $$PWD/musicabstractmovedialog.cpp \
    $$PWD/musicabstractmovewidget.cpp \
    $$PWD/musicabstractmoveresizewidget.cpp \
    $$PWD/musicsongslistabstracttablewidget.cpp \
    $$PWD/musicfillitemtablewidget.cpp \
    $$PWD/musicquerytablewidget.cpp \
    $$PWD/musicqueryfoundtablewidget.cpp \
    $$PWD/musicslowmovingtablewidget.cpp \
    $$PWD/musicitemdelegate.cpp \
    $$PWD/musicmarqueewidget.cpp \
    $$PWD/musicpreviewlabel.cpp \
    $$PWD/musicmessagebox.cpp \
    $$PWD/musicprogresswidget.cpp \
    $$PWD/musicclickedlabel.cpp \
    $$PWD/musicclickedslider.cpp \
    $$PWD/musicmovinglabelslider.cpp \
    $$PWD/musictoastlabel.cpp \
    $$PWD/musicroundanimationlabel.cpp \
    $$PWD/musicanimationstackedwidget.cpp \
    $$PWD/musictransitionanimationlabel.cpp \
    $$PWD/musicfloatabstractwidget.cpp \
    $$PWD/musicfoundabstractwidget.cpp \
    $$PWD/musictimersliderwidget.cpp \
    $$PWD/musiccutsliderwidget.cpp \
    $$PWD/musictoolmenuwidget.cpp \
    $$PWD/musicgiflabelwidget.cpp \
    $$PWD/musicemojilabelwidget.cpp \
    $$PWD/musicopenfilewidget.cpp \
    $$PWD/musicsplititemclickedlabel.cpp \
    $$PWD/musicphotomodlabel.cpp \
    $$PWD/musicphotograblabel.cpp \
    $$PWD/musicresizegrabitemwidget.cpp \
    $$PWD/musiccodeareawidget.cpp \
    $$PWD/musiclayoutanimationwidget.cpp

}
