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
#include "plusmonowave.h"

PlusMonoWave::PlusMonoWave (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_x_scale = 0;
    m_running = false;
    m_rows = 0;
    m_cols = 0;
    m_analyzer_falloff = 2.2;
    m_pixPos = 0;

    setWindowTitle (tr("Plus MonoWave Widget"));
    setMinimumSize(2*300-30, 105);
    m_timer = new QTimer (this);
    m_timer->setInterval(40);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));

    clear();
}

PlusMonoWave::~PlusMonoWave()
{
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
    if(m_x_scale)
        delete [] m_x_scale;
}

void PlusMonoWave::start()
{
    m_running = true;
    if(isVisible())
        m_timer->start();
}

void PlusMonoWave::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void PlusMonoWave::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void PlusMonoWave::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void PlusMonoWave::hideEvent (QHideEvent *)
{
    m_timer->stop();
}

void PlusMonoWave::showEvent (QShowEvent *)
{
    if(m_running)
        m_timer->start();
}

void PlusMonoWave::paintEvent (QPaintEvent * e)
{
    Q_UNUSED(e);
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void PlusMonoWave::process ()
{
    static fft_state *state = 0;
    if (!state)
        state = fft_init();

    int rows = height();
    int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        m_pixPos = 0;

        if(m_intern_vis_data)
            delete [] m_intern_vis_data;
        if(m_x_scale)
            delete [] m_x_scale;
        m_intern_vis_data = new double[m_rows];
        m_x_scale = new int[m_rows + 1];
        m_backgroundImage = QImage(m_cols, m_rows, QImage::Format_RGB32);
        m_backgroundImage.fill(Qt::black);

        for(int i = 0; i < m_rows; ++i)
        {
            m_intern_vis_data[i] = 0;
        }
        for(int i = 0; i < m_rows + 1; ++i)
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_rows), i);
    }

    short dest[256];
    short y;
    int k, magnitude;

    calc_freq (dest, m_left_buffer);

    double y_scale = (double) 1.25 * m_cols / log(256);

    for (int i = 0; i < m_rows; i++)
    {
        y = 0;
        magnitude = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            y = dest[i];
        }
        for (k = m_x_scale[i]; k < m_x_scale[i + 1]; k++)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if (y)
        {
            magnitude = int(log (y) * y_scale);
            magnitude = qBound(0, magnitude, m_cols);
        }

        m_intern_vis_data[i] -= m_analyzer_falloff * m_cols / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}

void PlusMonoWave::draw (QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    float l = 1.0f;
    if(SoundCore::instance())
    {
        l = SoundCore::instance()->volume()*1.0/100;
    }

    for (int j = 1; j < m_rows; ++j)
    {
        int v = m_intern_vis_data[j - 1] * l;
        if(m_pixPos >= m_cols)
        {
            m_pixPos = m_cols - 1;
            m_backgroundImage = m_backgroundImage.copy(1, 0, m_cols, m_rows);
        }
        m_backgroundImage.setPixel(m_pixPos, m_rows - j, qRgb(v, v, v));
    }
    ++m_pixPos;
    p->drawImage(0, 0, m_backgroundImage);
}
