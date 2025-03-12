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

#ifndef VORBISMETADATAMODEL_H
#define VORBISMETADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include <taglib/vorbisfile.h>
#include <taglib/tfilestream.h>

class VorbisCommentModel;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VorbisMetaDataModel : public MetaDataModel
{
public:
    VorbisMetaDataModel(const QString &path, bool readOnly);
    virtual ~VorbisMetaDataModel();

    friend class VorbisCommentModel;

    virtual QList<TagModel*> tags() const override final;
    virtual QImage cover() const override final;
    virtual void setCover(const QImage &img) override final;
    virtual void removeCover() override final;
    virtual QString lyrics() const override final;

private:
    QString m_path;
    TagLib::Ogg::Vorbis::File *m_file;
    TagLib::Ogg::XiphComment *m_tag;
    QList<TagModel*> m_tags;
    TagLib::FileStream *m_stream;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VorbisCommentModel : public TagModel
{
public:
    explicit VorbisCommentModel(VorbisMetaDataModel *model);

    virtual QString name() const override final;
    virtual QString value(Qmmp::MetaData key) const override final;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override final;
    virtual void save() override final;

private:
    VorbisMetaDataModel *m_model;

};

#endif
