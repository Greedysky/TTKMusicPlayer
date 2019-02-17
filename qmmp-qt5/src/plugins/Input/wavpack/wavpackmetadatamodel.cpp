/***************************************************************************
 *   Copyright (C) 2009-2019 by Ilya Kotov                                 *
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

#include <QMap>
#include <qmmp/metadatamanager.h>
#include "wavpackmetadatamodel.h"

WavPackMetaDataModel::WavPackMetaDataModel(const QString &path, bool readOnly)
    : MetaDataModel(readOnly)
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

    char err[80] = {0};
    int flags = OPEN_WVC | OPEN_TAGS;
    if(!readOnly)
        flags |= OPEN_EDIT_TAGS;
#if defined(Q_OS_WIN) && defined(OPEN_FILE_UTF8)
    m_ctx = WavpackOpenFileInput (m_path.toUtf8().constData(),
                                  err, flags | OPEN_FILE_UTF8, 0);
#else
    m_ctx = WavpackOpenFileInput (m_path.toLocal8Bit().constData(), err,
                                  flags, 0);
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

QList<MetaDataItem> WavPackMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    if(!m_ctx)
        return ep;

    ep << MetaDataItem(tr("Ratio"), WavpackGetRatio(m_ctx));
    ep << MetaDataItem(tr("Version"), WavpackGetVersion(m_ctx));
    return ep;
}

QList<TagModel* > WavPackMetaDataModel::tags() const
{
    return m_tags;
}

QString WavPackMetaDataModel::coverPath() const
{
    return MetaDataManager::instance()->findCoverFile(m_path);
}

WavPackFileTagModel::WavPackFileTagModel(WavpackContext *ctx) : TagModel(TagModel::Save)
{
    m_ctx  = ctx;
}

WavPackFileTagModel::~WavPackFileTagModel()
{}

QString WavPackFileTagModel::name() const
{
    return "ID3v1/APEv2";
}

QString WavPackFileTagModel::value(Qmmp::MetaData key) const
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
