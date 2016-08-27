#ifndef MUSICQUERYITEMTABLEWIDGET_H
#define MUSICQUERYITEMTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QActionGroup>
#include "musicnetworkthread.h"
#include "musicfillitemtablewidget.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class of the query item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQueryItemTableWidget : public MusicFillItemTableWidget
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
    virtual void itemDoubleClicked(int row, int column) = 0;
    /*!
     * Item has double clicked.
     * Subclass should implement this function.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    MusicDownLoadQueryThreadAbstract *m_downLoadManager;

};



class MUSIC_WIDGET_EXPORT MusicQueryTableWidget : public MusicQueryItemTableWidget
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

public Q_SLOTS:
    virtual void actionGroupClick(QAction *action);
    /*!
     * Left context menu action group click by action.
     */

protected:
    void createContextMenu(QMenu &menu);
    /*!
     * Create context menu.
     */

    QActionGroup *m_actionGroup;

};



#endif // MUSICQUERYITEMTABLEWIDGET_H
