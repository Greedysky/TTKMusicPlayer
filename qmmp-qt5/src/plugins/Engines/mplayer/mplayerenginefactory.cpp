/***************************************************************************
 *   Copyright (C) 2008-2015 by Ilya Kotov                                 *
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

#include <QMessageBox>
#include <QTranslator>
#include "mplayermetadatamodel.h"
#include "settingsdialog.h"
#include "mplayerengine.h"
#include "mplayerenginefactory.h"


// MplayerEngineFactory

const EngineProperties MplayerEngineFactory::properties() const
{
    EngineProperties properties;
    properties.name = tr("Mplayer Plugin");
    properties.shortName = "mplayer";
    properties.filters = MplayerInfo::filters();
    properties.description = tr("Video Files");
    //properties.contentType = "application/ogg;audio/x-vorbis+ogg";
    properties.protocols << "file";
    properties.hasAbout = true;
    properties.hasSettings = true;
    return properties;
}

bool MplayerEngineFactory::supports(const QString &source) const
{
    QStringList filters = MplayerInfo::filters();
    foreach(QString filter, filters)
    {
        QRegExp regexp(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
        if (regexp.exactMatch(source))
            return true;
    }
    return false;
}

AbstractEngine *MplayerEngineFactory::create(QObject *parent)
{
    return new MplayerEngine(parent);
}

QList<FileInfo *> MplayerEngineFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *)
{
    Q_UNUSED(useMetaData);
    QList<FileInfo *> info;
    info << MplayerInfo::createFileInfo(fileName);
    return info;
}

MetaDataModel* MplayerEngineFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    return new MplayerMetaDataModel(path, parent);
}

void MplayerEngineFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}
