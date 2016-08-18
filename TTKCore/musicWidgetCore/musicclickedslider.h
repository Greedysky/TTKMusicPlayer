#ifndef MUSICCLICKEDSLIDER_H
#define MUSICCLICKEDSLIDER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QSlider>
#include "musicglobaldefine.h"

/*! @brief The class of the slider that can clicked any position.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicClickedSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MusicClickedSlider(QWidget *parent = 0);
    explicit MusicClickedSlider(Qt::Orientation orientation, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

protected:
    virtual bool event(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    /*!
     * Override the widget event.
     */

protected:
    int m_value;
    bool m_mousePress;

};

#endif // MUSICCLICKEDSLIDER_H
