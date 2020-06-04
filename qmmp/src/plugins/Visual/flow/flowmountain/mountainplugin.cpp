#include <QMenu>
#include <QBoxLayout>
#include "mountainwidget.h"
#include "mountainplugin.h"

MountainPlugin::MountainPlugin(QWidget *parent)
    : Florid(parent)
{
    m_useImage = false;
    m_running = false;

    setWindowTitle(tr("Flow Mountain Widget"));

    m_mountainWidget = new MountainWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_mountainWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_timer = new QTimer(this);
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

MountainPlugin::~MountainPlugin()
{
}

void MountainPlugin::start()
{
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
}

void MountainPlugin::stop()
{
    m_running = false;
    m_timer->stop();
}

void MountainPlugin::timeout()
{
    if(takeData(m_left, m_right))
    {
        for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; i++)
        {
            m_buf[0][i] = m_left[i];
        }

        m_mountainWidget->addBuffer(m_buf[0]);
    }
}

void MountainPlugin::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void MountainPlugin::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void MountainPlugin::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}
