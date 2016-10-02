#ifndef TTKMUSICPLAYER_H
#define TTKMUSICPLAYER_H

#include <QTimer>
#include "musicmobileglobaldefine.h"

class SoundCore;
class TTKMusicPlaylist;

/*! @brief The class of the music player.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicPlayer : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StoppedState = 0,   ///*stop state*/
        PlayingState,       ///*play state*/
        PausedState         ///*pause state*/
    };
    explicit TTKMusicPlayer(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKMusicPlayer();

    Q_INVOKABLE int state() const;
    /*!
     * Get current player state.
     */
    Q_INVOKABLE void setPlaylist(TTKMusicPlaylist *playlist);
    /*!
     * Set current play list.
     */
    Q_INVOKABLE TTKMusicPlaylist *playlist() const;
    /*!
     * Get current play list.
     */

    Q_INVOKABLE qint64 duration() const;
    /*!
     * Get current player duration.
     */
    Q_INVOKABLE qint64 position() const;
    /*!
     * Get current player postion.
     */
    Q_INVOKABLE void setPosition(qint64 position);
    /*!
     * Set current play pos.
     */

    static QStringList supportFormatsString();
    /*!
     * Get player supported formats.
     */
    static QStringList supportFormatsFilterString();
    /*!
     * Get player supported formats filter.
     */
    static QStringList supportFormatsFilterDialogString();
    /*!
     * Get player supported formats filter dialog.
     */

Q_SIGNALS:
    void stateChanged(int newState);
    /*!
     * Current state changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current duration changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current position changed.
     */

public Q_SLOTS:
    void play();
    /*!
     * Player to play.
     */
    void pause();
    /*!
     * Player to pause.
     */
    void stop();
    /*!
     * Player to stop.
     */

private Q_SLOTS:
    void update();
    /*!
     * Player one second time out.
     */
    void getCurrentDuration();
    /*!
     * Get current duration by time out.
     */

private:
    TTKMusicPlaylist *m_playlist;
    State m_state;
    SoundCore *m_music;
    QTimer m_timer;
    QString m_currentMedia;
    qint64 m_duration;

    int m_tryTimes;

};

#endif // TTKMUSICPLAYER_H
