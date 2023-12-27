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

set_property(GLOBAL PROPERTY MUSIC_WIDGET_LRC_KITS_HEADERS
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanager.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerfordesktop.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerforinterior.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainer.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerfordesktop.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforinterior.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforwallpaper.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcsearchwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcsearchtablewidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcphotomanagerwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatsettingwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatphotowidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatplaywidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmakerwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrclocallinkwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccommentswidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrctranslatedwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccolorwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcposterwidget.h
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcdownloadbatchwidget.h
)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_LRC_KITS_SOURCES
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanager.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerfordesktop.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmanagerforinterior.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforwallpaper.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainer.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerfordesktop.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccontainerforinterior.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcsearchwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcsearchtablewidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcphotomanagerwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatsettingwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatphotowidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcfloatplaywidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcmakerwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrclocallinkwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccommentswidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrctranslatedwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrccolorwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcposterwidget.cpp
  ${MUSIC_WIDGET_LRCMANAGER_DIR}/musiclrcdownloadbatchwidget.cpp
)
