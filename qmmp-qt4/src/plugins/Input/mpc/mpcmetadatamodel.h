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

#ifndef MPCMETADATAMODEL_H
#define MPCMETADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include <taglib/tag.h>
#include <taglib/mpcfile.h>
#include <taglib/tfilestream.h>

class QTextCodec;

class MPCMetaDataModel : public MetaDataModel
{
public:
    MPCMetaDataModel(const QString &path, bool readOnly);
    ~MPCMetaDataModel();

    QList<TagModel* > tags() const;

private:
    QList<TagModel* > m_tags;
    TagLib::MPC::File *m_file;
    TagLib::FileStream *m_stream;
};

class MPCFileTagModel : public TagModel
{
public:
    MPCFileTagModel(TagLib::MPC::File *file, TagLib::MPC::File::TagTypes tagType);
    ~MPCFileTagModel();

    QString name() const;
    QList<Qmmp::MetaData> keys() const;
    QString value(Qmmp::MetaData key) const;
    void setValue(Qmmp::MetaData key, const QString &value);
    bool exists() const;
    void create();
    void remove();
    void save();

private:
    QTextCodec *m_codec;
    TagLib::MPC::File *m_file;
    TagLib::Tag *m_tag;
    TagLib::MPC::File::TagTypes m_tagType;
};

#endif // MPCMETADATAMODEL_H
