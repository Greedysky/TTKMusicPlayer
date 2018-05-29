#ifndef MUSICSMOOTHMOVINGTABLEWIDGET_H
#define MUSICSMOOTHMOVINGTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicabstracttablewidget.h"

class QPropertyAnimation;

/*! @brief The class of the slider smooth moving table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSmoothMovingTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSmoothMovingTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSmoothMovingTableWidget(QWidget *parent = 0);

    virtual ~MusicSmoothMovingTableWidget();

    /*!
     * Set current moved scroll bar.
     */
    void setMovedScrollBar(QScrollBar *bar);

public Q_SLOTS:
    /*!
     * Time out to start animation.
     */
    void timeToAnimation();
    /*!
     * Current scroll bar value changed.
     */
    void valueChanged(int value);

protected:
    /*!
     * Override the widget event.
     */
    virtual void wheelEvent(QWheelEvent *event) override;

    bool m_isFirstInit;
    int m_previousValue, m_deltaValue;
    QScrollBar *m_scrollBar;
    QTimer *m_animationTimer;
    QPropertyAnimation *m_slowAnimation;

};

#endif // MUSICSMOOTHMOVINGTABLEWIDGET_H
