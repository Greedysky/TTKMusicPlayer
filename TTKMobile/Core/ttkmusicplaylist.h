#ifndef TTKMUSICPLAYLIST_H
#define TTKMUSICPLAYLIST_H

#include "musicsong.h"
#include "musicmobileglobaldefine.h"

/*! @brief The class of the music playlist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicPlaylist : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicPlaylist(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    Q_INVOKABLE int playbackMode() const;
    /*!
     * Get current play mode.
     */
    Q_INVOKABLE void setPlaybackMode(int mode);
    /*!
     * Set current play mode.
     */

    Q_INVOKABLE void addMedia(const QString &content);
    /*!
     * Add music media, not append remember.
     */
    Q_INVOKABLE void addMedia(const QStringList &items);
    /*!
     * Add music media list, not append remember.
     */
    Q_INVOKABLE QString media(int index) const;
    /*!
     * Get play music media.
     */
    Q_INVOKABLE QString mediaName(int index) const;
    /*!
     * Get play music media name.
     */
    Q_INVOKABLE QString mediaArtist(int index) const;
    /*!
     * Get play music media artist.
     */

    Q_INVOKABLE int currentIndex() const;
    /*!
     * Get current play index.
     */
    Q_INVOKABLE int mediaCount() const;
    /*!
     * Get current medias count.
     */
    Q_INVOKABLE bool isEmpty() const;
    /*!
     * Check current medias is empty.
     */
    Q_INVOKABLE bool clear();
    /*!
     * Clear current medias.
     */

    Q_INVOKABLE void playNext();
    /*!
     * Set current player to next music.
     */
    Q_INVOKABLE void playPrevious();
    /*!
     * Set current player to previous music.
     */

Q_SIGNALS:
    void currentIndexChanged(int index);
    /*!
     * Current play index changed emit.
     */

    void updateMedia(const QStringList &titles, const QStringList &artists);

public Q_SLOTS:
    void setCurrentIndex(int index = -2);
    /*!
     * Set current play index.
     */
    void appendMedia(const QString &content);
    /*!
     * Append music media.
     */
    void appendMedia(const QStringList &items);
    /*!
     * Append music medias.
     */

protected:
    int m_currentIndex;
    MusicSongs m_mediaList;
    int m_playbackMode;

};

#endif // TTKMUSICPLAYLIST_H
