/***************************************************************************
 *   Copyright (C) 2010-2024 by Ilya Kotov                                 *
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

#ifndef GMEHELPER_H
#define GMEHELPER_H

#include <QFile>
#include <gme/gme.h>
#include <qmmp/trackinfo.h>

/**
   @author Ilya Kotov <forkotov02@ya.ru>
*/
class GMEHelper
{
public:
    GMEHelper();
    ~GMEHelper();

    Music_Emu *load(const QString &path, int sample_rate = 44100);
    QList<TrackInfo*> createPlayList(TrackInfo::Parts parts);
    int fadeLength();

private:
    Music_Emu *m_emu = nullptr;
    QString m_path;
    int m_fade_length;

};

#endif
