/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
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

#include <QtPlugin>
#include "outputwasapi.h"
#include "outputwasapifactory.h"

const OutputProperties OutputWASAPIFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("WASAPI Plugin");
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.shortName = "wasapi";
    return properties;
}

Output* OutputWASAPIFactory::create()
{
    return new OutputWASAPI();
}

Volume *OutputWASAPIFactory::createVolume()
{
    return new VolumeWASAPI;
}

Q_EXPORT_PLUGIN2(wasapi,OutputWASAPIFactory)
