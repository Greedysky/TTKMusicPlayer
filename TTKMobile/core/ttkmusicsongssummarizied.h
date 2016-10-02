#ifndef TTKMUSICSONGSSUMMARIZIED_H
#define TTKMUSICSONGSSUMMARIZIED_H

#include "musicsong.h"

class TTKMusicSongsSummarizied : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicSongsSummarizied(QObject *parent = 0);

    void addMusicLists(const MusicSongItems &names);
    /*!
     * Add music datas into container.
     */
    void appendMusicLists(const MusicSongItems &names);
    /*!
     * Append music datas into container.
     */
    MusicSongItem getMusicList(int index);
    /*!
     * Get music data from container.
     */
    inline const MusicSongItems& getMusicLists() const  { return m_songItems;}
    /*!
     * Get music datas from container.
     */
    void importOtherMusicSongs(const QStringList &filelist);
    /*!
     * Input orther imported music datas into container.
     */
    QStringList getMusicSongsFileName(int index) const;
    /*!
     * Get music songs file name by index.
     */
    QStringList getMusicSongsFilePath(int index) const;
    /*!
     * Get music songs file path by index.
     */

    inline void setToolBoxIndex(int index) { m_currentToolIndex = index; }
    inline int getToolBoxIndex() const { return m_currentToolIndex; }

    inline void setCurrentIndex(int index) { m_currentPlayIndex = index; }
    inline int getCurrentIndex() const { return m_currentPlayIndex; }

signals:
public slots:

protected:
    int m_currentPlayIndex, m_currentToolIndex;
    MusicSongItems m_songItems;

};

#endif // TTKMUSICSONGSSUMMARIZIED_H
