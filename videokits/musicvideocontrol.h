#ifndef MUSICVIDEOCONTROL_H
#define MUSICVIDEOCONTROL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QWidget>
#include "musicuiobject.h"
#include "musiclibexportglobal.h"

class QSlider;
class QToolButton;
class QPushButton;
class QWidgetAction;

class MUSIC_VIDEO_EXPORT MusicVideoControl : public QWidget
{
    Q_OBJECT
public:
    explicit MusicVideoControl(QWidget *parent = 0);
    ~MusicVideoControl();

    void setValue(qint64 position) const;
    void durationChanged(qint64 duration) const;
    void setButtonStyle(bool style) const;

protected:
    QMenu m_popupVolumn;
    QWidgetAction *m_widgetAction;
    QSlider *m_timeSlider;
    QSlider *m_volumnSlider;
    QToolButton *m_menuButton;
    QPushButton *m_playButton;
    QToolButton *m_volumnButton;

};

#endif // MUSICVIDEOCONTROL_H
