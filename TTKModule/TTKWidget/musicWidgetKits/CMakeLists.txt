# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2025 Greedysky Studio
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

cmake_minimum_required(VERSION 3.18.0)

set_property(GLOBAL PROPERTY TTK_WIDGET_KITS_HEADER_FILES
  ${TTK_WIDGET_DIR}/musicsettingwidget.h
  ${TTK_WIDGET_DIR}/musicbackgroundskindialog.h
  ${TTK_WIDGET_DIR}/musicsystemtraymenu.h
  ${TTK_WIDGET_DIR}/musicequalizerdialog.h
  ${TTK_WIDGET_DIR}/musicplatformextras.h
  ${TTK_WIDGET_DIR}/musicsongslistiteminfowidget.h
  ${TTK_WIDGET_DIR}/musicsongsearchonlinewidget.h
  ${TTK_WIDGET_DIR}/musicsongslistplaywidget.h
  ${TTK_WIDGET_DIR}/musicsongslistplayedwidget.h
  ${TTK_WIDGET_DIR}/musicsongslistfunctionwidget.h
  ${TTK_WIDGET_DIR}/musicsongslistplaytablewidget.h
  ${TTK_WIDGET_DIR}/musicsongslistplayedtablewidget.h
  ${TTK_WIDGET_DIR}/musicbackgroundlistwidget.h
  ${TTK_WIDGET_DIR}/musicbackgroundpalettewidget.h
  ${TTK_WIDGET_DIR}/musicbackgroundpopwidget.h
  ${TTK_WIDGET_DIR}/musicbackgroundremotewidget.h
  ${TTK_WIDGET_DIR}/musicdownloadwidget.h
  ${TTK_WIDGET_DIR}/musicdownloadmgmtwidget.h
  ${TTK_WIDGET_DIR}/musicdownloadstatusmodule.h
  ${TTK_WIDGET_DIR}/musicsongdailywidget.h
  ${TTK_WIDGET_DIR}/musicsongsharingwidget.h
  ${TTK_WIDGET_DIR}/musicsourceupdatewidget.h
  ${TTK_WIDGET_DIR}/musicglobalhotkeyedit.h
  ${TTK_WIDGET_DIR}/musicsimilarquerywidget.h
  ${TTK_WIDGET_DIR}/musicvolumepopwidget.h
  ${TTK_WIDGET_DIR}/musicmorefunctionspopwidget.h
  ${TTK_WIDGET_DIR}/musicalbumquerywidget.h
  ${TTK_WIDGET_DIR}/musicartistquerywidget.h
  ${TTK_WIDGET_DIR}/musicsoundeffectswidget.h
  ${TTK_WIDGET_DIR}/musicmessageaboutdialog.h
  ${TTK_WIDGET_DIR}/musicplaylistquerywidget.h
  ${TTK_WIDGET_DIR}/musicplaylistqueryinfowidget.h
  ${TTK_WIDGET_DIR}/musicenhancedpopwidget.h
  ${TTK_WIDGET_DIR}/musicplaybackmodepopwidget.h
  ${TTK_WIDGET_DIR}/musicqualitywidget.h
  ${TTK_WIDGET_DIR}/musicplayedlistpopwidget.h
  ${TTK_WIDGET_DIR}/musicplaylistcategorywidget.h
  ${TTK_WIDGET_DIR}/musicplaylistquerycategorypopwidget.h
  ${TTK_WIDGET_DIR}/musicplaylistquerycommentswidget.h
  ${TTK_WIDGET_DIR}/musicfuntionanimationwidget.h
  ${TTK_WIDGET_DIR}/musictoplistquerywidget.h
  ${TTK_WIDGET_DIR}/musictoplistquerycategorypopwidget.h
  ${TTK_WIDGET_DIR}/musicdownloadbatchwidget.h
  ${TTK_WIDGET_DIR}/musicadvancedsearchedwidget.h
  ${TTK_WIDGET_DIR}/musicrecommendquerywidget.h
  ${TTK_WIDGET_DIR}/musicartistlistquerywidget.h
  ${TTK_WIDGET_DIR}/musicartistlistquerycategorypopwidget.h
  ${TTK_WIDGET_DIR}/musicblurspecturmmodule.h
  ${TTK_WIDGET_DIR}/musicpluginwidget.h
  ${TTK_WIDGET_DIR}/musicfileassociationwidget.h
)

set_property(GLOBAL PROPERTY TTK_WIDGET_KITS_SOURCE_FILES
  ${TTK_WIDGET_DIR}/musicsettingwidget.cpp
  ${TTK_WIDGET_DIR}/musicbackgroundskindialog.cpp
  ${TTK_WIDGET_DIR}/musicsystemtraymenu.cpp
  ${TTK_WIDGET_DIR}/musicequalizerdialog.cpp
  ${TTK_WIDGET_DIR}/musicplatformextras.cpp
  ${TTK_WIDGET_DIR}/musicsongslistiteminfowidget.cpp
  ${TTK_WIDGET_DIR}/musicsongsearchonlinewidget.cpp
  ${TTK_WIDGET_DIR}/musicsongslistplaywidget.cpp
  ${TTK_WIDGET_DIR}/musicsongslistplayedwidget.cpp
  ${TTK_WIDGET_DIR}/musicsongslistfunctionwidget.cpp
  ${TTK_WIDGET_DIR}/musicsongslistplaytablewidget.cpp
  ${TTK_WIDGET_DIR}/musicsongslistplayedtablewidget.cpp
  ${TTK_WIDGET_DIR}/musicbackgroundlistwidget.cpp
  ${TTK_WIDGET_DIR}/musicbackgroundpalettewidget.cpp
  ${TTK_WIDGET_DIR}/musicbackgroundpopwidget.cpp
  ${TTK_WIDGET_DIR}/musicbackgroundremotewidget.cpp
  ${TTK_WIDGET_DIR}/musicdownloadwidget.cpp
  ${TTK_WIDGET_DIR}/musicdownloadmgmtwidget.cpp
  ${TTK_WIDGET_DIR}/musicdownloadstatusmodule.cpp
  ${TTK_WIDGET_DIR}/musicsongdailywidget.cpp
  ${TTK_WIDGET_DIR}/musicsongsharingwidget.cpp
  ${TTK_WIDGET_DIR}/musicsourceupdatewidget.cpp
  ${TTK_WIDGET_DIR}/musicglobalhotkeyedit.cpp
  ${TTK_WIDGET_DIR}/musicsimilarquerywidget.cpp
  ${TTK_WIDGET_DIR}/musicvolumepopwidget.cpp
  ${TTK_WIDGET_DIR}/musicmorefunctionspopwidget.cpp
  ${TTK_WIDGET_DIR}/musicalbumquerywidget.cpp
  ${TTK_WIDGET_DIR}/musicartistquerywidget.cpp
  ${TTK_WIDGET_DIR}/musicsoundeffectswidget.cpp
  ${TTK_WIDGET_DIR}/musicmessageaboutdialog.cpp
  ${TTK_WIDGET_DIR}/musicplaylistquerywidget.cpp
  ${TTK_WIDGET_DIR}/musicplaylistqueryinfowidget.cpp
  ${TTK_WIDGET_DIR}/musicenhancedpopwidget.cpp
  ${TTK_WIDGET_DIR}/musicplaybackmodepopwidget.cpp
  ${TTK_WIDGET_DIR}/musicqualitywidget.cpp
  ${TTK_WIDGET_DIR}/musicplayedlistpopwidget.cpp
  ${TTK_WIDGET_DIR}/musicplaylistcategorywidget.cpp
  ${TTK_WIDGET_DIR}/musicplaylistquerycategorypopwidget.cpp
  ${TTK_WIDGET_DIR}/musicplaylistquerycommentswidget.cpp
  ${TTK_WIDGET_DIR}/musicfuntionanimationwidget.cpp
  ${TTK_WIDGET_DIR}/musictoplistquerywidget.cpp
  ${TTK_WIDGET_DIR}/musictoplistquerycategorypopwidget.cpp
  ${TTK_WIDGET_DIR}/musicdownloadbatchwidget.cpp
  ${TTK_WIDGET_DIR}/musicadvancedsearchedwidget.cpp
  ${TTK_WIDGET_DIR}/musicrecommendquerywidget.cpp
  ${TTK_WIDGET_DIR}/musicartistlistquerywidget.cpp
  ${TTK_WIDGET_DIR}/musicartistlistquerycategorypopwidget.cpp
  ${TTK_WIDGET_DIR}/musicblurspecturmmodule.cpp
  ${TTK_WIDGET_DIR}/musicpluginwidget.cpp
  ${TTK_WIDGET_DIR}/musicfileassociationwidget.cpp
)
