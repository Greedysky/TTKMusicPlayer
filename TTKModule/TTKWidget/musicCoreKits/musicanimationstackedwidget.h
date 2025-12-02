#ifndef MUSICANIMATIONSTACKEDWIDGET_H
#define MUSICANIMATIONSTACKEDWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include <QStackedWidget>
#include <QPropertyAnimation>
#include "musicglobaldefine.h"

/*! @brief The class of the statckwidget switch animation.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    enum class Module
    {
        LeftToRight,    /*!< Animation from left to right */
        RightToLeft,    /*!< Animation from right to left */
        TopToBottom,    /*!< Animation from top to bottom */
        BottomToTop     /*!< Animation from bottom to top */
    };

    /*!
     * Object constructor.
     */
    explicit MusicAnimationStackedWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAnimationStackedWidget();

    /*!
     * Start to anmiation by index.
     */
    void start(int index);
    /*!
     * Set previoud and current index.
     */
    void setIndex(int previous, int current);
    /*!
     * Set animation start and end pos distance and animation type.
     */
    void setLength(int length, Module type);
    /*!
     * Get previoud index.
     */
    int previousIndex() const noexcept;
    /*!
     * Get current index.
     */
    int currentIndex() const noexcept;

    /*!
     * Set animation duration.
     */
    void setDuration(int duration);
    /*!
     * Get animation duration.
     */
    int duration() const;

private Q_SLOTS:
    /*!
     * Animation value changed.
     */
    void valueChanged(const QVariant &value);
    /*!
     * Animation finished.
     */
    void animationFinished();

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;
    /*!
     * Render previous widget.
     */
    void renderPreviousWidget(QPainter *painter);
    /*!
     * Render current widget.
     */
    void renderCurrentWidget(QPainter *painter);

    bool m_isAnimating;
    float m_currentValue;
    int m_currentIndex, m_previousIndex;
    Module m_type;
    QPropertyAnimation *m_animation;

};

#endif // MUSICANIMATIONSTACKEDWIDGET_H
