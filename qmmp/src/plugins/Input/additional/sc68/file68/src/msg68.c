/*
 * @file    msg68.c
 * @brief   message logging
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
#include "file68_api.h"
#include "file68_msg.h"
#include "file68_str.h"

static msg68_t   output = 0;           /* Output function.  */
static void    * cookie = 0;           /* User data.        */

/** Default message filter mask.
 *
 *  Corresponding bit have to be cleared to mask/filter a message
 *  category. The default value depend on compilation.
 */
static unsigned int msg68_bitmsk =
#if defined(DEBUGMSG_MASK)
  DEBUGMSG_MASK
#elif defined(DEBUG)
  (1 << (msg68_DEBUG+1)) - 1
#else
  (1 << (msg68_INFO+1)) - 1
#endif
  ;

#define MAX_CATEGORIES (((sizeof(int)<<3)))

struct struct_cat_bit {
  int          bit;  /* equal to its array's index when entry is used. */
  const char * name; /* category name.                                 */
  const char * desc; /* short descrition.                              */
} cat_bits[MAX_CATEGORIES] = {
  { msg68_CRITICAL, "critical", "critical error message" },
  { msg68_ERROR   , "error"   , "error message"          },
  { msg68_WARNING , "warning" , "warning message"        },
  { msg68_INFO    , "info"    , "informational message"  },
  { msg68_NOTICE  , "notice"  , "notice message"         },
  { msg68_DEBUG   , "debug"   , "debug message"          },
  { msg68_TRACE   , "trace"   , "trace message"          }
};

msg68_t msg68_set_handler(msg68_t handler)
{
  msg68_t old = output;
  output = handler;
  return old;
}

void * msg68_set_cookie(void * userdata)
{
  void * old = cookie;
  cookie = userdata;
  return old;
}

/* Print message (variable argument). */
void msg68x_va(int cat, void * cookie, const char * fmt, va_list list)
{
  if (output) {
    switch (cat) {
    default:
      if (cat >= 0) {
        const int bit = cat & (MAX_CATEGORIES-1);
        const int msk = (1 << bit) | ((bit > msg68_TRACE) << msg68_TRACE);
        if ( ! (msg68_bitmsk & msk) )
          break;
      } else break;
    case msg68_ALWAYS:
      output(cat, cookie, fmt, list);
    case msg68_NEVER:
      break;
    }
  }
}

void msg68x(int bit, void * cookie, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68x_va(bit, cookie, fmt, list);
  va_end(list);
}

void msg68_va(int bit, const char * fmt, va_list list)
{
  msg68x_va(bit, cookie, fmt, list);
}

void msg68(int bit, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68x_va(bit, cookie, fmt, list);
  va_end(list);
}

void msg68_trace(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_TRACE, fmt, list);
  va_end(list);
}

void msg68x_trace(void * userdata, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68x_va(msg68_TRACE, userdata, fmt, list);
  va_end(list);
}

void msg68_debug(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_DEBUG, fmt, list);
  va_end(list);
}

void msg68x_debug(void * userdata, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68x_va(msg68_DEBUG, userdata, fmt, list);
  va_end(list);
}

void msg68_info(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_INFO, fmt, list);
  va_end(list);
}

void msg68x_info(void * userdata, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68x_va(msg68_INFO, userdata, fmt, list);
  va_end(list);
}

void msg68_notice(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_NOTICE, fmt, list);
  va_end(list);
}

void msg68x_notice(void * userdata, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68x_va(msg68_NOTICE, userdata, fmt, list);
  va_end(list);
}

void msg68_warning(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_WARNING, fmt, list);
  va_end(list);
}

void msg68x_warning(void * userdata, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68x_va(msg68_WARNING, userdata, fmt, list);
  va_end(list);
}

void msg68_error(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_ERROR, fmt, list);
  va_end(list);
}

void msg68x_error(void * userdata, const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68x_va(msg68_ERROR, userdata, fmt, list);
  va_end(list);
}

void msg68_critical(const char * fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  msg68_va(msg68_CRITICAL, fmt, list);
  va_end(list);
}

void msg68x_critical(void * userdata, const char * fmt, ...)
{
  va_list list; va_start(list, fmt);
  msg68x_va(msg68_CRITICAL, userdata, fmt, list);
  va_end(list);
}

void msg68_always(const char * fmt, ...)
{
  va_list list; va_start(list, fmt);
  msg68_va(msg68_ALWAYS, fmt, list);
  va_end(list);
}

void msg68x_always(void * userdata, const char * fmt, ...)
{
  va_list list; va_start(list, fmt);
  msg68x_va(msg68_ALWAYS, userdata, fmt, list);
  va_end(list);
}

static inline int is_valid_category(const int i)
{
  return i>=0 && i<MAX_CATEGORIES;
}

static inline int get_category(const char *name)
{
  int i;
  for (i=MAX_CATEGORIES; --i>=0 && strcmp68(name,cat_bits[i].name);)
    ;
  return i;
}

static inline int is_free_category(const int i)
{
  return cat_bits[i].bit != i;
}

static inline int get_free_category(void)
{
  int i;
  for (i=MAX_CATEGORIES; --i>=0 && !is_free_category(i);)
    ;
  if (i>=0)
    cat_bits[i].bit = i;
  return i;
}


/* Get named categories. */
int msg68_cat_bit(const char * name)
{
  return name ? get_category(name) : -1;
}

/* Create/Modify a category. */
int msg68_cat(const char * name, const char * desc, int masked)
{
  int i = msg68_NEVER;

  if (name) {
    i = get_category(name);
    if (i < 0) {
      i = get_free_category();
    }
    if (i >= 0) {
      cat_bits[i].name = name;
      cat_bits[i].desc = desc ? desc : "N/A";
      if (masked) {
        msg68_bitmsk |= 1<<i;
      } else {
        msg68_bitmsk &= ~(1UL<<i);
      }
    }
  }

  return i;
}

/* Free/Destroy a debug category. */
void msg68_cat_free(int category)
{
  if (is_valid_category(category) && category > msg68_TRACE) {
    cat_bits[category].bit = -1;
    msg68_bitmsk |= 1 << category;
  }
}

/* Set all predefined categories mask according to given level. */
int msg68_cat_level(int category)
{
  if (category == msg68_NEVER)
    msg68_bitmsk &= ~((1<<(msg68_TRACE+1))-1);
  else if (category == msg68_ALWAYS)
    msg68_bitmsk |= ((1<<(msg68_TRACE+1))-1);
  else if (category >= msg68_CRITICAL && category <= msg68_TRACE) {
    msg68_bitmsk &= ~((1<<(msg68_TRACE+1))-1);
    msg68_bitmsk |= (1<<(category+1))-1;
  }
  return msg68_bitmsk;
}

/* Get info on category */
int msg68_cat_info(int category, const char **pname,
                   const char **pdesc, int *pnext)
{
  int ret = -1, next = category;
  if (is_valid_category(category)) {
    if (pname) *pname = cat_bits[category].name;
    if (pdesc) *pdesc = cat_bits[category].desc;
    ret = 1 & (msg68_bitmsk>>category);
  } else {
    next = -1;
  }
  if (pnext) {
    while (++next<MAX_CATEGORIES && is_free_category(next))
      ;
    if (category<MAX_CATEGORIES) {
      *pnext = next;
    }
  }
  return ret;
}

void msg68_cat_help(void * cookie, msg68_help_t fct)
{
  if (fct) {
    int i;
    for (i=0; i<MAX_CATEGORIES; ++i) {
      const int isfree = is_free_category(i);
      if (!isfree)
        fct (cookie, i, cat_bits[i].name, cat_bits[i].desc);
    }
  }
}

unsigned int msg68_cat_filter(unsigned int clr, unsigned int set)
{
  msg68_bitmsk &= ~clr;
  msg68_bitmsk |=  set;
  return msg68_bitmsk;
}
