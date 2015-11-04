#ifndef MUSICTIMERSLIDERWIDGET_H
#define MUSICTIMERSLIDERWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class QSlider;
class QLabel;

class MUSIC_WIDGET_EXPORT MusicTimerSliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicTimerSliderWidget(QWidget *parent = 0);
    ~MusicTimerSliderWidget();

    void setPlayState(bool state);
    void setValue(qint64 value) const;
    void setRange(int min, int max);
    void setObject(QObject *object) const;

signals:

public slots:
    void sliderMovedAt(int pos) const;
    void setSliderStyleByType(int type);
    void timeout();

protected:
    QLabel *m_label;
    QSlider *m_slider;
    QTimer m_timer;
    qint64 m_duration;
    int m_picIndex;
    bool m_reverse;
};

#endif // MUSICTIMERSLIDERWIDGET_H
