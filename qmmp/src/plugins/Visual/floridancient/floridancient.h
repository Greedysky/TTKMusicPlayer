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

#ifndef FLORIDANCIENT_H
#define FLORIDANCIENT_H

#include <qmmp/florid.h>

class QTimer;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;
class QPropertyAnimation;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AncientLabel : public QWidget
{
    Q_OBJECT
public:
    explicit AncientLabel(QWidget *parent = nullptr);
    virtual ~AncientLabel();

    void start();
    void setColor(const QColor &color);

protected Q_SLOTS:
    void timeout();
    void finished();
    void posValueChanged(const QVariant &value);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    float m_opacity;
    int m_size;
    QPoint m_pos;
    QColor m_color;
    QPropertyAnimation *m_posAnimation;

};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class FloridAncient : public Florid
{
    Q_OBJECT
public:
    explicit FloridAncient(QWidget *parent = nullptr);
    virtual ~FloridAncient();

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
    virtual void resizeEvent(QResizeEvent *e) override;

    void process();
    void draw(QPainter *p);

    QTimer *m_timer;
    double *m_intern_vis_data;
    double m_analyzer_falloff;
    float m_left_buffer[QMMP_VISUAL_NODE_SIZE];
    float m_right_buffer[QMMP_VISUAL_NODE_SIZE];
    int *m_x_scale, m_cols, m_rows;
    bool m_running;

    QSize m_cell_size;
    QList<AncientLabel*> m_labels;

};

#endif
