/***************************************************************************
 *   Copyright (C) 2006-2019 by Ilya Kotov                                 *
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

#include "optimfrogmetadatamodel.h"
#include "optimfroghelper.h"

OptimFROGMetaDataModel::OptimFROGMetaDataModel(const QString &path) : MetaDataModel(true)
{
    m_path = path;
}

QList<MetaDataItem> OptimFROGMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    QFile file(m_path);
    if(file.open(QIODevice::ReadOnly))
    {
        OptimFROGHelper helper(&file);
        if(!helper.initialize())
        {
            return ep;
        }

        ep << MetaDataItem(tr("Version"), QString::number(helper.version()));
        ep << MetaDataItem(tr("Compression ratio"), QString::number(helper.compression()));
    }

    return ep;
}
