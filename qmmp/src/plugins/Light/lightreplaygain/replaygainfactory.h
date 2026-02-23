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

#ifndef LIGHTREPLAYGAINFACTORY_H
#define LIGHTREPLAYGAINFACTORY_H

#include <qmmp/lightfactory.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class LightReplaygGainFactory : public QObject, public LightFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.LightFactoryInterface.1.0")
    Q_INTERFACES(LightFactory)
public:
    virtual LightProperties properties() const override final;
    virtual Light *create(QWidget *parent) override final;

};

#endif
