/***************************************************************************
 *   Copyright (C) 2006-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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
#ifndef MPLAYERENGINEFACTORY_H
#define MPLAYERENGINEFACTORY_H

#include <QObject>
#include <QString>
#include <QIODevice>
#include <QWidget>

#include <qmmp/abstractengine.h>
#include <qmmp/enginefactory.h>
#include <qmmp/fileinfo.h>
#include <qmmp/metadatamodel.h>

class MplayerEngineFactory : public QObject, EngineFactory
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.EngineFactoryInterface.1.0")
Q_INTERFACES(EngineFactory)

public:
    const EngineProperties properties() const;
    bool supports(const QString &source) const;
    AbstractEngine *create(QObject *parent = 0);
    QList<FileInfo *> createPlayList(const QString &fileName, bool useMetaData, QStringList *);
    MetaDataModel* createMetaDataModel(const QString &path, QObject *parent = 0);
    void showSettings(QWidget *parent);

};

#endif
