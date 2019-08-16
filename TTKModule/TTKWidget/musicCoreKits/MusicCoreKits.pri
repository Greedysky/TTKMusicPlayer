# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2019 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
#
# * You should have received a copy of the GNU General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
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
    $$PWD/musicsmoothmovingwidget.h \
    $$PWD/musicitemdelegate.h \
    $$PWD/musiccolordialog.h \
    $$PWD/musicmarqueewidget.h \
    $$PWD/musicpreviewlabel.h \
    $$PWD/musicmessagebox.h \
    $$PWD/musicprogresswidget.h \
    $$PWD/musicclickedlabel.h \
    $$PWD/musicclickedslider.h \
    $$PWD/musicmovingclickedslider.h \
    $$PWD/musicmovinglabelslider.h \
    $$PWD/musictoastlabel.h \
    $$PWD/musicroundanimationlabel.h \
    $$PWD/musicanimationstackedwidget.h \
    $$PWD/musictransitionanimationlabel.h \
    $$PWD/musicfloatabstractwidget.h \
    $$PWD/musicfoundabstractwidget.h \
    $$PWD/musiccutsliderwidget.h \
    $$PWD/musictextsliderwidget.h \
    $$PWD/musictoolmenuwidget.h \
    $$PWD/musicgiflabelwidget.h \
    $$PWD/musicemojilabelwidget.h \
    $$PWD/musicopenfilewidget.h \
    $$PWD/musicsplititemclickedlabel.h \
    $$PWD/musicphotomodlabel.h \
    $$PWD/musicphotograblabel.h \
    $$PWD/musicresizegrabitemwidget.h \
    $$PWD/musiccodeareawidget.h \
    $$PWD/musiclayoutanimationwidget.h \
    $$PWD/musiccommentswidget.h \
    $$PWD/musicdownloadabstracttablewidget.h \
    $$PWD/musicfunctiontoolboxwidget.h \
    $$PWD/musicsongitemselecteddialog.h \
    $$PWD/musictimesliderwidget.h

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
    $$PWD/musicsmoothmovingwidget.cpp \
    $$PWD/musicitemdelegate.cpp \
    $$PWD/musiccolordialog.cpp \
    $$PWD/musicmarqueewidget.cpp \
    $$PWD/musicpreviewlabel.cpp \
    $$PWD/musicmessagebox.cpp \
    $$PWD/musicprogresswidget.cpp \
    $$PWD/musicclickedlabel.cpp \
    $$PWD/musicclickedslider.cpp \
    $$PWD/musicmovingclickedslider.cpp \
    $$PWD/musicmovinglabelslider.cpp \
    $$PWD/musictoastlabel.cpp \
    $$PWD/musicroundanimationlabel.cpp \
    $$PWD/musicanimationstackedwidget.cpp \
    $$PWD/musictransitionanimationlabel.cpp \
    $$PWD/musicfloatabstractwidget.cpp \
    $$PWD/musicfoundabstractwidget.cpp \
    $$PWD/musiccutsliderwidget.cpp \
    $$PWD/musictextsliderwidget.cpp \
    $$PWD/musictoolmenuwidget.cpp \
    $$PWD/musicgiflabelwidget.cpp \
    $$PWD/musicemojilabelwidget.cpp \
    $$PWD/musicopenfilewidget.cpp \
    $$PWD/musicsplititemclickedlabel.cpp \
    $$PWD/musicphotomodlabel.cpp \
    $$PWD/musicphotograblabel.cpp \
    $$PWD/musicresizegrabitemwidget.cpp \
    $$PWD/musiccodeareawidget.cpp \
    $$PWD/musiclayoutanimationwidget.cpp \
    $$PWD/musiccommentswidget.cpp \
    $$PWD/musicdownloadabstracttablewidget.cpp \
    $$PWD/musicfunctiontoolboxwidget.cpp \
    $$PWD/musicsongitemselecteddialog.cpp \
    $$PWD/musictimesliderwidget.cpp

}
