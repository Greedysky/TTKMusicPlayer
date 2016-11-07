#ifndef MUSICSONGSPLAYEDLISTWIDGET_H
#define MUSICSONGSPLAYEDLISTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the songs played list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsPlayedListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsPlayedListWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongsPlayedListWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:

public Q_SLOTS:

protected:

};

#endif // MUSICSONGSPLAYEDLISTWIDGET_H
