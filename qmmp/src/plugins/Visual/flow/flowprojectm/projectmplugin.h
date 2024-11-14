/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef PROJECTMPLUGIN_H
#define PROJECTMPLUGIN_H

#include <QSplitter>
#include <qmmp/visual.h>

class QMenu;
class QListWidget;
#ifdef PROJECTM_4
class ProjectM4Widget;
#else
class ProjectMWidget;
#endif

/*!
 * @author Greedysky <greedysky@163.com>
 */
class ProjectMPlugin : public Visual
{
    Q_OBJECT
public:
    explicit ProjectMPlugin(QWidget *parent = nullptr);

private:
    virtual void contextMenuEvent(QContextMenuEvent *) override final;

    virtual void process(float *left, float *right) override final;

    QMenu *m_menu;
    QSplitter *m_splitter;
    QListWidget *m_itemWidget;
#ifdef PROJECTM_4
    ProjectM4Widget *m_projectMWidget;
#else
    ProjectMWidget *m_projectMWidget;
#endif

};

#endif
