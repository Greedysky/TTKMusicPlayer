#ifndef MUSICSPEEDMETERWIDGET_H
#define MUSICSPEEDMETERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#define S_LONG  10
#define S_SPAOK 7
#define S_SHORT 5
#define S_SPACE 3
#define S_ANGLE 10

#include <QtGui>
#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the speed meter widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpeedMeterWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSpeedMeterWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set meter value.
     */
    void setValue(qreal value);
    /*!
     * Get meter value.
     */
    qreal value() const { return m_value;}
    /*!
     * Set meter ratio.
     */
    void setRatio(qreal value);
    /*!
     * Get meter ratio.
     */
    qreal ratio() const { return m_ratio;}
    /*!
     * Set meter animation.
     */
    void setAnimating(bool enable);
    /*!
     * Get meter animation.
     */
    bool isAnimating() const { return m_bAnimating;}

private Q_SLOTS:
    /*!
     * Update graph path.
     */
    void updateGraph();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event);
    /*!
     * Override the sizeHint function.
     */
    QSize sizeHint() const;
    /*!
     * Override the minimumSizeHint function.
     */
    QSize minimumSizeHint() const;

private:
    /*!
     * Init variablesn.
     */
    void initVariables();
    /*!
     * Reset variables.
     */
    void resetVariables();
    /*!
     * Draw outer circle.
     */
    void drawOuterCircle(QPainter* painter);
    /*!
     * Draw inner circle.
     */
    void drawInnerCircle(QPainter* painter);
    /*!
     * Draw mark.
     */
    void drawMark(QPainter* painter);
    /*!
     * Draw cover circle.
     */
    void drawCoverCircle(QPainter* painter);
    /*!
     * Draw color pie.
     */
    void drawColorPie(QPainter* painter);
    /*!
     * Draw indicator.
     */
    void drawIndicator(QPainter* painter);
    /*!
     * Draw cover ball.
     */
    void drawCoverBall(QPainter* painter);
    /*!
     * Draw text rect.
     */
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

#endif // MUSICSPEEDMETERWIDGET_H
