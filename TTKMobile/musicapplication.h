#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

#include <QObject>

class QQmlContext;
class TTKMusicUtils;
class TTKMusicPlaylist;
class TTKMusicPlayer;
class TTKNetworkHelper;
class TTKMusicLyricModel;
class TTKMusicSongsSummarizied;
class MusicDownloadStatusLabel;

class MusicApplication : public QObject
{
    Q_OBJECT
public:
    explicit MusicApplication(QQmlContext *parent = 0);
    ~MusicApplication();

    Q_INVOKABLE void importOtherMusicSongs(const QStringList &filelist);
    /*!
     * Input orther imported music datas into container.
     */

    Q_INVOKABLE bool empty() const;

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
    QString getCurrentFileName() const;
    Q_INVOKABLE void musicLoadCurrentSongLrc();

signals:
    void updateCurrentArtist();
    void importSongFinished();
    void currentIndexChanged(int index);

public slots:
    void currentMusicSongChanged(int index);

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
