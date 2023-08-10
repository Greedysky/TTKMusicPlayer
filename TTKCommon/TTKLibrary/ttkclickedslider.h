#ifndef TTKCLICKEDSLIDER_H
#define TTKCLICKEDSLIDER_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QSlider>
#include "ttkmoduleexport.h"

/*! @brief The class of the slider that can clicked any position.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKClickedSlider : public QSlider
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKClickedSlider)
public:
    /*!
     * Object constructor.
     */
    explicit TTKClickedSlider(QWidget *parent = nullptr);
    explicit TTKClickedSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

Q_SIGNALS:
    /*!
     * Click the item.
     */
    void clicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual bool event(QEvent *event) override final;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    int m_value;
    bool m_mousePress;

};

#endif // TTKCLICKEDSLIDER_H
