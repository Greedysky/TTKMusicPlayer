#include <QBoxLayout>
#include "soniquewidget.h"
#include "soniqueplugin.h"

#include <QMenu>

SoniquePlugin::SoniquePlugin(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow Sonique Widget"));

    m_soniqueWidget = new SoniqueWidget(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_soniqueWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

SoniquePlugin::~SoniquePlugin()
{

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

void SoniquePlugin::process(float *left, float *right)
{
    m_soniqueWidget->addBuffer(left, right);
}
