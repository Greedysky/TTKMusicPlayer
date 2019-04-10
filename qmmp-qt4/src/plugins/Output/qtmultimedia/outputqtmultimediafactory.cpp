/***************************************************************************
 *   Copyright (C) 2015 by Ivan Ponomarev                                  *
 *   ivantrue@gmail.com                                                    *
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

#include "outputqtmultimediafactory.h"
#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "outputqtmultimedia.h"


const OutputProperties OutputQtMultimediaFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("Qt Multimedia Plugin");
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.shortName = "qtmultimedia";
    return properties;
}

Output* OutputQtMultimediaFactory::create()
{
    return new OutputQtMultimedia();
}

Volume *OutputQtMultimediaFactory::createVolume()
{
    return 0;
}

Q_EXPORT_PLUGIN2(qtmultimedia, OutputQtMultimediaFactory)
