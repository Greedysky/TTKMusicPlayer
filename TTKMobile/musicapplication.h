#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicmobileglobaldefine.h"

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
     * Get play music media name.
     */
    Q_INVOKABLE QStringList mediaArtists(int index) const;
    /*!
     * Get play music media artist.
     */
    Q_INVOKABLE QString artistImagePath() const;
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
    Q_INVOKABLE void setCurrentIndex(int index);
    /*!
     * Set current play index.
     */

    bool checkMusicListCurrentIndex() const;
    /*!
     * Check current list index is -1 or not.
     */
    QString getCurrentFileName() const;
    /*!
     * Get current file name.
     */
    Q_INVOKABLE void musicLoadCurrentSongLrc();
    /*!
     * Load current song lrc.
     */

signals:
    void updateCurrentArtist();
    /*!
     * Download current artist iamge finished emit.
     */
    void importSongFinished();
    /*!
     * Imported music datas into container finished emit.
     */
    void currentIndexChanged(int index);
    /*!
     * Current play index changed emit.
     */

public slots:
    void currentMusicSongChanged(int index);
    /*!
     * Current play index changed.
     */

protected:
    void readXMLConfigFromText();
    /*!
     * Read XML config from text.
     */
    void writeXMLConfigToText();
    /*!
     * Write XML config to text.
     */

    TTKMusicUtils *m_ttkUtils;
    TTKMusicPlaylist *m_ttkPlaylist;
    TTKMusicPlayer *m_ttkPlayer;
    TTKNetworkHelper *m_networkHelper;
    TTKMusicLyricModel *m_ttkLrcModel;
    TTKMusicSongsSummarizied *m_songsSummarizied;
    MusicDownloadStatusLabel *m_downloadStatus;

};

#endif // MUSICAPPLICATION_H
