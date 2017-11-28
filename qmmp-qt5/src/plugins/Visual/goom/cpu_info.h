/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   Based on FFmpeg code                                                  *
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

#ifndef CPU_INFO_H
#define CPU_INFO_H

#ifdef CPU_X86

#define FF_MM_MMX      0x0001 ///< standard MMX
#define FF_MM_3DNOW    0x0004 ///< AMD 3DNOW
#define FF_MM_MMX2     0x0002 ///< SSE integer functions or AMD MMX ext
#define FF_MM_SSE      0x0008 ///< SSE functions
#define FF_MM_SSE2     0x0010 ///< PIV SSE2 functions
#define FF_MM_3DNOWEXT 0x0020 ///< AMD 3DNowExt
#define FF_MM_SSE3     0x0040 ///< Prescott SSE3 functions
#define FF_MM_SSSE3    0x0080 ///< Conroe SSSE3 functions
#define FF_MM_SSE4     0x0100 ///< Penryn SSE4.1 functions
#define FF_MM_SSE42    0x0200 ///< Nehalem SSE4.2 functions
#define FF_MM_IWMMXT   0x0100 ///< XScale IWMMXT
#define FF_MM_ALTIVEC  0x0001 ///< standard AltiVec

/* Function to test if multimedia instructions are supported...  */
int mm_support(void);

#endif //CPU_X86
#endif //CPU_INFO_H
