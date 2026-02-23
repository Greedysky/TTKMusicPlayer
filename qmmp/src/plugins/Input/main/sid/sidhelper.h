/***************************************************************************
 *   Copyright (C) 2013-2026 by Ilya Kotov                                 *
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

#ifndef SIDHELPER_H
#define SIDHELPER_H

#include <sidplayfp/SidTune.h>
#include <sidplayfp/SidTuneInfo.h>
#include <sidplayfp/SidDatabase.h>
#include <qmmp/trackinfo.h>

/**
   @author Ilya Kotov <forkotov02@ya.ru>
*/
class SIDHelper
{
public:
    explicit SIDHelper(SidDatabase *db);
    ~SIDHelper();

    SidTune *load(const QString &path);
    QList<TrackInfo*> createPlayList(TrackInfo::Parts parts);

private:
    QString m_path;
    SidTune *m_tune = nullptr;
    SidDatabase *m_db;

};

#endif
