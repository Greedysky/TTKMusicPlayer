/***************************************************************************
 *   Copyright (C) 2009-2025 by Ilya Kotov                                 *
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

#include <taglib/mpegfile.h>
#include <taglib/tfilestream.h>
#include <qmmp/metadatamodel.h>

class QmmpTextCodec;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class MPEGMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(MPEGMetaDataModel)
public:
    MPEGMetaDataModel(const QString &path, bool readOnly);
    virtual ~MPEGMetaDataModel();

    virtual QList<MetaDataItem> extraProperties() const override final;
    virtual QList<TagModel*> tags() const override final;
    virtual QImage cover() const override final;
    virtual void setCover(const QImage &img) override final;
    virtual void removeCover() override final;
    virtual QString lyrics() const override final;

private:
    QList<TagModel*> m_tags;
    TagLib::MPEG::File *m_file;
    TagLib::FileStream *m_stream;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class MpegFileTagModel : public TagModel
{
public:
    MpegFileTagModel(TagLib::MPEG::File *file, TagLib::MPEG::File::TagTypes type);

    virtual QString name() const override final;
    virtual QList<Qmmp::MetaData> keys() const override final;
    virtual QString value(Qmmp::MetaData key) const override final;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override final;
    virtual bool exists() const override final;
    virtual void create() override final;
    virtual void remove() override final;
    virtual void save() override final;

    QString lyrics() const;

private:
    QmmpTextCodec *m_codec;
    TagLib::MPEG::File *m_file;
    TagLib::Tag *m_tag;
    TagLib::MPEG::File::TagTypes m_type;

};

#endif
