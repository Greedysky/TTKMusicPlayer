#include "musicjspfconfigmanager.h"

#include "qjson/json.h"

MusicJSPFConfigManager::MusicJSPFConfigManager()
    : MusicPlaylistRenderer()
    , MusicPlaylistInterface()
{

}

bool MusicJSPFConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(m_file.readAll(), &ok);
    if(QJsonParseError::NoError != ok.error)
    {
        return false;
    }

    QVariantMap value = json.toVariant().toMap();
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
            meta["title"] = song.title();
            meta["creator"] = song.artist();
            meta["duration"] = song.duration();
            meta["album"] = QString();
            meta["trackNum"] = QString();
            meta["year"] = QString();

            QVariantMap track;
            track["track"] = meta;
            tracks.append(track);
        }

        if(!tracks.isEmpty())
        {
            datas["trackList"] = tracks;
        }
    }

    const QJsonDocument &json = QJsonDocument::fromVariant(datas);
#if TTK_QT_VERSION_CHECK(5,0,0) && !TTK_QT_VERSION_CHECK(5,1,0)
    m_file.write(json.toJson());
#else
    m_file.write(json.toJson(QJsonDocument::Indented));
#endif
    m_file.close();
    return true;
}
