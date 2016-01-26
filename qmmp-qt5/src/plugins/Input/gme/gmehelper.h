/***************************************************************************
 *   Copyright (C) 2010-2012 by Ilya Kotov                                 *
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

#ifndef GMEHELPER_H
#define GMEHELPER_H

#include <QString>
#include <QList>
#include <gme/gme.h>
#include <qmmp/fileinfo.h>

/**
   @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class GmeHelper
{
public:
    GmeHelper();
    ~GmeHelper();
    Music_Emu *load(const QString &url, int sample_rate = 44100);
    QList <FileInfo*> createPlayList(bool meta);
    int fadeLength();

private:
    Music_Emu *m_emu;
    QString m_path;
    int m_fade_length;
};

#endif // GMEHELPER_H
