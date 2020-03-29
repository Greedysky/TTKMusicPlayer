/*
  ayemu - AY/YM sound chip emulator and music file loader
  Copyright (C) 2003-2004 Sashnov Alexander

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Alexander Sashnov 
  sashnov@ngs.ru
*/

/*!
 * \mainpage libayemu library
 *
 * Homepage on sourceforge.net: http://sourceforge.net/projects/libayemu
 *
 * This library will help you to add AY/YM music in your own game, demos, etc.
 *
 * For example of how to use it see playvtx and xmms-vtx packages.
 *
 * Python wrapper and example comming soon.
 *
 */

#ifndef _AYEMU_H
#define _AYEMU_H

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif /* __cplusplus */

/* Make sure the correct platform symbols are defined */
#if !defined(WIN32) && defined(_WIN32)
#define WIN32
#endif /* Windows */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  ifdef __BORLANDC__
#   ifdef BUILD_SDL
#    define DECLSPEC 
#   else
#    define DECLSPEC __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC	__declspec(dllexport)
#  endif
# else
#  define DECLSPEC
# endif
# endif
#endif

#define EXTERN extern DECLSPEC


/* include other library headers */
#include "ayemu_8912.h"
#include "ayemu_vtxfile.h"

#endif
