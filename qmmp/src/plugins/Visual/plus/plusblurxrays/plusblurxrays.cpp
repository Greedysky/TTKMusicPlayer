#include "plusblurxrays.h"
#include "colorwidget.h"
#include "inlines.h"

#include <QMenu>
#include <QSettings>
#include <QPainter>
#include <math.h>
#include <qmmp/qmmp.h>

PlusBlurXRays::PlusBlurXRays(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Plus Blur XRays Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    readSettings();
}

PlusBlurXRays::~PlusBlurXRays()
{
    if(m_image)
    {
        delete[] m_image;
    }
}

void PlusBlurXRays::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusBlurXRays");
    m_color = ColorWidget::readSingleColorConfig(settings.value("color").toString());
    settings.endGroup();
}

void PlusBlurXRays::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusBlurXRays");
    settings.setValue("color", ColorWidget::writeSingleColorConfig(m_color));
    settings.endGroup();
}

void PlusBlurXRays::changeColor()
{
    ColorWidget c;
    c.setSingleColorMode(true);
    c.setColor(m_color);
    if(c.exec())
    {
        m_color = c.color();
    }
}

void PlusBlurXRays::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    draw(&painter);
}

void PlusBlurXRays::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(m_screenAction);
    menu.addSeparator();
    menu.addAction(tr("Color"), this, SLOT(changeColor()));
    menu.exec(QCursor::pos());
}

void PlusBlurXRays::process(float *left, float *)
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

        m_intern_vis_data = new int[m_cols]{0};
        m_image_size = (m_cols << 2) * (m_rows + 2);

        if(m_image)
        {
            delete[] m_image;
        }

        m_image = new unsigned int[m_image_size]{0};
        m_corner = m_image + m_cols + 1;
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_intern_vis_data[i] = int(left[pos >> 8] * m_rows / 2);
        m_intern_vis_data[i] = qBound(-m_rows / 2, m_intern_vis_data[i], m_rows / 2);
    }
}

void PlusBlurXRays::blur()
{
    for(int y = 0; y < m_rows; ++y)
    {
        uint32_t * p = m_corner + m_cols * y;
        uint32_t * end = p + m_cols;
        uint32_t * plast = p - m_cols;
        uint32_t * pnext = p + m_cols;

        /* We do a quick and dirty average of four color values, first masking
         * off the lowest two bits.  Over a large area, this masking has the net
         * effect of subtracting 1.5 from each value, which by a happy chance
         * is just right for a gradual fade effect. */
        for(; p < end; ++p)
        {
            *p = ((*plast ++ &0xFCFCFC) + (p[-1] & 0xFCFCFC) + (p[1] & 0xFCFCFC) + (*pnext ++ &0xFCFCFC)) >> 2;
        }
    }
}

void PlusBlurXRays::drawLine(int x, int y1, int y2)
{
    int y, h;

    if(y1 < y2)
    {
        y = y1 + 1;
        h = y2 - y1;
    }
    else if(y2 < y1)
    {
        y = y2;
        h = y1 - y2;
    }
    else
    {
        y = y1;
        h = 1;
    }

    unsigned int *p = m_corner + y * m_cols + x;

    for(; h--; p += m_cols)
    {
        *p = !m_color.isValid() ? 0xFF3F7F : m_color.rgba();
    }
}

void PlusBlurXRays::draw(QPainter *p)
{
    if(m_rows == 0)
    {
        return;
    }

    blur();

    int value = m_rows / 2 - m_intern_vis_data[0];
    value = qBound(0, value, m_rows - 1);

    for(int i = 0; i < m_cols; ++i)
    {
        int y = m_rows / 2 - m_intern_vis_data[i];
        y = qBound(0, y, m_rows - 1);
        drawLine(i, value, y);
        value = y;
    }

    p->drawImage(0, 0, QImage((unsigned char *)m_image, m_cols, m_rows, QImage::Format_RGB32));
}
