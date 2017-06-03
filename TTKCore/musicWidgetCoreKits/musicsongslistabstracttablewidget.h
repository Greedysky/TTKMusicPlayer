#ifndef MUSICSONGSLISTABSTRACTTABLEWIDGET_H
#define MUSICSONGSLISTABSTRACTTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include "musicsong.h"
#include "musicslowmovingtablewidget.h"

/*! @brief The class of the songs list abstract table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListAbstractTableWidget : public MusicSlowMovingTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListAbstractTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongsListAbstractTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void setSongsFileName(MusicSongs *songs);
    /*!
     * Set songs file names.
     */
    virtual void updateSongsFileName(const MusicSongs &songs);
    /*!
     * Update songs file names in table.
     */
    virtual void selectRow(int index);
    /*!
     * Select the current play row by given index.
     */

    void setParentToolIndex(int index);
    /*!
     * Set parent tool index.
     */
    inline void setPlayRowIndex(int index) { m_playRowIndex = index; }
    /*!
     * Get the current play row.
     */
    inline int getPlayRowIndex() const { return m_playRowIndex; }
    /*!
     * Get the current play row.
     */

Q_SIGNALS:
    void isCurrentIndexs(bool &state);
    /*!
     * Check is current play stack widget.
     */

public Q_SLOTS:
    void musicPlayClicked();
    /*!
     * Music item has double been clicked.
     */
    virtual void setDeleteItemAt();
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemAll();
    /*!
     * Delete all items from list.
     */
    void musicOpenFileDir();
    /*!
     * Open the music at local path.
     */
    void musicFileInformation();
    /*!
     * Open music file information widget.
     */
    void musicSongMovieFound();
    /*!
     * To search song mv by song name.
     */
    void musicAlbumFoundWidget();
    /*!
     * Open music album found widget.
     */
    void musicSimilarFoundWidget();
    /*!
     * Open music similar found widget.
     */
    void musicSongSharedWidget();
    /*!
     * Open music song shared widget.
     */
    void musicSongKMicroWidget();
    /*!
     * Open music song KMicro widget.
     */
    void musicSongTransferWidget();
    /*!
     * Open music song transfer widget.
     */
    void musicSongDownload();
    /*!
     * Open music song download widget.
     */

protected:
    void createMoreMenu(QMenu *menu);
    /*!
     * Create more menu information.
     */
    QString getCurrentSongPath() const;
    /*!
     * Get current song path.
     */
    QString getSongPath(int index) const;
    /*!
     * Get song path.
     */
    QString getCurrentSongName() const;
    /*!
     * Get current song name.
     */
    QString getSongName(int index) const;
    /*!
     * Get song name.
     */

    int m_playRowIndex, m_parentToolIndex;
    MusicSongs *m_musicSongs;
    bool m_hasParentToolIndex;

};

#endif // MUSICSONGSLISTABSTRACTTABLEWIDGET_H
