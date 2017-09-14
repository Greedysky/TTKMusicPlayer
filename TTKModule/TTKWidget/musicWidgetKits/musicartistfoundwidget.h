#ifndef MUSICARTISTFOUNDTABLEWIDGET_H
#define MUSICARTISTFOUNDTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfoundabstractwidget.h"
#include "musicqueryfoundtablewidget.h"

/*! @brief The class of the artist music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistFoundTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistFoundTableWidget(QWidget *parent = 0);

    virtual ~MusicArtistFoundTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set network query input.
     */
    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);

};


/*! @brief The class of artist music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistFoundWidget(QWidget *parent = 0);

    virtual ~MusicArtistFoundWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() override;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();
    /*!
     * Query artist musics is finished.
     */
    void queryArtistFinished();
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data);
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
    QLabel *m_iconLabel;
    MusicArtistFoundTableWidget *m_artistTableWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadThread;

};

#endif // MUSICARTISTFOUNDTABLEWIDGET_H
