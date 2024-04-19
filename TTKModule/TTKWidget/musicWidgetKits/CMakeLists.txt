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

set_property(GLOBAL PROPERTY MUSIC_WIDGET_KITS_HEADERS
  ${MUSIC_WIDGET_DIR}/musicsettingwidget.h
  ${MUSIC_WIDGET_DIR}/musicbackgroundskindialog.h
  ${MUSIC_WIDGET_DIR}/musicsystemtraymenu.h
  ${MUSIC_WIDGET_DIR}/musicequalizerdialog.h
  ${MUSIC_WIDGET_DIR}/musicplatformextras.h
  ${MUSIC_WIDGET_DIR}/musicsongslistiteminfowidget.h
  ${MUSIC_WIDGET_DIR}/musicsongsearchonlinewidget.h
  ${MUSIC_WIDGET_DIR}/musicsongslistplaywidget.h
  ${MUSIC_WIDGET_DIR}/musicsongslistplayedwidget.h
  ${MUSIC_WIDGET_DIR}/musicsongslistfunctionwidget.h
  ${MUSIC_WIDGET_DIR}/musicsongslistplaytablewidget.h
  ${MUSIC_WIDGET_DIR}/musicsongslistplayedtablewidget.h
  ${MUSIC_WIDGET_DIR}/musicbackgroundlistwidget.h
  ${MUSIC_WIDGET_DIR}/musicbackgroundpalettewidget.h
  ${MUSIC_WIDGET_DIR}/musicbackgroundpopwidget.h
  ${MUSIC_WIDGET_DIR}/musicbackgroundremotewidget.h
  ${MUSIC_WIDGET_DIR}/musicdownloadwidget.h
  ${MUSIC_WIDGET_DIR}/musicdownloadmgmtwidget.h
  ${MUSIC_WIDGET_DIR}/musicdownloadstatusmodule.h
  ${MUSIC_WIDGET_DIR}/musicsongsharingwidget.h
  ${MUSIC_WIDGET_DIR}/musicsourceupdatewidget.h
  ${MUSIC_WIDGET_DIR}/musicglobalhotkeyedit.h
  ${MUSIC_WIDGET_DIR}/musicsimilarquerywidget.h
  ${MUSIC_WIDGET_DIR}/musicvolumepopwidget.h
  ${MUSIC_WIDGET_DIR}/musicmorefunctionspopwidget.h
  ${MUSIC_WIDGET_DIR}/musicalbumquerywidget.h
  ${MUSIC_WIDGET_DIR}/musicartistquerywidget.h
  ${MUSIC_WIDGET_DIR}/musicsoundeffectswidget.h
  ${MUSIC_WIDGET_DIR}/musicmessageaboutdialog.h
  ${MUSIC_WIDGET_DIR}/musicplaylistquerywidget.h
  ${MUSIC_WIDGET_DIR}/musicplaylistqueryinfowidget.h
  ${MUSIC_WIDGET_DIR}/musicplaylistquerytablewidget.h
  ${MUSIC_WIDGET_DIR}/musicenhancedpopwidget.h
  ${MUSIC_WIDGET_DIR}/musicplaybackmodepopwidget.h
  ${MUSIC_WIDGET_DIR}/musicqualitywidget.h
  ${MUSIC_WIDGET_DIR}/musicplayedlistpopwidget.h
  ${MUSIC_WIDGET_DIR}/musicplaylistquerycategorypopwidget.h
  ${MUSIC_WIDGET_DIR}/musicplaylistquerycommentswidget.h
  ${MUSIC_WIDGET_DIR}/musicfuntionanimationwidget.h
  ${MUSIC_WIDGET_DIR}/musictoplistquerywidget.h
  ${MUSIC_WIDGET_DIR}/musictoplistquerycategorypopwidget.h
  ${MUSIC_WIDGET_DIR}/musicdownloadbatchwidget.h
  ${MUSIC_WIDGET_DIR}/musicadvancedsearchedwidget.h
  ${MUSIC_WIDGET_DIR}/musicrecommendquerywidget.h
  ${MUSIC_WIDGET_DIR}/musicartistlistquerywidget.h
  ${MUSIC_WIDGET_DIR}/musicartistlistquerycategorypopwidget.h
  ${MUSIC_WIDGET_DIR}/musicblurspecturmmodule.h
  ${MUSIC_WIDGET_DIR}/musicpluginwidget.h
  ${MUSIC_WIDGET_DIR}/musicfileassociationwidget.h
)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_KITS_SOURCES
  ${MUSIC_WIDGET_DIR}/musicsettingwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicbackgroundskindialog.cpp
  ${MUSIC_WIDGET_DIR}/musicsystemtraymenu.cpp
  ${MUSIC_WIDGET_DIR}/musicequalizerdialog.cpp
  ${MUSIC_WIDGET_DIR}/musicplatformextras.cpp
  ${MUSIC_WIDGET_DIR}/musicsongslistiteminfowidget.cpp
  ${MUSIC_WIDGET_DIR}/musicsongsearchonlinewidget.cpp
  ${MUSIC_WIDGET_DIR}/musicsongslistplaywidget.cpp
  ${MUSIC_WIDGET_DIR}/musicsongslistplayedwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicsongslistfunctionwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicsongslistplaytablewidget.cpp
  ${MUSIC_WIDGET_DIR}/musicsongslistplayedtablewidget.cpp
  ${MUSIC_WIDGET_DIR}/musicbackgroundlistwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicbackgroundpalettewidget.cpp
  ${MUSIC_WIDGET_DIR}/musicbackgroundpopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicbackgroundremotewidget.cpp
  ${MUSIC_WIDGET_DIR}/musicdownloadwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicdownloadmgmtwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicdownloadstatusmodule.cpp
  ${MUSIC_WIDGET_DIR}/musicsongsharingwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicsourceupdatewidget.cpp
  ${MUSIC_WIDGET_DIR}/musicglobalhotkeyedit.cpp
  ${MUSIC_WIDGET_DIR}/musicsimilarquerywidget.cpp
  ${MUSIC_WIDGET_DIR}/musicvolumepopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicmorefunctionspopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicalbumquerywidget.cpp
  ${MUSIC_WIDGET_DIR}/musicartistquerywidget.cpp
  ${MUSIC_WIDGET_DIR}/musicsoundeffectswidget.cpp
  ${MUSIC_WIDGET_DIR}/musicmessageaboutdialog.cpp
  ${MUSIC_WIDGET_DIR}/musicplaylistquerywidget.cpp
  ${MUSIC_WIDGET_DIR}/musicplaylistqueryinfowidget.cpp
  ${MUSIC_WIDGET_DIR}/musicplaylistquerytablewidget.cpp
  ${MUSIC_WIDGET_DIR}/musicenhancedpopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicplaybackmodepopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicqualitywidget.cpp
  ${MUSIC_WIDGET_DIR}/musicplayedlistpopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicplaylistquerycategorypopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicplaylistquerycommentswidget.cpp
  ${MUSIC_WIDGET_DIR}/musicfuntionanimationwidget.cpp
  ${MUSIC_WIDGET_DIR}/musictoplistquerywidget.cpp
  ${MUSIC_WIDGET_DIR}/musictoplistquerycategorypopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicdownloadbatchwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicadvancedsearchedwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicrecommendquerywidget.cpp
  ${MUSIC_WIDGET_DIR}/musicartistlistquerywidget.cpp
  ${MUSIC_WIDGET_DIR}/musicartistlistquerycategorypopwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicblurspecturmmodule.cpp
  ${MUSIC_WIDGET_DIR}/musicpluginwidget.cpp
  ${MUSIC_WIDGET_DIR}/musicfileassociationwidget.cpp
)
