#ifndef MUSICRATINGLABEL_H
#define MUSICRATINGLABEL_H

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

#include <QLabel>
#include <qmath.h>
#include "ttkmoduleexport.h"

/*! @brief The class of the rating label.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRatingLabel : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRatingLabel)
public:
    /*!
     * Object constructor.
     */
    explicit MusicRatingLabel(QWidget *parent = nullptr);
    explicit MusicRatingLabel(int value, QWidget *parent = nullptr);

    /*!
     * Set input value (0 - 5).
     */
    void setValue(int value);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    int m_value;

};

#endif // MUSICRATINGLABEL_H
