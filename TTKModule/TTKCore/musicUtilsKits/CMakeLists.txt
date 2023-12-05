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

set_property(GLOBAL PROPERTY MUSIC_CORE_UTILS_KITS_HEADERS
  ${MUSIC_CORE_UTILS_DIR}/musiccoreutils.h
  ${MUSIC_CORE_UTILS_DIR}/musicwidgetutils.h
  ${MUSIC_CORE_UTILS_DIR}/musicnumberutils.h
  ${MUSIC_CORE_UTILS_DIR}/musicstringutils.h
  ${MUSIC_CORE_UTILS_DIR}/musicalgorithmutils.h
  ${MUSIC_CORE_UTILS_DIR}/musicurlutils.h
  ${MUSIC_CORE_UTILS_DIR}/musicqmmputils.h
  ${MUSIC_CORE_UTILS_DIR}/musiccodecutils.h
  ${MUSIC_CORE_UTILS_DIR}/musicfileutils.h
  ${MUSIC_CORE_UTILS_DIR}/musicimageutils.h
)

set_property(GLOBAL PROPERTY MUSIC_CORE_UTILS_KITS_SOURCES
  ${MUSIC_CORE_UTILS_DIR}/musiccoreutils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musicwidgetutils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musicnumberutils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musicstringutils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musicalgorithmutils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musicurlutils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musicqmmputils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musiccodecutils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musicfileutils.cpp
  ${MUSIC_CORE_UTILS_DIR}/musicimageutils.cpp
)
