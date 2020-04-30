/*
 * @file    emu68/lines68.c
 * @brief   The whole 68k emulator in a single file.
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef EMU68_MONOLITIC
# error "EMU68_MONOLITIC should be defined when compiling this file."
#else
# include "emu68_api.h"

/* Monolitic build: remove all forced inlines */
# undef  inline
# define inline

#if defined(__GNUC__) && (__GNUC__*100+__GNUC_MINOR__ >= 402)
 #pragma GCC diagnostic ignored "-Wunused-function"
#endif

# include "lines68.h"
# include "lines/line0.c"
# include "lines/line1.c"
# include "lines/line2.c"
# include "lines/line3.c"
# include "lines/line4.c"
# include "lines/line5.c"
# include "lines/line6.c"
# include "lines/line7.c"
# include "lines/line8.c"
# include "lines/line9.c"
# include "lines/lineA.c"
# include "lines/lineB.c"
# include "lines/lineC.c"
# include "lines/lineD.c"
# include "lines/lineE.c"
# include "lines/lineF.c"
# include "lines/table.c"
#endif
