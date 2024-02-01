#ifndef MUSICDOWNLOADRECORDWIDGET_H
#define MUSICDOWNLOADRECORDWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#include "musicfunctiontoolboxwidget.h"
#include "musicabstractdownloadtablewidget.h"

/*! @brief The class of the download record table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadRecordTableWidget : public MusicAbstractDownloadTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadRecordTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadRecordTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicDownloadRecordTableWidget();

private:
    /*!
     * Create cell item by input record.
     */
    virtual void addCellItem(int index, const MusicSong &record) override final;

};

/*! @brief The class of the download tool box widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadToolBoxWidget : public MusicFunctionToolBoxWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadToolBoxWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadToolBoxWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicDownloadToolBoxWidget();

public Q_SLOTS:
    /*!
     * Update item title.
     */
    void updateItemTitle(int index);

private:
    /*!
     * Create widget item.
     */
    void createWidgetItem(MusicAbstractDownloadTableWidget *widget, const QString &text, int index);
    /*!
     * Create item.
     */
    virtual MusicFunctionToolBoxWidgetItem* initialItem(QWidget *item, const QString &text) override final;

    MusicSongItemList m_songItems;
    MusicDownloadRecordTableWidget *m_tableWidget;

};

#endif // MUSICDOWNLOADRECORDWIDGET_H
