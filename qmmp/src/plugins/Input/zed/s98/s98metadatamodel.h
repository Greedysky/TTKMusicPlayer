/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef S98METADATAMODEL_H
#define S98METADATAMODEL_H

#include <qmmp/metadatamodel.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class S98MetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(S98MetaDataModel)
public:
    explicit S98MetaDataModel(const QString &path);

    virtual QList<MetaDataItem> extraProperties() const override final;

private:
    QString m_path;

};

#endif
