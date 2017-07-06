#ifndef MUSICSONGCHECKTOOLSUNIT_H
#define MUSICSONGCHECKTOOLSUNIT_H

#include "musicsong.h"
#include "musicobject.h"

namespace MusicObject
{
    enum MusicSongCheckToolsMode
    {
        Check,
        Apply
    };
}

typedef struct MUSIC_TOOLSET_EXPORT MusicSongCheckToolsRename
{
    QString m_locaName;
    QString m_RecommendName;
    QString m_filePath;

    MusicSongCheckToolsRename(const QString &locaName, const QString &recommendName, const QString &path)
    {
        m_locaName = locaName;
        m_RecommendName = recommendName;
        m_filePath = path;
    }
}MusicSongCheckToolsRename;
TTK_DECLARE_LISTS(MusicSongCheckToolsRename)


typedef struct MUSIC_TOOLSET_EXPORT MusicSongCheckToolsDuplicate
{
    MusicSong m_song;
    QString m_bitRate;

    MusicSongCheckToolsDuplicate(const MusicSong &song, const QString &bitRate)
    {
        m_song = song;
        m_bitRate = bitRate;
    }
}MusicSongCheckToolsDuplicate;
TTK_DECLARE_LISTS(MusicSongCheckToolsDuplicate)


typedef struct MUSIC_TOOLSET_EXPORT MusicSongCheckToolsQuality
{
    MusicSong m_song;
    QString m_bitRate;
    int m_quality;

    MusicSongCheckToolsQuality(const MusicSong &song, const QString &bitRate, int quality)
    {
        m_song = song;
        m_bitRate = bitRate;
        m_quality = quality;
    }
}MusicSongCheckToolsQuality;
TTK_DECLARE_LISTS(MusicSongCheckToolsQuality)

#endif // MUSICSONGCHECKTOOLSUNIT_H
