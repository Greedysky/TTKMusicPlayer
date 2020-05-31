#include <QBoxLayout>
#include "soniquewidget.h"
#include "soniqueplugin.h"

#include <QMenu>

SoniquePlugin::SoniquePlugin(QWidget *parent)
    : Florid(parent)
{
    m_useImage = false;
    m_running = false;

    setWindowTitle(tr("Flow Sonique Widget"));

    m_soniqueWidget = new SoniqueWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_soniqueWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_timer = new QTimer(this);
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

SoniquePlugin::~SoniquePlugin()
{
}

void SoniquePlugin::start()
{
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
}

void SoniquePlugin::stop()
{
    m_running = false;
    m_timer->stop();
}

void SoniquePlugin::timeout()
{
    if(takeData(m_left, m_right))
    {
        float left[QMMP_VISUAL_NODE_SIZE];
        float right[QMMP_VISUAL_NODE_SIZE];
        for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; i++)
        {
            left[i] = m_left[i];
            right[i] = m_right[i];
        }

        m_soniqueWidget->addBuffer(left, right);
    }
}

void SoniquePlugin::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void SoniquePlugin::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void SoniquePlugin::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.addSeparator();
    menu.addAction(tr("&Next Preset"), m_soniqueWidget, SLOT(nextPreset()), tr("N"));
    menu.addAction(tr("&Previous Preset"), m_soniqueWidget, SLOT(previousPreset()), tr("P"));
    menu.addAction(tr("&Random Preset"), m_soniqueWidget, SLOT(randomPreset()), tr("R"));
    menu.exec(QCursor::pos());
}
