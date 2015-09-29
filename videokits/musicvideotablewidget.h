#ifndef MUSICVIDEOTABLEWIDGET_H
#define MUSICVIDEOTABLEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythread.h"
#include "musicquerytablewidget.h"

class MUSIC_VIDEO_EXPORT MusicVideoTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicVideoTableWidget(QWidget *parent = 0);
    virtual ~MusicVideoTableWidget();

    void startSearchQuery(const QString &text);
    void musicDownloadLocal(int row);
    void resizeWindow(float delta);

signals:
    void mvURLChanged(const QString &data);

public slots:
    void listCellClicked(int row, int col);
    void clearAllItems();
    void creatSearchedItems(const QString &songname,
                            const QString &artistname,
                            const QString &time);
    void itemDoubleClicked(int row, int column);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    QString randToGetStrength() const;
    QString m_currentSongName;

};

#endif // MUSICVIDEOTABLEWIDGET_H
