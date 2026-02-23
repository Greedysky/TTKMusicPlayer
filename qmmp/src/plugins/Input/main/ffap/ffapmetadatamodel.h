/***************************************************************************
 *   Copyright (C) 2011-2026 by Ilya Kotov                                 *
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

#ifndef FFAPMETADATAMODEL_H
#define FFAPMETADATAMODEL_H

#include <taglib/apefile.h>
#include <taglib/tfilestream.h>
#include <qmmp/metadatamodel.h>

class QmmpTextCodec;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class FFapMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(FFapMetaDataModel)
public:
    FFapMetaDataModel(const QString &path, bool readOnly);
    virtual ~FFapMetaDataModel();

    virtual QList<MetaDataItem> extraProperties() const override final;
    virtual QList<TagModel*> tags() const override final;
    virtual QString coverPath() const override final;

private:
    QString m_path;
    QList<TagModel*> m_tags;
    TagLib::APE::File *m_file = nullptr;
    TagLib::FileStream *m_stream = nullptr;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class FFapFileTagModel : public TagModel
{
public:
    FFapFileTagModel(TagLib::APE::File *file, TagLib::APE::File::TagTypes type);

    virtual QString name() const override final;
    virtual QList<Qmmp::MetaData> keys() const override final;
    virtual QString value(Qmmp::MetaData key) const override final;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override final;
    virtual bool exists() const override final;
    virtual void create() override final;
    virtual void remove() override final;
    virtual void save() override final;

private:
    QmmpTextCodec *m_codec;
    TagLib::APE::File *m_file;
    TagLib::Tag *m_tag;
    TagLib::APE::File::TagTypes m_type;

};

#endif
