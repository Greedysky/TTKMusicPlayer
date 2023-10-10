#include "musicjspfconfigmanager.h"

#include "qjson/parser.h"
#include "qjson/serializer.h"

MusicJSPFConfigManager::MusicJSPFConfigManager()
    : MusicPlaylistRenderer()
    , MusicPlaylistInterface()
{

}

bool MusicJSPFConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(m_file.readAll(), &ok);
    if(!ok)
    {
        return false;
    }

    QVariantMap value = data.toMap();
    const QVariantList &datas = value["trackList"].toList();
    for(const QVariant &var : qAsConst(datas))
    {
        if(var.isNull())
        {
            continue;
        }

        value = var.toMap();
        value = value["track"].toMap();

        const QString &path = value["location"].toString();
        if(!path.isEmpty())
        {
            item.m_songs << MusicSong(path, value["duration"].toString());
        }
    }

    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicJSPFConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    QVariantMap datas;
    for(int i = 0; i < items.count(); ++i)
    {
        QVariantList tracks;
        const MusicSongItem &item = items[i];
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            QVariantMap meta;
            meta["location"] = song.path();
            meta["title"] = song.artistBack();
            meta["creator"] = song.artistFront();
            meta["duration"] = song.playTime();
            meta["album"] = QString();
            meta["trackNum"] = QString();
            meta["year"] = QString();

            QVariantMap track;
            track["track"] = meta;
            tracks.push_back(track);
        }

        if(!tracks.isEmpty())
        {
            datas["trackList"] = tracks;
        }
    }

    QJson::Serializer json;
    bool ok = false;
    const QByteArray &output = json.serialize(datas, &ok);
    if(!ok)
    {
        return false;
    }

    m_file.write(output);
    m_file.close();
    return true;
}
