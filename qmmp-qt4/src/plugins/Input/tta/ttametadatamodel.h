/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
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

#ifndef TTAMETADATAMODEL_H
#define TTAMETADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include "ttahelper.h"

class TTAMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(TTAMetaDataModel)
public:
    TTAMetaDataModel(const QString &path);
    virtual ~TTAMetaDataModel();

    virtual QList<TagModel* > tags() const override;

private:
    QList<TagModel* > m_tags;
    TTAHelper* m_tta;
};

class TTAFileTagModel : public TagModel
{
public:
    TTAFileTagModel(TTAHelper* tta, stdio_meta_type stdio_meta);
    virtual ~TTAFileTagModel();

    virtual QString name() const override;
    virtual QList<Qmmp::MetaData> keys() const override;
    virtual QString value(Qmmp::MetaData key) const override;
    virtual void setValue(Qmmp::MetaData key, const QString &value) override;

private:
    TTAHelper* m_tta;
    stdio_meta_type m_stdio_meta;
};

#endif // TTAMETADATAMODEL_H
