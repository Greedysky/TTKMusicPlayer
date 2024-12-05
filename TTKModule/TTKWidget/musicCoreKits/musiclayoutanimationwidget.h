#ifndef MUSICLAYOUTANIMATIONWIDGET_H
#define MUSICLAYOUTANIMATIONWIDGET_H

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

#include <QWidget>
#include "musicglobaldefine.h"

class QBoxLayout;
class QPropertyAnimation;

/*! @brief The class of the layout float animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLayoutAnimationWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLayoutAnimationWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLayoutAnimationWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLayoutAnimationWidget();

    /*!
     * Start to anmiation.
     */
    virtual void start();
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
    void addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
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
    bool m_isAnimating;
    float m_currentValue;
    QWidget *m_mainWidget;
    QBoxLayout *m_widgetLayout;
    QPropertyAnimation *m_animation;

};


/*! @brief The class of the vertical layout float animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicVLayoutAnimationWidget : public MusicLayoutAnimationWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVLayoutAnimationWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicVLayoutAnimationWidget(QWidget *parent = nullptr);

    /*!
     * Start to anmiation.
     */
    virtual void start() override final;

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

};


/*! @brief The class of the horizontal layout float animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicHLayoutAnimationWidget : public MusicLayoutAnimationWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicHLayoutAnimationWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicHLayoutAnimationWidget(QWidget *parent = nullptr);

    /*!
     * Start to anmiation.
     */
    virtual void start() override final;

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

};

#endif // MUSICLAYOUTANIMATIONWIDGET_H
