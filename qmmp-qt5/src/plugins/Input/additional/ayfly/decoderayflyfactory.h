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

#ifndef DECODERAYFLYFACTORY_H
#define DECODERAYFLYFACTORY_H

#include <qmmp/decoderfactory.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class DecoderAyflyFactory : public QObject, DecoderFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.DecoderFactoryInterface.1.0")
    Q_INTERFACES(DecoderFactory)
public:
    virtual bool canDecode(QIODevice *input) const override;
    virtual DecoderProperties properties() const override;
    virtual Decoder *create(const QString &path, QIODevice *input) override;
    virtual QList<TrackInfo*> createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredFiles) override;
    virtual MetaDataModel* createMetaDataModel(const QString &path, bool readOnly) override;

};

#endif
