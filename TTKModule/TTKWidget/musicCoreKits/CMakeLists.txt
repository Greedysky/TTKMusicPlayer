# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2024 Greedysky Studio
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
# ***************************************************************************

cmake_minimum_required(VERSION 3.0.0)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_CORE_KITS_HEADERS
  ${MUSIC_WIDGET_CORE_DIR}/musicabstracttablewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractmovedialog.h
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractmovewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractmoveresizewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractdownloadtablewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractfloatwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractitemquerywidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractsongslisttablewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicwidgetrenderer.h
  ${MUSIC_WIDGET_CORE_DIR}/musicfillitemtablewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicquerytablewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicitemsearchtablewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicitemquerytablewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicitemrenameedit.h
  ${MUSIC_WIDGET_CORE_DIR}/musicsmoothmovingwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicitemdelegate.h
  ${MUSIC_WIDGET_CORE_DIR}/musiccolordialog.h
  ${MUSIC_WIDGET_CORE_DIR}/musicmarqueewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicpreviewlabel.h
  ${MUSIC_WIDGET_CORE_DIR}/musicmessagebox.h
  ${MUSIC_WIDGET_CORE_DIR}/musicprogresswidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicmovingclickedslider.h
  ${MUSIC_WIDGET_CORE_DIR}/musicmovinglabelslider.h
  ${MUSIC_WIDGET_CORE_DIR}/musictoastlabel.h
  ${MUSIC_WIDGET_CORE_DIR}/musicroundanimationlabel.h
  ${MUSIC_WIDGET_CORE_DIR}/musicanimationstackedwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musictransitionanimationlabel.h
  ${MUSIC_WIDGET_CORE_DIR}/musiccutsliderwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musictextsliderwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musictoolmenuwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicgiflabelwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicemojilabelwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicopenfilewidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicsplititemclickedlabel.h
  ${MUSIC_WIDGET_CORE_DIR}/musiclayoutanimationwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musiccommentswidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicfunctiontoolboxwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicsongitemselectedareawidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musictimesliderwidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicpagequerywidget.h
  ${MUSIC_WIDGET_CORE_DIR}/musicinputdialog.h
  ${MUSIC_WIDGET_CORE_DIR}/musicratinglabel.h
  ${MUSIC_WIDGET_CORE_DIR}/musicsinglemanager.h
)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_CORE_KITS_SOURCES
  ${MUSIC_WIDGET_CORE_DIR}/musicabstracttablewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractmovedialog.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractmovewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractmoveresizewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractdownloadtablewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractfloatwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractitemquerywidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicabstractsongslisttablewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicwidgetrenderer.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicfillitemtablewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicquerytablewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicitemsearchtablewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicitemquerytablewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicitemrenameedit.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicsmoothmovingwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicitemdelegate.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musiccolordialog.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicmarqueewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicpreviewlabel.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicmessagebox.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicprogresswidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicmovingclickedslider.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicmovinglabelslider.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musictoastlabel.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicroundanimationlabel.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicanimationstackedwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musictransitionanimationlabel.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musiccutsliderwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musictextsliderwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musictoolmenuwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicgiflabelwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicemojilabelwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicopenfilewidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicsplititemclickedlabel.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musiclayoutanimationwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musiccommentswidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicfunctiontoolboxwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicsongitemselectedareawidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musictimesliderwidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicpagequerywidget.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicinputdialog.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicratinglabel.cpp
  ${MUSIC_WIDGET_CORE_DIR}/musicsinglemanager.cpp
)
