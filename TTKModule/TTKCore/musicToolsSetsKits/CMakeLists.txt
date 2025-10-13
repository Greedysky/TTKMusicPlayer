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

set_property(GLOBAL PROPERTY TTK_CORE_TOOLSETS_KITS_HEADER_FILES
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicaudiorecordermodule.h
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicbackupmodule.h
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperthread.h
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musictimerautomodule.h
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicsongsmanagerthread.h
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsunit.h
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsthread.h
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicnetworktestthread.h
)

set_property(GLOBAL PROPERTY TTK_CORE_TOOLSETS_KITS_SOURCE_FILES
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicaudiorecordermodule.cpp
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicbackupmodule.cpp
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperthread.cpp
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musictimerautomodule.cpp
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicsongsmanagerthread.cpp
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsthread.cpp
  ${TTK_CORE_TOOLSETSWIDGET_DIR}/musicnetworktestthread.cpp
)
