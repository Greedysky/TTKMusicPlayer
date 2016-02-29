#ifndef MUSICSPEEDMETER_H
#define MUSICSPEEDMETER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#define S_LONG 10
#define S_OK 7
#define S_SHORT 5
#define S_SPACE 3
#define S_ANGLE 10

#include <QtGui>
#include <QWidget>
#include "musicglobaldefine.h"

class MUSIC_TOOLSET_EXPORT MusicSpeedMeter : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSpeedMeter(QWidget *parent = 0);

    void setValue(qreal value);
    qreal value() const { return m_value;}
    void setRatio(qreal value);
    qreal ratio() const { return m_ratio;}
    void setAnimating(bool enable);
    bool isAnimating() const { return m_bAnimating;}

private Q_SLOTS:
    void updateGraph();

protected:
    virtual void paintEvent(QPaintEvent *event);
    /*!
     * Override the widget event.
     */
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

private:
    void initVariables();
    void resetVariables();
    void drawOuterCircle(QPainter* painter);
    void drawInnerCircle(QPainter* painter);
    void drawMark(QPainter* painter);
    void drawCoverCircle(QPainter* painter);
    void drawColorPie(QPainter* painter);
    void drawIndicator(QPainter* painter);
    void drawCoverBall(QPainter* painter);
    void drawTextRect(QPainter* painter);

    qreal m_outerRadius, m_innerRadius;
    qreal m_coverCircleRadius, m_colorCircleRadius;
    qreal m_coverBallRadius;
    QPointF m_center;
    QRectF m_colorCircleRect;
    qreal m_value, m_ratio, m_currentValue;
    QTimer* updateTimer;
    bool m_bReverse;
    QTimer* singleTimer;
    bool m_bAnimating;

};

#endif // MUSICSPEEDMETER_H
