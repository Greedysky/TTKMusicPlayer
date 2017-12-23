#ifndef MUSICLAYOUTANIMATIONWIDGET_H
#define MUSICLAYOUTANIMATIONWIDGET_H

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

#include <QWidget>
#include "musicglobaldefine.h"

class QVBoxLayout;
class QPropertyAnimation;

/*! @brief The class of the layout float animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicLayoutAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLayoutAnimationWidget(QWidget *parent = 0);

    ~MusicLayoutAnimationWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to anmiation.
     */
    void start();
    /*!
     * Stop to anmiation.
     */
    void stop();

    /*!
     * Connect to parent.
     */
    void connectTo(QObject *parent);
    /*!
     * Add widget layout stretch.
     */
    void addStretch(int stretch = 0);
    /*!
     * Add widget to container.
     */
    void addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = 0);
    /*!
     * Remove widget to container.
     */
    void removeWidget(QWidget *widget);

private Q_SLOTS:
    /*!
     * Animation value changed.
     */
    void valueChanged(const QVariant &value);
    /*!
     * Animation finished.
     */
    void animationFinished();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    bool m_isAnimating;
    float m_currentValue;
    QWidget *m_mainWidget;
    QVBoxLayout *m_widgetLayout;
    QPropertyAnimation *m_animation;

};

#endif // MUSICLAYOUTANIMATIONWIDGET_H
