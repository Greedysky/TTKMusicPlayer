#ifndef MUSICTIMERSLIDERWIDGET_H
#define MUSICTIMERSLIDERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QMouseEvent>
#include "musicobject.h"
#include "musicglobaldefine.h"

class MusicGifLabelWidget;
class MusicMovingLabelSlider;

/*! @brief The class of the timer slider widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicTimerSliderWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTimerSliderWidget(QWidget *parent = 0);

    ~MusicTimerSliderWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current play state.
     */
    void setPlayState(bool state);
    /*!
     * Set current play index value.
     */
    void setValue(qint64 value) const;
    /*!
     * Set current play range from min and max.
     */
    void setRange(int min, int max);
    /*!
     * Set connect current object.
     */
    void setObject(QObject *object) const;

public Q_SLOTS:
    /*!
     * Current slider move at pos.
     */
    void sliderMovedAt(int pos) const;
    /*!
     * Set slider style diff by diff enhanced effect.
     */
    void setSliderStyleByType(int type);

protected:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;

    MusicGifLabelWidget *m_label;
    MusicMovingLabelSlider *m_slider;

};

#endif // MUSICTIMERSLIDERWIDGET_H
