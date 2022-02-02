/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#ifndef NETWORKINPUTSOURCE_H
#define NETWORKINPUTSOURCE_H

#include <qmmp/inputsource.h>

class NetworkStreamReader;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class NetworkInputSource : public InputSource
{
Q_OBJECT
public:
    explicit NetworkInputSource(const QString &path, QObject *parent = nullptr);

    virtual QIODevice *ioDevice() const override final;
    virtual bool initialize() override final;
    virtual bool isReady() const override final;
    virtual bool isWaiting() const override final;
    virtual QString contentType() const override final;

private:
    NetworkStreamReader *m_reader = nullptr;

};

#endif
