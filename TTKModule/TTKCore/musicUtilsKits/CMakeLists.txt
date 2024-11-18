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

set_property(GLOBAL PROPERTY TTK_CORE_UTILS_KITS_HEADERS
  ${TTK_CORE_UTILS_DIR}/musiccoreutils.h
  ${TTK_CORE_UTILS_DIR}/musicwidgetutils.h
  ${TTK_CORE_UTILS_DIR}/musicnumberutils.h
  ${TTK_CORE_UTILS_DIR}/musicstringutils.h
  ${TTK_CORE_UTILS_DIR}/musicalgorithmutils.h
  ${TTK_CORE_UTILS_DIR}/musicurlutils.h
  ${TTK_CORE_UTILS_DIR}/musicqmmputils.h
  ${TTK_CORE_UTILS_DIR}/musiccodecutils.h
  ${TTK_CORE_UTILS_DIR}/musicfileutils.h
  ${TTK_CORE_UTILS_DIR}/musicimageutils.h
)

set_property(GLOBAL PROPERTY TTK_CORE_UTILS_KITS_SOURCES
  ${TTK_CORE_UTILS_DIR}/musiccoreutils.cpp
  ${TTK_CORE_UTILS_DIR}/musicwidgetutils.cpp
  ${TTK_CORE_UTILS_DIR}/musicnumberutils.cpp
  ${TTK_CORE_UTILS_DIR}/musicstringutils.cpp
  ${TTK_CORE_UTILS_DIR}/musicalgorithmutils.cpp
  ${TTK_CORE_UTILS_DIR}/musicurlutils.cpp
  ${TTK_CORE_UTILS_DIR}/musicqmmputils.cpp
  ${TTK_CORE_UTILS_DIR}/musiccodecutils.cpp
  ${TTK_CORE_UTILS_DIR}/musicfileutils.cpp
  ${TTK_CORE_UTILS_DIR}/musicimageutils.cpp
)
