/***************************************************************************
 *   Copyright (C) 2009-2022 by Ilya Kotov                                 *
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

class FFmpegMetaDataModel : public MetaDataModel
{
public:
    explicit FFmpegMetaDataModel(const QString &path, bool readOnly);
    virtual ~FFmpegMetaDataModel();

    virtual QList<TagModel*> tags() const override;
    virtual QPixmap cover() const override;
    virtual QString coverPath() const override;
    virtual QString cue() const override;
    virtual void setCue(const QString &content) override;
    virtual void removeCue() override;

private:
    QPixmap m_pixmap;
    QList<TagModel*> m_tags;
    TagLib::APE::Tag *m_tag;
    TagLib::APE::File *m_file = nullptr;
    TagLib::FileStream *m_stream = nullptr;
    QString m_path;

};

class ApeTagModel : public TagModel
{
public:
    explicit ApeTagModel(TagLib::APE::File *file);
    virtual ~ApeTagModel();

    virtual QString name() const override;
    virtual QList<Qmmp::MetaData> keys() const override;
    virtual QString value(Qmmp::MetaData key) const override;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override;
    virtual bool exists() const override;
    virtual void create() override;
    virtual void remove() override;
    virtual void save() override;

private:
    TagLib::APE::File *m_file;
    bool m_strip = false;

};

#endif
