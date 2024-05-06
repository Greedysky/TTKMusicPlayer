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

#ifndef DECODERXMPFACTORY_H
#define DECODERXMPFACTORY_H

#include <qmmp/decoderfactory.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderXMPFactory : public QObject, DecoderFactory
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
    virtual QDialog *createSettings(QWidget *parent) override final;

};

#endif
