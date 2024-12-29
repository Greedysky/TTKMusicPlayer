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

#ifndef MPCMETADATAMODEL_H
#define MPCMETADATAMODEL_H

#include <taglib/mpcfile.h>
#include <taglib/tfilestream.h>
#include <qmmp/metadatamodel.h>

class QTextCodec;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class MPCMetaDataModel : public MetaDataModel
{
public:
    MPCMetaDataModel(const QString &path, bool readOnly);
    virtual ~MPCMetaDataModel();

    virtual QList<TagModel*> tags() const override final;

private:
    QList<TagModel*> m_tags;
    TagLib::MPC::File *m_file;
    TagLib::FileStream *m_stream;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class MPCFileTagModel : public TagModel
{
public:
    MPCFileTagModel(TagLib::MPC::File *file, TagLib::MPC::File::TagTypes type);

    virtual QString name() const override final;
    virtual QList<Qmmp::MetaData> keys() const override final;
    virtual QString value(Qmmp::MetaData key) const override final;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override final;
    virtual bool exists() const override final;
    virtual void create() override final;
    virtual void remove() override final;
    virtual void save() override final;

private:
    QTextCodec *m_codec;
    TagLib::MPC::File *m_file;
    TagLib::Tag *m_tag;
    TagLib::MPC::File::TagTypes m_type;

};

#endif
