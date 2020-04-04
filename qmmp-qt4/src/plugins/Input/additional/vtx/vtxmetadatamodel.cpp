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

#include "vtxmetadatamodel.h"

VTXMetaDataModel::VTXMetaDataModel(const QString &path) :
    MetaDataModel(true)
{
    m_vtx = new VTXHelper(path);
    m_tags << new VTXFileTagModel(m_vtx);
}

VTXMetaDataModel::~VTXMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_vtx;
}

QList<TagModel* > VTXMetaDataModel::tags() const
{
    return m_tags;
}


VTXFileTagModel::VTXFileTagModel(VTXHelper* vtx)
    : TagModel()
{
    m_vtx = vtx;
}

VTXFileTagModel::~VTXFileTagModel()
{}

QString VTXFileTagModel::name() const
{
    return "ID3v1";
}

QList<Qmmp::MetaData> VTXFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    list.removeAll(Qmmp::GENRE);
    list.removeAll(Qmmp::YEAR);
    return list;
}

QString VTXFileTagModel::value(Qmmp::MetaData key) const
{
    if(m_vtx && m_vtx->initialize())
    {
        m_vtx->readMetaTags();
        switch((int) key)
        {
        case Qmmp::TITLE: return m_vtx->title();
        case Qmmp::ARTIST: return m_vtx->artist();
        case Qmmp::ALBUM: return m_vtx->album();
        case Qmmp::COMMENT: return m_vtx->comment();
        case Qmmp::TRACK: return m_vtx->tracker();
        }
    }
    return QString();
}

void VTXFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
