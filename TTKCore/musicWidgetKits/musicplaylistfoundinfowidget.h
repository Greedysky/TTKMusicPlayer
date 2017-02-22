#ifndef MUSICPLAYLISTFOUNDINFOWIDGET_H
#define MUSICPLAYLISTFOUNDINFOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfoundabstractwidget.h"
#include "musicdownloadquerythreadabstract.h"

class QPushButton;
class QStackedWidget;
class MusicPlaylistFoundTableWidget;

/*! @brief The class of the playlist music found info widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistFoundInfoWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicPlaylistFoundInfoWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicPlaylistFoundInfoWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void resizeWindow();
    /*!
     * Resize window bound by widgte resize called.
     */

    void setMusicPlaylistItem(const MusicPlaylistItem &item, QObject *obj);
    /*!
     * Set music playlist item.
     */
    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Set network query input.
     */

public Q_SLOTS:
    void queryAllFinished();
    /*!
     * Query all quality musics is finished.
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
    void initFirstWidget();
    /*!
     * Init the first widget.
     */
    void initSecondWidget();
    /*!
     * Init the second widget.
     */

    QLabel *m_iconLabel, *m_infoLabel;
    QStackedWidget *m_container;
    QPushButton *m_songButton;
    MusicPlaylistFoundTableWidget *m_playlistTableWidget;

};

#endif // MUSICPLAYLISTFOUNDINFOWIDGET_H
