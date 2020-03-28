/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
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

#include "shnmetadatamodel.h"

SHNMetaDataModel::SHNMetaDataModel(const QString &path) :
    MetaDataModel(true)
{
    m_shn = new SHNHelper(path);
    m_tags << new SHNFileTagModel(m_shn, stdio_id3v1);
    m_tags << new SHNFileTagModel(m_shn, stdio_id3v2);
    m_tags << new SHNFileTagModel(m_shn, stdio_apev2);
}

SHNMetaDataModel::~SHNMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_shn;
}

QList<TagModel* > SHNMetaDataModel::tags() const
{
    return m_tags;
}


SHNFileTagModel::SHNFileTagModel(SHNHelper* shn, stdio_meta_type stdio_meta)
    : TagModel()
{
    m_shn = shn;
    m_stdio_meta = stdio_meta;
}

SHNFileTagModel::~SHNFileTagModel()
{}

QString SHNFileTagModel::name() const
{
    if(m_stdio_meta == stdio_id3v1)
        return "ID3v1";
    else if(m_stdio_meta == stdio_id3v2)
        return "ID3v2";
    return "APE";
}

QList<Qmmp::MetaData> SHNFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    if(m_stdio_meta == stdio_id3v2)
        return list;
    else if(m_stdio_meta == stdio_apev2)
    {
        list.removeAll(Qmmp::DISCNUMBER);
        return list;
    }
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    return list;
}

QString SHNFileTagModel::value(Qmmp::MetaData key) const
{
    if(m_shn)
    {
        const QVariantMap &meta = m_shn->readTags(m_stdio_meta);
        switch((int) key)
        {
        case Qmmp::TITLE: return meta.value("title", QString()).toString();
        case Qmmp::ARTIST: return meta.value("artist", QString()).toString();
        case Qmmp::ALBUMARTIST: return meta.value("albumartist", QString()).toString();
        case Qmmp::ALBUM: return meta.value("album", QString()).toString();
        case Qmmp::COMMENT: return meta.value("comment", QString()).toString();
        case Qmmp::GENRE: return meta.value("genre", QString()).toString();
        case Qmmp::COMPOSER: return meta.value("COMPOSER", QString()).toString();
        case Qmmp::YEAR: return meta.value("year", QString()).toString();
        case Qmmp::TRACK: return meta.value("track", QString()).toString();
        case Qmmp::DISCNUMBER: return meta.value("TPOS", QString()).toString();
        }
    }
    return QString();
}

void SHNFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
