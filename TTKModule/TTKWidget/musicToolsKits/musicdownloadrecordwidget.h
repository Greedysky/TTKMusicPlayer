#ifndef MUSICDOWNLOADRECORDWIDGET_H
#define MUSICDOWNLOADRECORDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadRecordWidget(QWidget *parent = 0);

    virtual ~MusicDownloadRecordWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read all config from file and insert items.
     */
    void musicSongsFileName();
    /*!
     * Clear All Items.
     */
    void clearAllItems();

Q_SIGNALS:
    /*!
     * Add current selected song to play lists.
     */
    void addSongToPlay(const QStringList &list);

public Q_SLOTS:
    /*!
     * Add selected music song path to list.
     */
    void musicPlay();
    /*!
     * Delete item from list at current row.
     */
    virtual void setDeleteItemAt() override;
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell double click.
     */
    void listCellDoubleClicked(int row, int column);
    /*!
     * Update download percent\ total time and current time progress.
     */
    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    /*!
     * Create download item from download name and total time.
     */
    void createDownloadItem(const QString &name, qint64 time);

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Create item by index and name and size and time.
     */
    void createItem(int index, const MusicDownloadRecord &record, qint64 time);

    MusicProgressBarDelegate *m_delegate;
    MusicDownloadRecords m_musicRecords;
    int m_loadRecordCount;

};

#endif // MUSICDOWNLOADRECORDWIDGET_H
