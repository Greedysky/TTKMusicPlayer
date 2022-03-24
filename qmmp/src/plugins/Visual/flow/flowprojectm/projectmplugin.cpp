#include "projectmplugin.h"
#include "projectmwidget.h"

#include <QMenu>
#include <QBoxLayout>

ProjectMPlugin::ProjectMPlugin(QWidget *parent)
    : Visual(parent)
{
    setlocale(LC_NUMERIC, "C"); //fixes problem with non-english locales
    setWindowTitle(tr("Flow ProjectM Widget"));

    m_projectMWidget = new ProjectMWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_projectMWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

ProjectMPlugin::~ProjectMPlugin()
{

}

void ProjectMPlugin::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.addSeparator();
    menu.addAction(tr("&Next Preset"), m_projectMWidget, SLOT(nextPreset()), tr("N"));
    menu.addAction(tr("&Previous Preset"), m_projectMWidget, SLOT(previousPreset()), tr("P"));
    menu.addAction(tr("&Random Preset"), m_projectMWidget, SLOT(randomPreset()), tr("R"));
    menu.exec(QCursor::pos());
}

void ProjectMPlugin::process(float *left, float *right)
{
    projectM *instance = m_projectMWidget->projectMInstance();
    if(!instance)
    {
        return;
    }

    short buf[2][QMMP_VISUAL_NODE_SIZE];
    for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
    {
        buf[0][i] = left[i] * 32767.0;
        buf[1][i] = right[i] * 32767.0;
    }

    m_projectMWidget->projectMInstance()->pcm()->addPCM16(buf);
    m_projectMWidget->update();
}
