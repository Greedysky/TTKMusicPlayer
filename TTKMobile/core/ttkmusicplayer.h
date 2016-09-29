#ifndef TTKMUSICPLAYER_H
#define TTKMUSICPLAYER_H

#include <QObject>
#include "musicmobileglobaldefine.h"

class QMediaPlayer;

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

    Q_INVOKABLE int volume() const;
    /*!
     * Get current player volume.
     */
    Q_INVOKABLE void setVolume(int volume);
    /*!
     * Set current player volume.
     */

    Q_INVOKABLE void setMedia(const QString &url);

Q_SIGNALS:
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

private:
    State m_state;
    QMediaPlayer *m_player;

};

#endif // TTKMUSICPLAYER_H
