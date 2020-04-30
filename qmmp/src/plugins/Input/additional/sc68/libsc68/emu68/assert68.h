/**
 * @ingroup   lib_emu68
 * @file      emu68/assert68.h
 * @brief     assert feature header.
 * @author    Benjamin Gerard
 * @date      2009/06/12
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef EMU68_ASSERT68_H
#define EMU68_ASSERT68_H

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif
#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif
#ifndef assert
# error "assert macro must be defined"
#endif
#if defined(NDEBUG_LIBSC68) && !defined(NDEBUG)
# error "Compile libsc68 in release mode with assert enable"
#endif

#ifndef EMU68_BREAK
# define EMU68_BREAK 1                  /**< Don't break anymore */
#endif

#endif
