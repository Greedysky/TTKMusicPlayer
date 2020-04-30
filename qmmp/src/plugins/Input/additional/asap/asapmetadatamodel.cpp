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

#include "asapmetadatamodel.h"

AsapMetaDataModel::AsapMetaDataModel(const QString &path) :
    MetaDataModel(true)
{
    m_asap = new AsapHelper(path);
    m_tags << new AsapFileTagModel(m_asap);
}

AsapMetaDataModel::~AsapMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_asap;
}

QList<TagModel* > AsapMetaDataModel::tags() const
{
    return m_tags;
}

QList<MetaDataItem> AsapMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    if(m_asap->initialize())
    {
        const QVariantMap &data = m_asap->readMetaTags();
        for(const QString &key : data.keys())
        {
            ep << MetaDataItem(key, data[key]);
        }
    }

    return ep;
}


AsapFileTagModel::AsapFileTagModel(AsapHelper* asap)
    : TagModel()
{
    m_asap = asap;
}

AsapFileTagModel::~AsapFileTagModel()
{}

QString AsapFileTagModel::name() const
{
    return "ID3v1";
}

QList<Qmmp::MetaData> AsapFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    list.removeAll(Qmmp::GENRE);
    list.removeAll(Qmmp::COMMENT);
    list.removeAll(Qmmp::TRACK);
    list.removeAll(Qmmp::ALBUM);
    return list;
}

QString AsapFileTagModel::value(Qmmp::MetaData key) const
{
    if(m_asap && m_asap->initialize())
    {
        m_asap->readMetaTags();
        switch((int) key)
        {
        case Qmmp::TITLE: return m_asap->title();
        case Qmmp::ARTIST: return m_asap->artist();
        case Qmmp::YEAR: return m_asap->year();
        }
    }
    return QString();
}

void AsapFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
