#ifndef MUSICMYDOWNLOADRECORDWIDGET_H
#define MUSICMYDOWNLOADRECORDWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstracttablewidget.h"
#include "musicmydownloadrecordobject.h"

#define MUSIC_TIMES_ROLE Qt::UserRole + 1

class MusicProgressBarDelegate;

class MUSIC_TOOL_EXPORT MusicMyDownloadRecordWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicMyDownloadRecordWidget(QWidget *parent = 0);
    virtual ~MusicMyDownloadRecordWidget();

    void musicSongsFileName();
    void clearAllItems();

signals:
    void addSongToPlay(const QStringList &list);

public slots:
    void musicPlay();
    void setDeleteItemAt();
    void setDeleteItemAll();
    void listCellClicked(int row, int column);
    void listCellDoubleClicked(int row, int column);
    void musicOpenFileDir();

    void downloadProgressChanged(float percent, const QString &total, qint64 time);
    void createDownloadItem(const QString &name, qint64 time);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    void createItem(int index, const QString &name, const QString &size, qint64 time);

    MusicProgressBarDelegate *m_delegate;
    MusicDownloadRecord m_musicRecord;
    int m_loadRecordCount;

};

#endif // MUSICMYDOWNLOADRECORDWIDGET_H
