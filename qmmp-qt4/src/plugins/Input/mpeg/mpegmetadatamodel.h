/***************************************************************************
 *   Copyright (C) 2009-2019 by Ilya Kotov                                 *
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

#ifndef MPEGMETADATAMODEL_H
#define MPEGMETADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include <taglib/mpegfile.h>
#include <taglib/tfilestream.h>

class QTextCodec;

class MPEGMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(MPEGMetaDataModel)
public:
    MPEGMetaDataModel(bool using_rusxmms, const QString &path, bool readOnly);
    ~MPEGMetaDataModel();

    QList<MetaDataItem> extraProperties() const;
    QList<TagModel* > tags() const;
    QPixmap cover() const;
    void setCover(const QPixmap &pix);
    void removeCover();

private:
    QList<TagModel* > m_tags;
    TagLib::MPEG::File *m_file;
    TagLib::FileStream *m_stream;
};

class MpegFileTagModel : public TagModel
{
public:
    MpegFileTagModel(bool using_rusxmms, TagLib::MPEG::File *file, TagLib::MPEG::File::TagTypes tagType);
    ~MpegFileTagModel();

    QString name() const;
    QList<Qmmp::MetaData> keys() const;
    QString value(Qmmp::MetaData key) const;
    void setValue(Qmmp::MetaData key, const QString &value);
    bool exists() const;
    void create();
    void remove();
    void save();

private:
    bool m_using_rusxmms;
    QTextCodec *m_codec;
    TagLib::MPEG::File *m_file;
    TagLib::Tag *m_tag;
    TagLib::MPEG::File::TagTypes m_tagType;
};

#endif // MPEGMETADATAMODEL_H
