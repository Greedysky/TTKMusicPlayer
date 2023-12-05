# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2023 Greedysky Studio
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

set_property(GLOBAL PROPERTY MUSIC_WIDGET_TOOLSETS_KITS_HEADERS
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicmobiledeviceswidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicmobilesongsmanagerwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicreplaygainwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musictimerwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musictransformwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicfileinformationwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicnetworkconnectiontestwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongringtonemakerwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicidentifysongwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolswidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolstablewidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumlayoutwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongdlnatransferwidget.h
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicscreensaverwidget.h
)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_TOOLSETS_KITS_SOURCES
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicmobiledeviceswidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicmobilesongsmanagerwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicreplaygainwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musictimerwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musictransformwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicfileinformationwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicnetworkconnectiontestwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongringtonemakerwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicidentifysongwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolswidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolstablewidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumlayoutwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicsongdlnatransferwidget.cpp
  ${MUSIC_WIDGET_TOOLSETSWIDGET_DIR}/musicscreensaverwidget.cpp
)
