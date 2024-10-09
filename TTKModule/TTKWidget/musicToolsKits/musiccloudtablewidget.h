#ifndef MUSICCLOUDTABLEWIDGET_H
#define MUSICCLOUDTABLEWIDGET_H

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

#include "musicclouddataitem.h"
#include "musicabstractdownloadtablewidget.h"

/*! @brief The class of the cloud shared song download table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCloudDownloadTableWidget : public MusicAbstractDownloadTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCloudDownloadTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicCloudDownloadTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicCloudDownloadTableWidget();

private:
    /*!
     * Create cell item by input record.
     */
    virtual void addCellItem(int index, const MusicSong &record) override final;

};


/*! @brief The class of the cloud shared song upload table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCloudUploadTableWidget : public MusicAbstractDownloadTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCloudUploadTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicCloudUploadTableWidget(QWidget *parent = nullptr);

Q_SIGNALS:
    /*!
     * Reupload files to server.
     */
    void reuploadFilesToServer(const QStringList &items);

public Q_SLOTS:
    /*!
     * Upload file error occurred.
     */
    void uploadFileError(const MusicCloudDataItem &item);
    /*!
     * Reupload failed file.
     */
    void reuploadFile();
    /*!
     * Reupload failed files.
     */
    void reuploadFiles();

private:
    /*!
     * Create cell item by input record.
     */
    virtual void addCellItem(int index, const MusicSong &record) override final;
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

};

#endif // MUSICCLOUDTABLEWIDGET_H
