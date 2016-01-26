/***************************************************************************
 *   Copyright (C) 2007-2008 by Zhuravlev Uriy                             *
 *   stalkerg@gmail.com                                                    *
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

#include <QTranslator>
#include <QMessageBox>
#include <qmmp/qmmp.h>
#include "outputjack.h"
#include "outputjackfactory.h"


const OutputProperties OutputJACKFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("JACK Plugin");
    properties.hasAbout = true;
    properties.hasSettings = false;
    properties.shortName = "jack";
    return properties;
}

Output* OutputJACKFactory::create()
{
    return new OutputJACK();
}

Volume *OutputJACKFactory::createVolume()
{
    return 0;
}

void OutputJACKFactory::showSettings(QWidget*)
{

}
