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
#include "musicabstracttablewidget.h"

class QNSimpleListData;
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

signals:
    void updateLabelMessage(const QString &text);

public Q_SLOTS:
    virtual void listCellClicked(int row, int column);
    /*!
     * Table widget list cell click.
     */
    void updateList();
    void receiveDataFinshed(const QNDataItems &items);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    QNSimpleListData *m_qnListData;
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

signals:

public slots:
    void updateLabelMessage(const QString &text);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    MusicCloudSharedSongTableWidget *m_tableWidget;
    QLabel *m_statusLabel;

};

#endif // MUSICCLOUDSHAREDSONGWIDGET_H
