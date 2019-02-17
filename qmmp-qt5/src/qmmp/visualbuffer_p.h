/***************************************************************************
 *   Copyright (C) 2017 by Ilya Kotov                                      *
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

#ifndef VISUALBUFFER_P_H
#define VISUALBUFFER_P_H

#include <QtGlobal>
#include <QTime>
#include <QMutex>
#include "qmmp_export.h"

#define VISUAL_BUFFER_SIZE 128 //number of nodes

class QMMP_EXPORT VisualNode
{
public:
    float data[2][512];
    bool used;
    qint64 ts;

    VisualNode()
    {
        used = false;
        ts = 0;
    }
};

class QMMP_EXPORT VisualBuffer
{
public:
    VisualBuffer();

    void add(float *pcm, int samples, int channels, qint64 ts, qint64 delay);
    VisualNode *take();
    void clear();
    QMutex *mutex();

private:
    VisualNode m_buffer[VISUAL_BUFFER_SIZE];
    qint64 m_elapsed;
    int m_take_index;
    int m_add_index;
    QTime m_time;
    QMutex m_mutex;

};

#endif // VISUALBUFFER_P_H
