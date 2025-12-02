/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#ifndef FLORIDELECTRIC_H
#define FLORIDELECTRIC_H

#include <qmmp/florid.h>

class QPropertyAnimation;

#define POINT_SIZE          8
#define ANIMATION_SIZE      20
#define ANIMATION_DURATION  10000

/*!
 * @author Greedysky <greedysky@163.com>
 */
class ElectricPointLabel : public QWidget
{
    Q_OBJECT
public:
    explicit ElectricPointLabel(QWidget *parent = nullptr);
    virtual ~ElectricPointLabel();

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
    QColor m_color = QColor(255, 255, 255, 0);
    QPropertyAnimation *m_posAnimation;

};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class ElectricCircleLabel : public QWidget
{
    Q_OBJECT
public:
    explicit ElectricCircleLabel(QWidget *parent = nullptr);
    virtual ~ElectricCircleLabel();

    void start();
    void setColor(const QColor &color);

protected slots:
    void timeout();
    void finished();
    void sizeValueChanged(const QVariant &value);

protected:
    virtual void paintEvent(QPaintEvent *e) override final;

    float m_opacity = 1;
    int m_size = POINT_SIZE;
    QColor m_color = QColor(255, 255, 255, 0);
    QPropertyAnimation *m_posAnimation;

};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class ElectricLabel
{
public:
    explicit ElectricLabel(QWidget *parent = nullptr);
    virtual ~ElectricLabel();

    void start();
    void setColor(const QColor &color);
    void resizeWindow(const QRect &rect);

protected:
    QList<ElectricPointLabel*> m_pointLabels;
    QList<ElectricCircleLabel*> m_circleLabels;

};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class FloridElectric : public Florid
{
    Q_OBJECT
public:
    explicit FloridElectric(QWidget *parent = nullptr);

public slots:
    virtual void start() override final;
    virtual void stop() override final;

private:
    virtual void resizeEvent(QResizeEvent *e) override final;

    virtual void processData(float *left, float *right) override final;

    ElectricLabel *m_label;

};

#endif
