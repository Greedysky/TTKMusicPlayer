/***************************************************************************
 *   Copyright (C) 2008-2016 by Ilya Kotov                                 *
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

#include <QObject>
#include <QList>
#include <QMetaObject>
#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
#include <QDialog>
#include <QDir>
#include <QPluginLoader>
#include "visualfactory.h"
#include "output.h"
#include "visual.h"

Visual::Visual(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, false);
    m_decoder = 0;
    m_output = 0;
}

Visual::~Visual()
{
    qDebug("Visual::~Visual()");
}

QMutex *Visual::mutex()
{
    return &m_mutex;
}

void Visual::closeEvent (QCloseEvent *event)
{
    m_visuals.removeAll(this);
    if (event->spontaneous () && m_vis_map.key(this))
    {
        VisualFactory *factory = m_vis_map.key(this);
        m_vis_map.remove(factory);
        Visual::setEnabled(factory, false);
        emit closedByUser();
    }
    else
    {
        if (m_vis_map.key(this))
        {
            VisualFactory *factory = m_vis_map.key(this);
            m_vis_map.remove(factory);
        }
    }
    QWidget::closeEvent(event);
}

//static members
QList<VisualFactory*> *Visual::m_factories = 0;
QHash <VisualFactory*, QString> *Visual::m_files = 0;
QList<Visual*> Visual::m_visuals;
QHash<VisualFactory*, Visual*> Visual::m_vis_map;
QWidget *Visual::m_parentWidget = 0;
QObject *Visual::m_receiver = 0;
const char *Visual::m_member = 0;

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
    if (!m_factories->contains(factory))
        return;

    QString name = factory->properties().shortName;
    QSettings settings ( Qmmp::configFile(), QSettings::IniFormat );
    QStringList visList = settings.value("Visualization/enabled_plugins").toStringList();

    if (enable)
    {
        if (!visList.contains(name))
            visList << name;
        if (!m_vis_map.value(factory) && m_parentWidget)
        {
            Visual* visual = factory->create(m_parentWidget);
            if (m_receiver && m_member)
                connect(visual, SIGNAL(closedByUser()), m_receiver, m_member);
            visual->setWindowFlags(Qt::Window);
            m_vis_map.insert (factory, visual);
            m_visuals.append(visual);
            visual->show();
        }
    }
    else
    {
        visList.removeAll(name);
        if (m_vis_map.value(factory))
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
    if (!m_factories->contains(factory))
        return false;
    QString name = factory->properties().shortName;
    QSettings settings ( Qmmp::configFile(), QSettings::IniFormat );
    QStringList visList = settings.value("Visualization/enabled_plugins").toStringList();
    return visList.contains(name);
}

void Visual::add(Visual *visual)
{
    if (!m_visuals.contains(visual))
        m_visuals.append(visual);
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
        if (isEnabled(factory))
        {
            Visual* visual = factory->create(parent);
            if (m_receiver && m_member)
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

void Visual::checkFactories()
{
    if (!m_factories)
    {
        m_factories = new QList<VisualFactory *>;
        m_files = new QHash <VisualFactory*, QString>;

        QDir pluginsDir (Qmmp::pluginsPath());
        pluginsDir.cd("Visual");
        QStringList filters;
        filters << "*.dll" << "*.so";
        foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files))
        {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader.instance();
            if (loader.isLoaded())
                qDebug("Visual: loaded plugin %s", qPrintable(fileName));
            else
                qWarning("Visual: %s", qPrintable(loader.errorString ()));

            VisualFactory *factory = 0;
            if (plugin)
                factory = qobject_cast<VisualFactory *>(plugin);

            if (factory)
            {
                m_factories->append(factory);
                m_files->insert(factory, pluginsDir.absoluteFilePath(fileName));
            }
        }
    }
}
