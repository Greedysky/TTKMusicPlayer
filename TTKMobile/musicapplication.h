#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicmobileglobaldefine.h"

class QTimer;
class QQmlContext;
class TTKMusicUtils;
class TTKMusicPlaylist;
class TTKMusicPlayer;
class TTKNetworkHelper;
class TTKMusicLyricModel;
class TTKMusicSongsSummarizied;
class MusicDownloadStatusLabel;

/*! @brief The class of the music application.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT MusicApplication : public QObject
{
    Q_OBJECT
public:
    explicit MusicApplication(QQmlContext *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicApplication();

    Q_INVOKABLE void importOtherMusicSongs(const QStringList &filelist);
    /*!
     * Input orther imported music datas into container.
     */
    Q_INVOKABLE void importLovestMusicSongs();
    /*!
     * Input lovest imported music datas into container.
     */
    Q_INVOKABLE void importDownloadMusicSongs(int type, const QString &path);
    /*!
     * Input network download imported music datas into container.
     */
    Q_INVOKABLE void importNetworkMusicSongs(const QString &key, const QString &path);
    /*!
     * Input network cached imported music datas into container.
     */
    Q_INVOKABLE void removeMusicSongs();
    /*!
     * Remove music data from container.
     */
    Q_INVOKABLE void removeMusicSongs(int index);
    /*!
     * Remove music data from container by index.
     */

    Q_INVOKABLE bool checkLovestMusicSong() const;
    /*!
     * Check current song is in the lovest container or not.
     */

    Q_INVOKABLE bool empty() const;
    /*!
     * Check current playlist is empty or not.
     */
    Q_INVOKABLE int mediaCount(int index) const;
    /*!
     * Get current medias count.
     */

    Q_INVOKABLE QString mediaName() const;
    /*!
     * Get play music media name.
     */
    Q_INVOKABLE QString mediaArtist() const;
    /*!
     * Get play music media artist.
     */
    Q_INVOKABLE QStringList mediaNames(int index) const;
    /*!
     * Get play music media names.
     */
    Q_INVOKABLE QStringList mediaArtists(int index) const;
    /*!
     * Get play music media artists.
     */
    Q_INVOKABLE QStringList mediaPaths(int index) const;
    /*!
     * Get play music media paths.
     */
    Q_INVOKABLE QString mediaPath(int tool, int index) const;
    /*!
     * Get play music media path.
     */
    Q_INVOKABLE int mediaPlayCount(int index) const;
    /*!
     * Get play music counts.
     */
    Q_INVOKABLE QString artistImagePath() const;
    /*!
     * Get play music artist image's path.
     */
    Q_INVOKABLE QString artistBgImagePath() const;
    /*!
     * Get play music artist image's path.
     */

    Q_INVOKABLE int playbackMode() const;
    /*!
     * Get current play mode.
     */
    Q_INVOKABLE void setPlaybackMode(int mode);
    /*!
     * Set current play mode.
     */

    Q_INVOKABLE void playNext();
    /*!
     * Set current player to next music.
     */
    Q_INVOKABLE void playPrevious();
    /*!
     * Set current player to previous music.
     */

    Q_INVOKABLE void setToolBoxIndex(int index);
    /*!
     * Set current tool box index.
     */
    Q_INVOKABLE void setCurrentIndex(int id, int index);
    /*!
     * Set current play index.
     */
    Q_INVOKABLE int getCurrentIndex();
    /*!
     * Get current play index.
     */

    Q_INVOKABLE bool checkMusicListCurrentIndex() const;
    /*!
     * Check current list index is -1 or not.
     */
    Q_INVOKABLE QString getCurrentFileName() const;
    /*!
     * Get current file name.
     */
    Q_INVOKABLE void musicLoadCurrentSongLrc();
    /*!
     * Load current song lrc.
     */

    ///////////////////////////////////////////////////////////////////////////////////
    Q_INVOKABLE bool timeToQuitAppIsSet() const;
    /*!
     * Check time to quit app is be set or not.
     */
    Q_INVOKABLE void setTimeToQuitApp(int time) const;
    /*!
     * Set time to quit app.
     */


Q_SIGNALS:
    void updateCurrentArtist();
    /*!
     * Download current artist image finished emit.
     */
    void updateCurrentBgArtist();
    /*!
     * Download current bg artist image finished emit.
     */
    void importSongFinished(int index);
    /*!
     * Imported music datas into container finished emit.
     */
    void currentIndexChanged(int index);
    /*!
     * Current play index changed emit.
     */
    void updateItemShowCount();
    /*!
     * Update item show count.
     */
    void removeItemFromPlayerCenter(int index);
    /*!
     * Remove item from player center.
     */
    void emptyPlayerCenter(bool state);
    /*!
     * Empty the current player center.
     */

public Q_SLOTS:
    void currentMusicSongChanged(int index);
    /*!
     * Current play index changed.
     */

protected:
    void removeMusicSongs(int tool, int current, int index);
    /*!
     * Remove music data from container by index.
     */
    void readXMLConfigFromText();
    /*!
     * Read XML config from text.
     */
    void writeXMLConfigToText();
    /*!
     * Write XML config to text.
     */

    QTimer *m_timeToQuitTimer;

    TTKMusicUtils *m_ttkUtils;
    TTKMusicPlaylist *m_ttkPlaylist;
    TTKMusicPlayer *m_ttkPlayer;
    TTKNetworkHelper *m_networkHelper;
    TTKMusicLyricModel *m_ttkLrcModel;
    TTKMusicSongsSummarizied *m_songsSummarizied;
    MusicDownloadStatusLabel *m_downloadStatus;

};

#endif // MUSICAPPLICATION_H
