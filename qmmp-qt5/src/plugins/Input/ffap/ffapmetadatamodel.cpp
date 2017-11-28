/***************************************************************************
 *   Copyright (C) 2011-2016 by Ilya Kotov                                 *
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

#include <QTextCodec>
#include <QRegExp>
#include <taglib/tag.h>
#include <taglib/id3v1tag.h>
#include <taglib/apetag.h>
#include <qmmp/metadatamanager.h>
#include "ffapmetadatamodel.h"

FFapMetaDataModel::FFapMetaDataModel(const QString &path, QObject *parent) : MetaDataModel(parent)
{
    if(path.contains("://"))
    {
        QString p = path;
        p.remove("ape://");
        p.remove(QRegExp("#\\d+$"));
        m_path = p;
    }
    else
        m_path = path;

    m_file = new TagLib::APE::File (QStringToFileName(m_path));
    if(!path.contains("://"))
    {
        m_tags << new FFapFileTagModel(m_file, TagLib::APE::File::ID3v1);
        m_tags << new FFapFileTagModel(m_file, TagLib::APE::File::APE);
    }
}

FFapMetaDataModel::~FFapMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
     delete m_file;
}

QHash<QString, QString> FFapMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    QString text = QString("%1").arg(m_file->audioProperties()->length()/60);
    text +=":"+QString("%1").arg(m_file->audioProperties()->length()%60,2,10,QChar('0'));
    ap.insert(tr("Length"), text);
    ap.insert(tr("Sample rate"), QString("%1 " + tr("Hz")).arg(m_file->audioProperties()->sampleRate()));
    ap.insert(tr("Channels"), QString("%1").arg(m_file->audioProperties()->channels()));
    ap.insert(tr("Bitrate"), QString("%1 " + tr("kbps")).arg(m_file->audioProperties()->bitrate()));
    ap.insert(tr("File size"), QString("%1 "+tr("KB")).arg(m_file->length()/1024));
    return ap;
}

QList<TagModel* > FFapMetaDataModel::tags()
{
    return m_tags;
}

QString FFapMetaDataModel::coverPath()
{
    return MetaDataManager::instance()->findCoverFile(m_path);
}

FFapFileTagModel::FFapFileTagModel(TagLib::APE::File *file, TagLib::APE::File::TagTypes tagType)
        : TagModel()
{
    m_tagType = tagType;
    m_file = file;
    if (m_tagType == TagLib::APE::File::ID3v1)
    {
        m_tag = m_file->ID3v1Tag();
        m_codec = QTextCodec::codecForName("ISO-8859-1");
    }
    else
    {
        m_tag = m_file->APETag();
        m_codec = QTextCodec::codecForName ("UTF-8");
    }
}

FFapFileTagModel::~FFapFileTagModel()
{}

const QString FFapFileTagModel::name()
{
    if (m_tagType == TagLib::APE::File::ID3v1)
        return "ID3v1";
    return "APE";
}

QList<Qmmp::MetaData> FFapFileTagModel::keys()
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::DISCNUMBER);
    return list;
}

const QString FFapFileTagModel::value(Qmmp::MetaData key)
{
    if (m_tag)
    {
        bool utf = m_codec->name().contains("UTF");
        TagLib::String str;
        switch((int) key)
        {
        case Qmmp::TITLE:
            str = m_tag->title();
            break;
        case Qmmp::ARTIST:
            str = m_tag->artist();
            break;
        case Qmmp::ALBUM:
            str = m_tag->album();
            break;
        case Qmmp::COMMENT:
            str = m_tag->comment();
            break;
        case Qmmp::GENRE:
            str = m_tag->genre();
            break;
        case Qmmp::YEAR:
            return QString::number(m_tag->year());
        case Qmmp::TRACK:
            return QString::number(m_tag->track());
        }
        return m_codec->toUnicode(str.toCString(utf)).trimmed();
    }
    return QString();
}

void FFapFileTagModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
        return;
    TagLib::String::Type type = TagLib::String::Latin1;

    if (m_tagType == TagLib::APE::File::ID3v1)
    {
        if(m_codec->name().contains("UTF")) //utf is unsupported
            return;
    }
    else if(m_tagType == TagLib::APE::File::APE)
        type = TagLib::String::UTF8;

    TagLib::String str = TagLib::String(m_codec->fromUnicode(value).constData(), type);

    switch((int) key)
    {
    case Qmmp::TITLE:
        m_tag->setTitle(str);
        break;
    case Qmmp::ARTIST:
        m_tag->setArtist(str);
        break;
    case Qmmp::ALBUM:
        m_tag->setAlbum(str);
        break;
    case Qmmp::COMMENT:
        m_tag->setComment(str);
        break;
    case Qmmp::GENRE:
        m_tag->setGenre(str);
        break;
    case Qmmp::YEAR:
        m_tag->setYear(value.toInt());
        break;
    case Qmmp::TRACK:
        m_tag->setTrack(value.toInt());
    }
}

bool FFapFileTagModel::exists()
{
    return (m_tag != 0);
}

void FFapFileTagModel::create()
{
    if (m_tag)
        return;
    if (m_tagType == TagLib::APE::File::ID3v1)
        m_tag = m_file->ID3v1Tag(true);
    else
        m_tag = m_file->APETag(true);
}

void FFapFileTagModel::remove()
{
    m_tag = 0;
}

void FFapFileTagModel::save()
{
    if(!m_tag)
        m_file->strip(m_tagType);
    m_file->save();
}
