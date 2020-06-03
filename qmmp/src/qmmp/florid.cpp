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

#include "florid.h"

#include <soundcore.h>
#include <qmath.h>
#include <QBitmap>
#include <QPainter>
#include <QMenu>
#include <QKeyEvent>

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
    m_pixmap = pix.scaled(2 * DISTANCE, 2 * DISTANCE);

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
    const QRect rotatedRect(-self.width() / 2, -self.height() / 2, self.width(), self.height());
    const int cx = self.width() / 2;
    const int cy = self.height() / 2;

    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.translate(cx, cy);
    painter.rotate(m_rotateAngle);
    painter.drawPixmap(rotatedRect, m_pixmap);
    painter.restore();
}



Florid::Florid(QWidget *parent)
    : Visual(parent)
{
    setMinimumSize(580, 320);

    m_useImage = true;
    m_renderLabel = false;
    m_averageColor = QColor(255, 255, 255);
    m_gradientOn = false;
    m_roundLabel = new RoundAnimationLabel(this);

    m_screenAction = new QAction(tr("Fullscreen"), this);
    m_screenAction->setCheckable(true);
    connect(m_screenAction, SIGNAL(triggered(bool)), this, SLOT(changeFullScreen(bool)));
    connect(SoundCore::instance(), SIGNAL(trackInfoChanged()), SLOT(mediaUrlChanged()));
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

    m_image = pix.toImage();
    update();
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

void Florid::mediaUrlChanged()
{
    m_renderLabel = false;
    update();
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

void Florid::reRenderLabel()
{
    if(!m_image.isNull() && !m_renderLabel)
    {
        reRenderImage(m_averageColor, &m_image);

        m_roundLabel->setPixmap(QPixmap::fromImage(m_image));
        m_renderLabel = true;
    }
}

void Florid::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(!m_image.isNull())
    {
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.drawImage(0, 0, m_image.scaled(size()));
        painter.fillRect(rect(), QColor(150, 150, 150, 150));

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
            painter.drawEllipse(pt.x() - length, pt.y() - length, 2 * length, 2 * length);
        }

        reRenderLabel();
        m_roundLabel->setGeometry(pt.x() - DISTANCE, pt.y() - DISTANCE, 2 * DISTANCE, 2 * DISTANCE);
    }
    else
    {
        painter.fillRect(rect(), Qt::black);
    }
}

void Florid::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);

    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}
