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

#include "florid.h"

#include <qmath.h>
#include <QBitmap>
#include <QPainter>

RoundAnimationLabel::RoundAnimationLabel(QWidget *parent)
    : QWidget(parent)
{
    m_rotateAngle = 0;

    m_timer.setInterval(100);
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

RoundAnimationLabel::~RoundAnimationLabel()
{
    stop();
}

void RoundAnimationLabel::setPixmap(const QPixmap &pix)
{
    m_pixmap = pix.scaled(2*DISTANCE, 2*DISTANCE);

    QRect rect(QPoint(0, 0), m_pixmap.size());
    QBitmap mask(rect.size());
    QPainter painter(&mask);
    painter.fillRect(rect, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawRoundedRect(rect, DISTANCE, DISTANCE);
    m_pixmap.setMask(mask);
}

void RoundAnimationLabel::start()
{
    if(!m_timer.isActive())
    {
        m_timer.start();
    }
}

void RoundAnimationLabel::stop()
{
    m_timer.stop();
}

void RoundAnimationLabel::timeout()
{
    if(++m_rotateAngle >= 360)
    {
        m_rotateAngle -= 360;
    }
    update();
}

void RoundAnimationLabel::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    const QSize &self = size();
    const QRect rotatedRect(-self.width()/2, -self.height()/2, self.width(), self.height());
    const int cx = self.width() / 2;
    const int cy = self.height() / 2;

    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.translate(cx, cy);
    painter.rotate(m_rotateAngle);
    painter.drawPixmap(rotatedRect, m_pixmap);
    painter.restore();
}



Florid::Florid(QWidget *parent) :
    Visual(parent)
{
    setMinimumSize(580, 320);

    m_useImage = true;
    m_scale = false;
    m_averageColor = QColor(255, 255, 255);
    m_gradientOn = false;
    m_roundLabel = new RoundAnimationLabel(this);
}

Florid::~Florid()
{

}

void Florid::setPixmap(const QPixmap &pix)
{
    if(!m_useImage)
    {
        return;
    }

    m_scale = false;
    m_image = pix.toImage();
}

void Florid::start()
{
    if(!m_useImage)
    {
        return;
    }
    m_roundLabel->start();
}

void Florid::stop()
{
    if(!m_useImage)
    {
        return;
    }
    m_roundLabel->stop();
}

void Florid::gaussBlur(QImage &img, int radius)
{
    int* pix = (int*)img.bits();
    int w = img.width();
    int h = img.height();
    float sigma =  1.0 * radius / 2.57;
    float deno  =  1.0 / (sigma * sqrt(2.0 * M_PI));
    float nume  = -1.0 / (2.0 * sigma * sigma);

    float* gaussMatrix = (float*)malloc(sizeof(float)* (radius + radius + 1));
    float gaussSum = 0.0;
    for (int i = 0, x = -radius; x <= radius; ++x, ++i)
    {
        float g = deno * exp(1.0 * nume * x * x);

        gaussMatrix[i] = g;
        gaussSum += g;
    }

    int len = radius + radius + 1;
    for (int i = 0; i < len; ++i)
    {
        gaussMatrix[i] /= gaussSum;
    }

    int* rowData  = (int*)malloc(w * sizeof(int));
    int* listData = (int*)malloc(h * sizeof(int));

    for (int y = 0; y < h; ++y)
    {
        memcpy(rowData, pix + y * w, sizeof(int) * w);

        for (int x = 0; x < w; ++x)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for (int i = -radius; i <= radius; ++i)
            {
                int k = x + i;

                if (0 <= k && k <= w)
                {
                    int color = rowData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);

                    r += cr * gaussMatrix[i + radius];
                    g += cg * gaussMatrix[i + radius];
                    b += cb * gaussMatrix[i + radius];

                    gaussSum += gaussMatrix[i + radius];
                }
            }

            int cr = (int)(r / gaussSum);
            int cg = (int)(g / gaussSum);
            int cb = (int)(b / gaussSum);

            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            listData[y] = pix[y * w + x];
        }

        for (int y = 0; y < h; ++y)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for (int j = -radius; j <= radius; ++j)
            {
                int k = y + j;

                if (0 <= k && k <= h)
                {
                    int color = listData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);

                    r += cr * gaussMatrix[j + radius];
                    g += cg * gaussMatrix[j + radius];
                    b += cb * gaussMatrix[j + radius];

                    gaussSum += gaussMatrix[j + radius];
                }
            }

            int cr = (int)(r / gaussSum);
            int cg = (int)(g / gaussSum);
            int cb = (int)(b / gaussSum);

            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    free(gaussMatrix);
    free(rowData);
    free(listData);
}

void Florid::reRenderImage(QColor &avg, const QImage *input)
{
   if(input->isNull())
   {
       return;
   }

   QRgb r = 0, g = 0, b = 0;
   for(int w=0; w<input->width(); w++)
   {
       for(int h=0; h<input->height(); h++)
       {
           QRgb rgb = input->pixel(w, h);
           r += qRed(rgb);
           g += qGreen(rgb);
           b += qBlue(rgb);
       }
   }

   const int size = input->width() * input->height();
   avg.setRed(r /= size);
   avg.setGreen(g /= size);
   avg.setBlue(b /= size);
}

void Florid::reRenderImage(int delta, const QImage *input, QImage *output)
{
    for(int w=0; w<input->width(); w++)
    {
        for(int h=0; h<input->height(); h++)
        {
            const QRgb rgb = input->pixel(w, h);
            output->setPixel(w, h, qRgb(colorBurnTransform(qRed(rgb), delta),
                                        colorBurnTransform(qGreen(rgb), delta),
                                        colorBurnTransform(qBlue(rgb), delta)));
        }
    }
}

int Florid::colorBurnTransform(int c, int delta)
{
    if(0 > delta || delta > 0xFF)
    {
        return c;
    }

    int result = (c - (int)(c*delta)/(0xFF - delta));
    if(result > 0xFF)
    {
        result = 0xFF;
    }
    else if(result < 0)
    {
        result = 0;
    }
    return result;
}

void Florid::resizeEvent(QResizeEvent *event)
{
    Visual::resizeEvent(event);
    if(!m_image.isNull() && !m_scale)
    {
        reRenderImage(m_averageColor, &m_image);
        m_roundLabel->setPixmap(QPixmap::fromImage(m_image));
        reRenderImage(50, &m_image, &m_image);
        gaussBlur(m_image, 50);
        m_scale = true;
    }
}

void Florid::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(!m_image.isNull())
    {
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.drawImage(0, 0, m_image.scaled(size()));

        const QPoint &pt = rect().center();
        if(m_gradientOn)
        {
            const int length = DISTANCE + 20;
            QRadialGradient gradient(pt.x(), pt.y(), length, pt.x(), pt.y());
            painter.setPen(Qt::NoPen);
            gradient.setColorAt(0.7, m_averageColor);
            QColor color = m_averageColor;
            color.setAlpha(0);
            gradient.setColorAt(1.0, color);
            painter.setBrush(gradient);
            painter.drawEllipse(pt.x() - length, pt.y() - length, 2*length, 2*length);
        }
        m_roundLabel->setGeometry(pt.x() - DISTANCE, pt.y() - DISTANCE, 2*DISTANCE, 2*DISTANCE);
    }
    else
    {
        painter.fillRect(rect(), Qt::black);
    }
}
