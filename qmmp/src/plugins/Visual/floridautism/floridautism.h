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

#ifndef FLORIDAUTISM_H
#define FLORIDAUTISM_H

#include <qmmp/florid.h>

class QTimer;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;

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

protected Q_SLOTS:
    void updateRender();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    bool m_circleOn;
    QTimer *m_circleTimer;
    int m_crValue;
    QPoint m_pos;
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

public slots:
    virtual void start() override;
    virtual void stop() override;

private slots:
    void timeout();

private:
    virtual void hideEvent(QHideEvent *e) override;
    virtual void showEvent(QShowEvent *e) override;
    virtual void paintEvent(QPaintEvent *) override;
    virtual void resizeEvent(QResizeEvent *e) override;

    int m_index;
    QTimer *m_timer;
    bool m_running;
    float m_left_buffer[QMMP_VISUAL_NODE_SIZE];
    float m_right_buffer[QMMP_VISUAL_NODE_SIZE];
    QList<AutismLabel*> m_labels;

};

#endif
