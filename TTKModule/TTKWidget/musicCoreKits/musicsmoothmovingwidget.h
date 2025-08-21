#ifndef MUSICSMOOTHMOVINGWIDGET_H
#define MUSICSMOOTHMOVINGWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#include <QScrollArea>
#include "musicabstracttablewidget.h"

class QPropertyAnimation;

/*! @brief The class of the slider smooth moving interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSmoothMovingInterface : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSmoothMovingInterface(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSmoothMovingInterface();

    /*!
     * Set current moved scroll bar.
     */
    void setScrollBar(QScrollBar *bar);
    /*!
     * Get current moved scroll bar.
     */
    inline QScrollBar* scrollBar() const noexcept { return m_scrollBar; }

    /*!
     * Update current scroll bar value.
     */
    void updateValue(int value);

public Q_SLOTS:
    /*!
     * Time out to start animation.
     */
    void timeToAnimation();
    /*!
     * Current scroll bar value changed.
     */
    void valueChanged(int value);

private:
    bool m_initialized;
    int m_previousValue, m_deltaValue;
    QScrollBar *m_scrollBar;
    QTimer *m_animationTimer;
    QPropertyAnimation *m_moveAnimation;

};




/*! @brief The class of the slider smooth moving table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSmoothMovingTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSmoothMovingTableWidget(QWidget *parent = nullptr);

    /*!
     * Set current moved scroll bar.
     */
    void setMovedScrollBar(QScrollBar *bar = nullptr);

protected:
    /*!
     * Override the widget event.
     */
    virtual void wheelEvent(QWheelEvent *event) override;

    /*!
     * Get current moved scroll bar.
     */
    inline QScrollBar* scrollBar() const noexcept { return m_interface.scrollBar(); }

private:
    MusicSmoothMovingInterface m_interface;

};


/*! @brief The class of the slider smooth moving scroll area widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSmoothMovingScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSmoothMovingScrollArea(QWidget *parent = nullptr);

    /*!
     * Set current moved scroll bar.
     */
    void setMovedScrollBar(QScrollBar *bar = nullptr);

protected:
    /*!
     * Override the widget event.
     */
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    MusicSmoothMovingInterface m_interface;

};

#endif // MUSICSMOOTHMOVINGWIDGET_H
