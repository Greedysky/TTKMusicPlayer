#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include "fft.h"
#include "inlines.h"
#include "volumewave.h"

#define VISUAL_NODE_SIZE 512 //samples
#define VISUAL_BUFFER_SIZE (5*VISUAL_NODE_SIZE)

VolumeWave::VolumeWave (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_x_scale = 0;
    m_buffer_at = 0;
    m_rows = 0;
    m_cols = 0;

    setWindowTitle (tr("VolumeWave"));
    setMinimumSize(2*300-30, 105);
    m_timer = new QTimer (this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    m_left_buffer = new float[VISUAL_BUFFER_SIZE];
    m_right_buffer = new float[VISUAL_BUFFER_SIZE];

    m_analyzer_falloff = 1.2;
    m_timer->setInterval(40);

    clear();
}

VolumeWave::~VolumeWave()
{
    delete [] m_left_buffer;
    delete [] m_right_buffer;

    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
    if(m_x_scale)
        delete [] m_x_scale;
}

void VolumeWave::add (float *data, size_t samples, int chan)
{
    if (!m_timer->isActive ())
        return;

    if(VISUAL_BUFFER_SIZE == m_buffer_at)
    {
        m_buffer_at -= VISUAL_NODE_SIZE;
        memmove(m_left_buffer, m_left_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
        memmove(m_right_buffer, m_right_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
        return;
    }

    int frames = qMin(int(samples/chan), VISUAL_BUFFER_SIZE - m_buffer_at);

    stereo_from_multichannel(m_left_buffer + m_buffer_at,
                             m_right_buffer + m_buffer_at, data, frames, chan);

    m_buffer_at += frames;
}

void VolumeWave::clear()
{
    m_buffer_at = 0;
    m_rows = 0;
    m_cols = 0;
    update();
}

void VolumeWave::timeout()
{
    mutex()->lock();
    if(m_buffer_at < VISUAL_NODE_SIZE)
    {
        mutex()->unlock ();
        return;
    }

    process (m_left_buffer, m_right_buffer);
    m_buffer_at -= VISUAL_NODE_SIZE;
    memmove(m_left_buffer, m_left_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    memmove(m_right_buffer, m_right_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    mutex()->unlock ();
    update();
}

void VolumeWave::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void VolumeWave::showEvent (QShowEvent *)
{
    m_timer->start();
}

void VolumeWave::paintEvent (QPaintEvent * e)
{
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void VolumeWave::process (float *left, float *right)
{
    int rows = height();
    int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
            delete [] m_intern_vis_data;
        if(m_x_scale)
            delete [] m_x_scale;
        m_intern_vis_data = new double[2];
        m_x_scale = new int[2];

        for(int i = 0; i < 2; ++i)
        {
            m_intern_vis_data[i] = 0;
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest_l[256];
    short dest_r[256];
    short yl, yr;
    int k, magnitude_l, magnitude_r;

    calc_freq (dest_l, left);
    calc_freq (dest_r, right);

    double y_scale = (double) 1.25 * m_rows / log(256);

    yl = yr = 0;
    magnitude_l = magnitude_r = 0;

    if(m_x_scale[0] == m_x_scale[1])
    {
        yl = dest_l[0];
        yr = dest_r[0];
    }
    for (k = m_x_scale[0]; k < m_x_scale[1]; k++)
    {
        yl = qMax(dest_l[k], yl);
        yr = qMax(dest_r[k], yr);
    }

    yl >>= 7; //256
    yr >>= 7;

    if (yl)
    {
        magnitude_l = int(log (yl) * y_scale);
        magnitude_l = qBound(0, magnitude_l, m_rows);
    }
    if (yr)
    {
        magnitude_r = int(log (yr) * y_scale);
        magnitude_r = qBound(0, magnitude_r, m_rows);
    }

    m_intern_vis_data[0] -= m_analyzer_falloff * m_rows / 15;
    m_intern_vis_data[0] = magnitude_l > m_intern_vis_data[0] ? magnitude_l : m_intern_vis_data[0];

    m_intern_vis_data[1] -= m_analyzer_falloff * m_rows / 15;
    m_intern_vis_data[1] = magnitude_r > m_intern_vis_data[1] ? magnitude_r : m_intern_vis_data[1];

}

void VolumeWave::draw (QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QLinearGradient line(0, 0, width(), 0);
    line.setColorAt(0.0f, QColor(0, 0xff, 0).dark());
    line.setColorAt(0.65f, QColor(0xff, 0xff, 0).dark());
    line.setColorAt(1.0f, QColor(0xff, 0, 0).dark());
    p->fillRect(0, 0, width(), height(), line);

    line.setColorAt(0.0f, QColor(0, 0xff, 0));
    line.setColorAt(0.65f, QColor(0xff, 0xff, 0));
    line.setColorAt(1.0f, QColor(0xff, 0, 0));

    if(m_intern_vis_data)
    {
        int wid = ceil(m_rows/2);
        p->fillRect(0, 0, m_intern_vis_data[0]*m_cols/m_rows, wid, line);
        p->fillRect(0, wid, m_intern_vis_data[1]*m_cols/m_rows, wid, line);
    }

    p->setPen(Qt::white);
    p->drawText(10, height()/4, "L");
    p->drawText(10, height()*3/4, "R");

}
