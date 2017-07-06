#ifndef MUSICCLOUDSHAREDSONGWIDGET_H
#define MUSICCLOUDSHAREDSONGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "qnsimplelistdata.h"
#include "qnsimpleuploaddata.h"
#include "qnsimpledeletedata.h"
#include "musiccloudfilemanagerdialog.h"

class MusicOpenFileWidget;

/*! @brief The class of the cloud shared song table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudSharedSongTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicCloudSharedSongTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicCloudSharedSongTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    bool getKey();
    /*!
     * Get query cloud id keys.
     */
    inline const MusicUploadDatas& getUploadDatas() const { return m_waitedFiles;}
    /*!
     * Get upload datas.
     */

Q_SIGNALS:
    void updateLabelMessage(const QString &text);
    /*!
     * To update message label text.
     */
    void getKeyFinished();
    /*!
     * Get key data from net finished.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    void keyDownLoadFinished(const QByteArray &data);
    /*!
     * Download key data from net finished.
     */
    void receiveDataFinshed(const QNDataItems &items);
    /*!
     * Receive data from qiniu finshed.
     */
    void uploadFileFinished(const QString &name);
    /*!
     * Upload data to qiniu finshed.
     */
    void deleteFileFinished(bool state);
    /*!
     * Delete data to qiniu finshed.
     */

    void updateListToServer();
    /*!
     * Update list to server.
     */
    void deleteFileToServer();
    /*!
     * Delete file to server.
     */
    void deleteFilesToServer();
    /*!
     * Delete files to server.
     */
    void downloadFileToServer();
    /*!
     * Download file to server.
     */

    void uploadFileToServer();
    /*!
     * Upload file to server.
     */
    void uploadFilesToServer();
    /*!
     * Upload files to server.
     */
    void reuploadFileToServer();
    /*!
     * Reupload files to server.
     */

    void openFileManagerDialog();
    /*!
     * Open file manager dialog.
     */
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    /*!
     * Show upload progress.
     */
    void uploadDone();
    /*!
     * All files upload finsihed.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createUploadFileWidget();
    /*!
     * Create upload file widget.
     */
    void startToUploadFile();
    /*!
     * Start to upload files to server.
     */
    bool uploadHasFailed();
    /*!
     * Check upload has failed.
     */

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
    explicit MusicCloudSharedSongWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicCloudSharedSongWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void getKey();
    /*!
     * Get query cloud id keys.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    QLabel *m_statusLabel;
    MusicCloudSharedSongTableWidget *m_tableWidget;

};

#endif // MUSICCLOUDSHAREDSONGWIDGET_H
