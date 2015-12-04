#ifndef MUSICDOWNLOADWIDGET_H
#define MUSICDOWNLOADWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"
#include "musicdownloadquerythread.h"

#define ROW_HEIGHT  25

class MusicDownLoadQueryThread;

namespace Ui {
class MusicDownloadWidget;
}
class QLabel;

class MUSIC_WIDGET_EXPORT MusicDownloadTableItem : public QWidget
{
    Q_OBJECT
public:
    explicit MusicDownloadTableItem(QWidget *parent = 0);
    ~MusicDownloadTableItem();

    void setIcon(const QString &name);
    void setInfomation(const QString &info);
    void setText(const QString &text);

protected:
    QLabel *m_text, *m_infomation, *m_icon;

};

#define BITRATE_ROLE Qt::UserRole + 1

class MUSIC_WIDGET_EXPORT MusicDownloadTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicDownloadTableWidget(QWidget *parent = 0);
    virtual ~MusicDownloadTableWidget();

    void clearAllItems();
    void createItem(int bitrate, const QString &type, const QString &info,
                    const QString &icon);
    int getCurrentBitrate();

public slots:
    virtual void listCellClicked(int , int) override {}

protected:
    QList<QObject*> m_items;

};


class MUSIC_WIDGET_EXPORT MusicDownloadWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicDownloadWidget(QWidget *parent = 0);
    virtual ~MusicDownloadWidget();

    void setSongName(const QString &name, MusicDownLoadQueryThread::QueryType type);

public slots:
    virtual int exec();
    void queryAllFinished();
    void downloadDirSelected();
    void startToDownload();

protected:
    void initWidget();
    void controlEnable(bool enable);

    void queryAllFinishedMusic();
    void queryAllFinishedMovie();
    void startToDownloadMusic();
    void startToDownloadMovie();

    Ui::MusicDownloadWidget *ui;
    MusicDownLoadQueryThread *m_downloadThread;
    MusicDownLoadQueryThread::QueryType m_queryType;

};

#endif // MUSICDOWNLOADWIDGET_H
