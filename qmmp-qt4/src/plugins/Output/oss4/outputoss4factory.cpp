/***************************************************************************
 *   Copyright (C) 2007 by Zhuravlev Uriy                                  *
 *   stalkerg@gmail.com                                                    *
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

#include <QtGui>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"
#include "outputoss4.h"
#include "outputoss4factory.h"


Output* OutputOSS4Factory::create()
{
    return new OutputOSS4();
}

const OutputProperties OutputOSS4Factory::properties() const
{
    OutputProperties properties;
    properties.name = tr("OSS4 Plugin");
    properties.shortName = "oss4";
    properties.hasAbout = true;
    properties.hasSettings = true;
    return properties;
}

Volume *OutputOSS4Factory::createVolume()
{
    return new VolumeOSS4;
}

void OutputOSS4Factory::showSettings(QWidget* parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}


Q_EXPORT_PLUGIN2(oss4,OutputOSS4Factory)
