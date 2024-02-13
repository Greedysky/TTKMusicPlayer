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

set_property(GLOBAL PROPERTY MUSIC_WIDGET_TOOLS_KITS_HEADERS
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicsongstoolboxwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicsongscontainerwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musictoolsetswidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnectlocalwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransferwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransfertablewidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiclocalmanagerwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicttkfmradioplaywidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebfmradioplaywidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradiocategorywidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradiowidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradioquerywidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradioinfowidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioquerywidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioquerycategorypopwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioinfowidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebradioview.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudsharedsongwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudmanagerwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicdownloadrecordwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudfileinformationwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudtoolboxwidget.h
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudtablewidget.h
)

set_property(GLOBAL PROPERTY MUSIC_WIDGET_TOOLS_KITS_SOURCES
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicsongstoolboxwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicsongscontainerwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musictoolsetswidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnectlocalwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransferwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransfertablewidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiclocalmanagerwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicttkfmradioplaywidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebfmradioplaywidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradiocategorywidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradiowidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradioquerywidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradioinfowidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioquerywidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioquerycategorypopwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioinfowidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicwebradioview.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudsharedsongwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudmanagerwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musicdownloadrecordwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudfileinformationwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudtoolboxwidget.cpp
  ${MUSIC_WIDGET_TOOLSWIDGET_DIR}/musiccloudtablewidget.cpp
)
