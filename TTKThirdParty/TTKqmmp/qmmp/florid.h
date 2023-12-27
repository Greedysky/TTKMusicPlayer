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

#ifndef FLORID_H
#define FLORID_H

#include <QTimer>
#include "qmmp.h"
#include "visual.h"

#define LABEL_RADIUS    100

/*! @brief Helper class to round animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT RoundAnimationLabel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit RoundAnimationLabel(QWidget *parent = nullptr);
    ~RoundAnimationLabel();

    /*!
     * Set background pixmap.
     */
    void setPixmap(const QPixmap &pix);

public slots:
    /*!
     * Start to rotating widget.
     */
    void start();
    /*!
     * Stop to rotating widget.
     */
    void stop();
    /*!
     * rotating timeout.
     */
    void timeout();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

protected:
    QPixmap m_pixmap;
    QTimer m_timer;
    int m_rotateAngle = 0;

};


/*! @brief Helper class to florid widget.
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT Florid : public Visual
{
    Q_OBJECT
public:
    /*!
     * Constructor.
     */
    explicit Florid(QWidget *parent = nullptr);
    /*!
     * Destructor.
     */
    virtual ~Florid();

public slots:
    /*!
    * Set background pixmap.
    */
   void setPixmap(const QPixmap &pix);
    /*!
     * Starts visualization.
     */
    virtual void start() override;
    /*!
     * Stops visualization.
     */
    virtual void stop() override;

private slots:
    /*!
     * Current media url changed.
     */
    void mediaUrlChanged();

protected:
    /*!
     * Rerender the image label.
    */
    void reRenderLabel();
    /*!
    * Rerender the image by color burn transform.
    */
    void reRenderImage(QColor &avg, const QImage *input);
    /*!
     * Process current visual data by state.
     */
    virtual void process(bool state) override final;

    virtual void hideEvent(QHideEvent *event) override final;
    virtual void showEvent(QShowEvent *event) override final;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

protected:
    QImage m_image;
    bool m_renderLabel = false;
    bool m_enabledLabel = true;
    bool m_gradientLabel = true;
    QColor m_averageColor = QColor(255, 255, 255);
    RoundAnimationLabel *m_roundLabel = nullptr;

};

#endif
