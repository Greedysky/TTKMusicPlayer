/***************************************************************************
 *   Copyright (C) 2009 by Ilya Kotov                                      *
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

#ifndef REPLAYGAINREADER_H
#define REPLAYGAINREADER_H

#include <QMap>
#include <QString>
#include <taglib/xiphcomment.h>
#include <qmmp/qmmp.h>

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class ReplayGainReader
{
public:
    ReplayGainReader(const QString &path);
    QMap <Qmmp::ReplayGainKey, double> replayGainInfo() const;

private:
    void readVorbisComment(TagLib::Ogg::XiphComment *comment);
    void setValue(Qmmp::ReplayGainKey key, QString value);
    QMap <Qmmp::ReplayGainKey, double> m_values;
};

#endif // REPLAYGAINREADER_H
