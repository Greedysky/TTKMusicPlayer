#ifndef MUSICALBUMFOUNDWIDGET_H
#define MUSICALBUMFOUNDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicquerytablewidget.h"

/*! @brief The class of the album music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAlbumFoundTableWidget : public MusicQueryItemTableWidget
{
    Q_OBJECT
public:
    explicit MusicAlbumFoundTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicAlbumFoundTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Set network query input.
     */
    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Start search query by text.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Data download to local file.
     */
    const MusicObject::MusicSongInfomations& getMusicSongInfos() const;
    /*!
     * Return the current song container.
     */

Q_SIGNALS:
    void addSearchMusicToPlayList(int row, bool play);
    /*!
     * Add search music to play list by index.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Subclass should implement this function.
     */
    virtual void clearAllItems() override;
    /*!
     * Clear All Items.
     */
    virtual void createSearchedItems(const QString &songname, const QString &artistname,
                                     const QString &time) override;
    /*!
     * Create searched items.
     */

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */

};


/*! @brief The class of album music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAlbumFoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicAlbumFoundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicAlbumFoundWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSongName(const QString &name);
    /*!
     * Set current name to search founds.
     */

Q_SIGNALS:
    void muiscSongToPlayListChanged(const QString &name, const QString &time,
                                    const QString &format, bool play);
    /*!
     * Add current network music to download to local.
     */

public Q_SLOTS:
    void queryAllFinished();
    /*!
     * Query all quality musics is finished.
     */
    void queryAlbumFinished();
    /*!
     * Query album musics is finished.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Send recieved data from net.
     */
    void playAllButtonClicked();
    /*!
     * Play all button clicked now.
     */
    void shareButtonClicked();
    /*!
     * Share button clicked now.
     */
    void playButtonClicked();
    /*!
     * Play button clicked now.
     */
    void downloadButtonClicked();
    /*!
     * Download button clicked now.
     */
    void addButtonClicked();
    /*!
     * Add button clicked now.
     */
    void addSearchMusicToPlayList(int row, bool play);
    /*!
     * Add search music to play list by index.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createNoAlbumLabel();
    /*!
     * Create no album label widget.
     */
    void downloadDataFrom(bool play);
    /*!
     * Download data from net and just play or not.
     */
    bool downloadDataFrom(const MusicObject::MusicSongInfomation &downloadInfo, bool play);
    /*!
     * Download data from net and just play or not.
     */

    QString m_songNameFull;
    QWidget *m_mainWindow;
    QLabel *m_statusLabel, *m_iconLabel;
    QList<QLabel*> m_resizeWidget;
    MusicAlbumFoundTableWidget *m_albumTableWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadThread;

};

#endif // MUSICALBUMFOUNDWIDGET_H
