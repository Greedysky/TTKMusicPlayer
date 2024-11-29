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

set_property(GLOBAL PROPERTY TTK_WIDGET_REMOTE_KITS_HEADER_FILES
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidget.h
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsquare.h
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforrectangle.h
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsimplestyle.h
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforcomplexstyle.h
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforstrip.h
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforripple.h
)

set_property(GLOBAL PROPERTY TTK_WIDGET_REMOTE_KITS_SOURCE_FILES
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidget.cpp
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsquare.cpp
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforrectangle.cpp
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforsimplestyle.cpp
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforcomplexstyle.cpp
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforstrip.cpp
  ${TTK_WIDGET_REMOTEWIDGET_DIR}/musicremotewidgetforripple.cpp
)
