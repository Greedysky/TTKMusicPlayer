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

set_property(GLOBAL PROPERTY TTK_WIDGET_LRC_KITS_HEADER_FILES
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcmanager.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerfordesktop.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerforinterior.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccontainer.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccontainerfordesktop.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforinterior.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforwallpaper.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcsearchwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcsearchtablewidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcphotomanagerwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcfloatwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcfloatsettingwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcfloatphotowidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcfloatplaywidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcmakerwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrclocallinkwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccommentswidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrctranslatedwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccolorwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcposterwidget.h
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcdownloadbatchwidget.h
)

set_property(GLOBAL PROPERTY TTK_WIDGET_LRC_KITS_SOURCE_FILES
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcmanager.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerfordesktop.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerforinterior.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforwallpaper.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccontainer.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccontainerfordesktop.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforinterior.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcsearchwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcsearchtablewidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcphotomanagerwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcfloatwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcfloatsettingwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcfloatphotowidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcfloatplaywidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcmakerwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrclocallinkwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccommentswidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrctranslatedwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrccolorwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcposterwidget.cpp
  ${TTK_WIDGET_LRCMANAGER_DIR}/musiclrcdownloadbatchwidget.cpp
)
