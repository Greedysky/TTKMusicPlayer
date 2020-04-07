/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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
 ================================================= */

#include "sc68metadatamodel.h"

SC68MetaDataModel::SC68MetaDataModel(const QString &path) :
    MetaDataModel(true)
{
    m_sc68 = new SC68Helper(path);
    m_tags << new SC68FileTagModel(m_sc68);
}

SC68MetaDataModel::~SC68MetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_sc68;
}

QList<TagModel* > SC68MetaDataModel::tags() const
{
    return m_tags;
}

QList<MetaDataItem> SC68MetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    if(m_sc68->initialize())
    {
        const QVariantMap &data = m_sc68->readMetaTags();
        for(const QString &key : data.keys())
        {
            ep << MetaDataItem(key, data[key]);
        }
    }

    return ep;
}


SC68FileTagModel::SC68FileTagModel(SC68Helper* sc68)
    : TagModel()
{
    m_sc68 = sc68;
}

SC68FileTagModel::~SC68FileTagModel()
{}

QString SC68FileTagModel::name() const
{
    return "ID3v1";
}

QList<Qmmp::MetaData> SC68FileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    list.removeAll(Qmmp::COMMENT);
    return list;
}

QString SC68FileTagModel::value(Qmmp::MetaData key) const
{
    if(m_sc68 && m_sc68->initialize())
    {
        m_sc68->readMetaTags();
        switch((int) key)
        {
        case Qmmp::TITLE: return m_sc68->title();
        case Qmmp::ARTIST: return m_sc68->artist();
        case Qmmp::ALBUM: return m_sc68->album();
        case Qmmp::YEAR: return m_sc68->year();
        case Qmmp::TRACK: return m_sc68->track();
        case Qmmp::GENRE: return m_sc68->genre();
        }
    }
    return QString();
}

void SC68FileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
