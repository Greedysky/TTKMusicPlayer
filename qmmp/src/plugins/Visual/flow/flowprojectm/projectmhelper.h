/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef PROJECTMHELPER_H
#define PROJECTMHELPER_H

#include <QDir>
#include <qmmp/qmmp.h>
#include <qmmp/visual.h>

#ifndef PROJECTM_PRESET_PATH
#define PROJECTM_PRESET_PATH Qmmp::ttkPluginPath() + "/config/presets"
#endif

#ifndef PROJECTM_TEXTURE_PATH
#define PROJECTM_TEXTURE_PATH Qmmp::ttkPluginPath() + "/config/textures"
#endif

QFileInfoList fileListByPath(const QString &dpath, const QStringList &filter);

#endif
