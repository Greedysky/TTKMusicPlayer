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

#ifndef FFMPEGMETADATAMODEL_H
#define FFMPEGMETADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include <taglib/apetag.h>
#include <taglib/apefile.h>
#include <taglib/tfilestream.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class FFmpegMetaDataModel : public MetaDataModel
{
public:
    explicit FFmpegMetaDataModel(const QString &path, bool readOnly);
    virtual ~FFmpegMetaDataModel();

    virtual QList<TagModel*> tags() const override final;
    virtual QImage cover() const override final;
    virtual QString coverPath() const override final;
    virtual QString cue() const override final;
    virtual void setCue(const QString &content) override final;
    virtual void removeCue() override final;

private:
    QImage m_image;
    QList<TagModel*> m_tags;
    TagLib::APE::Tag *m_tag;
    TagLib::APE::File *m_file = nullptr;
    TagLib::FileStream *m_stream = nullptr;
    QString m_path;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class ApeTagModel : public TagModel
{
public:
    explicit ApeTagModel(TagLib::APE::File *file);

    virtual QString name() const override final;
    virtual QList<Qmmp::MetaData> keys() const override final;
    virtual QString value(Qmmp::MetaData key) const override final;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override final;
    virtual bool exists() const override final;
    virtual void create() override final;
    virtual void remove() override final;
    virtual void save() override final;

private:
    TagLib::APE::File *m_file;
    bool m_strip = false;

};

#endif
