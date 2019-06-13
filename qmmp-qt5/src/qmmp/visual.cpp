/***************************************************************************
 *   Copyright (C) 2008-2019 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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

#include <QCloseEvent>
#include <QSettings>
#include <QDir>
#include <QObject>
#include <QList>
#include <QMetaObject>
#include <QApplication>
#include <QPluginLoader>
#include "statehandler.h"
#include "visualfactory.h"
#include "visualbuffer_p.h"
#include "visual.h"
#include "soundcore.h"

Visual::Visual(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, false);
}

Visual::~Visual()
{
    qDebug("Visual::~Visual()");
}

void Visual::closeEvent (QCloseEvent *event)
{
    m_visuals.removeAll(this);
    if(event->spontaneous () && m_vis_map.key(this))
    {
        VisualFactory *factory = m_vis_map.key(this);
        m_vis_map.remove(factory);
        Visual::setEnabled(factory, false);
        emit closedByUser();
    }
    else
    {
        if(m_vis_map.key(this))
        {
            VisualFactory *factory = m_vis_map.key(this);
            m_vis_map.remove(factory);
        }
    }
    QWidget::closeEvent(event);
}

bool Visual::takeData(float *left, float *right)
{
    m_buffer.mutex()->lock();
    VisualNode *node = m_buffer.take();
    if(node)
    {
        if(left && right)
        {
            memcpy(left, node->data[0], QMMP_VISUAL_NODE_SIZE * sizeof(float));
            memcpy(right, node->data[1], QMMP_VISUAL_NODE_SIZE * sizeof(float));
        }
        else if(left && !right)
        {
            for(int i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
                left[i] = qBound(-1.0f, (node->data[0][i] + node->data[1][i]) / 2, 1.0f);
        }
    }
    m_buffer.mutex()->unlock();
    return node != nullptr;
}

float Visual::takeMaxRange() const
{
    float range = 1.0f;
    if(SoundCore::instance())
    {
        range = SoundCore::instance()->volume() * 1.0 / 100;
    }
    return range;
}

//static members
QList<VisualFactory*> *Visual::m_factories = nullptr;
QHash <VisualFactory*, QString> *Visual::m_files = nullptr;
QList<Visual*> Visual::m_visuals;
QHash<VisualFactory*, Visual*> Visual::m_vis_map;
QWidget *Visual::m_parentWidget = nullptr;
QObject *Visual::m_receiver = nullptr;
const char *Visual::m_member = nullptr;
VisualBuffer Visual::m_buffer;

QList<VisualFactory *> Visual::factories()
{
    checkFactories();
    return *m_factories;
}

QString Visual::file(VisualFactory *factory)
{
    checkFactories();
    return m_files->value(factory);
}

void Visual::setEnabled(VisualFactory* factory, bool enable)
{
    checkFactories();
    if(!m_factories->contains(factory))
        return;

    QString name = factory->properties().shortName;
    QSettings settings ( Qmmp::configFile(), QSettings::IniFormat );
    QStringList visList = settings.value("Visualization/enabled_plugins").toStringList();

    if(enable)
    {
        if(!visList.contains(name))
            visList << name;
        if(!m_vis_map.value(factory) && m_parentWidget)
        {
            Visual* visual = factory->create(m_parentWidget);
            if(m_receiver && m_member)
                connect(visual, SIGNAL(closedByUser()), m_receiver, m_member);
            visual->setWindowFlags(Qt::Window);
            m_vis_map.insert (factory, visual);
            Qmmp::State st = StateHandler::instance()->state();
            if(st == Qmmp::Playing || st == Qmmp::Buffering || st == Qmmp::Paused)
                visual->start();
            m_visuals.append(visual);
            visual->show();
        }
    }
    else
    {
        visList.removeAll(name);
        if(m_vis_map.value(factory))
        {
            m_visuals.removeAll(m_vis_map.value(factory));
            m_vis_map.value(factory)->close();
            m_vis_map.remove (factory);
        }
    }
    settings.setValue("Visualization/enabled_plugins", visList);
}

bool Visual::isEnabled(VisualFactory* factory)
{
    checkFactories();
    if(!m_factories->contains(factory))
        return false;
    QString name = factory->properties().shortName;
    QSettings settings ( Qmmp::configFile(), QSettings::IniFormat );
    QStringList visList = settings.value("Visualization/enabled_plugins").toStringList();
    return visList.contains(name);
}

void Visual::add(Visual *visual)
{
    if(!m_visuals.contains(visual))
    {
        Qmmp::State st = StateHandler::instance()->state();
        if(st == Qmmp::Playing || st == Qmmp::Buffering || st == Qmmp::Paused)
            visual->start();
        m_visuals.append(visual);
    }
}

void Visual::remove(Visual *visual)
{
    m_visuals.removeAll(visual);
}

void Visual::initialize(QWidget *parent , QObject *receiver, const char *member)
{
    m_receiver = receiver;
    m_member = member;
    m_parentWidget = parent;
    foreach(VisualFactory* factory, factories())
    {
        if(isEnabled(factory))
        {
            Visual* visual = factory->create(parent);
            if(m_receiver && m_member)
                connect(visual, SIGNAL(closedByUser()), m_receiver, m_member);
            visual->setWindowFlags(visual->windowFlags() | Qt::Window);
            qDebug("Visual: added visualization: %s", qPrintable(factory->properties().name));
            m_vis_map.insert (factory, visual);
            m_visuals.append(visual);
            visual->show();
        }
    }
}

QList<Visual*>* Visual::visuals()
{
    return &m_visuals;
}

void Visual::addAudio(float *pcm, int samples, int channels, qint64 ts, qint64 delay)
{
    m_buffer.mutex()->lock();
    m_buffer.add(pcm, samples, channels, ts, delay);
    m_buffer.mutex()->unlock();
}

void Visual::clearBuffer()
{
    m_buffer.mutex()->lock();
    m_buffer.clear();
    m_buffer.mutex()->unlock();
}

void Visual::checkFactories()
{
    if(!m_factories)
    {
        m_factories = new QList<VisualFactory *>;
        m_files = new QHash <VisualFactory*, QString>;

        foreach(QString filePath, Qmmp::findPlugins("Visual"))
        {
            QPluginLoader loader(filePath);
            QObject *plugin = loader.instance();
            if(loader.isLoaded())
                qDebug("Visual: loaded plugin %s", qPrintable(QFileInfo(filePath).fileName()));
            else
                qWarning("Visual: %s", qPrintable(loader.errorString ()));

            VisualFactory *factory = nullptr;
            if(plugin)
                factory = qobject_cast<VisualFactory *>(plugin);

            if(factory)
            {
                m_factories->append(factory);
                m_files->insert(factory, filePath);
            }
        }
    }
}
