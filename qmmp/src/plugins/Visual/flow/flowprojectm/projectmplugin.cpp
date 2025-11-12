#include "projectmplugin.h"
#ifdef PROJECTM_4
#include "projectm4widget.h"
#else
#include "projectmwidget.h"
#endif

#include <QMenu>
#include <QSplitter>
#include <QScrollBar>
#include <QBoxLayout>

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
#ifdef PROJECTM_4
    m_container = new ProjectM4Widget(m_itemWidget, m_splitter);
#else
    m_container = new ProjectMWidget(m_itemWidget, m_splitter);
#endif
    m_splitter->addWidget(m_itemWidget);
    m_splitter->addWidget(m_container);
    m_splitter->setStretchFactor(1, 1);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_splitter);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_menu = new QMenu(this);
    m_menu->addAction(m_screenAction);
    m_menu->addSeparator();
#ifndef PROJECTM_4
    m_menu->addAction(tr("&Help"), m_container, SLOT(showHelp()), {"F1"})->setCheckable(true);
    m_menu->addAction(tr("&Show Song Title"), m_container, SLOT(showTitle()), {"F2"})->setCheckable(true);
    m_menu->addAction(tr("&Show Preset Name"), m_container, SLOT(showPresetName()), {"F3"})->setCheckable(true);
#endif
    m_menu->addAction(tr("&Show Menu"), m_itemWidget, SLOT(setVisible(bool)), {"M"})->setCheckable(true);
    m_menu->addSeparator();
    m_menu->addAction(tr("&Next Preset"), m_container, SLOT(nextPreset()), {"N"});
    m_menu->addAction(tr("&Previous Preset"), m_container, SLOT(previousPreset()), {"P"});
#ifdef PROJECTM_4
    m_menu->addAction(tr("&Shuffle"), m_container, SLOT(setShuffle(bool)), {"R"})->setCheckable(true);
#else
    m_menu->addAction(tr("&Random Preset"), m_container, SLOT(randomPreset()), {"R"});
#endif
    m_menu->addAction(tr("&Lock Preset"), m_container, SLOT(lockPreset(bool)), {"L"})->setCheckable(true);
}

void ProjectMPlugin::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
}

void ProjectMPlugin::processData(float *left, float *right)
{
    m_container->addBuffer(left, right);
    m_container->update();
}
