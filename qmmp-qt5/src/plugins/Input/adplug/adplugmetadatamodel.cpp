/***************************************************************************
 *   Copyright (C) 2006-2019 by Ilya Kotov                                 *
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

#include "adplugmetadatamodel.h"
#include "adplugwrap.h"

AdplugMetaDataModel::AdplugMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    try
    {
        AdplugWrap adplug(path.toUtf8().constData());
        fill_in_extra_properties(&adplug);
        fill_in_descriptions(&adplug);
    }
    catch(const AdplugWrap::InvalidFile &)
    {
    }
}

void AdplugMetaDataModel::fill_in_extra_properties(AdplugWrap *wrap)
{
    if(wrap->instrument_count() != 0)
    {
        QString text;
        for(const std::string &s : wrap->instruments())
        {
            text += QString::fromStdString(s) + "\n";
        }
        desc << MetaDataItem(tr("Instruments"), text);
    }
}

void AdplugMetaDataModel::fill_in_descriptions(AdplugWrap *wrap)
{
    ap << MetaDataItem(tr("Title"), QString::fromStdString(wrap->title()));
    ap << MetaDataItem(tr("Title"), QString::fromStdString(wrap->title()));
    ap << MetaDataItem(tr("Format"), QString::fromStdString(wrap->format()));
    ap << MetaDataItem(tr("Author"), QString::fromStdString(wrap->author()));
    ap << MetaDataItem(tr("Description"), QString::fromStdString(wrap->author()));
    if(wrap->pattern_count() != 0)
    {
        ap << MetaDataItem(tr("Patterns"), QString::number(wrap->pattern_count()));
    }
    if(wrap->instrument_count() != 0)
    {
        ap << MetaDataItem(tr("Instruments"), QString::number(wrap->instrument_count()));
    }
}

QList<MetaDataItem> AdplugMetaDataModel::extraProperties() const
{
    return ap;
}

QList<MetaDataItem> AdplugMetaDataModel::descriptions() const
{
    return desc;
}
