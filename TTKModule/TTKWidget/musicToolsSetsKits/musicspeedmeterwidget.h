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
    explicit MusicSpeedMeterWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setValue(qreal value);
    /*!
     * Set meter value.
     */
    qreal value() const { return m_value;}
    /*!
     * Get meter value.
     */
    void setRatio(qreal value);
    /*!
     * Set meter ratio.
     */
    qreal ratio() const { return m_ratio;}
    /*!
     * Get meter ratio.
     */
    void setAnimating(bool enable);
    /*!
     * Set meter animation.
     */
    bool isAnimating() const { return m_bAnimating;}
    /*!
     * Get meter animation.
     */

private Q_SLOTS:
    void updateGraph();
    /*!
     * Update graph path.
     */

protected:
    virtual void paintEvent(QPaintEvent *event);
    /*!
     * Override the widget event.
     */
    QSize sizeHint() const;
    /*!
     * Override the sizeHint function.
     */
    QSize minimumSizeHint() const;
    /*!
     * Override the minimumSizeHint function.
     */

private:
    void initVariables();
    /*!
     * Init variablesn.
     */
    void resetVariables();
    /*!
     * Reset variables.
     */
    void drawOuterCircle(QPainter* painter);
    /*!
     * Draw outer circle.
     */
    void drawInnerCircle(QPainter* painter);
    /*!
     * Draw inner circle.
     */
    void drawMark(QPainter* painter);
    /*!
     * Draw mark.
     */
    void drawCoverCircle(QPainter* painter);
    /*!
     * Draw cover circle.
     */
    void drawColorPie(QPainter* painter);
    /*!
     * Draw color pie.
     */
    void drawIndicator(QPainter* painter);
    /*!
     * Draw indicator.
     */
    void drawCoverBall(QPainter* painter);
    /*!
     * Draw cover ball.
     */
    void drawTextRect(QPainter* painter);
    /*!
     * Draw text rect.
     */

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
