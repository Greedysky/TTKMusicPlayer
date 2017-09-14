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
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistFoundInfoWidget(QWidget *parent = 0);

    virtual ~MusicPlaylistFoundInfoWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow();

    /*!
     * Set music playlist item.
     */
    void setMusicPlaylistItem(const MusicPlaylistItem &item, QObject *obj);
    /*!
     * Set network query input.
     */
    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();
    /*!
     * Query all quality musics is finished.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Play all button clicked now.
     */
    void playAllButtonClicked();
    /*!
     * Share button clicked now.
     */
    void shareButtonClicked();
    /*!
     * Play button clicked now.
     */
    void playButtonClicked();
    /*!
     * Download button clicked now.
     */
    void downloadButtonClicked();
    /*!
     * Add button clicked now.
     */
    void addButtonClicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Init the first widget.
     */
    void initFirstWidget();
    /*!
     * Init the second widget.
     */
    void initSecondWidget();

    QLabel *m_iconLabel, *m_infoLabel;
    QStackedWidget *m_container;
    QPushButton *m_songButton;
    MusicPlaylistFoundTableWidget *m_playlistTableWidget;

};

#endif // MUSICPLAYLISTFOUNDINFOWIDGET_H
