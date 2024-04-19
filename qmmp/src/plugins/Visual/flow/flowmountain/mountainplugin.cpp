#include "mountainplugin.h"
#include "mountainwidget.h"
#include "inlines.h"

#include <QMenu>
#include <QBoxLayout>

MountainPlugin::MountainPlugin(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow Mountain Widget"));

    m_mountainWidget = new MountainWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_mountainWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void MountainPlugin::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}

void MountainPlugin::process(float *left, float *)
{
    constexpr int size = QMMP_VISUAL_NODE_SIZE / 2;
    short dest_l[size];

    calc_freq(dest_l, left);

    float buffer[size];
    for(int i = 0; i < size; ++i)
    {
        buffer[i] = dest_l[i] / ((QMMP_VISUAL_NODE_SIZE << 8) / (8.0 / 2));
    }

    m_mountainWidget->addBuffer(buffer);
}
