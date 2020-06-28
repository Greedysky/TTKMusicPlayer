#include <QPainter>
#include <QPaintEvent>
#include <QMenu>
#include <qmath.h>
#include <stdlib.h>

#include "inlines.h"
#include "wavemono.h"

uint32_t spectrum(double level)
{
    level *= 0.6625;
    double r = 0.0, g = 0.0, b = 0.0;
    if(level >= 0 && level < 0.15)
    {
        r = (0.15 - level) / (0.15 + 0.075);
        g = 0.0;
        b = 1.0;
    }
    else if(level >= 0.15 && level < 0.275)
    {
        r = 0.0;
        g = (level - 0.15) / (0.275 - 0.15);
        b = 1.0;
    }
    else if(level >= 0.275 && level < 0.325)
    {
        r = 0.0;
        g = 1.0;
        b = (0.325 - level) / (0.325 - 0.275);
    }
    else if(level >= 0.325 && level < 0.5)
    {
        r = (level - 0.325) / (0.5 - 0.325);
        g = 1.0;
        b = 0.0;
    }
    else if(level >= 0.5 && level < 0.6625)
    {
        r = 1.0;
        g = (0.6625 - level) / (0.6625 - 0.5f);
        b = 0.0;
    }

    // Intensity correction.
    double cf = 1.0;
    if(level >= 0.0 && level < 0.1)
    {
        cf = level / 0.1;
    }
    cf *= 255.0;

    // Pack RGB values into a 32-bit uint.
    uint32_t rr = (uint32_t) (r * cf + 0.5);
    uint32_t gg = (uint32_t) (g * cf + 0.5);
    uint32_t bb = (uint32_t) (b * cf + 0.5);
    return (rr << 16) + (gg << 8) + bb;
}

// The default palette used by SoX and written by Rob Sykes.
uint32_t sox(double level)
{
    double r = 0.0;
    if(level >= 0.13 && level < 0.73)
    {
        r = sin((level - 0.13) / 0.60 * M_PI / 2.0);
    }
    else if(level >= 0.73)
    {
        r = 1.0;
    }

    double g = 0.0;
    if(level >= 0.6 && level < 0.91)
    {
        g = sin((level - 0.6) / 0.31 * M_PI / 2.0);
    }
    else if(level >= 0.91)
    {
        g = 1.0;
    }

    double b = 0.0;
    if(level < 0.60)
    {
        b = 0.5 * sin(level / 0.6 * M_PI);
    }
    else if(level >= 0.78)
    {
        b = (level - 0.78) / 0.22;
    }

    // Pack RGB values into a 32-bit uint.
    uint32_t rr = (uint32_t) (r * 255.0 + 0.5);
    uint32_t gg = (uint32_t) (g * 255.0 + 0.5);
    uint32_t bb = (uint32_t) (b * 255.0 + 0.5);
    return (rr << 16) + (gg << 8) + bb;
}

uint32_t mono(double level)
{
    uint32_t v = (uint32_t) (level * 255.0 + 0.5);
    return (v << 16) + (v << 8) + v;
}


WaveMono::WaveMono(QWidget *parent)
    : Visual(parent)
{
    m_palette = PALETTE_MONO;
    m_x_scale = nullptr;
    m_analyzer_falloff = 2.2;
    m_pixPos = 0;

    setWindowTitle(tr("Wave Mono Widget"));
    setMinimumSize(2*300-30, 105);
}

WaveMono::~WaveMono()
{
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void WaveMono::typeChanged(QAction *action)
{
    switch(action->data().toInt())
    {
        case 10: m_palette = PALETTE_SPECTRUM; break;
        case 20: m_palette = PALETTE_SOX; break;
        case 30: m_palette = PALETTE_MONO; break;
        default: break;
    }

    initialize();
}

void WaveMono::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void WaveMono::contextMenuEvent(QContextMenuEvent *event)
{
    Visual::contextMenuEvent(event);
    QMenu menu(this);
    QMenu typeMenu(tr("Type"), &menu);
    typeMenu.addAction(tr("Spectrum"))->setData(10);
    typeMenu.addAction(tr("Sox"))->setData(20);
    typeMenu.addAction(tr("Mono"))->setData(30);
    connect(&typeMenu, SIGNAL(triggered(QAction*)), this, SLOT(typeChanged(QAction*)));
    menu.addMenu(&typeMenu);
    menu.exec(QCursor::pos());
}

void WaveMono::process(float *left, float *)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        if(m_x_scale)
        {
            delete[] m_x_scale;
        }

        m_intern_vis_data = new int[m_rows]{0};
        m_x_scale = new int[m_rows + 1]{0};

        initialize();

        for(int i = 0; i < m_rows + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_rows), i);
        }
    }

    short dest[256];
    short y;
    int k, magnitude;

    calc_freq(dest, left);
    double y_scale = (double) 1.25 * m_cols / log(256);

    for(int i = 0; i < m_rows; i++)
    {
        y = 0;
        magnitude = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            y = dest[i];
        }

        for(k = m_x_scale[i]; k < m_x_scale[i + 1]; k++)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log(y) * y_scale);
            magnitude = qBound(0, magnitude, m_cols);
        }

        m_intern_vis_data[i] -= m_analyzer_falloff * m_cols / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}

void WaveMono::draw(QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    for(int j = 1; j < m_rows; ++j)
    {
        if(m_pixPos >= m_cols)
        {
            m_pixPos = m_cols - 1;
            m_backgroundImage = m_backgroundImage.copy(1, 0, m_cols, m_rows);
        }

        QRgb rgb;
        const double level = qBound(0, m_intern_vis_data[j - 1] / 2, 255) / 255.0;

        switch(m_palette)
        {
        case PALETTE_SPECTRUM:
            rgb = spectrum(level);
            break;
        case PALETTE_SOX:
            rgb = sox(level);
            break;
        case PALETTE_MONO:
            rgb = mono(level);
            break;
        default: break;
        }

        m_backgroundImage.setPixel(m_pixPos, m_rows - j, rgb);
    }

    ++m_pixPos;
    if(!m_backgroundImage.isNull())
    {
        p->drawImage(0, 0, m_backgroundImage);
    }
}

void WaveMono::initialize()
{
    if(m_rows != 0 && m_cols != 0)
    {
        m_pixPos = 0;
        m_backgroundImage = QImage(m_cols, m_rows, QImage::Format_RGB32);
        m_backgroundImage.fill(Qt::black);
    }
}
