/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#ifndef FLOWETHEREALITY_H
#define FLOWETHEREALITY_H

#include <qmmp/visual.h>

class QTimer;
class QHideEvent;
class QShowEvent;
class Ethereality;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FlowEthereality : public Visual
{
    Q_OBJECT
public:
    explicit FlowEthereality(QWidget *parent = nullptr);
    virtual ~FlowEthereality();

private:
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void paintEvent(QPaintEvent *e) override;

    virtual void process(float *left, float *right) override;
    virtual void processPatch(bool state) override;

    int m_pos_x = 0, m_pos_y = 0;
    QList<Ethereality*> m_etherealitys;

};

#endif
