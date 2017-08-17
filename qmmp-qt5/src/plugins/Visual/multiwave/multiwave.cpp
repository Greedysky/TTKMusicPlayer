#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <qmmp/soundcore.h>
#include "fft.h"
#include "inlines.h"
#include "multiwave.h"

#define VISUAL_NODE_SIZE 512 //samples
#define VISUAL_BUFFER_SIZE (5*VISUAL_NODE_SIZE)

MultiWave::MultiWave (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_x_scale = 0;
    m_buffer_at = 0;
    m_rows = 0;
    m_cols = 0;
    m_analyzer_falloff = 2.2;
    m_pixPos = 0;

    setWindowTitle (tr("MultiWave"));
    setMinimumSize(2*300-30, 105);

    m_timer = new QTimer (this);
    m_timer->setInterval(40);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));
    m_buffer = new float[VISUAL_BUFFER_SIZE];

    clear();
}

MultiWave::~MultiWave()
{
    delete [] m_buffer;

    if(m_x_scale)
        delete [] m_x_scale;
}

void MultiWave::add (float *data, size_t samples, int chan)
{
    if (!m_timer->isActive ())
        return;

    if(VISUAL_BUFFER_SIZE == m_buffer_at)
    {
        m_buffer_at -= VISUAL_NODE_SIZE;
        memmove(m_buffer, m_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
        return;
    }

    int frames = qMin(int(samples/chan), VISUAL_BUFFER_SIZE - m_buffer_at);
    mono_from_multichannel(m_buffer + m_buffer_at, data, frames, chan);

    m_buffer_at += frames;
}

void MultiWave::clear()
{
    m_buffer_at = 0;
    m_rows = 0;
    m_cols = 0;
    update();
}

void MultiWave::timeout()
{
    mutex()->lock();
    if(m_buffer_at < VISUAL_NODE_SIZE)
    {
        mutex()->unlock ();
        return;
    }

    process (m_buffer);
    m_buffer_at -= VISUAL_NODE_SIZE;
    memmove(m_buffer, m_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    mutex()->unlock ();
    update();
}

void MultiWave::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void MultiWave::showEvent (QShowEvent *)
{
    m_timer->start();
}

void MultiWave::paintEvent (QPaintEvent * e)
{
    Q_UNUSED(e);
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void MultiWave::process (float *buffer)
{
    int rows = height();
    int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        m_pixPos = 0;

        if(m_x_scale)
            delete [] m_x_scale;
        m_intern_vis_data = 0;
        m_x_scale = new int[2];
        m_backgroundImage = QImage(m_cols, m_rows, QImage::Format_RGB32);
        m_backgroundImage.fill(Qt::black);
        for(int i=0; i<m_cols; ++i)
            m_backgroundImage.setPixel(i, m_rows/2, qRgb(0xff, 0xff, 0xff));

        for(int i = 0; i < 2; ++i)
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
    }

    short dest[256];
    short y = 0;
    int k, magnitude = 0;
    calc_freq (dest, buffer);
    double y_scale = (double) 1.25 * m_rows / log(256);

    if(m_x_scale[0] == m_x_scale[1])
    {
        y = dest[0];
    }
    for (k = m_x_scale[0]; k < m_x_scale[1]; k++)
    {
        y = qMax(dest[k], y);
    }

    y >>= 7; //256

    if (y)
    {
        magnitude = int(log (y) * y_scale);
        magnitude = qBound(0, magnitude, m_rows);
    }

    m_intern_vis_data -= m_analyzer_falloff * m_rows / 15;
    m_intern_vis_data = magnitude > m_intern_vis_data ? magnitude : m_intern_vis_data;
}

void MultiWave::draw (QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    float l = 1.0f;
    if(SoundCore::instance())
    {
        l = SoundCore::instance()->volume()*1.0/100;
    }

    if(m_cols != 0)
    {
        if(m_pixPos >= m_cols)
        {
            m_pixPos = m_cols - 1;
            m_backgroundImage = m_backgroundImage.copy(1, 0, m_cols, m_rows);
        }
        for(int i=0; i<m_intern_vis_data*l/2; ++i)
        {
            m_backgroundImage.setPixel(m_pixPos, qMax(m_rows/2 - i, 0), qRgb(0xff, 0, 0));
            m_backgroundImage.setPixel(m_pixPos, qMin(m_rows/2 + i, m_rows), qRgb(0xff, 0, 0));
        }
        m_backgroundImage.setPixel(m_pixPos, m_rows/2, qRgb(0xff, 0xff, 0xff));
    }
    ++m_pixPos;

    if(!m_backgroundImage.isNull())
    {
        p->drawImage(0, 0, m_backgroundImage);
    }
}
