/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#ifndef FLORIDAUTISM_H
#define FLORIDAUTISM_H

#include <qmmp/florid.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AutismLabel : public QWidget
{
    Q_OBJECT
public:
    explicit AutismLabel(QWidget *parent = nullptr);
    virtual ~AutismLabel();

    void start(const QPoint &pos);
    void setColor(const QColor &color);

protected slots:
    void updateRender();

protected:
    virtual void paintEvent(QPaintEvent *e) override final;

    bool m_enabled = false;
    QTimer *m_circleTimer;
    int m_crValue = LABEL_RADIUS;
    QPoint m_offset;
    QColor m_color;

};

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FloridAutism : public Florid
{
    Q_OBJECT
public:
    explicit FloridAutism(QWidget *parent = nullptr);
    virtual ~FloridAutism();

private:
    virtual void resizeEvent(QResizeEvent *e) override final;

    virtual void process(float *left, float *right) override final;

    int m_index = 0;
    QList<AutismLabel*> m_labels;

};

#endif
