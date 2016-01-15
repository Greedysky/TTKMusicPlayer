#ifndef MUSICDOWNLOADWIDGET_H
#define MUSICDOWNLOADWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"
#include "musicdownloadquerysinglethread.h"

#define ROW_HEIGHT  25

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

public Q_SLOTS:
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

    void setSongName(const QString &name, MusicDownLoadQueryThreadAbstract::QueryType type);

public Q_SLOTS:
    virtual int exec();
    void queryAllFinished();
    void downloadDirSelected();
    void startToDownload();

protected:
    void initWidget();
    void controlEnable(bool enable);
    void resizeWindow();
    void setFixedHeightWidget(QWidget *w, int height);
    void setMoveWidget(QWidget *w, int pos);

    void queryAllFinishedMusic();
    void queryAllFinishedMovie();
    void startToDownloadMusic();
    void startToDownloadMovie();

    Ui::MusicDownloadWidget *ui;
    MusicDownLoadQueryThreadAbstract *m_downloadThread;
    MusicDownLoadQueryThreadAbstract::QueryType m_queryType;

};

#endif // MUSICDOWNLOADWIDGET_H
