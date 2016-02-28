#ifndef MUSICSONGSEARCHONLINEWIDGET_H
#define MUSICSONGSEARCHONLINEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"
#include "musicquerytablewidget.h"

#define AUDITION_PLAY MStatic_cast(int, Qt::yellow)
#define AUDITION_STOP MStatic_cast(int, Qt::transparent)

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

Q_SIGNALS:
    void muiscSongToPlayListChanged(const QString &name, const QString &size,
                                    const QString &format);
    void auditionIsPlaying(bool play);
    void getQualityString(QString &string);

public Q_SLOTS:
    void listCellClicked(int row, int column) override;
    void clearAllItems() override;
    void creatSearchedItems(const QString &songname,
                            const QString &artistname,
                            const QString &time) override;
    void itemDoubleClicked(int row, int column) override;
    void actionGroupClick(QAction *action) override;
    void researchQueryByQuality();
    void searchDataDwonloadFinished();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    void addSearchMusicToPlayList(int row);

    MusicCoreMPlayer *m_audition;
    int m_previousAuditionRow;
    QString m_searchText;
    QStringList m_downloadDatas;

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

public Q_SLOTS:
    void buttonClicked(int index);
    void auditionIsPlaying(bool play);

protected:
    void createToolWidget();

    QLabel *m_textLabel;
    QPushButton *m_playButton;
    MusicSongSearchOnlineTableWidget *m_searchTableWidget;

};

#endif // MUSICSONGSEARCHONLINEWIDGET_H
