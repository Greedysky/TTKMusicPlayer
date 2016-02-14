#ifndef MUSICLOCALSONGSTABLEWIDGET_H
#define MUSICLOCALSONGSTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstracttablewidget.h"

class MUSIC_WIDGET_EXPORT MusicLocalSongsTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongsTableWidget(QWidget *parent = 0);

    void clearShowlist();
    void clearShowPath();
    void createShowlist();
    void createShowPath();

public Q_SLOTS:
    virtual void listCellClicked(int, int) override {}

};

#endif // MUSICLOCALSONGSTABLEWIDGET_H
