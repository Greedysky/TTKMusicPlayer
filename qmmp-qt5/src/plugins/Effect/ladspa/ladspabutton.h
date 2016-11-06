/***************************************************************************
 *   Copyright (C) 2010 by Ilya Kotov                                      *
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

#ifndef LADSPABUTTON_H
#define LADSPABUTTON_H

#include <QCheckBox>
#include "ladspa.h"

class LADSPAHost;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class LADSPAButton : public QCheckBox
{
Q_OBJECT
public:
    LADSPAButton(LADSPA_Data *value, QWidget *parent = 0);

private slots:
    void enable(bool yes);

private:
    LADSPA_Data *m_value;
};

#endif // LADSPABUTTON_H
