/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#ifndef SILENCEPLUGIN_H
#define SILENCEPLUGIN_H

#include <QMutex>
#include <qmmp/effect.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SilencePlugin : public Effect
{
public:
    SilencePlugin();
    virtual ~SilencePlugin();

    virtual void applyEffect(Buffer *b) override final;

    void setThreshold(double value);

    static SilencePlugin* instance();

private:
    QMutex m_mutex;
    size_t m_size = 0;
    double m_threshold = -40;
    static SilencePlugin *m_instance;

};

#endif
