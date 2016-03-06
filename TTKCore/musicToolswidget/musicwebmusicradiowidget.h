#ifndef MUSICWEBMUSICRADIOWIDGET_H
#define MUSICWEBMUSICRADIOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"

class QNetworkCookieJar;
class MusicRadioSongsThread;
class MusicRadioPlayListThread;

namespace Ui {
class MusicWebMusicRadioWidget;
}

/*! @brief The class of the web music radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMusicRadioWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicWebMusicRadioWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicWebMusicRadioWidget();

    void setNetworkCookie(QNetworkCookieJar *jar);
    /*!
     * Set network cookie.
     */
    void updateRadioList(const QString &category);
    /*!
     * Update radio list by given category.
     */

public Q_SLOTS:
    void getPlayListFinished();
    /*!
     * Get playList finished.
     */
    void getSongInfoFinished();
    /*!
     * Get song information finished.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    Ui::MusicWebMusicRadioWidget *ui;
    MusicRadioPlayListThread *m_playListThread;
    MusicRadioSongsThread *m_songsThread;
    QStringList m_playListIds;

};

#endif // MUSICWEBMUSICRADIOWIDGET_H
