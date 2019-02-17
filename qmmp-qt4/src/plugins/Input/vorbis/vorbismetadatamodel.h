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

#ifndef VORBISMETADATAMODEL_H
#define VORBISMETADATAMODEL_H

#include <taglib/vorbisfile.h>
#include <taglib/xiphcomment.h>
#include <qmmp/metadatamodel.h>
#include <taglib/tfilestream.h>

class VorbisCommentModel;

#if (TAGLIB_MAJOR_VERSION > 1) || ((TAGLIB_MAJOR_VERSION == 1) && (TAGLIB_MINOR_VERSION >= 11))
#define HAS_PICTURE_LIST
#endif

class VorbisMetaDataModel : public MetaDataModel
{
public:
    VorbisMetaDataModel(const QString &path, bool readOnly);
    ~VorbisMetaDataModel();

    friend class VorbisCommentModel;

    QList<TagModel* > tags() const;
    QPixmap cover() const;
#ifdef HAS_PICTURE_LIST
    void setCover(const QPixmap &pix);
    void removeCover();
#endif

private:
    QString m_path;
    TagLib::Ogg::Vorbis::File *m_file;
    TagLib::Ogg::XiphComment *m_tag;
    QList<TagModel* > m_tags;
    TagLib::FileStream *m_stream;
};

class VorbisCommentModel : public TagModel
{
public:
    VorbisCommentModel(VorbisMetaDataModel *model);
    ~VorbisCommentModel();

    QString name() const;
    QString value(Qmmp::MetaData key) const;
    void setValue(Qmmp::MetaData key, const QString &value);
    void save();

private:
    VorbisMetaDataModel *m_model;
};

#endif // VORBISMETADATAMODEL_H
