/***************************************************************************
 *   Copyright (C) 2010-2013 by Ilya Kotov                                 *
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

#ifndef WILDMIDIHELPER_H
#define WILDMIDIHELPER_H

#include <QObject>
#include <QMutex>
#include <QStringList>

class WildMidiHelper : public QObject
{
Q_OBJECT
public:
    explicit WildMidiHelper(QObject *parent = 0);
    ~WildMidiHelper();
    bool initialize();
    void readSettings();
    quint32 sampleRate();
    void addPtr(void *);
    void removePtr(void *);
    QStringList configFiles() const;
    static WildMidiHelper *instance();

private:
    static WildMidiHelper *m_instance;
    bool m_inited;
    QMutex m_mutex;
    QList <void *> m_ptrs;
    quint32 m_sample_rate;
};

#endif // WILDMIDIHELPER_H
