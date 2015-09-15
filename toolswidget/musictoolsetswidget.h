#ifndef MUSICTOOLSETSWIDGET_H
#define MUSICTOOLSETSWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QListWidget>
#include "musicuiobject.h"
#include "musiclibexportglobal.h"

class QProcess;
class MusicSpectrumWidget;
class MusicDesktopWallpaperWidget;

class MUSIC_TOOL_EXPORT MusicToolSetsWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicToolSetsWidget(QWidget *parent = 0);
    ~MusicToolSetsWidget();

signals:
    void getCurrentPlayList(QStringList &list);

public slots:
    void itemHasClicked(QListWidgetItem *item);

protected:
    void setTransparent(int angle);
    void clearAllItems();
    void addListWidgetItem();

    MusicSpectrumWidget *m_musicSpectrumWidget;
    MusicDesktopWallpaperWidget *m_wallpaper;
    QProcess *m_process;

};

#endif // MUSICTOOLSETSWIDGET_H
