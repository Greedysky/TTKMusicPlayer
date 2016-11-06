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
#ifndef PROJECTMPLUGIN_H
#define PROJECTMPLUGIN_H

#include <QWidget>
#include <qmmp/visual.h>

class QTimer;
class QSplitter;
class Buffer;
class ProjectMWidget;

class ProjectMPlugin : public Visual
{
    Q_OBJECT
public:
    ProjectMPlugin(QWidget *parent = 0);
    virtual ~ProjectMPlugin();

    void add(float *data, size_t samples, int chan);
    void clear();

private slots:
    void setFullScreen(bool yes);

private:
    void closeEvent(QCloseEvent *event);
    QTimer *m_timer;
    ProjectMWidget *m_projectMWidget;
    QSplitter *m_splitter;
    short m_buf[2][512];
};


#endif
