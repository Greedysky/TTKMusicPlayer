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

set_property(GLOBAL PROPERTY TTK_WIDGET_TOOLS_KITS_HEADERS
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicsongstoolboxwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicsongscontainerwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musictoolsetswidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicconnectlocalwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransferwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransfertablewidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiclocalmanagerwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicttkfmradioplaywidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicpersonalradioquerywidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebfmradioplaywidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradiocategorywidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradiowidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradioquerywidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradioinfowidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioquerywidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioquerycategorypopwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioinfowidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebradioview.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudsharedsongwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudmanagerwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicdownloadrecordwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudfileinformationwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudtoolboxwidget.h
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudtablewidget.h
)

set_property(GLOBAL PROPERTY TTK_WIDGET_TOOLS_KITS_SOURCES
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicsongstoolboxwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicsongscontainerwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musictoolsetswidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicconnectlocalwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransferwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicconnecttransfertablewidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiclocalmanagerwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicttkfmradioplaywidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicpersonalradioquerywidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebfmradioplaywidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradiocategorywidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradiowidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradioquerywidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebdjradioinfowidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioquerywidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioquerycategorypopwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebmvradioinfowidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicwebradioview.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudsharedsongwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudmanagerwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musicdownloadrecordwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudfileinformationwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudtoolboxwidget.cpp
  ${TTK_WIDGET_TOOLSWIDGET_DIR}/musiccloudtablewidget.cpp
)
