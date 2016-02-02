#ifndef MUSICWEBRADIOLISTVIEW_H
#define MUSICWEBRADIOLISTVIEW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QListWidget>
#include "musicobject.h"
#include "musicglobaldefine.h"

class MusicWebRadioWidget;

class MUSIC_TOOL_EXPORT MusicWebRadioListView : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicWebRadioListView(QWidget *parent = 0);
    ~MusicWebRadioListView();

Q_SIGNALS:

public Q_SLOTS:
    void itemHasClicked(QListWidgetItem *item);

protected:
    void setTransparent(int angle);
    void clearAllItems();
    void addListWidgetItem();

    MusicWebRadioWidget *m_radioDialog;

};

#endif // MUSICWEBRADIOLISTVIEW_H
