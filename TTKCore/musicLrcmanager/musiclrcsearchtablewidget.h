#ifndef MUSICLRCSEARCHTABLEWIDGET_H
#define MUSICLRCSEARCHTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicquerytablewidget.h"

/*! @brief The class of the lrc search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcSearchTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicLrcSearchTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcSearchTableWidget();

    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Start search query by text.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Data download to local file.
     */
    inline void setCurrentSongName(const QString &name){ m_currentSongName = name;}
    /*!
     * Set current song name.
     */

Q_SIGNALS:
    void resolvedSuccess();
    /*!
     * Lrc search state is finished.
     */
    void lrcDownloadStateChanged(const QString &name);
    /*!
     * Lrc download state is finished.
     */

public Q_SLOTS:
    void clearAllItems() override;
    /*!
     * Clear All Items.
     */
    void createSearchedItems(const QString &songname,
                             const QString &artistname, const QString &time) override;
    /*!
     * Create searched items.
     */
    void itemDoubleClicked(int row, int column) override;
    /*!
     * Item has double clicked.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    QString m_currentSongName;

};

#endif // MUSICLRCSEARCHTABLEWIDGET_H
