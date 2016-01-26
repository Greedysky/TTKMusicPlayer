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

#include "emptyinputsource_p.h"

EmptyInputSource::EmptyInputSource(const QString &url, QObject *parent) : InputSource(url,parent)
{
    m_ok = false;
}

QIODevice *EmptyInputSource::ioDevice()
{
    return 0;
}

bool EmptyInputSource::initialize()
{
    m_ok = true; //check decoders
    if(m_ok)
        emit ready();
    return m_ok;
}

bool EmptyInputSource::isReady()
{
    return m_ok;
}
