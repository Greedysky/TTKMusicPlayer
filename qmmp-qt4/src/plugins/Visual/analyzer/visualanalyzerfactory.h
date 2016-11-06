/***************************************************************************
 *   Copyright (C) 2008-2014 by Ilya Kotov                                 *
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
#ifndef VISUALANALYZERFACTORY_H
#define VISUALANALYZERFACTORY_H


#include <QObject>

#include <qmmp/visualfactory.h>
#include <qmmp/visual.h>

/**
	@author Ilya Kotov <forkotov02@hotmail.ru>
*/
class VisualAnalyzerFactory : public QObject, public VisualFactory
{
Q_OBJECT
Q_INTERFACES(VisualFactory)

public:
    const VisualProperties properties() const;
    Visual *create(QWidget *parent);

};


#endif
