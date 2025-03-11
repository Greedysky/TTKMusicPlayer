#ifndef MUSICROUNDANIMATIONLABEL_H
#define MUSICROUNDANIMATIONLABEL_H

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

#include <QTimer>
#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the round animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRoundAnimationLabel : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRoundAnimationLabel)
public:
    /*!
     * Object constructor.
     */
    explicit MusicRoundAnimationLabel(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicRoundAnimationLabel();

    /*!
     * Set rotating widget background pixmap.
     */
    void setPixmap(const QPixmap &pix) noexcept;
    /*!
     * Set rotating widget interval.
     */
    void setInterval(int value);

public Q_SLOTS:
    /*!
     * Start to rotating widget.
     */
    void start();
    /*!
     * Stop to rotating widget.
     */
    void stop();

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    QPixmap m_pixmap;
    QTimer m_timer;
    int m_rotateAngle;

};

#endif // MUSICROUNDANIMATIONLABEL_H
