#ifndef MUSICCLOUDSHAREDSONGWIDGET_H
#define MUSICCLOUDSHAREDSONGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "qnsimplelistdata.h"
#include "qnsimpleuploaddata.h"
#include "qnsimpledeletedata.h"
#include "musiccloudfilemanagerdialog.h"

class MusicUploadFileWidget;

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

    inline const UploadDatas& getUploadDatas() const { return m_waitedFiles;}
    /*!
     * Get upload datas.
     */

Q_SIGNALS:
    void updateLabelMessage(const QString &text);
    /*!
     * To update message label text.
     */
    void uploadDone();
    /*!
     * One file upload finsihed.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
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
    void downloadFileToServer();

    void uploadFileToServer();
    /*!
     * Upload file to server.
     */
    void uploadFilesToServer();
    /*!
     * Upload files to server.
     */
    void startToUploadFile();
    /*!
     * Start to upload files to server.
     */

    void openFileManagerDialog();
    /*!
     * Open file manager dialog.
     */
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    /*!
     * Show upload progress.
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

    bool m_uploading;
    QEventLoop m_eventLoop;
    QTimer *m_timerToUpload;
    UploadData m_currentUploadData;
    UploadDatas m_waitedFiles;
    QNSimpleListData *m_qnListData;
    QNSimpleDeleteData *m_qnDeleteData;
    QNSimpleUploadData *m_qnUploadData;
    QNetworkAccessManager *m_networkManager;
    MusicUploadFileWidget *m_uploadFileWidget;
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
