#ifndef MUSICTABLEQUERYWIDGET_H
#define MUSICTABLEQUERYWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictablewidgetabstract.h"
#include "musicdownloadquerythread.h"

class MUSIC_WIDGET_EXPORT MusicTableQueryWidget : public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicTableQueryWidget(QWidget *parent = 0);

    virtual void startSearchQuery(const QString &text) = 0;

signals:
    void showDownLoadInfoFinished(const QString &name);
    void MuiscSongToPlayListChanged(const QString &name);
    void showDownLoadInfoFor(DownLoadType);

public slots:
    virtual void clearAllItems() = 0;
    virtual void creatSearchedItems(const QString &songname,
                                    const QString &artistname,
                                    const QString &time) = 0;
    virtual void itemDoubleClicked(int row, int column) = 0;

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    MusicDownLoadQueryThread *m_downLoadManager;

};

#endif // MUSICTABLEQUERYWIDGET_H
