#ifndef MUSICTOOLSETSWIDGET_H
#define MUSICTOOLSETSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QListWidget>
#include "musicuiobject.h"
#include "musicglobaldefine.h"

class QProcess;

class MUSIC_TOOL_EXPORT MusicToolSetsWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicToolSetsWidget(QWidget *parent = 0);
    ~MusicToolSetsWidget();

Q_SIGNALS:
    void getCurrentPlayList(QStringList &list);

public Q_SLOTS:
    void itemHasClicked(QListWidgetItem *item);
    void addListWidgetItem();

protected:
    void clearAllItems();

    QProcess *m_process;

};

#endif // MUSICTOOLSETSWIDGET_H
