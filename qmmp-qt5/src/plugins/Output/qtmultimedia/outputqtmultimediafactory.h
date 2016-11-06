/***************************************************************************
 *   Copyright (C) 2015 by Ivan Ponomarev                                  *
 *   ivantrue@gmail.com                                                    *
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
#ifndef OUTPUTQTMULTIMEDIAFACTORY_H
#define OUTPUTQTMULTIMEDIAFACTORY_H


#include <QObject>
#include <QString>
#include <QIODevice>
#include <QWidget>

#include <qmmp/output.h>
#include <qmmp/outputfactory.h>


class OutputQtMultimediaFactory : public QObject, OutputFactory
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.OutputFactoryInterface.1.0")
Q_INTERFACES(OutputFactory)

public:
    virtual const OutputProperties properties() const;
    virtual Output* create();
    virtual Volume *createVolume();

};

#endif
