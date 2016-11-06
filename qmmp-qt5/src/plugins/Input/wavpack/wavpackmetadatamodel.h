/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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

#ifndef WAVPACKMETADATAMODEL_H
#define WAVPACKMETADATAMODEL_H

extern "C"{
#include <wavpack/wavpack.h>
}
#include <qmmp/metadatamodel.h>

class WavPackMetaDataModel : public MetaDataModel
{
Q_OBJECT
public:
    WavPackMetaDataModel(const QString &path, QObject *parent);
    ~WavPackMetaDataModel();
    QHash<QString, QString> audioProperties();
    QList<TagModel* > tags();
    QPixmap cover();
    QString coverPath();

private:
    WavpackContext *m_ctx;
    QList <TagModel *> m_tags;
    QString m_path;
};

class WavPackFileTagModel : public TagModel
{
public:
    WavPackFileTagModel(WavpackContext *ctx);
    ~WavPackFileTagModel();
    const QString name();
    const QString value(Qmmp::MetaData key);
    void setValue(Qmmp::MetaData key, const QString &value);
    void save();

private:
     WavpackContext *m_ctx;
     QString m_path;
};

#endif // WAVPACKMETADATAMODEL_H
