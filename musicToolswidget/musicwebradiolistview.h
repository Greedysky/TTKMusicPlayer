#ifndef MUSICWEBRADIOLISTVIEW_H
#define MUSICWEBRADIOLISTVIEW_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QListWidget>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class MusicWebRadioWidget;

class MUSIC_TOOL_EXPORT MusicWebRadioListView : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicWebRadioListView(QWidget *parent = 0);
    ~MusicWebRadioListView();

signals:

public slots:
    void itemHasClicked(QListWidgetItem *item);

protected:
    void setTransparent(int angle);
    void clearAllItems();
    void addListWidgetItem();

    MusicWebRadioWidget *m_radioDialog;

};

#endif // MUSICWEBRADIOLISTVIEW_H
