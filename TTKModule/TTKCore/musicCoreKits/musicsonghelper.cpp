#include "musicsonghelper.h"
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
    MusicSongMeta meta;
    return meta.read(TTK::generateNetworkSongPath(path)) ? meta.duration() : TTK_DEFAULT_STR;
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
