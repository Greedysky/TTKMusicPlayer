#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>
#include <QPaintEvent>
#include <math.h>
#include <qmmp/qmmp.h>

#include "inlines.h"
#include "normalanalyzer.h"

NormalAnalyzer::NormalAnalyzer(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Normal Analyzer Widget"));
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

    createMenu();
    readSettings();
}

NormalAnalyzer::~NormalAnalyzer()
{
    qDeleteAll(m_starPoints);
    if(m_peaks)
    {
        delete[] m_peaks;
    }

    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void NormalAnalyzer::start()
{
    Visual::start();
    if(isVisible())
    {
        m_starTimer->start();
    }
}

void NormalAnalyzer::stop()
{
    Visual::stop();
    m_starTimer->stop();
}

void NormalAnalyzer::starTimeout()
{
    for(StarPoint *point : qAsConst(m_starPoints))
    {
        point->m_alpha = qrand() % 255;
        point->m_pt = QPoint(qrand() % width(), qrand() % height());
    }
}

void NormalAnalyzer::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalAnalyzer");
    m_peaks_falloff = settings.value("peak_falloff", 0.2).toDouble();
    m_analyzer_falloff = settings.value("analyzer_falloff", 2.2).toDouble();
    m_show_peaks = settings.value("show_peaks", true).toBool();
    m_timer->setInterval(1000 / settings.value("refresh_rate", 30).toInt());
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    m_starAction->setChecked(settings.value("show_star", false).toBool());
    m_starColor = ColorWidget::readSingleColorConfig(settings.value("star_color").toString());
    settings.endGroup();

    if(m_update)
    {
        return;
    }

    m_update = true;
    m_peaksAction->setChecked(m_show_peaks);

    for(QAction *act : m_fpsGroup->actions())
    {
        if(m_timer->interval() == 1000 / act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }

    for(QAction *act : m_peaksFalloffGroup->actions())
    {
        if(m_peaks_falloff == act->data().toDouble())
        {
            act->setChecked(true);
            break;
        }
    }

    for(QAction *act : m_analyzerFalloffGroup->actions())
    {
        if(m_analyzer_falloff == act->data().toDouble())
        {
            act->setChecked(true);
            break;
        }
    }

    //fallback
    if(!m_fpsGroup->checkedAction())
    {
        m_fpsGroup->actions().at(3)->setChecked(true);
        m_timer->setInterval(QMMP_VISUAL_INTERVAL);
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

void NormalAnalyzer::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalAnalyzer");
    QAction *act = m_fpsGroup->checkedAction();
    settings.setValue("refresh_rate", act ? act->data().toInt() : 30);
    act = m_peaksFalloffGroup->checkedAction();
    settings.setValue("peak_falloff", act ? act->data().toDouble() : 0.2);
    act = m_analyzerFalloffGroup->checkedAction();
    settings.setValue("analyzer_falloff", act ? act->data().toDouble() : 2.2);
    settings.setValue("show_peaks", m_peaksAction->isChecked());
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.setValue("show_star", m_starAction->isChecked());
    settings.setValue("star_color", ColorWidget::writeSingleColorConfig(m_starColor));
    settings.endGroup();

    readSettings();
}

void NormalAnalyzer::changeColor()
{
    ColorWidget c;
    c.setColors(m_colors);
    if(c.exec())
    {
        m_colors = c.colors();
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
    c.setSingleColorMode(true);
    c.setColor(m_starColor);
    if(c.exec())
    {
        m_starColor = c.color();
        update();
    }
}

void NormalAnalyzer::hideEvent(QHideEvent *e)
{
    Visual::hideEvent(e);
    m_starTimer->stop();
}

void NormalAnalyzer::showEvent(QShowEvent *e)
{
    Visual::showEvent(e);
    if(m_running)
    {
        m_starTimer->start();
    }
}

void NormalAnalyzer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void NormalAnalyzer::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
        m_menu->exec(e->globalPos());
    }
}

void NormalAnalyzer::process(float *left, float *right)
{
    const int rows = (height() - 2) / m_cell_size.height();
    const int cols = (width() - 2) / m_cell_size.width() / 2;

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_peaks)
        {
            delete[] m_peaks;
        }

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        if(m_x_scale)
        {
            delete[] m_x_scale;
        }

        m_peaks = new double[m_cols * 2]{0};
        m_intern_vis_data = new int[m_cols * 2]{0};
        m_x_scale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest_l[256];
    short dest_r[256];

    calc_freq(dest_l, left);
    calc_freq(dest_r, right);

    const double y_scale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        int j = m_cols * 2 - i - 1; //mirror index
        short yl = 0;
        short yr = 0;
        int magnitude_l = 0;
        int magnitude_r = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            yl = dest_l[i];
            yr = dest_r[i];
        }

        for(int k = m_x_scale[i]; k < m_x_scale[i + 1]; ++k)
        {
            yl = qMax(dest_l[k], yl);
            yr = qMax(dest_r[k], yr);
        }

        yl >>= 7; //256
        yr >>= 7;

        if(yl)
        {
            magnitude_l = int(log(yl) * y_scale);
            magnitude_l = qBound(0, magnitude_l, m_rows);
        }

        if(yr)
        {
            magnitude_r = int(log(yr) * y_scale);
            magnitude_r = qBound(0, magnitude_r, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[i] = magnitude_l > m_intern_vis_data[i] ? magnitude_l : m_intern_vis_data[i];

        m_intern_vis_data[j] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[j] = magnitude_r > m_intern_vis_data[j] ? magnitude_r : m_intern_vis_data[j];

        if(m_show_peaks)
        {
            m_peaks[i] -= m_peaks_falloff * m_rows / 15;
            m_peaks[i] = magnitude_l > m_peaks[i] ? magnitude_l : m_peaks[i];

            m_peaks[j] -= m_peaks_falloff * m_rows / 15;
            m_peaks[j] = magnitude_r > m_peaks[j] ? magnitude_r : m_peaks[j];
        }
    }
}

void NormalAnalyzer::draw(QPainter *p)
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

    const int rdx = qMax(0, width() - 2 * m_cell_size.width() * m_cols);

    for(int i = 0; i < m_cols * 2; ++i)
    {
        int x = i * m_cell_size.width() + 1;
        if(i >= m_cols)
        {
            x += rdx; //correct right part position
        }

        for(int j = 0; j <= m_intern_vis_data[i]; ++j)
        {
            p->fillRect(x, height() - j * m_cell_size.height() + 1, m_cell_size.width() - 2, m_cell_size.height() - 2, line);
        }

        if(m_show_peaks)
        {
            p->fillRect(x, height() - int(m_peaks[i]) * m_cell_size.height() + 1, m_cell_size.width() - 2, m_cell_size.height() - 2, "Cyan");
        }
    }
}

void NormalAnalyzer::createMenu()
{
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    m_peaksAction = m_menu->addAction(tr("Peaks"));
    m_peaksAction->setCheckable(true);

    QMenu *refreshRate = m_menu->addMenu(tr("Refresh Rate"));
    m_fpsGroup = new QActionGroup(this);
    m_fpsGroup->setExclusive(true);
    m_fpsGroup->addAction(tr("60 fps"))->setData(60);
    m_fpsGroup->addAction(tr("50 fps"))->setData(50);
    m_fpsGroup->addAction(tr("40 fps"))->setData(40);
    m_fpsGroup->addAction(tr("30 fps"))->setData(30);
    m_fpsGroup->addAction(tr("20 fps"))->setData(20);
    for(QAction *act : m_fpsGroup->actions())
    {
        act->setCheckable(true);
        refreshRate->addAction(act);
    }

    QMenu *analyzerFalloff = m_menu->addMenu(tr("Analyzer Falloff"));
    m_analyzerFalloffGroup = new QActionGroup(this);
    m_analyzerFalloffGroup->setExclusive(true);
    m_analyzerFalloffGroup->addAction(tr("Slowest"))->setData(1.2);
    m_analyzerFalloffGroup->addAction(tr("Slow"))->setData(1.8);
    m_analyzerFalloffGroup->addAction(tr("Medium"))->setData(2.2);
    m_analyzerFalloffGroup->addAction(tr("Fast"))->setData(2.4);
    m_analyzerFalloffGroup->addAction(tr("Fastest"))->setData(2.8);
    for(QAction *act : m_analyzerFalloffGroup->actions())
    {
        act->setCheckable(true);
        analyzerFalloff->addAction(act);
    }

    QMenu *peaksFalloff = m_menu->addMenu(tr("Peaks Falloff"));
    m_peaksFalloffGroup = new QActionGroup(this);
    m_peaksFalloffGroup->setExclusive(true);
    m_peaksFalloffGroup->addAction(tr("Slowest"))->setData(0.05);
    m_peaksFalloffGroup->addAction(tr("Slow"))->setData(0.1);
    m_peaksFalloffGroup->addAction(tr("Medium"))->setData(0.2);
    m_peaksFalloffGroup->addAction(tr("Fast"))->setData(0.4);
    m_peaksFalloffGroup->addAction(tr("Fastest"))->setData(0.8);
    for(QAction *act : m_peaksFalloffGroup->actions())
    {
        act->setCheckable(true);
        peaksFalloff->addAction(act);
    }

    m_menu->addSeparator();
    m_menu->addAction(m_screenAction);
    m_menu->addSeparator();
    m_menu->addAction(tr("Color"), this, SLOT(changeColor()));
    m_menu->addAction(m_starAction);
    m_menu->addAction(tr("StarColor"), this, SLOT(changeStarColor()));

    update();
}
