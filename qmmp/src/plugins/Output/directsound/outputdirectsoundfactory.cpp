/***************************************************************************
 *   Copyright (C) 2014-2020 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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

#include "outputdirectsound.h"
#include "outputdirectsoundfactory.h"

OutputProperties OutputDirectSoundFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("DirectSound Plugin");
    properties.hasSettings = false;
    properties.shortName = "directsound";
    return properties;
}

Output* OutputDirectSoundFactory::create()
{
    return new OutputDirectSound();
}

Volume *OutputDirectSoundFactory::createVolume()
{
    return new VolumeDirectSound();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(directsound, OutputDirectSoundFactory)
#endif
