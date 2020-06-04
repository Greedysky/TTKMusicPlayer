/* =================================================
 * This file is part of the TTK qmmp plugin project
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

#ifndef FLOWETHEREALITY_H
#define FLOWETHEREALITY_H

#include <qmmp/florid.h>

class QTimer;
class QHideEvent;
class QShowEvent;
class Ethereality;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FlowEthereality : public Florid
{
    Q_OBJECT
public:
    explicit FlowEthereality(QWidget *parent = nullptr);
    virtual ~FlowEthereality();

public slots:
    virtual void start() override;
    virtual void stop() override;

private:
    virtual void hideEvent(QHideEvent *e) override;
    virtual void showEvent(QShowEvent *e) override;
    virtual void resizeEvent(QResizeEvent *e) override;

    virtual void process(float *left, float *right) override;

    int m_pos_x, m_pos_y;
    QList<Ethereality*> m_etherealitys;

};

#endif
