#include "plusblurxrays.h"
#include "colorwidget.h"

#include <QMenu>
#include <QSettings>
#include <QPainter>
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
    delete[] m_image;
}

void PlusBlurXRays::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusBlurXRays");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    settings.endGroup();
}

void PlusBlurXRays::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("PlusBlurXRays");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.endGroup();
}

void PlusBlurXRays::changeColor()
{
    ColorWidget dialog;
    dialog.setSingleMode(true);
    dialog.setColors(m_colors);
    if(dialog.exec())
    {
        m_colors = dialog.colors();
    }
}

void PlusBlurXRays::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.fillRect(rect(), Qt::black);

    if(m_rows == 0)
    {
        return;
    }

    blur();

    int value = m_rows / 2 - m_visualData[0];
        value = qBound(0, value, m_rows - 1);

    for(int i = 0; i < m_cols; ++i)
    {
        int y = m_rows / 2 - m_visualData[i];
            y = qBound(0, y, m_rows - 1);
        drawLine(i, value, y);
        value = y;
    }

    painter.drawImage(0, 0, QImage((uchar*)m_image, m_cols, m_rows, QImage::Format_RGB32));
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

void PlusBlurXRays::processData(float *left, float *)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_visualData;
        delete[] m_image;

        m_visualData = new int[m_cols]{0};
        m_imageSize = (m_cols << 2) * (m_rows + 2);

        m_image = new unsigned int[m_imageSize]{0};
        m_corner = m_image + m_cols + 1;
    }

    const int step = (QMMP_VISUAL_NODE_SIZE << 8) / m_cols;
    int pos = 0;

    for(int i = 0; i < m_cols; ++i)
    {
        pos += step;
        m_visualData[i] = int(left[pos >> 8] * m_rows / 2);
        m_visualData[i] = qBound(-m_rows / 2, m_visualData[i], m_rows / 2);
    }
}

void PlusBlurXRays::blur()
{
    for(int y = 0; y < m_rows; ++y)
    {
        uint32_t *p = m_corner + m_cols * y;
        uint32_t *end = p + m_cols;
        uint32_t *plast = p - m_cols;
        uint32_t *pnext = p + m_cols;

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
    const QColor &color = m_colors.first();

    for(; h--; p += m_cols)
    {
        *p = !color.isValid() ? 0xFF3F7F : color.rgba();
    }
}
