#ifndef MUSICSONGSEARCHONLINEWIDGET_H
#define MUSICSONGSEARCHONLINEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythread.h"
#include "musicquerytablewidget.h"

#define AUDITION_PLAY static_cast<int>(Qt::yellow)
#define AUDITION_STOP static_cast<int>(Qt::transparent)

class MusicCoreMPlayer;
class MusicDataDownloadThread;
class MusicTextDownLoadThread;

class MUSIC_WIDGET_EXPORT MusicSongSearchOnlineTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongSearchOnlineTableWidget(QWidget *parent = 0);
    virtual ~MusicSongSearchOnlineTableWidget();

    virtual void startSearchQuery(const QString &text) override;
    virtual void musicDownloadLocal(int row) override;

    void auditionToMusic(int row);
    void auditionToMusicStop(int row);

signals:
    void muiscSongToPlayListChanged(const QString &name, const QString &size,
                                    const QString &format);
    void auditionIsPlaying(bool play);
    void getQualityString(QString &string);

public slots:
    void listCellClicked(int row, int column) override;
    void clearAllItems() override;
    void creatSearchedItems(const QString &songname,
                            const QString &artistname,
                            const QString &time) override;
    void itemDoubleClicked(int row, int column) override;
    void actionGroupClick(QAction *action) override;
    void researchQueryByQuality();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    void addSearchMusicToPlayList(int row);

    MusicCoreMPlayer *m_audition;
    int m_previousAuditionRow;
    QString m_searchText;

};


class QLabel;
class QPushButton;

class MUSIC_WIDGET_EXPORT MusicSongSearchOnlineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongSearchOnlineWidget(QWidget *parent = 0);
    ~MusicSongSearchOnlineWidget();

    void startSearchQuery(const QString &name) const;

public slots:
    void buttonClicked(int index);
    void auditionIsPlaying(bool play);

protected:
    void createToolWidget();
    QLabel *m_textLabel;
    QPushButton *m_playButton;
    MusicSongSearchOnlineTableWidget *m_searchTableWidget;

};

#endif // MUSICSONGSEARCHONLINEWIDGET_H
