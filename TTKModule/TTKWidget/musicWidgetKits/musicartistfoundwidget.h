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
    explicit MusicArtistFoundTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicArtistFoundTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Set network query input.
     */

};


/*! @brief The class of artist music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicArtistFoundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicArtistFoundWidget();

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
    void queryArtistFinished();
    /*!
     * Query artist musics is finished.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Send recieved data from net.
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
    QLabel *m_iconLabel;
    MusicArtistFoundTableWidget *m_artistTableWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadThread;

};

#endif // MUSICARTISTFOUNDTABLEWIDGET_H
