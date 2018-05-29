#ifndef MUSICSONGCHECKTOOLSUNIT_H
#define MUSICSONGCHECKTOOLSUNIT_H

#include "musicsong.h"
#include "musicobject.h"

namespace MusicObject
{
    enum MusicSongCheckToolsMode
    {
        Check,      /*!< Song Check Mode*/
        Apply       /*!< Song Apply Mode*/
    };
}

/*! @brief The class of the song check tools rename.
 * @author Greedysky <greedysky@163.com>
 */
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


/*! @brief The class of the song check tools duplicate.
 * @author Greedysky <greedysky@163.com>
 */
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


/*! @brief The class of the song check tools quality.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_TOOLSET_EXPORT MusicSongCheckToolsQuality
{
    MusicSong m_song;
    QString m_bitRate;

    MusicSongCheckToolsQuality(const MusicSong &song, const QString &bitRate)
    {
        m_song = song;
        m_bitRate = bitRate;
    }
}MusicSongCheckToolsQuality;
TTK_DECLARE_LISTS(MusicSongCheckToolsQuality)

#endif // MUSICSONGCHECKTOOLSUNIT_H
