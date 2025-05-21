#include "mountainplugin.h"
#include "mountainmidwidget.h"
#include "mountainsidewidget.h"
#include "inlines.h"

#include <QMenu>
#include <QBoxLayout>

static void actionChecked(QAction *action, int value, int data)
{
    action->setData(value);
    action->setCheckable(true);
    action->setChecked(data == value);
}


MountainPlugin::MountainPlugin(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow Mountain Widget"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    createMoudle(Type::Complex);
}

void MountainPlugin::positionChanged(QAction *action)
{
    createMoudle(static_cast<Type>(action->data().toInt()));
}

void MountainPlugin::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.addSeparator();

    QMenu positionMenu(tr("Type"), &menu);
    actionChecked(positionMenu.addAction(tr("Simple")), Type::Simple, m_type);
    actionChecked(positionMenu.addAction(tr("Complex")), Type::Complex, m_type);
    connect(&positionMenu, SIGNAL(triggered(QAction*)), this, SLOT(positionChanged(QAction*)));
    menu.addMenu(&positionMenu);
    menu.exec(QCursor::pos());
}

void MountainPlugin::processData(float *left, float *)
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

void MountainPlugin::createMoudle(Type type)
{
    m_type = type;

    if(m_mountainWidget)
    {
        layout()->removeWidget(m_mountainWidget);
        m_mountainWidget->hide();
        m_mountainWidget->deleteLater();
        m_mountainWidget = nullptr;
    }

    switch(m_type)
    {
    case Type::Simple:
        m_mountainWidget = new MountainSideWidget(this);
        break;
    case Type::Complex:
        m_mountainWidget = new MountainMidWidget(this);
        break;
    default:
        break;
    }

    layout()->addWidget(m_mountainWidget);
}
