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
#include "musicabstracttablewidget.h"

typedef struct UploadData{
    QString m_path;
    QString m_name;
    int m_state;
    ///0 waited, 1 successed, 2 error
}UploadData;
typedef QList<UploadData> UploadDatas;

class QNetworkAccessManager;

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

Q_SIGNALS:
    void updateLabelMessage(const QString &text);
    void uploadDone();

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    void updateList();
    void receiveDataFinshed(const QNDataItems &items);
    void uploadFileFinished(const QString &name);

    void uploadFileToServer();
    void uploadFilesToServer();

    void startToUploadFile();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createUploadFileWidget();

    bool m_uploading;
    QTimer *m_timerToUpload;
    QWidget *m_uploadFileWidget;
    QString m_currentUploadFileName;
    UploadDatas m_waitedFiles;
    QNSimpleListData *m_qnListData;
    QNSimpleUploadData *m_qnUploadData;
    QNetworkAccessManager *m_networkManager;

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

Q_SIGNALS:

public Q_SLOTS:
    void updateLabelMessage(const QString &text);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    MusicCloudSharedSongTableWidget *m_tableWidget;
    QLabel *m_statusLabel;

};

#endif // MUSICCLOUDSHAREDSONGWIDGET_H
