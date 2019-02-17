/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#ifndef VISUALSPECTRUMSPEKFACTORY_H
#define VISUALSPECTRUMSPEKFACTORY_H

#include <qmmp/spekfactory.h>

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class VisualSpectrumSpekFactory : public QObject, public SpekFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.SpekFactoryInterface.1.0")
    Q_INTERFACES(SpekFactory)
public:
    virtual const SpekProperties properties() const override;
    virtual Spek *create(QWidget *parent) override;

};

#endif
