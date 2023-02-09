#include "flowvumeter.h"

#include <QMenu>
#include <QPainter>
#include <qmath.h>

FlowVUMeter::FlowVUMeter(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow VU Meter Widget"));

    m_pixmap.load(":/data/vu");
    setMinimumSize(m_pixmap.size());
}

FlowVUMeter::~FlowVUMeter()
{

}

void FlowVUMeter::rangeChanged(QAction *action)
{
    m_rangeValue = action->data().toInt();
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
    painter.drawLine(x, y, x + radius * cos(offset), y + radius * sin(offset));
}

void FlowVUMeter::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);

    QMenu rangeMenu(tr("Range"), &menu);
    rangeMenu.addAction(tr("30 DB"))->setData(30);
    rangeMenu.addAction(tr("50 DB"))->setData(50);
    rangeMenu.addAction(tr("70 DB"))->setData(70);
    rangeMenu.addAction(tr("90 DB"))->setData(90);
    rangeMenu.addAction(tr("110 DB"))->setData(110);
    rangeMenu.addAction(tr("130 DB"))->setData(130);
    rangeMenu.addAction(tr("150 DB"))->setData(150);
    rangeMenu.addAction(tr("170 DB"))->setData(170);
    connect(&rangeMenu, SIGNAL(triggered(QAction*)), this, SLOT(rangeChanged(QAction*)));
    menu.addMenu(&rangeMenu);
    menu.exec(QCursor::pos());
}

void FlowVUMeter::process(float *left, float *right)
{
    const int channels = qBound(m_channels, 1, CHANNEL_MAX);

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
        const float db = m_rangeValue + (20.0f * log10f(peaks[i]));

        if(db > m_values[i])
        {
            m_values[i] = db;
        }
    }
}
