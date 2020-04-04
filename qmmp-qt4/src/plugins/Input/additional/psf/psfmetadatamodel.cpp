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

#include "psfmetadatamodel.h"

PSFMetaDataModel::PSFMetaDataModel(const QString &path) :
    MetaDataModel(true)
{
    m_psf = new PSFHelper(path);
    m_tags << new PSFFileTagModel(m_psf);
}

PSFMetaDataModel::~PSFMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_psf;
}

QList<TagModel* > PSFMetaDataModel::tags() const
{
    return m_tags;
}

QList<MetaDataItem> PSFMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    if(m_psf->initialize())
    {
        const QVariantMap &data = m_psf->readMetaTags();
        for(const QString &key : data.keys())
        {
            ep << MetaDataItem(key, data[key]);
        }
    }

    return ep;
}


PSFFileTagModel::PSFFileTagModel(PSFHelper* psf)
    : TagModel()
{
    m_psf = psf;
}

PSFFileTagModel::~PSFFileTagModel()
{}

QString PSFFileTagModel::name() const
{
    return "ID3v1";
}

QList<Qmmp::MetaData> PSFFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    list.removeAll(Qmmp::GENRE);
    list.removeAll(Qmmp::COMMENT);
    list.removeAll(Qmmp::TRACK);
    return list;
}

QString PSFFileTagModel::value(Qmmp::MetaData key) const
{
    if(m_psf && m_psf->initialize())
    {
        m_psf->readMetaTags();
        switch((int) key)
        {
        case Qmmp::TITLE: return m_psf->title();
        case Qmmp::ARTIST: return m_psf->artist();
        case Qmmp::ALBUM: return m_psf->album();
        case Qmmp::YEAR: return m_psf->year();
        }
    }
    return QString();
}

void PSFFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
