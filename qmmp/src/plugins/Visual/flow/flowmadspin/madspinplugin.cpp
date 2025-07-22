#include "madspinplugin.h"
#include "madspinwidget.h"
#include "inlines.h"

#include <QMenu>
#include <QBoxLayout>

static void actionChecked(QAction *action, int value, int data)
{
    action->setData(value);
    action->setCheckable(true);
    action->setChecked(data == value);
}


MadSpinPlugin::MadSpinPlugin(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow MadSpin Widget"));

    m_container = new MadSpinWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_container);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void MadSpinPlugin::numChanged(QAction *action)
{
    m_container->setStars(action->data().toInt());
}

void MadSpinPlugin::speedChanged(QAction *action)
{
    m_container->setSpeed(action->data().toInt());
}

void MadSpinPlugin::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.addSeparator();

    QMenu numMenu(tr("Number"), &menu);
    const int num = m_container->getStars();
    actionChecked(numMenu.addAction(tr("Slowest")), 512, num);
    actionChecked(numMenu.addAction(tr("Slow")), 1024, num);
    actionChecked(numMenu.addAction(tr("Medium")), 2048, num);
    actionChecked(numMenu.addAction(tr("Fast")), 4096, num);
    actionChecked(numMenu.addAction(tr("Fastest")), 8192, num);
    connect(&numMenu, SIGNAL(triggered(QAction*)), this, SLOT(numChanged(QAction*)));
    menu.addMenu(&numMenu);

    QMenu speedMenu(tr("Speed"), &menu);
    const int speed = m_container->getSpeed();
    actionChecked(speedMenu.addAction(tr("Slowest")), 1000, speed);
    actionChecked(speedMenu.addAction(tr("Slow")), 2500, speed);
    actionChecked(speedMenu.addAction(tr("Medium")), 4000, speed);
    actionChecked(speedMenu.addAction(tr("Fast")), 6000, speed);
    actionChecked(speedMenu.addAction(tr("Fastest")), 8200, speed);
    connect(&speedMenu, SIGNAL(triggered(QAction*)), this, SLOT(speedChanged(QAction*)));
    menu.addMenu(&speedMenu);
    menu.exec(QCursor::pos());
}

void MadSpinPlugin::processData(float *left, float *)
{
    constexpr int size = QMMP_VISUAL_NODE_SIZE / 2;
    short dest[size];

    calc_freq(dest, left);

    float buffer[size];
    for(int i = 0; i < size; ++i)
    {
        buffer[i] = dest[i] / ((QMMP_VISUAL_NODE_SIZE << 8) / (8.0 / 2));
    }

    m_container->addBuffer(buffer);
}
