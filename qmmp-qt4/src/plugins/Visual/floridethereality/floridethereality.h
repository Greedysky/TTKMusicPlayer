/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#ifndef FLORIDETHEREALITY_H
#define FLORIDETHEREALITY_H

#include <qmmp/florid.h>

class QTimer;
class QHideEvent;
class QShowEvent;
class Ethereality;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FloridEthereality : public Florid
{
    Q_OBJECT
public:
    explicit FloridEthereality(QWidget *parent = nullptr);
    virtual ~FloridEthereality();

public Q_SLOTS:
    virtual void start() override;
    virtual void stop() override;

private Q_SLOTS:
    void timeout();

private:
    void clear();
    virtual void hideEvent(QHideEvent *e) override;
    virtual void showEvent(QShowEvent *e) override;

    void process();

    int m_pos_x;
    int m_pos_y;
    QTimer *m_timer;
    int *m_intern_vis_data;
    float m_left_buffer[QMMP_VISUAL_NODE_SIZE];
    float m_right_buffer[QMMP_VISUAL_NODE_SIZE];
    int m_cols, m_rows;
    bool m_running;
    QList<Ethereality*> m_etherealitys;

};

#endif
