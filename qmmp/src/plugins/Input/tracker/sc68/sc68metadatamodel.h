/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#ifndef SC68METADATAMODEL_H
#define SC68METADATAMODEL_H

#include <qmmp/metadatamodel.h>

class SC68Helper;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SC68MetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(SC68MetaDataModel)
public:
    explicit SC68MetaDataModel(const QString &path);

    virtual QList<MetaDataItem> extraProperties() const override final;

private:
    void fillProperties(SC68Helper *helper);

    QList<MetaDataItem> m_ep;

};

#endif
