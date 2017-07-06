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

    inline void setMode(MusicObject::MusicSongCheckToolsMode mode) { m_operateMode = mode; }
    /*!
     * Set music song check tool mode.
     */
    inline MusicObject::MusicSongCheckToolsMode getMode() const { return m_operateMode; }
    /*!
     * Get music song check tool mode.
     */
    inline void setItemLists(const MusicObject::MIntList &items) { m_itemIDs = items; }
    /*!
     * Set item lists.
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
    void finished(const MusicSongCheckToolsRenames &items);
    /*!
     * Rename check finished.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    virtual void run() override;
    /*!
     * Thread run now.
     */

protected:
    bool m_run;
    MusicSongs *m_songItems;
    MusicObject::MIntList m_itemIDs;
    MusicSongCheckToolsRenames m_datas;
    MusicObject::MusicSongCheckToolsMode m_operateMode;

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

    inline void setMode(MusicObject::MusicSongCheckToolsMode mode) { m_operateMode = mode; }
    /*!
     * Set music song check tool mode.
     */
    inline MusicObject::MusicSongCheckToolsMode getMode() const { return m_operateMode; }
    /*!
     * Get music song check tool mode.
     */
    inline void setItemLists(const MusicObject::MIntList &items) { m_itemIDs = items; }
    /*!
     * Set item lists.
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
    void finished(const MusicSongCheckToolsDuplicates &items);
    /*!
     * Duplicate check finished.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    virtual void run() override;
    /*!
     * Thread run now.
     */

protected:
    bool m_run;
    MusicSongs *m_songItems;
    MusicObject::MIntList m_itemIDs;
    MusicSongCheckToolsDuplicates m_datas;
    MusicObject::MusicSongCheckToolsMode m_operateMode;

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
    void finished(const MusicSongCheckToolsQualitys &items);
    /*!
     * Quality check finished.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    virtual void run() override;
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
