# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2026 Greedysky Studio
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

set_property(GLOBAL PROPERTY TTK_WIDGET_TOOLSETS_KITS_HEADER_FILES
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicmobiledeviceswidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicmobilesongsmanagerwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicreplaygainwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musictimerwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musictransformwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicfileinformationwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicnetworkconnectiontestwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicsongringtonemakerwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicidentifysongwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolswidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolstablewidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumlayoutwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicsongdlnatransferwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicscreensaverwidget.h
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicplaylistbackupwidget.h
)

set_property(GLOBAL PROPERTY TTK_WIDGET_TOOLSETS_KITS_SOURCE_FILES
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicmobiledeviceswidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicmobilesongsmanagerwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicreplaygainwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musictimerwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musictransformwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicfileinformationwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicnetworkconnectiontestwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicsongringtonemakerwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicidentifysongwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolswidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicsongchecktoolstablewidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicspectrumlayoutwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicsongdlnatransferwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicscreensaverwidget.cpp
  ${TTK_WIDGET_TOOLSETSWIDGET_DIR}/musicplaylistbackupwidget.cpp
)
