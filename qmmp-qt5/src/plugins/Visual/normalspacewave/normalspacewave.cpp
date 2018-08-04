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
#include "normalspacewave.h"

NormalSpaceWave::NormalSpaceWave (QWidget *parent) : Visual (parent)
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

    setWindowTitle (tr("Normal SpaceWave Widget"));
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

NormalSpaceWave::~NormalSpaceWave()
{
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
    if(m_x_scale)
        delete [] m_x_scale;
}

void NormalSpaceWave::start()
{
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
        m_starTimer->start();
    }
}

void NormalSpaceWave::stop()
{
    m_running = false;
    m_timer->stop();
    m_starTimer->stop();
    clear();
}

void NormalSpaceWave::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void NormalSpaceWave::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void NormalSpaceWave::starTimeout()
{
    for(int i=0; i<m_starPoints.count(); ++i)
    {
        StarPoint *sp = &m_starPoints[i];
        sp->m_alpha = rand()%255;
        sp->m_pt = QPoint(rand()%width(), rand()%height());
    }
}

void NormalSpaceWave::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalSpaceWave");
    m_starAction->setChecked(settings.value("show_star", false).toBool());
    m_starColor = ColorWidget::readSingleColorConfig(settings.value("star_color").toString());
}

void NormalSpaceWave::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalSpaceWave");
    settings.setValue("show_star", m_starAction->isChecked());
    settings.setValue("star_color", ColorWidget::writeSingleColorConfig(m_starColor));
    settings.endGroup();
}

void NormalSpaceWave::changeStarState(bool state)
{
    m_starAction->setChecked(state);
    update();
}

void NormalSpaceWave::changeStarColor()
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

void NormalSpaceWave::hideEvent (QHideEvent *)
{
    m_timer->stop();
    m_starTimer->stop();
}

void NormalSpaceWave::showEvent (QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
        m_starTimer->start();
    }
}

void NormalSpaceWave::paintEvent (QPaintEvent * e)
{
    Q_UNUSED(e);
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void NormalSpaceWave::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered (QAction *)), SLOT(writeSettings()));
    connect(&menu, SIGNAL(triggered (QAction *)), SLOT(readSettings()));

    menu.addAction(m_starAction);
    menu.addAction("StarColor", this, SLOT(changeStarColor()));

    menu.exec(QCursor::pos());
}

void NormalSpaceWave::process ()
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

void NormalSpaceWave::draw (QPainter *p)
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

    QLinearGradient line(0, 0, width(), 0);
    line.setColorAt(1.0*1/7, QColor(72, 176, 211));
    line.setColorAt(1.0*2/7, QColor(57, 255, 57));
    line.setColorAt(1.0*4/7, QColor(255, 247, 22));
    line.setColorAt(1.0*5/7, QColor(255, 64, 59));
    line.setColorAt(1.0*7/7, QColor(255, 64, 59));
    p->setPen(QPen(line, 0.5));
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
        {
            x += rdx; //correct right part position
        }

        for (int i = 0; i <= m_intern_vis_data[j]*l/2; ++i)
        {
            p->drawRect (x, height()/2 - i * m_cell_size.height() + 1,
                         m_cell_size.width() - 2, m_cell_size.height() - 2);
            p->drawRect (x, height()/2 + i * m_cell_size.height() + 1,
                         m_cell_size.width() - 2, m_cell_size.height() - 2);
        }
    }
    p->fillRect (0, height()/2, width(), height()/2, QColor(0, 0, 0, 188));
}
