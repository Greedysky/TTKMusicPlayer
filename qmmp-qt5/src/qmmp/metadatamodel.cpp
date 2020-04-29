/***************************************************************************
 *   Copyright (C) 2009-2020 by Ilya Kotov                                 *
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

#include "metadatamodel.h"

MetaDataItem::MetaDataItem(const QString &name, const QVariant &value, const QString &suffix) :
    m_name(name), m_suffix(suffix), m_value(value)
{}

const QString &MetaDataItem::name() const
{
    return m_name;
}

void MetaDataItem::setName(const QString &name)
{
    m_name = name;
}

const QVariant &MetaDataItem::value() const
{
    return m_value;
}

void MetaDataItem::setValue(const QString &value)
{
    m_value = value;
}

const QString &MetaDataItem::suffix() const
{
    return m_suffix;
}

void MetaDataItem::setSuffix(const QString &suffix)
{
    m_suffix = suffix;
}

MetaDataModel::MetaDataModel(bool readOnly, DialogHints hints) :
    m_readOnly(readOnly),
    m_dialogHints(hints)
{}

MetaDataModel::~MetaDataModel()
{}

QList<MetaDataItem> MetaDataModel::extraProperties() const
{
    return QList<MetaDataItem>();
}

QList<MetaDataItem> MetaDataModel::descriptions() const
{
    return QList<MetaDataItem>();
}

QList<TagModel* > MetaDataModel::tags() const
{
    return QList<TagModel* > ();
}

QPixmap MetaDataModel::cover() const
{
    return QPixmap();
}

void MetaDataModel::setCover(const QPixmap &pix)
{
    Q_UNUSED(pix);
}

void MetaDataModel::removeCover()
{}

QString MetaDataModel::coverPath() const
{
    return QString();
}

bool MetaDataModel::isReadOnly() const
{
    return m_readOnly;
}

const MetaDataModel::DialogHints &MetaDataModel::dialogHints() const
{
    return m_dialogHints;
}

void MetaDataModel::setDialogHints(const DialogHints &hints)
{
    m_dialogHints = hints;
}

void MetaDataModel::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
}
