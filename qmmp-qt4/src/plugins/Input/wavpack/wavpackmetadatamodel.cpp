/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
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

#include <QMap>
#include <qmmp/metadatamanager.h>
#include "wavpackmetadatamodel.h"

WavPackMetaDataModel::WavPackMetaDataModel(const QString &path, QObject *parent) : MetaDataModel(parent)
{
    if(path.contains("://"))
    {
        QString p = path;
        p.remove("wvpack://");
        p.remove(QRegExp("#\\d+$"));
        m_path = p;
    }
    else
        m_path = path;

    char err[80];
#if defined(Q_OS_WIN) && defined(OPEN_FILE_UTF8)
    m_ctx = WavpackOpenFileInput (m_path.toUtf8().constData(),
                                  err, OPEN_WVC | OPEN_TAGS | OPEN_FILE_UTF8, 0);
#else
    m_ctx = WavpackOpenFileInput (m_path.toLocal8Bit().constData(), err,
                                  OPEN_WVC | OPEN_EDIT_TAGS, 0);
#endif
    if (!m_ctx)
    {
        qWarning("WavPackMetaDataModel: error: %s", err);
        return;
    }
    if(!path.contains("://"))
        m_tags << new WavPackFileTagModel(m_ctx);
}

WavPackMetaDataModel::~WavPackMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    if(m_ctx)
        WavpackCloseFile (m_ctx);
}

QHash<QString, QString> WavPackMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    if(!m_ctx)
        return ap;
    int length = (int) WavpackGetNumSamples(m_ctx)/WavpackGetSampleRate(m_ctx);
    QString text = QString("%1").arg(length/60);
    text +=":"+QString("%1").arg(length % 60, 2, 10, QChar('0'));
    ap.insert(tr("Length"), text);
    ap.insert(tr("Sample rate"), QString("%1 " + tr("Hz")).arg((int) WavpackGetSampleRate(m_ctx)));
    ap.insert(tr("Channels"), QString("%1").arg((int) WavpackGetNumChannels(m_ctx)));
    ap.insert(tr("Bitrate"), QString("%1 " + tr("kbps"))
           .arg((int) WavpackGetAverageBitrate(m_ctx, WavpackGetNumChannels(m_ctx))/1000));
    ap.insert(tr("File size"), QString("%1 "+tr("KB")).arg(WavpackGetFileSize(m_ctx)/1024));
    ap.insert(tr("Ratio"), QString("%1").arg(WavpackGetRatio(m_ctx)));
    ap.insert(tr("Version"), QString("%1").arg(WavpackGetVersion(m_ctx)));
    return ap;
}

QList<TagModel* > WavPackMetaDataModel::tags()
{
    return m_tags;
}

QPixmap WavPackMetaDataModel::cover()
{
    QString cPath = coverPath();
    return cPath.isEmpty() ? QPixmap() : QPixmap(cPath);
}

QString WavPackMetaDataModel::coverPath()
{
    return MetaDataManager::instance()->getCoverPath(m_path);
}

WavPackFileTagModel::WavPackFileTagModel(WavpackContext *ctx) : TagModel(TagModel::Save)
{
    m_ctx  = ctx;
}

WavPackFileTagModel::~WavPackFileTagModel()
{}

const QString WavPackFileTagModel::name()
{
    return "ID3v1/APEv2";
}

const QString WavPackFileTagModel::value(Qmmp::MetaData key)
{
    char value[200];
    memset(value,0,sizeof(value));
    switch((int) key)
    {
    case Qmmp::TITLE:
        WavpackGetTagItem (m_ctx, "Title", value, sizeof(value));
        break;
    case Qmmp::ARTIST:
        WavpackGetTagItem (m_ctx, "Artist", value, sizeof(value));
        break;
    case Qmmp::ALBUM:
        WavpackGetTagItem (m_ctx, "Album", value, sizeof(value));
        break;
    case Qmmp::ALBUMARTIST:
        WavpackGetTagItem (m_ctx, "Album Artist", value, sizeof(value));
        break;
    case Qmmp::COMMENT:
        WavpackGetTagItem (m_ctx, "Comment", value, sizeof(value));
        break;
    case Qmmp::YEAR:
        WavpackGetTagItem (m_ctx, "Year", value, sizeof(value));
        break;
    case Qmmp::TRACK:
        WavpackGetTagItem (m_ctx, "Track", value, sizeof(value));
        break;
    case Qmmp::DISCNUMBER:
        WavpackGetTagItem (m_ctx, "Disc", value, sizeof(value));
        break;
    case Qmmp::GENRE:
        WavpackGetTagItem (m_ctx, "Genre", value, sizeof(value));
        break;
    case Qmmp::COMPOSER:
        WavpackGetTagItem (m_ctx, "Composer", value, sizeof(value));
   }
   return QString::fromUtf8(value);
}

void WavPackFileTagModel::setValue(Qmmp::MetaData key, const QString &value)
{
    int size = value.toUtf8().size();
    char *v = value.toUtf8().data();
    switch((int) key)
    {
    case Qmmp::TITLE:
        WavpackAppendTagItem (m_ctx, "Title", v, size);
        break;
    case Qmmp::ARTIST:
        WavpackAppendTagItem (m_ctx, "Artist", v, size);
        break;
    case Qmmp::ALBUM:
        WavpackAppendTagItem (m_ctx, "Album", v, size);
        break;
    case Qmmp::ALBUMARTIST:
        WavpackAppendTagItem (m_ctx, "Album Artist", v, size);
        break;
    case Qmmp::COMMENT:
        WavpackAppendTagItem (m_ctx, "Comment", v, size);
        break;
    case Qmmp::YEAR:
        WavpackAppendTagItem (m_ctx, "Year", v, size);
        break;
    case Qmmp::TRACK:
        WavpackAppendTagItem (m_ctx, "Track", v, size);
        break;
    case Qmmp::DISCNUMBER:
        WavpackAppendTagItem (m_ctx, "Disc", v, size);
        break;
    case Qmmp::GENRE:
        WavpackAppendTagItem (m_ctx, "Genre", v, size);
        break;
    case Qmmp::COMPOSER:
        WavpackAppendTagItem (m_ctx, "Composer", v, size);
    }
}

void WavPackFileTagModel::save()
{
    WavpackWriteTag (m_ctx);
}
