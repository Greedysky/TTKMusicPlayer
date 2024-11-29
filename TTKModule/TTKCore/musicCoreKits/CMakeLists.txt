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

set_property(GLOBAL PROPERTY TTK_CORE_KITS_HEADER_FILES
  ${TTK_CORE_DIR}/musicobject.h
  ${TTK_CORE_DIR}/musicformats.h
  ${TTK_CORE_DIR}/musicpluginproperty.h
  ${TTK_CORE_DIR}/musicglobaldefine.h
  ${TTK_CORE_DIR}/musichotkeymanager.h
  ${TTK_CORE_DIR}/musicconfigmanager.h
  ${TTK_CORE_DIR}/musicplayer.h
  ${TTK_CORE_DIR}/musicplaylist.h
  ${TTK_CORE_DIR}/musicbackgroundmanager.h
  ${TTK_CORE_DIR}/musicsettingmanager.h
  ${TTK_CORE_DIR}/musicconnectionpool.h
  ${TTK_CORE_DIR}/musicplatformmanager.h
  ${TTK_CORE_DIR}/musiccoremplayer.h
  ${TTK_CORE_DIR}/musicsong.h
  ${TTK_CORE_DIR}/musicsongmeta.h
  ${TTK_CORE_DIR}/musiccategoryconfigmanager.h
  ${TTK_CORE_DIR}/musicplaylistmanager.h
  ${TTK_CORE_DIR}/musicextractwrapper.h
  ${TTK_CORE_DIR}/musicruntimemanager.h
  ${TTK_CORE_DIR}/musicdispatchmanager.h
  ${TTK_CORE_DIR}/musicbackgroundconfigmanager.h
  ${TTK_CORE_DIR}/musicimagerenderer.h
  ${TTK_CORE_DIR}/musicprocessmanager.h
)

set_property(GLOBAL PROPERTY TTK_CORE_KITS_SOURCE_FILES
  ${TTK_CORE_DIR}/musichotkeymanager.cpp
  ${TTK_CORE_DIR}/musicformats.cpp
  ${TTK_CORE_DIR}/musicplayer.cpp
  ${TTK_CORE_DIR}/musicplaylist.cpp
  ${TTK_CORE_DIR}/musicconfigmanager.cpp
  ${TTK_CORE_DIR}/musicbackgroundmanager.cpp
  ${TTK_CORE_DIR}/musicconnectionpool.cpp
  ${TTK_CORE_DIR}/musicplatformmanager.cpp
  ${TTK_CORE_DIR}/musicsingleton.cpp
  ${TTK_CORE_DIR}/musiccoremplayer.cpp
  ${TTK_CORE_DIR}/musicsong.cpp
  ${TTK_CORE_DIR}/musicsongmeta.cpp
  ${TTK_CORE_DIR}/musiccategoryconfigmanager.cpp
  ${TTK_CORE_DIR}/musicplaylistmanager.cpp
  ${TTK_CORE_DIR}/musicextractwrapper.cpp
  ${TTK_CORE_DIR}/musicruntimemanager.cpp
  ${TTK_CORE_DIR}/musicbackgroundconfigmanager.cpp
  ${TTK_CORE_DIR}/musicimagerenderer.cpp
  ${TTK_CORE_DIR}/musicprocessmanager.cpp
)

if(UNIX)
  set_property(GLOBAL APPEND PROPERTY TTK_CORE_KITS_HEADER_FILES ${TTK_CORE_DIR}/musicmprisplayer.h)
  set_property(GLOBAL APPEND PROPERTY TTK_CORE_KITS_SOURCE_FILES ${TTK_CORE_DIR}/musicmprisplayer.cpp)
endif()
