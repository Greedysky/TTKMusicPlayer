/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

class CircleClickLabel : public QWidget
{
    Q_OBJECT
public:
    explicit CircleClickLabel(QWidget *parent = nullptr);
    virtual ~CircleClickLabel();

    void start(const QPoint &pos);

protected Q_SLOTS:
    void updateRender();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    bool m_circleOn;
    QTimer *m_circleTimer;
    int m_crValue;
    QPoint m_pos;

};

class FloridAutism : public Florid
{
    Q_OBJECT
public:
    FloridAutism(QWidget *parent = nullptr);
    virtual ~FloridAutism();

public slots:
    virtual void start() override;
    virtual void stop() override;

private slots:
    void timeout();

private:
    void clear();
    virtual void hideEvent(QHideEvent *e) override;
    virtual void showEvent(QShowEvent *e) override;
    virtual void paintEvent(QPaintEvent *) override;

    void draw(QPainter *p);

    QTimer *m_timer;
    bool m_running;
    QList<CircleClickLabel*> m_labels;

};

#endif
