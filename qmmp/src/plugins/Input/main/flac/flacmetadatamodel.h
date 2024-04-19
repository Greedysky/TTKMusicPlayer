/***************************************************************************
 *   Copyright (C) 2009-2024 by Ilya Kotov                                 *
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

#ifndef FLACMETADATAMODEL_H
#define FLACMETADATAMODEL_H

#include <taglib/flacfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/tfilestream.h>
#include <qmmp/metadatamodel.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class FLACMetaDataModel : public MetaDataModel
{
public:
    FLACMetaDataModel(const QString &path, bool readOnly);
    virtual ~FLACMetaDataModel();

    virtual QList<TagModel*> tags() const override final;
    virtual QPixmap cover() const override final;
    virtual QString coverPath() const override final;
    virtual void setCover(const QPixmap &pix) override final;
    virtual void removeCover() override final;
    virtual QString cue() const override final;
    virtual void setCue(const QString &content) override final;
    virtual void removeCue() override final;
    virtual QString lyrics() const override final;

private:
    QString m_path;
    QList<TagModel*> m_tags;
    TagLib::Ogg::XiphComment *m_tag = nullptr;
    TagLib::File *m_file = nullptr;
    TagLib::FileStream *m_stream = nullptr;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VorbisCommentModel : public TagModel
{
public:
    VorbisCommentModel(TagLib::Ogg::XiphComment *tag, TagLib::File *file);

    virtual QString name() const override final;
    virtual QString value(Qmmp::MetaData key) const override final;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override final;
    virtual void save() override final;

private:
    TagLib::File *m_file;
    TagLib::Ogg::XiphComment *m_tag;

};

#endif
