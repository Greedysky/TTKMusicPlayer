/*
 *   PCM time-domain equalizer
 *
 *   Copyright (C) 2002-2006  Felipe Rivera <liebremx at users.sourceforge.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   $Id: iir_fpu.h,v 1.3 2006/01/15 00:26:32 liebremx Exp $$
 */
#ifndef IIR_FPU_H
#define IIR_FPU_H

#define sample_t double

/*
 * Normal FPU implementation data structures
 */
/* Coefficient history for the IIR filter */
typedef struct
{
    sample_t x[3]; /* x[n], x[n-1], x[n-2] */
    sample_t y[3]; /* y[n], y[n-1], y[n-2] */
    sample_t dummy1; // Word alignment
    sample_t dummy2;
}sXYData;

#endif
