#ifndef MUSICSONGCHECKTOOLSCORE_H
#define MUSICSONGCHECKTOOLSCORE_H

#include <QThread>
#include "musicsong.h"
#include "musicsongchecktoolsunit.h"

/*! @brief The class of the song check tools rename thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsRenameCore : public QThread
{
    Q_OBJECT
public:
    explicit MusicSongCheckToolsRenameCore(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongCheckToolsRenameCore();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setRenameSongs(MusicSongs *songs);
    /*!
     * Set find file path by given path.
     */
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */

Q_SIGNALS:
    void finished(const SongCheckToolsRenames &items);
    /*!
     * Rename check finished.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    void run();
    /*!
     * Thread run now.
     */

protected:
    bool m_run;
    MusicSongs *m_songItems;

};


/*! @brief The class of the song check tools duplicate thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsDuplicateCore : public QThread
{
    Q_OBJECT
public:
    explicit MusicSongCheckToolsDuplicateCore(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongCheckToolsDuplicateCore();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setDuplicateSongs(MusicSongs *songs);
    /*!
     * Set find file path by given path.
     */
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */

Q_SIGNALS:
    void finished(const SongCheckToolsDuplicates &items);
    /*!
     * Duplicate check finished.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    void run();
    /*!
     * Thread run now.
     */

protected:
    bool m_run;
    MusicSongs *m_songItems;

};


/*! @brief The class of the song check tools quality thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsQualityCore : public QThread
{
    Q_OBJECT
public:
    explicit MusicSongCheckToolsQualityCore(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongCheckToolsQualityCore();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setQualitySongs(MusicSongs *songs);
    /*!
     * Set find file path by given path.
     */
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */

Q_SIGNALS:
    void finished(const SongCheckToolsQualitys &items);
    /*!
     * Quality check finished.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    void run();
    /*!
     * Thread run now.
     */

protected:
    int transfromBitrateToQuality(const QString &bitrate) const;
    /*!
     * Transfrom bitrate to quality.
     */

    bool m_run;
    MusicSongs *m_songItems;

};

#endif // MUSICSONGCHECKTOOLSCORE_H
