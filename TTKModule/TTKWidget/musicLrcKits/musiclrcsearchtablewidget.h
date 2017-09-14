#ifndef MUSICLRCSEARCHTABLEWIDGET_H
#define MUSICLRCSEARCHTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicquerytablewidget.h"

/*! @brief The class of the lrc search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcSearchTableWidget : public MusicQueryItemTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcSearchTableWidget(QWidget *parent = 0);

    virtual ~MusicLrcSearchTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start search query by text.
     */
    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Data download to local file.
     */
    virtual void musicDownloadLocal(int row) override;

Q_SIGNALS:
    /*!
     * Lrc search state is finished.
     */
    void resolvedSuccess();
    /*!
     * Lrc download state is finished.
     */
    void lrcDownloadStateChanged(const QString &name);

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
     * Clear All Items.
     */
    virtual void clearAllItems() override;
    /*!
     * Create searched items.
     */
    virtual void createSearchedItems(const MusicSearchedItem &songItem) override;
    /*!
     * Item has double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

};

#endif // MUSICLRCSEARCHTABLEWIDGET_H
