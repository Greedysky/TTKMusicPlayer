#include <QBoxLayout>
#include "projectmwidget.h"
#include "projectmplugin.h"

ProjectMPlugin::ProjectMPlugin (QWidget *parent)
    : Florid (parent)
{
    m_running = false;

    setlocale(LC_NUMERIC, "C"); //fixes problem with non-english locales
    setWindowTitle(tr("Florid ProjectM Widget"));
    setWindowIcon(parent->windowIcon());

    m_projectMWidget = new ProjectMWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_projectMWidget);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    addActions(m_projectMWidget->actions());

    m_timer = new QTimer(this);
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

ProjectMPlugin::~ProjectMPlugin()
{
}

void ProjectMPlugin::start()
{
    m_running = true;
    if(isVisible())
        m_timer->start();
}

void ProjectMPlugin::stop()
{
    m_running = false;
    m_timer->stop();
}

void ProjectMPlugin::timeout()
{
    projectM *instance = m_projectMWidget->projectMInstance();
    if (!instance)
        return;

    if(takeData(m_left, m_right))
    {
        for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; i++)
        {
            m_buf[0][i] = m_left[i] * 32767.0;
            m_buf[1][i] = m_right[i] * 32767.0;
        }

        m_projectMWidget->projectMInstance()->pcm()->addPCM16(m_buf);
        m_projectMWidget->update();
    }
}

void ProjectMPlugin::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void ProjectMPlugin::hideEvent(QHideEvent *)
{
    m_timer->stop();
}
