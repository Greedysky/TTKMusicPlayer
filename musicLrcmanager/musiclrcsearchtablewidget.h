#ifndef MUSICLRCSEARCHTABLEWIDGET_H
#define MUSICLRCSEARCHTABLEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythread.h"
#include "musicquerytablewidget.h"

class MUSIC_LRC_EXPORT MusicLrcSearchTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicLrcSearchTableWidget(QWidget *parent = 0);
    virtual ~MusicLrcSearchTableWidget();

    virtual void startSearchQuery(const QString &text) override;
    virtual void musicDownloadLocal(int row) override;
    inline void setCurrentSongName(const QString &name){ m_currentSongName = name;}

signals:
    void lrcDownloadStateChanged(const QString &name);

public slots:
    void clearAllItems() override;
    void creatSearchedItems(const QString &songname,
                            const QString &artistname, const QString &time) override;
    void itemDoubleClicked(int row, int column) override;

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    QString m_currentSongName;

};

#endif // MUSICLRCSEARCHTABLEWIDGET_H
