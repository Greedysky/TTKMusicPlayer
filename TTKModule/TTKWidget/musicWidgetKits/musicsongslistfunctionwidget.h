#ifndef MUSICSONGSLISTFUNCTIONWIDGET_H
#define MUSICSONGSLISTFUNCTIONWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QTimer>
#include <QLabel>
#include "musicglobaldefine.h"

class QPropertyAnimation;

/*! @brief The class of the songs list function widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsListFunctionWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongsListFunctionWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsListFunctionWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsListFunctionWidget();

    /*!
     * Set current animation active.
     */
    void active();

Q_SIGNALS:
    /*!
     * Delete self object.
     */
    void deleteObject();

private Q_SLOTS:
    /*!
     * Leave Time out.
     */
    void leaveTimeout();
    /*!
     * Animation finished.
     */
    void animationFinished();

private:
    /*!
     * Start to turn on animation.
     */
    void start(bool play, int end);
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;

    QTimer m_timer;
    QWidget *m_mainWidget;
    QPropertyAnimation *m_animation;
    float m_currentAnimationValue;

};

#endif // MUSICSONGSLISTFUNCTIONWIDGET_H
