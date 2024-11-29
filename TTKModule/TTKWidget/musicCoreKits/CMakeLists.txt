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

set_property(GLOBAL PROPERTY TTK_WIDGET_CORE_KITS_HEADER_FILES
  ${TTK_WIDGET_CORE_DIR}/musicabstracttablewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicabstractmovedialog.h
  ${TTK_WIDGET_CORE_DIR}/musicabstractmovewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicabstractmoveresizewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicabstractdownloadtablewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicabstractfloatwidget.h
  ${TTK_WIDGET_CORE_DIR}/musicabstractitemquerywidget.h
  ${TTK_WIDGET_CORE_DIR}/musicabstractsongslisttablewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicwidgetrenderer.h
  ${TTK_WIDGET_CORE_DIR}/musicfillitemtablewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicquerytablewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicitemsearchtablewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicitemquerytablewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicitemrenameedit.h
  ${TTK_WIDGET_CORE_DIR}/musicsmoothmovingwidget.h
  ${TTK_WIDGET_CORE_DIR}/musicitemdelegate.h
  ${TTK_WIDGET_CORE_DIR}/musiccolordialog.h
  ${TTK_WIDGET_CORE_DIR}/musicmarqueewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicpreviewlabel.h
  ${TTK_WIDGET_CORE_DIR}/musicmessagebox.h
  ${TTK_WIDGET_CORE_DIR}/musicprogresswidget.h
  ${TTK_WIDGET_CORE_DIR}/musicmovingclickedslider.h
  ${TTK_WIDGET_CORE_DIR}/musicmovinglabelslider.h
  ${TTK_WIDGET_CORE_DIR}/musictoastlabel.h
  ${TTK_WIDGET_CORE_DIR}/musicroundanimationlabel.h
  ${TTK_WIDGET_CORE_DIR}/musicanimationstackedwidget.h
  ${TTK_WIDGET_CORE_DIR}/musictransitionanimationlabel.h
  ${TTK_WIDGET_CORE_DIR}/musiccutsliderwidget.h
  ${TTK_WIDGET_CORE_DIR}/musictextsliderwidget.h
  ${TTK_WIDGET_CORE_DIR}/musictoolmenuwidget.h
  ${TTK_WIDGET_CORE_DIR}/musicgiflabelwidget.h
  ${TTK_WIDGET_CORE_DIR}/musicemojilabelwidget.h
  ${TTK_WIDGET_CORE_DIR}/musicopenfilewidget.h
  ${TTK_WIDGET_CORE_DIR}/musicsplititemclickedlabel.h
  ${TTK_WIDGET_CORE_DIR}/musiclayoutanimationwidget.h
  ${TTK_WIDGET_CORE_DIR}/musiccommentswidget.h
  ${TTK_WIDGET_CORE_DIR}/musicfunctiontoolboxwidget.h
  ${TTK_WIDGET_CORE_DIR}/musicsongitemselectedareawidget.h
  ${TTK_WIDGET_CORE_DIR}/musictimesliderwidget.h
  ${TTK_WIDGET_CORE_DIR}/musicpagequerywidget.h
  ${TTK_WIDGET_CORE_DIR}/musicinputdialog.h
  ${TTK_WIDGET_CORE_DIR}/musicratinglabel.h
  ${TTK_WIDGET_CORE_DIR}/musicsinglemanager.h
)

set_property(GLOBAL PROPERTY TTK_WIDGET_CORE_KITS_SOURCE_FILES
  ${TTK_WIDGET_CORE_DIR}/musicabstracttablewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicabstractmovedialog.cpp
  ${TTK_WIDGET_CORE_DIR}/musicabstractmovewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicabstractmoveresizewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicabstractdownloadtablewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicabstractfloatwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicabstractitemquerywidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicabstractsongslisttablewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicwidgetrenderer.cpp
  ${TTK_WIDGET_CORE_DIR}/musicfillitemtablewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicquerytablewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicitemsearchtablewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicitemquerytablewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicitemrenameedit.cpp
  ${TTK_WIDGET_CORE_DIR}/musicsmoothmovingwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicitemdelegate.cpp
  ${TTK_WIDGET_CORE_DIR}/musiccolordialog.cpp
  ${TTK_WIDGET_CORE_DIR}/musicmarqueewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicpreviewlabel.cpp
  ${TTK_WIDGET_CORE_DIR}/musicmessagebox.cpp
  ${TTK_WIDGET_CORE_DIR}/musicprogresswidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicmovingclickedslider.cpp
  ${TTK_WIDGET_CORE_DIR}/musicmovinglabelslider.cpp
  ${TTK_WIDGET_CORE_DIR}/musictoastlabel.cpp
  ${TTK_WIDGET_CORE_DIR}/musicroundanimationlabel.cpp
  ${TTK_WIDGET_CORE_DIR}/musicanimationstackedwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musictransitionanimationlabel.cpp
  ${TTK_WIDGET_CORE_DIR}/musiccutsliderwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musictextsliderwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musictoolmenuwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicgiflabelwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicemojilabelwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicopenfilewidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicsplititemclickedlabel.cpp
  ${TTK_WIDGET_CORE_DIR}/musiclayoutanimationwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musiccommentswidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicfunctiontoolboxwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicsongitemselectedareawidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musictimesliderwidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicpagequerywidget.cpp
  ${TTK_WIDGET_CORE_DIR}/musicinputdialog.cpp
  ${TTK_WIDGET_CORE_DIR}/musicratinglabel.cpp
  ${TTK_WIDGET_CORE_DIR}/musicsinglemanager.cpp
)
