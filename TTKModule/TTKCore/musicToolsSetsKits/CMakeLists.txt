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

set_property(GLOBAL PROPERTY MUSIC_CORE_TOOLSETS_KITS_HEADERS
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicaudiorecordermodule.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicbackupmodule.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperthread.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musictimerautomodule.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongsmanagerthread.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsunit.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsthread.h
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicnetworktestthread.h
)

set_property(GLOBAL PROPERTY MUSIC_CORE_TOOLSETS_KITS_SOURCES
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicaudiorecordermodule.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicbackupmodule.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicdesktopwallpaperthread.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musictimerautomodule.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongsmanagerthread.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicsongchecktoolsthread.cpp
  ${MUSIC_CORE_TOOLSETSWIDGET_DIR}/musicnetworktestthread.cpp
)
