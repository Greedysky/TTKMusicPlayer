# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2026 Greedysky Studio
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

set_property(GLOBAL PROPERTY TTK_WIDGET_UI_KITS_HEADER_FILES
  ${TTK_WIDGET_UI_DIR}/musicuiobject.h
  ${TTK_WIDGET_UI_DIR}/musiccontextuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicdesktoplrcuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicfunctionlistuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicfunctionuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicinteriorfloatuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicinteriorlrcuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicleftitemlistuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicmagicwidgetuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicqualitywidgetuiobject.h
  ${TTK_WIDGET_UI_DIR}/musictinyuiobject.h
  ${TTK_WIDGET_UI_DIR}/musictoolsetsuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicvideofloatuiobject.h
  ${TTK_WIDGET_UI_DIR}/musicvideouiobject.h
  ${TTK_WIDGET_UI_DIR}/musicwidgetheaders.h
)

set_property(GLOBAL PROPERTY TTK_UI_FILES
  ${TTK_WIDGET_UI_DIR}/musicApplication/musicapplication.ui
  ${TTK_WIDGET_UI_DIR}/musicLrcKits/musiclrcphotomanagerwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicLrcKits/musiclrcmakerwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicLrcKits/musiclrcsearchwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicLrcKits/musiclrclocallinkwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicLrcKits/musiclrccolorwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicLrcKits/musiclrcposterwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicLrcKits/musiclrcdownloadbatchwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musicfileinformationwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musicmobilesongsmanagerwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musictimerwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musictransformwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musicnetworkconnectiontestwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musicreplaygainwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musicsongchecktoolswidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musicsongringtonemakerwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musicspectrumwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsetsKits/musicsongdlnatransferwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsKits/musicwebfmradioplaywidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsKits/musicconnecttransferwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsKits/musiccloudfileinformationwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsKits/musictoolsetswidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsKits/musicttkfmradioplaywidget.ui
  ${TTK_WIDGET_UI_DIR}/musicToolsKits/musicttkfmradioinformationwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicbackgroundskindialog.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicbackgroundpalettewidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicdownloadwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicdownloadresetwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicequalizerdialog.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicfileassociationwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicmessagebox.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musiccolordialog.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicsettingwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicsongsharingwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicsongslistiteminfowidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicsourceupdatewidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicsoundeffectswidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicmessageaboutdialog.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicdownloadbatchwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicsongitemselecteddialog.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicqmmppluginwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicserverpluginwidget.ui
  ${TTK_WIDGET_UI_DIR}/musicWidgetKits/musicinputdialog.ui
)
