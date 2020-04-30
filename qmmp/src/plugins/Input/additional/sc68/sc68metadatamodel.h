/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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
 ================================================= */

#ifndef SC68METADATAMODEL_H
#define SC68METADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include "sc68helper.h"

class SC68MetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(SC68MetaDataModel)
public:
    SC68MetaDataModel(const QString &path);
    virtual ~SC68MetaDataModel();

    virtual QList<TagModel* > tags() const override;
    virtual QList<MetaDataItem> extraProperties() const override;

private:
    QList<TagModel* > m_tags;
    SC68Helper* m_sc68;
};

class SC68FileTagModel : public TagModel
{
public:
    SC68FileTagModel(SC68Helper* sc68);
    virtual ~SC68FileTagModel();

    virtual QString name() const override;
    virtual QList<Qmmp::MetaData> keys() const override;
    virtual QString value(Qmmp::MetaData key) const override;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override;

private:
    SC68Helper* m_sc68;
};

#endif // SC68METADATAMODEL_H
