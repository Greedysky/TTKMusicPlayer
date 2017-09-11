#ifndef MUSICALBUMFOUNDWIDGET_H
#define MUSICALBUMFOUNDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfoundabstractwidget.h"
#include "musicqueryfoundtablewidget.h"

/*! @brief The class of the album music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAlbumFoundTableWidget : public MusicQueryFoundTableWidget
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

};


/*! @brief The class of album music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAlbumFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicAlbumFoundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicAlbumFoundWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void setSongName(const QString &name) override;
    /*!
     * Set current name to search founds.
     */

    virtual void resizeWindow() override;
    /*!
     * Resize window bound by widgte resize called.
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

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */

    QLabel *m_iconLabel;
    MusicAlbumFoundTableWidget *m_albumTableWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadThread;

};

#endif // MUSICALBUMFOUNDWIDGET_H
