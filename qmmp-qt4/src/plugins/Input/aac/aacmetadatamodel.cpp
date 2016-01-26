/***************************************************************************
 *   Copyright (C) 2009 by Ilya Kotov                                      *
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

#include <QFile>
#include "aacfile.h"
#include "aacmetadatamodel.h"

AACMetaDataModel::AACMetaDataModel(const QString &path, QObject *parent) : MetaDataModel(parent)
{
    m_path = path;
}

AACMetaDataModel::~AACMetaDataModel()
{}

QHash<QString, QString> AACMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    QFile input(m_path);
    if (!input.open(QIODevice::ReadOnly))
        return ap;
    AACFile f(&input);
    QString text;
    text = QString("%1").arg(f.length()/60);
    text +=":"+QString("%1").arg(f.length()%60,2,10,QChar('0'));
    ap.insert(tr("Length"), text);
    text = QString("%1").arg(f.samplerate());
    ap.insert(tr("Sample rate"), text+" "+tr("Hz"));
    text = QString("%1").arg(f.bitrate());
    ap.insert(tr("Bitrate"), text+" "+tr("kbps"));
    text = QString("%1 "+tr("KB")).arg(input.size()/1024);
    ap.insert(tr("File size"), text);
    return ap;
}
