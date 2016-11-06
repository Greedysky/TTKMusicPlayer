/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "projectmwrapper.h"

ProjectMWrapper::ProjectMWrapper(std::string config_file, int flags, QObject *parent) :
    QObject(parent), projectM(config_file, flags)
{
}

ProjectMWrapper::ProjectMWrapper(projectM::Settings settings, int flags, QObject *parent) :
    QObject(parent), projectM(settings, flags)
{
}

ProjectMWrapper::~ProjectMWrapper()
{
}

void ProjectMWrapper::selectPreset(int index)
{
    if(index >= 0)
        projectM::selectPreset(index);
}

void ProjectMWrapper::presetSwitchedEvent(bool isHardCut, unsigned int index) const
{
    Q_UNUSED(isHardCut);
    emit currentPresetChanged(index);
}
