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

#ifndef FLORIDELECTRIC_H
#define FLORIDELECTRIC_H

#include <qmmp/florid.h>

class QPropertyAnimation;

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
    virtual void paintEvent(QPaintEvent *event) override;

    float m_opacity;
    int m_size;
    QColor m_color;
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
    virtual ~FloridElectric();

public slots:
    virtual void start() override;
    virtual void stop() override;

private:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void resizeEvent(QResizeEvent *e) override;

    virtual void process(float *left, float *right) override;

    ElectricLabel* m_label;

};

#endif
