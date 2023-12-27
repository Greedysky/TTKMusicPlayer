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

set_property(GLOBAL PROPERTY MUSIC_WIDGET_REMOTE_KITS_HEADERS
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidget.h
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsquare.h
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforrectangle.h
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsimplestyle.h
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforcomplexstyle.h
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforstrip.h
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforripple.h
)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_REMOTE_KITS_SOURCES
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidget.cpp
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsquare.cpp
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforrectangle.cpp
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsimplestyle.cpp
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforcomplexstyle.cpp
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforstrip.cpp
  ${MUSIC_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforripple.cpp
)
