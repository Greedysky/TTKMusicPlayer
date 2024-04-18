/***************************************************************************
 *   Copyright (C) 2008-2024 by Ilya Kotov                                 *
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

#ifndef XMPMETADATAMODEL_H
#define XMPMETADATAMODEL_H

#include <qmmp/metadatamodel.h>
#include <xmp.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class XMPMetaDataModel : public MetaDataModel
{
    Q_DECLARE_TR_FUNCTIONS(XMPMetaDataModel)
public:
    explicit XMPMetaDataModel(const QString &path);
    virtual ~XMPMetaDataModel();

    virtual QList<MetaDataItem> extraProperties() const override final;
    virtual QList<MetaDataItem> descriptions() const override final;

private:
    xmp_context m_ctx;

};

#endif
