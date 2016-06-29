#ifndef MUSICMYDOWNLOADRECORDWIDGET_H
#define MUSICMYDOWNLOADRECORDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstracttablewidget.h"
#include "musicmydownloadrecordconfigmanager.h"

#define MUSIC_TIMES_ROLE Qt::UserRole + 1

class MusicProgressBarDelegate;

/*! @brief The class of the download record widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicMyDownloadRecordWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicMyDownloadRecordWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicMyDownloadRecordWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void musicSongsFileName();
    /*!
     * Read all config from file and insert items.
     */
    void clearAllItems();
    /*!
     * Clear All Items.
     */

Q_SIGNALS:
    void addSongToPlay(const QStringList &list);
    /*!
     * Add current selected song to play lists.
     */

public Q_SLOTS:
    void musicPlay();
    /*!
     * Add selected music song path to list.
     */
    void setDeleteItemAt();
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemAll();
    /*!
     * Delete all items from list.
     */
    void listCellClicked(int row, int column);
    /*!
     * Table widget list cell click.
     */
    void listCellDoubleClicked(int row, int column);
    /*!
     * Table widget list cell double click.
     */
    void musicOpenFileDir();
    /*!
     * Open the music at local path.
     */
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    /*!
     * Update download percent\ total time and current time progress.
     */
    void createDownloadItem(const QString &name, qint64 time);
    /*!
     * Create download item from download name and total time.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createItem(int index, const QString &name, const QString &size, qint64 time);
    /*!
     * Create item by index and name and size and time.
     */

    MusicProgressBarDelegate *m_delegate;
    MusicDownloadRecord m_musicRecord;
    int m_loadRecordCount;

};

#endif // MUSICMYDOWNLOADRECORDWIDGET_H
