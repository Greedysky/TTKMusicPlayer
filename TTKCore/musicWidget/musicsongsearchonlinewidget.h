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

/*! @brief The class of the song search online table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongSearchOnlineTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongSearchOnlineTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongSearchOnlineTableWidget();

    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Start search query by text.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Data download to local file.
     */

    void auditionToMusic(int row);
    /*!
     * Start play audition to music by row.
     */
    void auditionToMusicStop(int row);
    /*!
     * Stop play audition to music by row.
     */

Q_SIGNALS:
    void muiscSongToPlayListChanged(const QString &name, const QString &size,
                                    const QString &format);
    /*!
     * Add current network music to download to local.
     */
    void auditionIsPlaying(bool play);
    /*!
     * Check current audtion is playing or not.
     */
    void getQualityString(QString &string);
    /*!
     * Get current quality string.
     */

public Q_SLOTS:
    void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
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
    void actionGroupClick(QAction *action) override;
    /*!
     * Left context menu action group click by action.
     */
    void researchQueryByQuality();
    /*!
     * Research query by quality it changed.
     */
    void searchDataDwonloadFinished();
    /*!
     * Search data dwonload finished.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void addSearchMusicToPlayList(int row);
    /*!
     * Add search music to play list by index.
     */

    MusicCoreMPlayer *m_audition;
    int m_previousAuditionRow;
    QString m_searchText;
    QStringList m_downloadDatas;

};


class QLabel;
class QPushButton;

/*! @brief The class of the song search online widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongSearchOnlineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongSearchOnlineWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongSearchOnlineWidget();

    void startSearchQuery(const QString &name) const;
    /*!
     * Start search query by text.
     */

public Q_SLOTS:
    void buttonClicked(int index);
    /*!
     * Set diff button clicked by index.
     */
    void auditionIsPlaying(bool play);
    /*!
     * Check current audtion is playing or not.
     */

protected:
    void createToolWidget();
    /*!
     * Create tool widget.
     */

    QLabel *m_textLabel;
    QPushButton *m_playButton;
    MusicSongSearchOnlineTableWidget *m_searchTableWidget;

};

#endif // MUSICSONGSEARCHONLINEWIDGET_H
