#include "musicsonghelper.h"
#include "musicresultinfo.h"
#include "musicsongmeta.h"
#include "musicformats.h"

#include <qmmp/trackinfo.h>

bool TTK::playlistRowValid(int index) noexcept
{
    return index != MUSIC_LOVEST_LIST && index != MUSIC_NETWORK_LIST && index != MUSIC_RECENT_LIST;
}

QString TTK::trackRelatedPath(const QString &path)
{
    return MusicFormats::isTrack(path) ? TrackInfo::pathFromUrl(path) : path;
}

QString TTK::generateSongName(const QString &title, const QString &artist) noexcept
{
    return (title.isEmpty() || artist.isEmpty()) ? artist + title : artist + " - " + title;
}

QString TTK::generateSongTitle(const QString &name, const QString &key)
{
    const QStringList &s = TTK::String::split(name);
    if(s.count() >= 2)
    {
        const int index = name.indexOf(key) + 1;
        return name.right(name.length() - index).trimmed();
    }
    return name;
}

QString TTK::generateSongArtist(const QString &name, const QString &key)
{
    const QStringList &s = TTK::String::split(name);
    if(s.count() >= 2)
    {
        const int index = name.indexOf(key);
        return name.left(index).trimmed();
    }
    return name;
}

QString TTK::generateNetworkSongTime(const QString &path)
{
    const QString &v = TTK::generateNetworkSongMetaPath(path);
    if(!QFile::exists(v))
    {
        MusicSongMeta meta;
        return meta.read(TTK::generateNetworkSongPath(path)) ? meta.duration() : TTK_DEFAULT_STR;
    }

    QFile file(v);
    if(file.open(QIODevice::ReadOnly))
    {
        const QString &data = file.readAll();
        file.close();

        const QStringList &datas = data.split(",");
        if(datas.count() >= 6)
        {
            // id, name, duration, format, size, url
            return datas[2];
        }
    }
    return TTK_DEFAULT_STR;
}

QString TTK::generateNetworkSongPath(const QString &path)
{
    QString v = path;
    if(TTK::String::isNetworkUrl(path))
    {
        /*Replace network url path to local path*/
        const QString &id = path.section("#", -1);
        if(id != path)
        {
            const QString &cachePath = CACHE_DIR_FULL + id;
            if(QFile::exists(cachePath))
            {
                v = cachePath;
            }
        }
    }
    return v;
}

QString TTK::generateNetworkSongMetaPath(const QString &path)
{
    QString v = path;
    if(TTK::String::isNetworkUrl(path))
    {
        /*Replace network url path to local path*/
        const QString &id = path.section("#", -1);
        if(id != path)
        {
            const QString &cachePath = CACHE_DIR_FULL + TTK::String::pefix(id) + TKB_FILE;
            if(QFile::exists(cachePath))
            {
                v = cachePath;
            }
        }
    }
    return v;
}

void TTK::generateNetworkSongMeta(const QString &id, const MusicResultDataItem &item)
{
    QFile file(QString("%1%2%3").arg(CACHE_DIR_FULL, id, TKB_FILE));
    if(file.open(QIODevice::WriteOnly))
    {
        QStringList datas;
        // id, name, duration, format, size, url
        datas << item.m_id << item.m_name << item.m_time << item.m_description << item.m_count << item.m_nickName;

        file.write(datas.join(",").toUtf8());
        file.close();
    }
}
