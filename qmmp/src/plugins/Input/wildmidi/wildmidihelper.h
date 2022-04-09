/***************************************************************************
 *   Copyright (C) 2010-2022 by Ilya Kotov                                 *
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

#ifndef WILDMIDIHELPER_H
#define WILDMIDIHELPER_H

#include <QMutex>
#include <QObject>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class WildMidiHelper : public QObject
{
    Q_OBJECT
public:
    explicit WildMidiHelper(QObject *parent = nullptr);
    ~WildMidiHelper();

    bool initialize();
    void readSettings();
    QString configFile() const;

    void addPtr(void *t);
    void removePtr(void *t);

    static WildMidiHelper *instance();

    int bitrate() const;
    int sampleRate() const;
    int channels() const;
    int bitsPerSample() const;

private:
    bool m_inited = false;
    QMutex m_mutex;
    QList<void*> m_ptrs;
    int m_sampleRate = 0;
    static WildMidiHelper *m_instance;

};

#endif
