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

#ifndef DECODERARCHIVEFACTORY_H
#define DECODERARCHIVEFACTORY_H

#include <qmmp/decoderfactory.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class DecoderArchiveFactory : public QObject, DecoderFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.DecoderFactoryInterface.1.0")
    Q_INTERFACES(DecoderFactory)
public:
    virtual bool canDecode(QIODevice *input) const override final;
    virtual DecoderProperties properties() const override final;
    virtual Decoder *create(const QString &path, QIODevice *input) override final;
    virtual QList<TrackInfo*> createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths) override final;
    virtual MetaDataModel* createMetaDataModel(const QString &path, bool readOnly) override final;
    virtual void showSettings(QWidget *parent) override final;

};

#endif
