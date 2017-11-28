/***************************************************************************
 *   Copyright (C) 2000-2004 by                                            *
 *   Jean-Christophe Hoelt <jeko@ios-software.com>                         *
 *   Guillaume Borios <gyom@ios-software.com>                              *
 *                                                                          *
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

#include "v3d.h"

void v3d_to_v2d(v3d *v3, int nbvertex, int width, int height, float distance, v2d *v2) {
    int i;
    for (i=0;i<nbvertex;++i) {
        if (v3[i].z > 2) {
            int Xp, Yp;
            F2I((distance * v3[i].x / v3[i].z),Xp);
            F2I((distance * v3[i].y / v3[i].z),Yp);
            v2[i].x = Xp + (width>>1);
            v2[i].y = -Yp + (height>>1);
        }
        else v2[i].x=v2[i].y=-666;
    }
}
