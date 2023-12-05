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

set_property(GLOBAL PROPERTY MUSIC_CORE_KITS_HEADERS
  ${MUSIC_CORE_DIR}/musicobject.h
  ${MUSIC_CORE_DIR}/musicformats.h
  ${MUSIC_CORE_DIR}/musicpluginproperty.h
  ${MUSIC_CORE_DIR}/musicglobaldefine.h
  ${MUSIC_CORE_DIR}/musichotkeymanager.h
  ${MUSIC_CORE_DIR}/musicconfigmanager.h
  ${MUSIC_CORE_DIR}/musicplayer.h
  ${MUSIC_CORE_DIR}/musicplaylist.h
  ${MUSIC_CORE_DIR}/musicbackgroundmanager.h
  ${MUSIC_CORE_DIR}/musicsettingmanager.h
  ${MUSIC_CORE_DIR}/musicconnectionpool.h
  ${MUSIC_CORE_DIR}/musicplatformmanager.h
  ${MUSIC_CORE_DIR}/musiccoremplayer.h
  ${MUSIC_CORE_DIR}/musicsong.h
  ${MUSIC_CORE_DIR}/musicsongmeta.h
  ${MUSIC_CORE_DIR}/musiccategoryconfigmanager.h
  ${MUSIC_CORE_DIR}/musicplaylistmanager.h
  ${MUSIC_CORE_DIR}/musicextractwrapper.h
  ${MUSIC_CORE_DIR}/musicruntimemanager.h
  ${MUSIC_CORE_DIR}/musicdispatchmanager.h
  ${MUSIC_CORE_DIR}/musicbackgroundconfigmanager.h
  ${MUSIC_CORE_DIR}/musicimagerenderer.h
)

set_property(GLOBAL PROPERTY MUSIC_CORE_KITS_SOURCES
  ${MUSIC_CORE_DIR}/musichotkeymanager.cpp
  ${MUSIC_CORE_DIR}/musicformats.cpp
  ${MUSIC_CORE_DIR}/musicplayer.cpp
  ${MUSIC_CORE_DIR}/musicplaylist.cpp
  ${MUSIC_CORE_DIR}/musicconfigmanager.cpp
  ${MUSIC_CORE_DIR}/musicbackgroundmanager.cpp
  ${MUSIC_CORE_DIR}/musicconnectionpool.cpp
  ${MUSIC_CORE_DIR}/musicplatformmanager.cpp
  ${MUSIC_CORE_DIR}/musicsingleton.cpp
  ${MUSIC_CORE_DIR}/musiccoremplayer.cpp
  ${MUSIC_CORE_DIR}/musicsong.cpp
  ${MUSIC_CORE_DIR}/musicsongmeta.cpp
  ${MUSIC_CORE_DIR}/musiccategoryconfigmanager.cpp
  ${MUSIC_CORE_DIR}/musicplaylistmanager.cpp
  ${MUSIC_CORE_DIR}/musicextractwrapper.cpp
  ${MUSIC_CORE_DIR}/musicruntimemanager.cpp
  ${MUSIC_CORE_DIR}/musicbackgroundconfigmanager.cpp
  ${MUSIC_CORE_DIR}/musicimagerenderer.cpp
)
  
if(UNIX)
  set_property(GLOBAL APPEND PROPERTY MUSIC_CORE_KITS_HEADERS ${MUSIC_CORE_DIR}/musicmprisplayer.h)
  set_property(GLOBAL APPEND PROPERTY MUSIC_CORE_KITS_SOURCES ${MUSIC_CORE_DIR}/musicmprisplayer.cpp)
endif()
