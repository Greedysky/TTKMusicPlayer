#ifndef MUSICSONGSLISTPLAYEDTABLEWIDGET_H
#define MUSICSONGSLISTPLAYEDTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsongslistabstracttablewidget.h"

/*! @brief The class of the songs played queue list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListPlayedTableWidget : public MusicSongsListAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListPlayedTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongsListPlayedTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void updateSongsFileName(const MusicSongs &songs) override;
    /*!
     * Update songs file names in table.
     */

    virtual void selectRow(int index) override;
    /*!
     * Select the current play row by given index.
     */
    void selectPlayedRow();
    /*!
     * Select the current played row.
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
    virtual void setDeleteItemAt() override;
    /*!
     * Delete item from list at current row.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

};

#endif // MUSICSONGSLISTPLAYEDTABLEWIDGET_H
