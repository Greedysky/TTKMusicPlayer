/***************************************************************************
 *   Copyright (C) 2013-2025 by Ilya Kotov                                 *
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

#ifndef OPUSMETADATAMODEL_H
#define OPUSMETADATAMODEL_H

#include <taglib/opusfile.h>
#include <taglib/tfilestream.h>
#include <qmmp/metadatamodel.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OpusMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(OpusMetaDataModel)
public:
    OpusMetaDataModel(const QString &path, bool readOnly);
    virtual ~OpusMetaDataModel();

    virtual QList<MetaDataItem> extraProperties() const override final;
    virtual QList<TagModel*> tags() const override final;
    virtual QImage cover() const override final;
    virtual void setCover(const QImage &img) override final;
    virtual void removeCover() override final;
    virtual QString lyrics() const override final;

private:
    QString m_path;
    QList<TagModel*> m_tags;
    TagLib::Ogg::Opus::File *m_file;
    TagLib::FileStream *m_stream;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VorbisCommentModel : public TagModel
{
public:
    explicit VorbisCommentModel(TagLib::Ogg::Opus::File *file);

    virtual QString name() const override final;
    virtual QString value(Qmmp::MetaData key) const override final;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override final;
    virtual void save() override final;

private:
    TagLib::Ogg::Opus::File *m_file;
    TagLib::Ogg::XiphComment *m_tag;

};

#endif
