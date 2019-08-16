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

#ifndef FLORID_H
#define FLORID_H

#include <QTimer>
#include "visual.h"

#define DISTANCE    100

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
     * Set rotating widget background pixmap.
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
    virtual void paintEvent(QPaintEvent *event) override;

protected:
    QPixmap m_pixmap;
    QTimer m_timer;
    int m_rotateAngle;
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
     /*!
     * Set background pixmap.
     */
    void setPixmap(const QPixmap &pix);

public slots:
    /*!
     * Starts visualization.
     */
    virtual void start() override;
    /*!
     * Stops visualization.
     */
    virtual void stop() override;

protected:
    /*!
    * Gauss Blur.
    */
    void gaussBlur(QImage &img, int radius);
    /*!
    * Rerender the image by color burn transform.
    */
    void reRenderImage(QColor &avg, const QImage *input);
    /*!
    * Rerender the image by color burn transform.
    */
    void reRenderImage(int delta, const QImage *input, QImage *output);
    /*!
     * Image color burn transform.
     */
    int colorBurnTransform(int c, int delta);
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

protected:
    QImage m_image;
    bool m_useImage, m_scale;
    QColor m_averageColor;
    RoundAnimationLabel *m_roundLabel;

};

#endif
