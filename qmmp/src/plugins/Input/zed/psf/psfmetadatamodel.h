/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#ifndef PSFMETADATAMODEL_H
#define PSFMETADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include "psfhelper.h"

class PSFMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(PSFMetaDataModel)
public:
    explicit PSFMetaDataModel(const QString &path);
    virtual ~PSFMetaDataModel();

    virtual QList<TagModel* > tags() const override;
    virtual QList<MetaDataItem> extraProperties() const override;

private:
    QList<TagModel* > m_tags;
    PSFHelper* m_helper;

};

class PSFFileTagModel : public TagModel
{
public:
    explicit PSFFileTagModel(PSFHelper* psf);
    virtual ~PSFFileTagModel();

    virtual QString name() const override;
    virtual QList<Qmmp::MetaData> keys() const override;
    virtual QString value(Qmmp::MetaData key) const override;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override;

private:
    PSFHelper* m_helper = nullptr;

};

#endif
