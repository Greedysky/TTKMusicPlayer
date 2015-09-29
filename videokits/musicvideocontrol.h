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
    explicit MusicVideoControl(bool popup, QWidget *parent = 0);
    ~MusicVideoControl();

    void setValue(qint64 position) const;
    void durationChanged(qint64 duration) const;
    void setButtonStyle(bool style) const;

signals:
    void musicVideoSetPopup(bool popup);
    void musicVideoFullscreen(bool full);

private slots:
    void inSideButtonClicked();
    void fullButtonClicked();

protected:
    QMenu m_popupVolumn;
    QWidgetAction *m_widgetAction;
    QSlider *m_timeSlider, *m_volumnSlider;
    QToolButton *m_menuButton, *m_volumnButton;
    QPushButton *m_playButton;
    QPushButton *m_inSideButton, *m_fullButton;
    bool m_widgetPopup;

};

#endif // MUSICVIDEOCONTROL_H
