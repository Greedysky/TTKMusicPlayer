/***************************************************************************
 *   Copyright (C) 2009 by Ilya Kotov                                      *
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

#ifndef MPLAYERMETADATAMODEL_H
#define MPLAYERMETADATAMODEL_H

#include <qmmp/metadatamodel.h>

class MplayerMetaDataModel : public MetaDataModel
{
Q_OBJECT
public:
    MplayerMetaDataModel(const QString &path, QObject *parent);
    ~MplayerMetaDataModel();
    QHash<QString, QString> audioProperties();

private:
    QString m_path;
};

#endif // MPLAYERMETADATAMODEL_H
