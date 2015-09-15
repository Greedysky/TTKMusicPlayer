#ifndef MUSICDESKTOPWALLPAPERWIDGET_H
#define MUSICDESKTOPWALLPAPERWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"

class MusicDesktopWallpaperThread;

namespace Ui {
class MusicDesktopWallpaperWidget;
}

class MUSIC_TOOLSET_EXPORT MusicDesktopWallpaperWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicDesktopWallpaperWidget(QWidget *parent = 0);
    ~MusicDesktopWallpaperWidget();

signals:
public slots:
    void netRadioButtonPressed();
    void localRadioButtonPressed();
    void playRadioButtonPressed();
    void viewButtonPressed();
    void confirmButtonPressed();
    void stopButtonPressed();
    void cancelButtonPressed();
    void parameterFinished();
    void show();

protected:
    void initWidgetStyle() const;
    void initParameters() const;
    void findFiles(const QString &path);
    void setAutoStart(bool autoStart) const;

    Ui::MusicDesktopWallpaperWidget *ui;
    MusicDesktopWallpaperThread *m_wallThread;
    QStringList m_path;
    int m_currentMode;

};

#endif // MUSICDESKTOPWALLPAPERWIDGET_H
