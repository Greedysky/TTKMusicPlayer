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
    /*!
     * Object contsructor.
     */
    explicit MusicSongsListPlayedTableWidget(QWidget *parent = 0);

    virtual ~MusicSongsListPlayedTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Update songs file names in table.
     */
    virtual void updateSongsFileName(const MusicSongs &songs) override;

    /*!
     * Select the current play row by given index.
     */
    virtual void selectRow(int index) override;
    /*!
     * Select the current played row.
     */
    void selectPlayedRow();

Q_SIGNALS:
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemAt(int index);

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Delete item from list at current row.
     */
    virtual void setDeleteItemAt() override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

};

#endif // MUSICSONGSLISTPLAYEDTABLEWIDGET_H
