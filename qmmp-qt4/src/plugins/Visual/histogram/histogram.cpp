#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <qmmp/soundcore.h>
#include "fft.h"
#include "inlines.h"
#include "histogram.h"

Histogram::Histogram (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_x_scale = 0;
    m_running = false;
    m_rows = 0;
    m_cols = 0;
    m_analyzer_falloff = 2.2;
    m_cell_size = QSize(15, 6);

    for(int i=0; i<50; ++i)
    {
        m_starPoints << StarPoint();
    }

    setWindowTitle (tr("Histogram"));
    setMinimumSize(2*300-30, 105);
    m_timer = new QTimer (this);
    m_timer->setInterval(40);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));

    m_starTimer = new QTimer (this);
    connect(m_starTimer, SIGNAL (timeout()), this, SLOT (starTimeout()));

    m_starAction = new QAction(tr("Star"), this);
    m_starAction->setCheckable(true);
    connect(m_starAction, SIGNAL(triggered(bool)), this, SLOT(changeStarState(bool)));

    m_starTimer->setInterval(1000);

    clear();
    readSettings();
}

Histogram::~Histogram()
{
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
    if(m_x_scale)
        delete [] m_x_scale;
}

void Histogram::start()
{
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
        m_starTimer->start();
    }
}

void Histogram::stop()
{
    m_running = false;
    m_timer->stop();
    m_starTimer->stop();
    clear();
}

void Histogram::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void Histogram::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void Histogram::starTimeout()
{
    for(int i=0; i<m_starPoints.count(); ++i)
    {
        StarPoint *sp = &m_starPoints[i];
        sp->m_alpha = rand()%255;
        sp->m_pt = QPoint(rand()%width(), rand()%height());
    }
}

void Histogram::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Histogram");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    m_starAction->setChecked(settings.value("show_star", false).toBool());
    m_starColor = ColorWidget::readSingleColorConfig(settings.value("star_color").toString());
}

void Histogram::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Histogram");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.setValue("show_star", m_starAction->isChecked());
    settings.setValue("star_color", ColorWidget::writeSingleColorConfig(m_starColor));
    settings.endGroup();
}

void Histogram::changeColor()
{
    ColorWidget c;
    c.setColors(m_colors);
    if(c.exec())
    {
        m_colors = c.getColors();
    }
}

void Histogram::changeStarState(bool state)
{
    m_starAction->setChecked(state);
    update();
}

void Histogram::changeStarColor()
{
    ColorWidget c;
    c.setColors(QList<QColor>() << m_starColor);
    if(c.exec())
    {
        QList<QColor> colors(c.getColors());
        if(!colors.isEmpty())
        {
            m_starColor = colors.first();
            update();
        }
    }
}

void Histogram::hideEvent (QHideEvent *)
{
    m_timer->stop();
    m_starTimer->stop();
}

void Histogram::showEvent (QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
        m_starTimer->start();
    }
}

void Histogram::paintEvent (QPaintEvent * e)
{
    Q_UNUSED(e);
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void Histogram::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered (QAction *)), SLOT(writeSettings()));
    connect(&menu, SIGNAL(triggered (QAction *)), SLOT(readSettings()));

    menu.addAction("Color", this, SLOT(changeColor()));
    menu.addAction(m_starAction);
    menu.addAction("StarColor", this, SLOT(changeStarColor()));

    menu.exec(QCursor::pos());
}

void Histogram::process ()
{
    static fft_state *state = 0;
    if (!state)
        state = fft_init();

    int rows = (height() - 2) / m_cell_size.height();
    int cols = (width() - 2) / m_cell_size.width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        if(m_intern_vis_data)
            delete [] m_intern_vis_data;
        if(m_x_scale)
            delete [] m_x_scale;
        m_intern_vis_data = new double[m_cols];
        m_x_scale = new int[m_cols + 1];

        for(int i = 0; i < m_cols; ++i)
        {
            m_intern_vis_data[i] = 0;
        }
        for(int i = 0; i < m_cols + 1; ++i)
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
    }

    short dest[256];
    short y;
    int k, magnitude;

    calc_freq (dest, m_left_buffer);

    double y_scale = (double) 1.25 * m_rows / log(256);

    for (int i = 0; i < m_cols; i++)
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
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}

void Histogram::draw (QPainter *p)
{
    if(m_starAction->isChecked())
    {
        for(int i=0; i<m_starPoints.count(); ++i)
        {
            StarPoint *sp = &m_starPoints[i];
            m_starColor.setAlpha(sp->m_alpha);
            p->setPen(QPen(m_starColor, 3));
            p->drawPoint(sp->m_pt);
        }
    }

    QLinearGradient line(0, 0, 0, height());
    for(int i=0; i<m_colors.count(); ++i)
    {
        line.setColorAt((i+1)*1.0/m_colors.count(), m_colors[i]);
    }
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    int x = 0;
    int rdx = qMax(0, width() - 2 * m_cell_size.width() * m_cols);

    float l = 1.0f;
    if(SoundCore::instance())
    {
        l = SoundCore::instance()->volume()*1.0/100;
    }

    for (int j = 0; j < m_cols; ++j)
    {
        x = j * m_cell_size.width() + 1;
        if(j >= m_cols)
            x += rdx; //correct right part position

        int hh = m_intern_vis_data[j] * l * m_cell_size.height();
        p->fillRect(x, height() - hh + 1, m_cell_size.width() - 2, hh - 2, line);
    }
}
