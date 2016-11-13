#ifndef MUSICSONGSPLAYEDLISTWIDGET_H
#define MUSICSONGSPLAYEDLISTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musicslowmovingtablewidget.h"

/*! @brief The class of the songs list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsPlayedListWidget : public MusicSlowMovingTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongsPlayedListWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongsPlayedListWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setSongsFileName(MusicPlayedSongs *songs);
    /*!
     * Set songs file names.
     */
    void updateSongsFileName(const MusicPlayedSongs &songs);
    /*!
     * Update songs file names in table.
     */

    void selectRow(int index);
    /*!
     * Select the current play row by given index.
     */
    void selectPlayedRow();
    /*!
     * Select the current played row.
     */
    inline int getPlayRowIndex() const { return m_playRowIndex; }
    /*!
     * Get the current play row.
     */

Q_SIGNALS:
    void updateCountLabel();
    /*!
     * Update count label.
     */

public Q_SLOTS:
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    void musicPlayClicked();
    /*!
     * Music item has double been clicked.
     */
    void setDeleteItemAt();
    /*!
     * Delete item from list at current row.
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
    void musicSongTransferWidget();
    /*!
     * Open music song transfer widget.
     */
    void musicSongDownload();
    /*!
     * Open music song download widget.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createMoreMenu();
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

    int m_playRowIndex;
    MusicPlayedSongs *m_musicSongs;

};

#endif // MUSICSONGSPLAYEDLISTWIDGET_H
