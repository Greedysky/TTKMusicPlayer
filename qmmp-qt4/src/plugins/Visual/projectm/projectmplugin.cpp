/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QMenu>
#include <QActionGroup>
#include <QHBoxLayout>
#include <QSplitter>
#include <QListWidget>
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <libprojectM/projectM.hpp>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include "projectmwidget.h"
#include "projectmplugin.h"

ProjectMPlugin::ProjectMPlugin (QWidget *parent)
        : Visual (parent, Qt::Window | Qt::MSWindowsOwnDC)
{
    setlocale(LC_NUMERIC, "C"); //fixes problem with none-english locales
    setWindowTitle(tr("ProjectM"));
    setWindowIcon(parent->windowIcon());

    m_splitter = new QSplitter(Qt::Horizontal, this);
    QListWidget *listWidget = new QListWidget(m_splitter);
    listWidget->setAlternatingRowColors(true);
    m_splitter->addWidget(listWidget);
    m_projectMWidget = new ProjectMWidget(listWidget, m_splitter);
    m_splitter->addWidget(m_projectMWidget);

    m_splitter->setStretchFactor(1,1);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_splitter);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    addActions(m_projectMWidget->actions());
    connect(m_projectMWidget, SIGNAL(showMenuToggled(bool)), listWidget, SLOT(setVisible(bool)));
    connect(m_projectMWidget, SIGNAL(fullscreenToggled(bool)), SLOT(setFullScreen(bool)));
    listWidget->hide();
    resize(600,400);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    restoreGeometry(settings.value("ProjectM/geometry").toByteArray());
    m_splitter->setSizes(QList<int>() << 300 << 300);
    m_splitter->restoreState(settings.value("ProjectM/splitter_sizes").toByteArray());
}

ProjectMPlugin::~ProjectMPlugin()
{}

void ProjectMPlugin::clear()
{
    update();
}

void ProjectMPlugin::setFullScreen(bool yes)
{
    if(yes)
        setWindowState(windowState() | Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);
}

void ProjectMPlugin::add (float *data, size_t samples, int chan)
{
    projectM *instance = m_projectMWidget->projectMInstance();
    if (!instance)
        return;

    size_t frames = qMin(samples / chan, (size_t)512);

    if(chan == 1)
    {
        for(size_t i = 0; i < frames; i++)
        {
            m_buf[0][i] = data[i*chan] * 32767.0;
            m_buf[1][i] = data[i*chan] * 32767.0;
        }

    }
    else
    {
        for(size_t i = 0; i < frames; i++)
        {
            m_buf[0][i] = data[i*chan] * 32767.0;
            m_buf[1][i] = data[i*chan+1] * 32767.0;
        }
    }

    m_projectMWidget->projectMInstance()->pcm()->addPCM16(m_buf);
}

void ProjectMPlugin::closeEvent (QCloseEvent *event)
{
    //save geometry
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("ProjectM/geometry", saveGeometry());
    settings.setValue("ProjectM/splitter_sizes", m_splitter->saveState());
    Visual::closeEvent(event); //removes visualization object
}
