/***************************************************************************
 *   Copyright (C) 2013-2021 by Ilya Kotov                                 *
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

#ifndef OPENMPTMETADATAMODEL_H
#define OPENMPTMETADATAMODEL_H

#include <qmmp/metadatamodel.h>

class OpenMPTHelper;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OpenMPTMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(OpenMPTMetaDataModel)
public:
    OpenMPTMetaDataModel(const QString &path);

    virtual QList<MetaDataItem> extraProperties() const override;
    virtual QList<MetaDataItem> descriptions() const override;

private:
    void fillInExtraProperties(OpenMPTHelper *helper);
    void fillInDescriptions(OpenMPTHelper *helper);

    QList<MetaDataItem> m_ap;
    QList<MetaDataItem> m_desc;

};

#endif
