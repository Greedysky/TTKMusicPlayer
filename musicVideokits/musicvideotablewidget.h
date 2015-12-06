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

    virtual void startSearchQuery(const QString &text) override;
    virtual void musicDownloadLocal(int row) override;
    void resizeWindow(float delta);

signals:
    void mvURLNameChanged(const QString &name, const QString &data);

public slots:
    void listCellClicked(int row, int col) override;
    void clearAllItems() override;
    void creatSearchedItems(const QString &songname,
                            const QString &artistname,
                            const QString &time) override;
    void itemDoubleClicked(int row, int column) override;
    void getMusicMvInfo(MusicSongAttributes &data);
    void downloadLocalFromControl();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    QString randToGetStrength() const;
    void downloadLocalMovie(int row);

    QString m_currentSongName;

};

#endif // MUSICVIDEOTABLEWIDGET_H
