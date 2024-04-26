/***************************************************************************
 *   Copyright (C) 2009-2024 by Ilya Kotov                                 *
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

#ifndef EFFECTLADSPAFACTORY_H
#define EFFECTLADSPAFACTORY_H

#include <qmmp/effectfactory.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class EffectLADSPAFactory : public QObject, public EffectFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.EffectFactoryInterface.1.0")
    Q_INTERFACES(EffectFactory)
public:
    virtual EffectProperties properties() const override final;
    virtual Effect *create() override final;
    virtual void showSettings(QWidget *parent) override final;

};

#endif
