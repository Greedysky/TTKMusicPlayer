#ifndef MUSICCLOUDSHAREDSONGWIDGET_H
#define MUSICCLOUDSHAREDSONGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include <QLabel>
#include "qiniu/qnsimplelistdata.h"
#include "qiniu/qnsimpleuploaddata.h"
#include "qiniu/qnsimpledeletedata.h"
#include "musiccloudfilemanagerdialog.h"

class MusicOpenFileWidget;

/*! @brief The class of the cloud shared song table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudSharedSongTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCloudSharedSongTableWidget(QWidget *parent = 0);

    virtual ~MusicCloudSharedSongTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Get query cloud id keys.
     */
    bool getKey();
    /*!
     * Get upload datas.
     */
    inline const MusicUploadDatas& getUploadDatas() const { return m_waitedFiles;}

Q_SIGNALS:
    /*!
     * To update message label text.
     */
    void updateLabelMessage(const QString &text);
    /*!
     * Get key data from net finished.
     */
    void getKeyFinished();

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Download key data from net finished.
     */
    void keyDownLoadFinished(const QByteArray &data);
    /*!
     * Receive data from qiniu finshed.
     */
    void receiveDataFinshed(const QNDataItems &items);
    /*!
     * Upload data to qiniu finshed.
     */
    void uploadFileFinished(const QString &name);
    /*!
     * Delete data to qiniu finshed.
     */
    void deleteFileFinished(bool state);

    /*!
     * Update list to server.
     */
    void updateListToServer();
    /*!
     * Delete file to server.
     */
    void deleteFileToServer();
    /*!
     * Delete files to server.
     */
    void deleteFilesToServer();
    /*!
     * Download file to server.
     */
    void downloadFileToServer();

    /*!
     * Upload file to server.
     */
    void uploadFileToServer();
    /*!
     * Upload files to server.
     */
    void uploadFilesToServer();
    /*!
     * Reupload files to server.
     */
    void reuploadFileToServer();

    /*!
     * Open file manager dialog.
     */
    void openFileManagerDialog();
    /*!
     * Show upload progress.
     */
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    /*!
     * All files upload finsihed.
     */
    void uploadDone();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Create upload file widget.
     */
    void createUploadFileWidget();
    /*!
     * Start to upload files to server.
     */
    void startToUploadFile();
    /*!
     * Check upload has failed.
     */
    bool uploadHasFailed();

    bool m_uploading;
    int m_currentUploadIndex;
    MusicUploadDatas m_waitedFiles;
    QNSimpleListData *m_qnListData;
    QNSimpleDeleteData *m_qnDeleteData;
    QNSimpleUploadData *m_qnUploadData;
    QNetworkAccessManager *m_networkManager;
    MusicOpenFileWidget *m_openFileWidget;
    MusicCloudFileManagerDialog *m_fileDialog;

};


/*! @brief The class of the cloud share song widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudSharedSongWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCloudSharedSongWidget(QWidget *parent = 0);

    ~MusicCloudSharedSongWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Get query cloud id keys.
     */
    void getKey();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    QLabel *m_statusLabel;
    MusicCloudSharedSongTableWidget *m_tableWidget;

};

#endif // MUSICCLOUDSHAREDSONGWIDGET_H
