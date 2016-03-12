#ifndef MUSICWEBMUSICRADIOLISTVIEW_H
#define MUSICWEBMUSICRADIOLISTVIEW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QListWidget>
#include "musicglobaldefine.h"

class QNetworkCookieJar;
class MusicRadioChannelThread;
class MusicWebMusicRadioWidget;

/*! @brief The class of the web music radio list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMusicRadioListView : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicWebMusicRadioListView(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicWebMusicRadioListView();

    void initListItems();
    /*!
     * To init list items.
     */

public Q_SLOTS:
    void itemHasClicked(QListWidgetItem *item);
    /*!
     * Radio list item has clicked.
     */
    void addListWidgetItem();
    /*!
     * Add radio list into list widget.
     */

protected:
    QNetworkCookieJar *m_cookJar;
    MusicRadioChannelThread *m_getChannelThread;
    MusicWebMusicRadioWidget *m_musicRadio;

};

#endif // MUSICWEBMUSICRADIOLISTVIEW_H
