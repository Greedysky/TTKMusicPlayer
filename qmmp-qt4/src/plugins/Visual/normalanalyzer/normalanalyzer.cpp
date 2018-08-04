#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <qmmp/soundcore.h>
#include "fft.h"
#include "inlines.h"
#include "normalanalyzer.h"

NormalAnalyzer::NormalAnalyzer (QWidget *parent) : Visual (parent)
{
    m_intern_vis_data = 0;
    m_peaks = 0;
    m_x_scale = 0;
    m_rows = 0;
    m_cols = 0;
    m_update = false;
    m_running = false;

    for(int i=0; i<50; ++i)
    {
        m_starPoints << StarPoint();
    }

    setWindowTitle (tr("Normal Analyzer Widget"));
    setMinimumSize(2*300-30, 105);
    m_timer = new QTimer (this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT (timeout()));

    m_starTimer = new QTimer (this);
    connect(m_starTimer, SIGNAL (timeout()), this, SLOT (starTimeout()));

    m_starAction = new QAction(tr("Star"), this);
    m_starAction->setCheckable(true);
    connect(m_starAction, SIGNAL(triggered(bool)), this, SLOT(changeStarState(bool)));

    m_starTimer->setInterval(1000);
    m_cell_size = QSize(15, 6);

    clear();
    createMenu();
    readSettings();
}

NormalAnalyzer::~NormalAnalyzer()
{
    if(m_peaks)
        delete [] m_peaks;
    if(m_intern_vis_data)
        delete [] m_intern_vis_data;
    if(m_x_scale)
        delete [] m_x_scale;
}

void NormalAnalyzer::start()
{
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
        m_starTimer->start();
    }
}

void NormalAnalyzer::stop()
{
    m_running = false;
    m_timer->stop();
    m_starTimer->stop();
    clear();
}

void NormalAnalyzer::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void NormalAnalyzer::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void NormalAnalyzer::starTimeout()
{
    for(int i=0; i<m_starPoints.count(); ++i)
    {
        StarPoint *sp = &m_starPoints[i];
        sp->m_alpha = rand()%255;
        sp->m_pt = QPoint(rand()%width(), rand()%height());
    }
}

void NormalAnalyzer::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalAnalyzer");
    m_peaks_falloff = settings.value("peak_falloff", 0.2).toDouble();
    m_analyzer_falloff = settings.value("analyzer_falloff", 2.2).toDouble();
    m_show_peaks = settings.value("show_peaks", true).toBool();
    m_timer->setInterval(1000 / settings.value("refresh_rate", 25).toInt());
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    m_starAction->setChecked(settings.value("show_star", false).toBool());
    m_starColor = ColorWidget::readSingleColorConfig(settings.value("star_color").toString());

    if(!m_update)
    {
        m_update = true;
        m_peaksAction->setChecked(m_show_peaks);

        foreach(QAction *act, m_fpsGroup->actions ())
        {
            if (m_timer->interval() == 1000 / act->data().toInt())
                act->setChecked(true);
        }
        foreach(QAction *act, m_peaksFalloffGroup->actions ())
        {
            if (m_peaks_falloff == act->data().toDouble())
                act->setChecked(true);
        }
        foreach(QAction *act, m_analyzerFalloffGroup->actions ())
        {
            if (m_analyzer_falloff == act->data().toDouble())
                act->setChecked(true);
        }

        //fallback
        if(!m_fpsGroup->checkedAction())
        {
            m_fpsGroup->actions().at(1)->setChecked(true);
            m_timer->setInterval(1000 / 25);
        }
        if(!m_peaksFalloffGroup->checkedAction())
        {
            m_peaksFalloffGroup->actions().at(1)->setChecked(2);
            m_peaks_falloff = 0.2;
        }
        if(!m_peaksFalloffGroup->checkedAction())
        {
            m_peaksFalloffGroup->actions().at(1)->setChecked(2);
            m_analyzer_falloff = 2.2;
        }
    }
}

void NormalAnalyzer::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalAnalyzer");
    QAction *act = m_fpsGroup->checkedAction ();
    settings.setValue("refresh_rate", act ? act->data().toInt() : 25);
    act = m_peaksFalloffGroup->checkedAction ();
    settings.setValue("peak_falloff", act ? act->data().toDouble() : 0.2);
    act = m_analyzerFalloffGroup->checkedAction ();
    settings.setValue("analyzer_falloff", act ? act->data().toDouble() : 2.2);
    settings.setValue("show_peaks", m_peaksAction->isChecked());
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.setValue("show_star", m_starAction->isChecked());
    settings.setValue("star_color", ColorWidget::writeSingleColorConfig(m_starColor));
    settings.endGroup();
}

void NormalAnalyzer::changeColor()
{
    ColorWidget c;
    c.setColors(m_colors);
    if(c.exec())
    {
        m_colors = c.getColors();
    }
}

void NormalAnalyzer::changeStarState(bool state)
{
    m_starAction->setChecked(state);
    update();
}

void NormalAnalyzer::changeStarColor()
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

void NormalAnalyzer::hideEvent (QHideEvent *)
{
    m_timer->stop();
    m_starTimer->stop();
}

void NormalAnalyzer::showEvent (QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
        m_starTimer->start();
    }
}

void NormalAnalyzer::paintEvent (QPaintEvent * e)
{
    QPainter painter (this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void NormalAnalyzer::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        m_menu->exec(e->globalPos());
}

void NormalAnalyzer::process ()
{
    static fft_state *state = 0;
    if (!state)
        state = fft_init();

    int rows = (height() - 2) / m_cell_size.height();
    int cols = (width() - 2) / m_cell_size.width() / 2;

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        if(m_peaks)
            delete [] m_peaks;
        if(m_intern_vis_data)
            delete [] m_intern_vis_data;
        if(m_x_scale)
            delete [] m_x_scale;
        m_peaks = new double[m_cols * 2];
        m_intern_vis_data = new double[m_cols * 2];
        m_x_scale = new int[m_cols + 1];

        for(int i = 0; i < m_cols * 2; ++i)
        {
            m_peaks[i] = 0;
            m_intern_vis_data[i] = 0;
        }
        for(int i = 0; i < m_cols + 1; ++i)
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
    }

    short dest_l[256];
    short dest_r[256];
    short yl, yr;
    int j, k, magnitude_l, magnitude_r;

    calc_freq (dest_l, m_left_buffer);
    calc_freq (dest_r, m_right_buffer);

    double y_scale = (double) 1.25 * m_rows / log(256);

    for (int i = 0; i < m_cols; i++)
    {
        j = m_cols * 2 - i - 1; //mirror index
        yl = yr = 0;
        magnitude_l = magnitude_r = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            yl = dest_l[i];
            yr = dest_r[i];
        }
        for (k = m_x_scale[i]; k < m_x_scale[i + 1]; k++)
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

        m_intern_vis_data[i] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[i] = magnitude_l > m_intern_vis_data[i] ? magnitude_l : m_intern_vis_data[i];

        m_intern_vis_data[j] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[j] = magnitude_r > m_intern_vis_data[j] ? magnitude_r : m_intern_vis_data[j];

        if (m_show_peaks)
        {
            m_peaks[i] -= m_peaks_falloff * m_rows / 15;
            m_peaks[i] = magnitude_l > m_peaks[i] ? magnitude_l : m_peaks[i];

            m_peaks[j] -= m_peaks_falloff * m_rows / 15;
            m_peaks[j] = magnitude_r > m_peaks[j] ? magnitude_r : m_peaks[j];
        }
    }
}

void NormalAnalyzer::draw (QPainter *p)
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

    for (int j = 0; j < m_cols * 2; ++j)
    {
        x = j * m_cell_size.width() + 1;
        if(j >= m_cols)
        {
            x += rdx; //correct right part position
        }

        for (int i = 0; i <= m_intern_vis_data[j]*l; ++i)
        {
            p->fillRect (x, height() - i * m_cell_size.height() + 1,
                         m_cell_size.width() - 2, m_cell_size.height() - 2, line);
        }

        if (m_show_peaks)
        {
            p->fillRect (x, height() - int(m_peaks[j]*l)*m_cell_size.height() + 1,
                         m_cell_size.width() - 2, m_cell_size.height() - 2, "Cyan");
        }
    }
}

void NormalAnalyzer::createMenu()
{
    m_menu = new QMenu (this);
    connect(m_menu, SIGNAL(triggered (QAction *)),SLOT(writeSettings()));
    connect(m_menu, SIGNAL(triggered (QAction *)),SLOT(readSettings()));

    m_peaksAction = m_menu->addAction(tr("Peaks"));
    m_peaksAction->setCheckable(true);

    QMenu *refreshRate = m_menu->addMenu(tr("Refresh Rate"));
    m_fpsGroup = new QActionGroup(this);
    m_fpsGroup->setExclusive(true);
    m_fpsGroup->addAction(tr("50 fps"))->setData(50);
    m_fpsGroup->addAction(tr("25 fps"))->setData(25);
    m_fpsGroup->addAction(tr("10 fps"))->setData(10);
    m_fpsGroup->addAction(tr("5 fps"))->setData(5);
    foreach(QAction *act, m_fpsGroup->actions ())
    {
        act->setCheckable(true);
        refreshRate->addAction(act);
    }

    QMenu *NormalAnalyzerFalloff = m_menu->addMenu(tr("Analyzer Falloff"));
    m_analyzerFalloffGroup = new QActionGroup(this);
    m_analyzerFalloffGroup->setExclusive(true);
    m_analyzerFalloffGroup->addAction(tr("Slowest"))->setData(1.2);
    m_analyzerFalloffGroup->addAction(tr("Slow"))->setData(1.8);
    m_analyzerFalloffGroup->addAction(tr("Medium"))->setData(2.2);
    m_analyzerFalloffGroup->addAction(tr("Fast"))->setData(2.4);
    m_analyzerFalloffGroup->addAction(tr("Fastest"))->setData(2.8);
    foreach(QAction *act, m_analyzerFalloffGroup->actions ())
    {
        act->setCheckable(true);
        NormalAnalyzerFalloff->addAction(act);
    }

    QMenu *peaksFalloff = m_menu->addMenu(tr("Peaks Falloff"));
    m_peaksFalloffGroup = new QActionGroup(this);
    m_peaksFalloffGroup->setExclusive(true);
    m_peaksFalloffGroup->addAction(tr("Slowest"))->setData(0.05);
    m_peaksFalloffGroup->addAction(tr("Slow"))->setData(0.1);
    m_peaksFalloffGroup->addAction(tr("Medium"))->setData(0.2);
    m_peaksFalloffGroup->addAction(tr("Fast"))->setData(0.4);
    m_peaksFalloffGroup->addAction(tr("Fastest"))->setData(0.8);
    foreach(QAction *act, m_peaksFalloffGroup->actions ())
    {
        act->setCheckable(true);
        peaksFalloff->addAction(act);
    }

    m_menu->addAction("Color", this, SLOT(changeColor()));
    m_menu->addAction(m_starAction);
    m_menu->addAction("StarColor", this, SLOT(changeStarColor()));

    update();
}
