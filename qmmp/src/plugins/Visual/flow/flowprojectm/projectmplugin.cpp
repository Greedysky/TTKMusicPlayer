#include "projectmplugin.h"
#ifdef PROJECTM_4
#include "projectm4widget.h"
#else
#include "projectmwidget.h"
#endif

#include <QMenu>
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
    m_projectMWidget = new ProjectM4Widget(m_itemWidget, m_splitter);
#else
    m_projectMWidget = new ProjectMWidget(m_itemWidget, m_splitter);
#endif
    m_splitter->addWidget(m_itemWidget);
    m_splitter->addWidget(m_projectMWidget);
    m_splitter->setStretchFactor(1, 1);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_splitter);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    m_menu = new QMenu(this);
    m_menu->addAction(m_screenAction);
#ifndef PROJECTM_4
    m_menu->addAction(tr("&Help"), m_projectMWidget, SLOT(showHelp()), tr("F1"))->setCheckable(true);
    m_menu->addAction(tr("&Show Song Title"), m_projectMWidget, SLOT(showTitle()), tr("F2"))->setCheckable(true);
    m_menu->addAction(tr("&Show Preset Name"), m_projectMWidget, SLOT(showPresetName()), tr("F3"))->setCheckable(true);
#endif
    m_menu->addAction(tr("&Show Menu"), m_itemWidget, SLOT(setVisible(bool)), tr("M"))->setCheckable(true);
    m_menu->addSeparator();
    m_menu->addAction(tr("&Next Preset"), m_projectMWidget, SLOT(nextPreset()), tr("N"));
    m_menu->addAction(tr("&Previous Preset"), m_projectMWidget, SLOT(previousPreset()), tr("P"));
#ifdef PROJECTM_4
    m_menu->addAction(tr("&Shuffle"), m_projectMWidget, SLOT(setShuffle(bool)), tr("R"))->setCheckable(true);
#else
    m_menu->addAction(tr("&Random Preset"), m_projectMWidget, SLOT(randomPreset()), tr("R"));
#endif
    m_menu->addAction(tr("&Lock Preset"), m_projectMWidget, SLOT(lockPreset(bool)), tr("L"))->setCheckable(true);
}

void ProjectMPlugin::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
}

void ProjectMPlugin::processData(float *left, float *right)
{
    m_projectMWidget->addPCM(left, right);
    m_projectMWidget->update();
}
