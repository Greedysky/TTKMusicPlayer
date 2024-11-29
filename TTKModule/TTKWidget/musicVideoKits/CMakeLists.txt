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

set_property(GLOBAL PROPERTY TTK_WIDGET_VIDEO_KITS_HEADER_FILES
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicbarragerecord.h
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicbarragewidget.h
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideocontrolwidget.h
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideoview.h
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideosearchtablewidget.h
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideoplaywidget.h
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideofloatwidget.h
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideoqualitypopwidget.h
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideobarragestylepopwidget.h
)

set_property(GLOBAL PROPERTY TTK_WIDGET_VIDEO_KITS_SOURCE_FILES
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicbarragewidget.cpp
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideocontrolwidget.cpp
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideoview.cpp
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideosearchtablewidget.cpp
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideoplaywidget.cpp
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideofloatwidget.cpp
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideoqualitypopwidget.cpp
  ${TTK_WIDGET_VIDEOKITS_DIR}/musicvideobarragestylepopwidget.cpp
)
