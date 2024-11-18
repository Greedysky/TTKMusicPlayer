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

set_property(GLOBAL PROPERTY TTK_WIDGET_SEARCH_KITS_HEADERS
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicsearchedit.h
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicitemsearchedit.h
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicsongsearchdialog.h
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicsongsearchinterioredit.h
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicsongsearchpopwidget.h
)

set_property(GLOBAL PROPERTY TTK_WIDGET_SEARCH_KITS_SOURCES
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicsearchedit.cpp
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicitemsearchedit.cpp
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicsongsearchdialog.cpp
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicsongsearchinterioredit.cpp
  ${TTK_WIDGET_LOCALSEARCH_DIR}/musicsongsearchpopwidget.cpp
)
