/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#ifndef LIGHTSPECTROGRAMFACTORY_H
#define LIGHTSPECTROGRAMFACTORY_H

#include <qmmp/lightfactory.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class LightSpectrogramFactory : public QObject, public LightFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.LightFactoryInterface.1.0")
    Q_INTERFACES(LightFactory)
public:
    virtual const LightProperties properties() const override final;
    virtual Light *create(QWidget *parent) override final;

};

#endif
