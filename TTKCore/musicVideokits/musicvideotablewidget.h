#ifndef MUSICVIDEOTABLEWIDGET_H
#define MUSICVIDEOTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"
#include "musicquerytablewidget.h"

class MUSIC_VIDEO_EXPORT MusicVideoTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicVideoTableWidget(QWidget *parent = 0);
    virtual ~MusicVideoTableWidget();

    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Start search query by text.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Data download to local file.
     */
    void resizeWindow(float delta);

Q_SIGNALS:
    void mvURLNameChanged(const QString &name, const QString &data);

public Q_SLOTS:
    void listCellClicked(int row, int col) override;
    void clearAllItems() override;
    /*!
     * Clear All Items.
     */
    void createSearchedItems(const QString &songname,
                             const QString &artistname,
                             const QString &time) override;
    /*!
     * Create searched items.
     */
    void itemDoubleClicked(int row, int column) override;
    /*!
     * Item has double clicked.
     */
    void getMusicMvInfo(MusicSongAttributes &data);
    void downloadLocalFromControl();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    QString randToGetStrength() const;
    void downloadLocalMovie(int row);

    QString m_currentSongName;

};

#endif // MUSICVIDEOTABLEWIDGET_H
