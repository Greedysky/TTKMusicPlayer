#ifndef MUSICSONGCHECKTOOLSTHREAD_H
#define MUSICSONGCHECKTOOLSTHREAD_H

#include "musicsong.h"
#include "musicabstractthread.h"
#include "musicsongchecktoolsunit.h"

/*! @brief The class of the song check tools rename thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsRenameThread : public MusicAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsRenameThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsRenameThread(QObject *parent = nullptr);

    /*!
     * Set music song check tool mode.
     */
    inline void setMode(MusicObject::MusicSongCheckToolsMode mode) { m_operateMode = mode; }
    /*!
     * Get music song check tool mode.
     */
    inline MusicObject::MusicSongCheckToolsMode getMode() const { return m_operateMode; }
    /*!
     * Set item lists.
     */
    inline void setItemLists(const TTKIntList &items) { m_itemIDs = items; }

    /*!
     * Set find file path by given path.
     */
    void setRenameSongs(MusicSongs *songs);

Q_SIGNALS:
    /*!
     * Rename check finished.
     */
    void finished(const MusicSongCheckToolsRenames &items);

protected:
    /*!
     * Thread run now.
     */
    virtual void run() override;

protected:
    MusicSongs *m_songItems;
    TTKIntList m_itemIDs;
    MusicSongCheckToolsRenames m_datas;
    MusicObject::MusicSongCheckToolsMode m_operateMode;

};


/*! @brief The class of the song check tools duplicate thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsDuplicateThread : public MusicAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsDuplicateThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsDuplicateThread(QObject *parent = nullptr);

    /*!
     * Set music song check tool mode.
     */
    inline void setMode(MusicObject::MusicSongCheckToolsMode mode) { m_operateMode = mode; }
    /*!
     * Get music song check tool mode.
     */
    inline MusicObject::MusicSongCheckToolsMode getMode() const { return m_operateMode; }
    /*!
     * Set item lists.
     */
    inline void setItemLists(const TTKIntList &items) { m_itemIDs = items; }

    /*!
     * Set find file path by given path.
     */
    void setDuplicateSongs(MusicSongs *songs);

Q_SIGNALS:
    /*!
     * Duplicate check finished.
     */
    void finished(const MusicSongCheckToolsDuplicates &items);

protected:
    virtual void run() override;
    /*!
     * Thread run now.
     */

protected:
    bool m_run;
    MusicSongs *m_songItems;
    TTKIntList m_itemIDs;
    MusicSongCheckToolsDuplicates m_datas;
    MusicObject::MusicSongCheckToolsMode m_operateMode;

};


/*! @brief The class of the song check tools quality thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsQualityThread : public MusicAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsQualityThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsQualityThread(QObject *parent = nullptr);

    /*!
     * Set find file path by given path.
     */
    void setQualitySongs(MusicSongs *songs);

Q_SIGNALS:
    /*!
     * Quality check finished.
     */
    void finished(const MusicSongCheckToolsQualitys &items);

protected:
    /*!
     * Thread run now.
     */
    virtual void run() override;

protected:
    MusicSongs *m_songItems;

};

#endif // MUSICSONGCHECKTOOLSTHREAD_H
