/***************************************************************************
 *   Copyright (C) 2010 by Ilya Kotov                                      *
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
#include <qmmp/qmmp.h>
#include "effectcrossfadefactory.h"
#include "settingsdialog.h"
#include "crossfadeplugin.h"

const EffectProperties EffectCrossfadeFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Crossfade Plugin");
    properties.shortName = "crossfade";
    properties.hasSettings = true;
    properties.hasAbout = true;
    properties.priority = EffectProperties::EFFECT_PRIORITY_LOW;
    return properties;
}

Effect *EffectCrossfadeFactory::create()
{
    return new CrossfadePlugin();
}

void EffectCrossfadeFactory::showSettings(QWidget *parent)
{
    SettingsDialog *dialog = new SettingsDialog(parent);
    dialog->show();
}

Q_EXPORT_PLUGIN2(crossfade,EffectCrossfadeFactory)
