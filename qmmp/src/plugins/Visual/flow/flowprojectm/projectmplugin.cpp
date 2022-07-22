#include "projectmplugin.h"
#include "projectmwidget.h"

#include <QMenu>
#include <QBoxLayout>
#include <QScrollBar>

ProjectMPlugin::ProjectMPlugin(QWidget *parent)
    : Visual(parent)
{
    setlocale(LC_NUMERIC, "C"); //fixes problem with non-english locales
    setWindowTitle(tr("Flow ProjectM Widget"));

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_itemWidget = new QListWidget(m_splitter);
    m_itemWidget->setVisible(false);
    m_itemWidget->setAlternatingRowColors(true);
    m_itemWidget->verticalScrollBar()->setStyleSheet(" \
            QScrollBar{ background:transparent; width:8px; padding-top:0px; padding-bottom:0px; } \
            QScrollBar::handle:vertical{ border-radius:4px; background:#CFCFCF; min-height: 30px; } \
            QScrollBar::handle:vertical::disabled{ background:#DBDBDB; } \
            QScrollBar::handle:vertical:hover{ background:#BBBBBB; } \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none; } \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none; }");
    m_itemWidget->horizontalScrollBar()->setStyleSheet(" \
            QScrollBar{ background:transparent; height:8px; padding-left:0px; padding-right:0px; } \
            QScrollBar::handle:horizontal{ border-radius:4px; background:#CFCFCF; min-width: 30px; } \
            QScrollBar::handle:horizontal::disabled{ background:#DBDBDB; } \
            QScrollBar::handle:horizontal:hover{ background:#BBBBBB; } \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none; } \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none; }");
    m_projectMWidget = new ProjectMWidget(m_itemWidget, m_splitter);
    m_splitter->addWidget(m_itemWidget);
    m_splitter->addWidget(m_projectMWidget);
    m_splitter->setStretchFactor(1, 1);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_splitter);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_menu = new QMenu(this);
    m_menu->addAction(m_screenAction);
    m_menu->addAction(tr("&Show Menu"), m_itemWidget, SLOT(setVisible(bool)), tr("M"))->setCheckable(true);
    m_menu->addSeparator();
    m_menu->addAction(tr("&Next Preset"), m_projectMWidget, SLOT(nextPreset()), tr("N"));
    m_menu->addAction(tr("&Previous Preset"), m_projectMWidget, SLOT(previousPreset()), tr("P"));
    m_menu->addAction(tr("&Random Preset"), m_projectMWidget, SLOT(randomPreset()), tr("R"));
    m_menu->addAction(tr("&Lock Preset"), m_projectMWidget, SLOT(lockPreset(bool)), tr("L"))->setCheckable(true);
}

ProjectMPlugin::~ProjectMPlugin()
{

}

void ProjectMPlugin::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
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
