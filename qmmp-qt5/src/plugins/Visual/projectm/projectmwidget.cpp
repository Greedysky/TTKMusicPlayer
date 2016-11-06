/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   Copyright (C) 2007 by  projectM team                                  *
 *                                                                         *
 *   Carmelo Piccione  carmelo.piccione+projectM@gmail.com                 *
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
#include <QDir>
#include <QKeyEvent>
#include <QMenu>
#include <QApplication>
#include <QListWidget>
#include <QOpenGLContext>
#include <qmmp/soundcore.h>
#include <qmmp/qmmp.h>
#include "projectmwrapper.h"
#include "projectmwidget.h"

#ifndef PROJECTM_CONFIG
#define PROJECTM_CONFIG "/usr/share/projectM/config.inp"
#endif

ProjectMWidget::ProjectMWidget(QListWidget *listWidget, QWidget *parent)
        : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    m_listWidget = listWidget;
    m_projectM = 0;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()),SLOT(update()));
    m_menu = new QMenu(this);
    connect(SoundCore::instance(), SIGNAL(metaDataChanged()), SLOT(updateTitle()));
    createActions();
}


ProjectMWidget::~ProjectMWidget()
{}

projectM *ProjectMWidget::projectMInstance()
{
    return m_projectM;
}

void ProjectMWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,0);
    // Setup our viewport
    glViewport(0, 0, width(), height());
    // Change to the projection matrix and set our viewing volume.
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glLineStipple(2, 0xAAAA);

    if (!m_projectM)
    {
#ifdef Q_OS_WIN
        projectM::Settings settings;
        settings.meshX = 32;
        settings.meshY = 24;
        settings.fps = 35;
        settings.textureSize = 1024;
        settings.windowWidth = 512;
        settings.windowHeight = 512;
        settings.presetURL = QString(qApp->applicationDirPath() + "/projectM/presets").toLocal8Bit().constData();
        settings.titleFontURL = QString(qApp->applicationDirPath() + "/projectM/fonts/Vera.ttf").toLocal8Bit().constData();
        settings.menuFontURL = QString(qApp->applicationDirPath() + "/projectM/fonts/VeraMono.ttf").toLocal8Bit().constData();
        settings.smoothPresetDuration = 5;
        settings.presetDuration = 30;
        settings.beatSensitivity = 1.0;
        settings.aspectCorrection = true;
        settings.easterEgg = 1.0;
        settings.shuffleEnabled = false;
        settings.softCutRatingsEnabled = false;
        m_projectM = new ProjectMWrapper(settings, projectM::FLAG_DISABLE_PLAYLIST_LOAD, this);
#else
        m_projectM = new ProjectMWrapper(PROJECTM_CONFIG, projectM::FLAG_DISABLE_PLAYLIST_LOAD, this);
#endif
        QString presetPath = QString::fromLocal8Bit(m_projectM->settings().presetURL.c_str());
        QDir presetDir(presetPath);
        presetDir.setFilter(QDir::Files);
        QStringList filters;
        filters << "*.prjm" << "*.milk";
        QFileInfoList l = presetDir.entryInfoList(filters);

        RatingList list;
        list.push_back(3);
        list.push_back(3);
        foreach (QFileInfo info, l)
        {
            m_projectM->addPresetURL (info.absoluteFilePath().toStdString(), info.fileName().toStdString(), list);
            m_listWidget->addItem(info.fileName());
            m_listWidget->setCurrentRow(0,QItemSelectionModel::Select);
        }
        connect(m_listWidget, SIGNAL(currentRowChanged(int)), m_projectM, SLOT(selectPreset(int)));
        connect(m_projectM, SIGNAL(currentPresetChanged(int)), SLOT(setCurrentRow(int)));
        updateTitle();
        m_timer->start(0);
    }
}

void ProjectMWidget::resizeGL (int w, int h)
{
    if (m_projectM)
    {
        m_projectM->projectM_resetGL(w, h);
        initializeGL();
    }
}

void ProjectMWidget::paintGL()
{
    if (m_projectM)
        m_projectM->renderFrame();
}

void ProjectMWidget::mousePressEvent (QMouseEvent *event)
{
    if (event->button () == Qt::RightButton)
        m_menu->exec(event->globalPos());
}

void ProjectMWidget::createActions()
{
    m_menu->addAction(tr("&Help"), this, SLOT(showHelp()), tr("F1"))->setCheckable(true);
    m_menu->addAction(tr("&Show Song Title"), this, SLOT(showTitle()), tr("F2"))->setCheckable(true);
    m_menu->addAction(tr("&Show Preset Name"), this, SLOT(showPresetName()), tr("F3"))->setCheckable(true);
    m_menu->addAction(tr("&Show Menu"), this, SIGNAL(showMenuToggled(bool)), tr("M"))->setCheckable(true);
    m_menu->addSeparator();
    m_menu->addAction(tr("&Next Preset"), this, SLOT(nextPreset()), tr("N"));
    m_menu->addAction(tr("&Previous Preset"), this, SLOT(previousPreset()), tr("P"));
    m_menu->addAction(tr("&Random Preset"), this, SLOT(randomPreset()), tr("R"));
    m_menu->addAction(tr("&Lock Preset"), this, SLOT(lockPreset(bool)), tr("L"))->setCheckable(true);
    m_menu->addSeparator();
    m_menu->addAction(tr("&Fullscreen"), this, SIGNAL(fullscreenToggled(bool)), tr("F"))->setCheckable(true);
    m_menu->addSeparator();
    addActions(m_menu->actions());
}

void ProjectMWidget::showHelp()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_F1, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::showPresetName()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_F3, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::showTitle()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_F2, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::nextPreset()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_n, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::previousPreset()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_p, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::randomPreset()
{
    m_projectM->key_handler(PROJECTM_KEYDOWN, PROJECTM_K_r, PROJECTM_KMOD_LSHIFT);
}

void ProjectMWidget::lockPreset(bool lock)
{
    m_projectM->setPresetLock(lock);
}

void ProjectMWidget::updateTitle()
{
    std::string artist = SoundCore::instance()->metaData(Qmmp::ARTIST).toLocal8Bit().constData();
    std::string title = SoundCore::instance()->metaData(Qmmp::TITLE).toLocal8Bit().constData();
    m_projectM->projectM_setTitle(artist + " - " + title);
}

void ProjectMWidget::setCurrentRow(int row)
{
    m_listWidget->setCurrentRow(row);
}
