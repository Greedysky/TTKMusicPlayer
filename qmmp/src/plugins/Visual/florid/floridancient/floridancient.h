/***************************************************************************
 * This file is part of the TTK qmmp plugin project
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

#ifndef FLORIDANCIENT_H
#define FLORIDANCIENT_H

#include <qmmp/florid.h>

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

protected slots:
    void timeout();
    void finished();
    void posValueChanged(const QVariant &value);

protected:
    virtual void paintEvent(QPaintEvent *e) override final;

    float m_opacity = 1;
    int m_size = 5;
    QPoint m_offset = QPoint(0, 0);
    QColor m_color = QColor(255, 255, 255);
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
    virtual void start() override final;
    virtual void stop() override final;

private:
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void resizeEvent(QResizeEvent *e) override final;

    virtual void process(float *left, float *right) override final;

    int *m_xscale = nullptr;
    const double m_analyzerSize = 1.2;
    const QSize m_cellSize = QSize(6, 2);
    QList<AncientLabel*> m_labels;

};

#endif
