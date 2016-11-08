#ifndef MUSICSONGSPLAYEDCONTAINWIDGET_H
#define MUSICSONGSPLAYEDCONTAINWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class MusicSongsPlayedListWidget;

/*! @brief The class of the songs played Contain widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsPlayedContainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsPlayedContainWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongsPlayedContainWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:

public Q_SLOTS:

protected:
    MusicSongsPlayedListWidget *m_playedListWidget;

};

#endif // MUSICSONGSPLAYEDCONTAINWIDGET_H
