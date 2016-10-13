#ifndef MUSICSONG_H
#define MUSICSONG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

#ifndef MUSIC_MOBILE
#define MUSIC_NORMAL_LIST   0
#define MUSIC_LOVEST_LIST   1
#define MUSIC_NETWORK_LIST  2
#else
#define MUSIC_NORMAL_LIST   0
#define MUSIC_DOWNLOAD_LIST 1
#define MUSIC_RECENT_LIST   2
#define MUSIC_LOVEST_LIST   3
#define MUSIC_DOWNMV_LIST   4
#define MUSIC_MUSICRG_LIST  5
#endif

/*! @brief The class of the music song info.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSong
{
public:
    MusicSong();
    /*!
     * Object contsructor.
     */
    explicit MusicSong(const QString &musicPath,
                       const QString &musicName = QString());
    /*!
     * Object contsructor by music path and name.
     */
    MusicSong(const QString &musicPath, int playCount,
              const QString &musicName = QString());
    /*!
     * Object contsructor by music path and name and playcout.
     */
    MusicSong(const QString &musicPath, const QString &type,
              int playCount, const QString &musicName = QString());
    /*!
     * Object contsructor by music path and name and playcout and format.
     */
    MusicSong(const QString &musicPath, const QString &type,
              const QString &time, int playCount,
              const QString &musicName = QString());
    /*!
     * Object contsructor by music path and name and playcout and format and time.
     */
    MusicSong(const QString &musicPath, int playCount, const QString &time,
              const QString &musicName);
    /*!
     * Object contsructor by music path and name and playcout and time.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    QString getMusicArtistFront() const;
    /*!
     * Get music artist name.
     */
    QString getMusicArtistBack() const;
    /*!
     * Get music name.
     */
    inline void setMusicName(const QString &n) { m_musicName = n;}
    /*!
     * Set music name.
     */
    inline QString getMusicName() const { return m_musicName;}
    /*!
     * Get music name.
     */
    inline void setMusicPath(const QString &p) { m_musicPath = p;}
    /*!
     * Set music path.
     */
    inline QString getMusicPath() const { return m_musicPath;}
    /*!
     * Get music path.
     */
    inline void setMusicType(const QString &t) { m_musicType = t;}
    /*!
     * Set music format.
     */
    inline QString getMusicType() const { return m_musicType;}
    /*!
     * Get music format.
     */
    inline void setMusicTime(const QString &t) { m_musicTime = t;}
    /*!
     * Set music time.
     */
    inline QString getMusicTime() const { return m_musicTime;}
    /*!
     * Get music time.
     */
    inline void setMusicSize(const qint64 s) { m_musicSize = s;}
    /*!
     * Set music size.
     */
    inline qint64 getMusicSize() const { return m_musicSize;}
    /*!
     * Get music size.
     */
    inline void setMusicPlayCount(const int c) { m_musicPlayCount = c;}
    /*!
     * Set music playcount.
     */
    inline int getMusicPlayCount() const { return m_musicPlayCount;}
    /*!
     * Get music playcount.
     */
    bool operator== (const MusicSong &other) const;
    /*!
     * Operator == function.
     */

protected:
    QString m_musicName, m_musicPath, m_musicType, m_musicTime;
    qint64  m_musicSize;
    int m_musicPlayCount;

};

TTK_DECLARE_LISTS(MusicSong)
class MusicSongsListWidget;

typedef struct MUSIC_CORE_EXPORT MusicSongItem
{
    int m_itemIndex;
    QString m_itemName;
    MusicSongs m_songs;
    MusicSongsListWidget* m_itemObject;

    MusicSongItem()
    {
        m_itemIndex = -1;
        m_itemObject = nullptr;
    }
}MusicSongItem;
TTK_DECLARE_LISTS(MusicSongItem)

#endif // MUSICSONG_H
