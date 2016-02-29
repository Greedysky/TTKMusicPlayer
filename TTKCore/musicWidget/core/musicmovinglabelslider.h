#ifndef MUSICMOVINGLABELSLIDER_H
#define MUSICMOVINGLABELSLIDER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QSlider>
#include <QLabel>
#include "musicglobaldefine.h"

/*! @brief The class of the slider that can show such as tooltip.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMovingLabelSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MusicMovingLabelSlider(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    explicit MusicMovingLabelSlider(Qt::Orientation orientation,
                                    QWidget *parent = 0);
    /*!
     * Object contsructor by slider orientation.
     */
    ~MusicMovingLabelSlider();

    inline bool isMoving() const { return m_isMoving;}
    /*!
     * Slider is now moving or not.
     */
    inline void setMoving(bool m) { m_isMoving = m;}
    /*!
     * Set slider moving or not.
     */

Q_SIGNALS:
    void sliderReleasedAt(int pos);
    /*!
     * Slider handle release at pos emit.
     */

public Q_SLOTS:
    void setValue(int value);
    /*!
     * Set slider current value.
     */

private Q_SLOTS:
    void sliderMovedChanged();
    /*!
     * Slider start to move or stop to move just.
     */
    void sliderReleasedChanged();
    /*!
     * Slider stop to move just.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */
    QPoint limitLableGeometry(int x, int y, int z);
    /*!
     * Set the tool tip gemetry.
     */

    bool m_isMoving;
    QLabel *m_textLabel;
    Qt::Orientation m_orientation;

};

#endif // MUSICMOVINGLABELSLIDER_H
