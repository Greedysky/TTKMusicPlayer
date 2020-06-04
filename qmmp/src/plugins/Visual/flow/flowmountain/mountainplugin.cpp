#include <QMenu>
#include <QBoxLayout>
#include "mountainwidget.h"
#include "mountainplugin.h"

MountainPlugin::MountainPlugin(QWidget *parent)
    : Florid(parent)
{
    m_useImage = false;

    setWindowTitle(tr("Flow Mountain Widget"));

    m_mountainWidget = new MountainWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_mountainWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

MountainPlugin::~MountainPlugin()
{

}

void MountainPlugin::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}

void MountainPlugin::process(float *left, float *)
{
    m_mountainWidget->addBuffer(left);
}
