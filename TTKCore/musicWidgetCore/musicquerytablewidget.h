#ifndef MUSICQUERYITEMTABLEWIDGET_H
#define MUSICQUERYITEMTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QActionGroup>
#include "musicnetworkthread.h"
#include "musicfillitemtablewidget.h"
#include "musicdownloadquerythreadabstract.h"

class MusicGifLabelWidget;

/*! @brief The class of the query table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQueryTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    explicit MusicQueryTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicQueryTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Set network query input.
     */
    virtual void startSearchQuery(const QString &text) = 0;
    /*!
     * Start search query by text.
     * Subclass should implement this function.
     */
    virtual void musicDownloadLocal(int row) = 0;
    /*!
     * Data download to local file.
     * Subclass should implement this function.
     */

Q_SIGNALS:
    void muiscSongToPlayListChanged(const QString &name, const QString &time,
                                    const QString &format, bool play);
    /*!
     * Add current network music to download to local.
     */
    void showDownLoadInfoFor(MusicObject::DownLoadType type);
    /*!
     * Show download state type.
     */
    void restartSearchQuery(const QString &name);
    /*!
     * Restart search query by name.
     */

public Q_SLOTS:
    virtual void clearAllItems() = 0;
    /*!
     * Clear All Items.
     * Subclass should implement this function.
     */
    virtual void createSearchedItems(const QString &songname,
                                     const QString &artistname,
                                     const QString &time) = 0;
    /*!
     * Create searched items.
     * Subclass should implement this function.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    MusicDownLoadQueryThreadAbstract *m_downLoadManager;

};


class MusicLabelDelegate;
/*! @brief The class of the query item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQueryItemTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicQueryItemTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicQueryItemTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startSearchQuery(const QString &text);
    /*!
     * Start search query by text.
     */

public Q_SLOTS:
    virtual void clearAllItems() override;
    /*!
     * Clear All Items.
     */
    virtual void actionGroupClick(QAction *action);
    /*!
     * Left context menu action group click by action.
     */
    virtual void itemDoubleClicked(int row, int column) = 0;
    /*!
     * Item has double clicked.
     * Subclass should implement this function.
     */
    void createFinishedItem();
    /*!
     * Create the search finished item.
     */

protected:
    void createContextMenu(QMenu &menu);
    /*!
     * Create context menu.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */

    MusicGifLabelWidget *m_loadingLabel;
    QActionGroup *m_actionGroup;
    MusicLabelDelegate *m_labelDelegate;

};

#endif // MUSICQUERYITEMTABLEWIDGET_H
