/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#ifndef HIVELYMETADATAMODEL_H
#define HIVELYMETADATAMODEL_H

#include <qmmp/metadatamodel.h>

class HivelyHelper;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class HivelyMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(HivelyMetaDataModel)
public:
    explicit HivelyMetaDataModel(const QString &path);

    virtual QList<MetaDataItem> extraProperties() const override final;
    virtual QList<MetaDataItem> descriptions() const override final;

private:
    void fillProperties(HivelyHelper *helper);

    QList<MetaDataItem> m_ep;
    QList<MetaDataItem> m_desc;

};

#endif
