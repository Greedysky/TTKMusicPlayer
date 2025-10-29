#include "flowvumeter.h"

#include <QMenu>
#include <QPainter>
#include <QSettings>
#include <qmath.h>
#include <qmmp/qmmp.h>

FlowVUMeter::FlowVUMeter(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow VU Meter Widget"));

    m_pixmap.load(":/data/vu");
    setMinimumSize(m_pixmap.size());

    createMenu();
    readSettings();
}

void FlowVUMeter::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("VUMeter");
    m_rangeValue = settings.value("range", 30).toInt();
    settings.endGroup();

    for(QAction *act : m_rangeActions->actions())
    {
        if(m_rangeValue == act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }
}

void FlowVUMeter::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("VUMeter");
    QAction *act = m_rangeActions->checkedAction();
    settings.setValue("range", m_rangeValue = (act ? act->data().toInt() : 30));
    settings.endGroup();
}

void FlowVUMeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.fillRect(rect(), Qt::black);

    const int w = (width() - m_pixmap.width()) / 2;
    const int h = (height() - m_pixmap.height()) / 2;
    painter.drawPixmap(w, h, m_pixmap);

    float value = 0;
    for(int c = 0; c < m_channels; ++c)
    {
        value = std::max(value, m_values[c]);
    }

    constexpr int radius = 130;
    const int x = w + m_pixmap.width() / 2;
    const int y = h + m_pixmap.height() - 174 / 2;
    const float offset = M_PI * (value / (m_rangeValue * 2.5) - 0.75);

    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(x, y, x + radius * std::cos(offset), y + radius * std::sin(offset));
}

void FlowVUMeter::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
}

void FlowVUMeter::processData(float *left, float *right)
{
    const int channels = qBound(m_channels, 1, MAX_CHANNELS);

    float peaks[channels];
    for(int i = 0; i < channels; ++i)
    {
        peaks[i] = fabsf(i == 0 ? left[0] : right[0]);
    }

    for(int i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
    {
        for(int j = 0; j < channels; ++j)
        {
            peaks[j] = fmaxf(peaks[j], fabsf(j == 0 ? left[i] : right[i]));
        }
    }

    for(int i = 0; i < channels; ++i)
    {
        m_values[i] = 0;
        const float db = m_rangeValue + (20.0f * std::log10(peaks[i]));

        if(db > m_values[i])
        {
            m_values[i] = db;
        }
    }
}

void FlowVUMeter::createMenu()
{
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    m_rangeActions = new QActionGroup(this);
    m_rangeActions->setExclusive(true);
    m_rangeActions->addAction(tr("30 DB"))->setData(30);
    m_rangeActions->addAction(tr("50 DB"))->setData(50);
    m_rangeActions->addAction(tr("70 DB"))->setData(70);
    m_rangeActions->addAction(tr("90 DB"))->setData(90);
    m_rangeActions->addAction(tr("110 DB"))->setData(110);
    m_rangeActions->addAction(tr("130 DB"))->setData(130);
    m_rangeActions->addAction(tr("150 DB"))->setData(150);
    m_rangeActions->addAction(tr("170 DB"))->setData(170);

    QMenu *rangeMenu = m_menu->addMenu(tr("Range"));
    for(QAction *act : m_rangeActions->actions())
    {
        act->setCheckable(true);
        rangeMenu->addAction(act);
    }
}

