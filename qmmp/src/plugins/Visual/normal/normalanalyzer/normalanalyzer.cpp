#include "normalanalyzer.h"
#include "inlines.h"

#include <QMenu>
#include <QTimer>
#include <QPainter>
#include <QSettings>
#include <math.h>
#include <qmmp/qmmp.h>

NormalAnalyzer::NormalAnalyzer(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Normal Analyzer Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    createMenu();
    readSettings();
}

NormalAnalyzer::~NormalAnalyzer()
{
    delete[] m_peaks;
    delete[] m_xscale;
}

void NormalAnalyzer::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalAnalyzer");
    m_peakSize = settings.value("peak_falloff", 0.2).toDouble();
    m_analyzerSize = settings.value("analyzer_falloff", 2.2).toDouble();
    m_showPeaks = settings.value("show_peaks", true).toBool();
    m_timer->setInterval(1000 / settings.value("refresh_rate", 30).toInt());
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    settings.endGroup();

    m_peaksAction->setChecked(m_showPeaks);

    for(QAction *act : m_fpsActions->actions())
    {
        if(m_timer->interval() == 1000 / act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }

    for(QAction *act : m_peakActions->actions())
    {
        if(m_peakSize == act->data().toDouble())
        {
            act->setChecked(true);
            break;
        }
    }

    for(QAction *act : m_analyzerActions->actions())
    {
        if(m_analyzerSize == act->data().toDouble())
        {
            act->setChecked(true);
            break;
        }
    }

    if(!m_fpsActions->checkedAction())
    {
        m_fpsActions->actions().at(3)->setChecked(true);
        m_timer->setInterval(QMMP_VISUAL_INTERVAL);
    }

    if(!m_peakActions->checkedAction())
    {
        m_peakActions->actions().at(1)->setChecked(2);
        m_peakSize = 0.2;
    }

    if(!m_peakActions->checkedAction())
    {
        m_peakActions->actions().at(1)->setChecked(2);
        m_analyzerSize = 2.2;
    }
}

void NormalAnalyzer::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NormalAnalyzer");
    QAction *act = m_fpsActions->checkedAction();
    const int fps = act ? act->data().toInt() : 30;
    settings.setValue("refresh_rate", fps);
    act = m_peakActions->checkedAction();
    settings.setValue("peak_falloff", m_peakSize = (act ? act->data().toDouble() : 0.2));
    act = m_analyzerActions->checkedAction();
    settings.setValue("analyzer_falloff", m_analyzerSize = (act ? act->data().toDouble() : 2.2));
    settings.setValue("show_peaks", m_showPeaks = m_peaksAction->isChecked());
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.endGroup();

    m_timer->setInterval(1000 / fps);
}

void NormalAnalyzer::changeColor()
{
    ColorWidget dialog;
    dialog.setColors(m_colors);
    if(dialog.exec())
    {
        m_colors = dialog.colors();
    }
}

void NormalAnalyzer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHints(QPainter::Antialiasing);

    QLinearGradient line(0, 0, 0, height());
    for(int i = 0; i < m_colors.count(); ++i)
    {
        line.setColorAt((i + 1) * 1.0 / m_colors.count(), m_colors[i]);
    }

    const int rdx = qMax(0, width() - 2 * m_cellSize.width() * m_cols);

    for(int i = 0; i < m_cols * 2; ++i)
    {
        int x = i * m_cellSize.width() + 1;
        if(i >= m_cols)
        {
            x += rdx; //correct right part position
        }

        for(int j = 0; j <= m_visualData[i]; ++j)
        {
            painter.fillRect(x, height() - j * m_cellSize.height() + 1, m_cellSize.width() - 2, m_cellSize.height() - 2, line);
        }

        if(m_showPeaks)
        {
            painter.fillRect(x, height() - int(m_peaks[i]) * m_cellSize.height() + 1, m_cellSize.width() - 2, m_cellSize.height() - 2, "Cyan");
        }
    }
}

void NormalAnalyzer::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
}

void NormalAnalyzer::processData(float *left, float *right)
{
    const int rows = (height() - 2) / m_cellSize.height();
    const int cols = (width() - 2) / m_cellSize.width() / 2;

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_peaks;
        delete[] m_visualData;
        delete[] m_xscale;

        m_peaks = new double[m_cols * 2]{0};
        m_visualData = new int[m_cols * 2]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = pow(255.0, float(i) / m_cols);
        }
    }

    short destl[256], destr[256];
    calc_freq(destl, left);
    calc_freq(destr, right);

    const double yscale = (double)1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        short yl = 0, yr = 0;
        int magnitudel = 0, magnituder = 0;

        if(m_xscale[i] == m_xscale[i + 1])
        {
            yl = (i >= 256 ? 0 : (destl[i] >> 7)); //128
            yr = (i >= 256 ? 0 : (destr[i] >> 7)); //128
        }

        for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            yl = (k >= 256 ? 0 : qMax(short(destl[k] >> 7), yl));
            yr = (k >= 256 ? 0 : qMax(short(destr[k] >> 7), yl));
        }

        if(yl > 0)
        {
            magnitudel = qBound(0, int(log(yl) * yscale), m_rows);
        }

        if(yr > 0)
        {
            magnituder = qBound(0, int(log(yr) * yscale), m_rows);
        }

        m_visualData[i] -= m_analyzerSize * m_rows / 15;
        m_visualData[i] = magnitudel > m_visualData[i] ? magnitudel : m_visualData[i];

        const int j = m_cols * 2 - i - 1; //mirror index
        m_visualData[j] -= m_analyzerSize * m_rows / 15;
        m_visualData[j] = magnituder > m_visualData[j] ? magnituder : m_visualData[j];

        if(m_showPeaks)
        {
            m_peaks[i] -= m_peakSize * m_rows / 15;
            m_peaks[i] = magnitudel > m_peaks[i] ? magnitudel : m_peaks[i];

            m_peaks[j] -= m_peakSize * m_rows / 15;
            m_peaks[j] = magnituder > m_peaks[j] ? magnituder : m_peaks[j];
        }
    }
}

void NormalAnalyzer::createMenu()
{
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    m_menu->addAction(m_screenAction);
    m_menu->addSeparator();

    m_peaksAction = m_menu->addAction(tr("Peaks"));
    m_peaksAction->setCheckable(true);

    QMenu *refreshMenu = m_menu->addMenu(tr("Refresh Rate"));
    m_fpsActions = new QActionGroup(this);
    m_fpsActions->setExclusive(true);
    m_fpsActions->addAction(tr("60 fps"))->setData(60);
    m_fpsActions->addAction(tr("50 fps"))->setData(50);
    m_fpsActions->addAction(tr("40 fps"))->setData(40);
    m_fpsActions->addAction(tr("30 fps"))->setData(30);
    m_fpsActions->addAction(tr("20 fps"))->setData(20);

    for(QAction *act : m_fpsActions->actions())
    {
        act->setCheckable(true);
        refreshMenu->addAction(act);
    }

    QMenu *analyzerMenu = m_menu->addMenu(tr("Analyzer Falloff"));
    m_analyzerActions = new QActionGroup(this);
    m_analyzerActions->setExclusive(true);
    m_analyzerActions->addAction(tr("Slowest"))->setData(1.2);
    m_analyzerActions->addAction(tr("Slow"))->setData(1.8);
    m_analyzerActions->addAction(tr("Medium"))->setData(2.2);
    m_analyzerActions->addAction(tr("Fast"))->setData(2.4);
    m_analyzerActions->addAction(tr("Fastest"))->setData(2.8);

    for(QAction *act : m_analyzerActions->actions())
    {
        act->setCheckable(true);
        analyzerMenu->addAction(act);
    }

    QMenu *peakMenu = m_menu->addMenu(tr("Peaks Falloff"));
    m_peakActions = new QActionGroup(this);
    m_peakActions->setExclusive(true);
    m_peakActions->addAction(tr("Slowest"))->setData(0.05);
    m_peakActions->addAction(tr("Slow"))->setData(0.1);
    m_peakActions->addAction(tr("Medium"))->setData(0.2);
    m_peakActions->addAction(tr("Fast"))->setData(0.4);
    m_peakActions->addAction(tr("Fastest"))->setData(0.8);

    for(QAction *act : m_peakActions->actions())
    {
        act->setCheckable(true);
        peakMenu->addAction(act);
    }

    m_menu->addSeparator();
    m_menu->addAction(tr("Color"), this, SLOT(changeColor()));
}
