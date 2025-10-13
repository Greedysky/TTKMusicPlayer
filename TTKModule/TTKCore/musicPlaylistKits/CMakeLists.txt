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

set_property(GLOBAL PROPERTY TTK_CORE_PLAYLIST_KITS_HEADER_FILES
  ${TTK_CORE_PLAYLIST_DIR}/musicplaylistmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musicasxconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musicdbplconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musicfplconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musicm3uconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musicplaylistinterface.h
  ${TTK_CORE_PLAYLIST_DIR}/musicplsconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musictkplconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musicwplconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musicxspfconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musiccsvconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musictxtconfigmanager.h
  ${TTK_CORE_PLAYLIST_DIR}/musicjspfconfigmanager.h
)

set_property(GLOBAL PROPERTY TTK_CORE_PLAYLIST_KITS_SOURCE_FILES
  ${TTK_CORE_PLAYLIST_DIR}/musicplaylistmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musicasxconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musicdbplconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musicfplconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musicm3uconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musicplsconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musictkplconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musicwplconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musicxspfconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musiccsvconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musictxtconfigmanager.cpp
  ${TTK_CORE_PLAYLIST_DIR}/musicjspfconfigmanager.cpp
)
