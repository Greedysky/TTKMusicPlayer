/*
 * attributes.h
 * Copyright (C) 1999-2000 Aaron Holtzman <aholtzma@ess.engr.uvic.ca>
 * Copyright (C) 2001-2008 xine developers
 *
 * This file was originally part of mpeg2dec, a free MPEG-2 video stream
 * decoder.
 *
 * mpeg2dec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * mpeg2dec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 */

/* use gcc attribs to align critical data structures */

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#ifdef XINE_COMPILE
# include "configure.h"
#else
# if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95 )
#  define SUPPORT_ATTRIBUTE_PACKED 1
# endif

# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3 )
#  define SUPPORT_ATTRIBUTE_DEPRECATED 1
#  define SUPPORT_ATTRIBUTE_FORMAT 1
#  define SUPPORT_ATTRIBUTE_FORMAT_ARG 1
#  define SUPPORT_ATTRIBUTE_MALLOC 1
#  define SUPPORT_ATTRIBUTE_UNUSED 1
#  define SUPPORT_ATTRIBUTE_CONST 1
# endif

# if __GNUC__ >= 4
#  define SUPPORT_ATTRIBUTE_VISIBILITY_DEFAULT 1
#  if __ELF__
#   define SUPPORT_ATTRIBUTE_VISIBILITY_PROTECTED 1
#  endif
#  define SUPPORT_ATTRIBUTE_SENTINEL 1
# endif
#endif


#ifdef ATTRIBUTE_ALIGNED_MAX
#define ATTR_ALIGN(align) __attribute__ ((__aligned__ ((ATTRIBUTE_ALIGNED_MAX < align) ? ATTRIBUTE_ALIGNED_MAX : align)))
#else
#define ATTR_ALIGN(align)
#endif

/* Export protected only for libxine functions */
#if defined(XINE_LIBRARY_COMPILE) && defined(SUPPORT_ATTRIBUTE_VISIBILITY_PROTECTED)
# define XINE_PROTECTED __attribute__((__visibility__("protected")))
#elif defined(XINE_LIBRARY_COMPILE) && defined(SUPPORT_ATTRIBUTE_VISIBILITY_DEFAULT)
# define XINE_PROTECTED __attribute__((__visibility__("default")))
#else
# define XINE_PROTECTED
#endif

#ifdef SUPPORT_ATTRIBUTE_SENTINEL
# define XINE_SENTINEL __attribute__((__sentinel__))
#else
# define XINE_SENTINEL
#endif

#if defined(SUPPORT_ATTRIBUTE_DEPRECATED) && !defined(XINE_COMPILE)
# define XINE_DEPRECATED __attribute__((__deprecated__))
#else
# define XINE_DEPRECATED
#endif

#ifdef SUPPORT_ATTRIBUTE_WEAK
# define XINE_WEAK __attribute__((weak))
#else
# define XINE_WEAK
#endif

#ifndef __attr_unused
# ifdef SUPPORT_ATTRIBUTE_UNUSED
#  define __attr_unused __attribute__((__unused__))
# else
#  define __attr_unused
# endif
#endif

/* Format attributes */
#ifdef SUPPORT_ATTRIBUTE_FORMAT
# define XINE_FORMAT_PRINTF(fmt,var) __attribute__((__format__(__printf__, fmt, var)))
# define XINE_FORMAT_SCANF(fmt,var) __attribute__((__format__(__scanf__, fmt, var)))
#else
# define XINE_FORMAT_PRINTF(fmt,var)
# define XINE_FORMAT_SCANF(fmt,var)
#endif
#ifdef SUPPORT_ATTRIBUTE_FORMAT_ARG
# define XINE_FORMAT_PRINTF_ARG(fmt) __attribute__((__format_arg__(fmt)))
#else
# define XINE_FORMAT_PRINTF_ARG(fmt)
#endif

#ifdef SUPPORT_ATTRIBUTE_MALLOC
# define XINE_MALLOC __attribute__((__malloc__))
#else
# define XINE_MALLOC
#endif

#ifdef SUPPORT_ATTRIBUTE_PACKED
# define XINE_PACKED __attribute__((__packed__))
#else
# define XINE_PACKED
#endif

#ifdef SUPPORT_ATTRIBUTE_CONST
# define XINE_CONST __attribute__((__const__))
#else
# define XINE_CONST
#endif

#endif /* ATTRIBUTE_H_ */
