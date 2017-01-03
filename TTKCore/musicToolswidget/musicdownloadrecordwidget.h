#ifndef MUSICDOWNLOADRECORDWIDGET_H
#define MUSICDOWNLOADRECORDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsongslistabstracttablewidget.h"
#include "musicdownloadrecordconfigmanager.h"

class MusicProgressBarDelegate;

/*! @brief The class of the download record widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicDownloadRecordWidget : public MusicSongsListAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicDownloadRecordWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicDownloadRecordWidget();

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
    virtual void setDeleteItemAt() override;
    /*!
     * Delete item from list at current row.
     */
    void listCellClicked(int row, int column);
    /*!
     * Table widget list cell click.
     */
    void listCellDoubleClicked(int row, int column);
    /*!
     * Table widget list cell double click.
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
    void createItem(int index, const MusicDownloadRecord &record, qint64 time);
    /*!
     * Create item by index and name and size and time.
     */

    MusicProgressBarDelegate *m_delegate;
    MusicDownloadRecords m_musicRecords;
    int m_loadRecordCount;

};

#endif // MUSICDOWNLOADRECORDWIDGET_H
