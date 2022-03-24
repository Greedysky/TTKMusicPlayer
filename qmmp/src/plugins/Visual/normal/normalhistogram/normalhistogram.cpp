#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QPaintEvent>
#include <math.h>
#include <qmmp/qmmp.h>

#include "inlines.h"
#include "normalhistogram.h"

NormalHistogram::NormalHistogram(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Normal Histogram Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    for(int i = 0; i < 50; ++i)
    {
        m_starPoints << new StarPoint();
    }

    m_starTimer = new QTimer(this);
    connect(m_starTimer, SIGNAL(timeout()), this, SLOT(starTimeout()));

    m_starAction = new QAction(tr("Star"), this);
    m_starAction->setCheckable(true);
    connect(m_starAction, SIGNAL(triggered(bool)), this, SLOT(changeStarState(bool)));

    m_starTimer->setInterval(1000);

    readSettings();
}

NormalHistogram::~NormalHistogram()
{
    qDeleteAll(m_starPoints);
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void NormalHistogram::start()
{
    Visual::start();
    if(isVisible())
    {
        m_starTimer->start();
    }
}

void NormalHistogram::stop()
{
    Visual::stop();
    m_starTimer->stop();
}

void NormalHistogram::starTimeout()
{
    for(StarPoint *point : qAsConst(m_starPoints))
    {
        point->m_alpha = qrand() % 255;
        point->m_pt = QPoint(qrand() % width(), qrand() % height());
    }
}

void NormalHistogram::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalHistogram");
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    m_starAction->setChecked(settings.value("show_star", false).toBool());
    m_starColor = ColorWidget::readSingleColorConfig(settings.value("star_color").toString());
    settings.endGroup();
}

void NormalHistogram::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalHistogram");
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.setValue("show_star", m_starAction->isChecked());
    settings.setValue("star_color", ColorWidget::writeSingleColorConfig(m_starColor));
    settings.endGroup();
}

void NormalHistogram::changeColor()
{
    ColorWidget c;
    c.setColors(m_colors);
    if(c.exec())
    {
        m_colors = c.colors();
    }
}

void NormalHistogram::changeStarState(bool state)
{
    m_starAction->setChecked(state);
    update();
}

void NormalHistogram::changeStarColor()
{
    ColorWidget c;
    c.setSingleColorMode(true);
    c.setColor(m_starColor);
    if(c.exec())
    {
        m_starColor = c.color();
        update();
    }
}

void NormalHistogram::hideEvent(QHideEvent *e)
{
    Visual::hideEvent(e);
    m_starTimer->stop();
}

void NormalHistogram::showEvent(QShowEvent *e)
{
    Visual::showEvent(e);
    if(m_running)
    {
        m_starTimer->start();
    }
}

void NormalHistogram::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void NormalHistogram::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(m_screenAction);
    menu.addSeparator();
    menu.addAction(tr("Color"), this, SLOT(changeColor()));
    menu.addAction(m_starAction);
    menu.addAction(tr("StarColor"), this, SLOT(changeStarColor()));
    menu.exec(QCursor::pos());
}

void NormalHistogram::process(float *left, float *)
{
    const int rows = (height() - 2) / m_cell_size.height();
    const int cols = (width() - 2) / m_cell_size.width();

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

        m_intern_vis_data = new int[m_cols]{0};
        m_x_scale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y;
    int k, magnitude;

    calc_freq(dest, left);
    const double y_scale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        y = 0;
        magnitude = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            y = dest[i];
        }

        for(k = m_x_scale[i]; k < m_x_scale[i + 1]; ++k)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log(y) * y_scale);
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}

void NormalHistogram::draw(QPainter *p)
{
    if(m_starAction->isChecked())
    {
        for(StarPoint *point : qAsConst(m_starPoints))
        {
            m_starColor.setAlpha(point->m_alpha);
            p->setPen(QPen(m_starColor, 3));
            p->drawPoint(point->m_pt);
        }
    }

    QLinearGradient line(0, 0, 0, height());
    for(int i = 0; i < m_colors.count(); ++i)
    {
        line.setColorAt((i + 1) * 1.0 / m_colors.count(), m_colors[i]);
    }
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    int x = 0;
    for(int i = 0; i < m_cols; ++i)
    {
        x = i * m_cell_size.width() + 1;
        const int offset = m_intern_vis_data[i] * m_cell_size.height();
        p->fillRect(x, height() - offset + 1, m_cell_size.width() - 2, offset - 2, line);
    }
}
