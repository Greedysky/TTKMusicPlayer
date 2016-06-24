#ifndef MUSICTIMERSLIDERWIDGET_H
#define MUSICTIMERSLIDERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include "musicobject.h"
#include "musicglobaldefine.h"

class QLabel;
class MusicMovingLabelSlider;

/*! @brief The class of the timer slider widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicTimerSliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicTimerSliderWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicTimerSliderWidget();

    void setPlayState(bool state);
    /*!
     * Set current play state.
     */
    void setValue(qint64 value) const;
    /*!
     * Set current play index value.
     */
    void setRange(int min, int max);
    /*!
     * Set current play range from min and max.
     */
    void setObject(QObject *object) const;
    /*!
     * Set connect current object.
     */

public Q_SLOTS:
    void sliderMovedAt(int pos) const;
    /*!
     * Current slider move at pos.
     */
    void setSliderStyleByType(int type);
    /*!
     * Set slider style diff by diff enhanced effect.
     */
    void timeout();
    /*!
     * To change the pic index that show animation.
     */

protected:
    int m_picIndex;
    bool m_reverse;
    QLabel *m_label;
    QTimer m_timer;
    MusicMovingLabelSlider *m_slider;

};

#endif // MUSICTIMERSLIDERWIDGET_H
