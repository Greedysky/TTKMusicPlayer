/*
 * @file    dial68.c
 * @brief   sc68 config file
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
#include "config.h"
#endif

/* sc68 */
#include "dial68.h"


#ifndef USE_DIALOG

int dial68(void * data, sc68_dial_f cntl)
{
  TRACE68(dial_cat,
          P "%s", "not implemented\n");
  return -1;
}

#else

/* libc */
#include <string.h>

/* defined in dial_conf.c */
SC68_EXTERN int dial68_new_conf(void ** pdata, sc68_dial_f * pcntl);
/* defined in dial_finf.c */
SC68_EXTERN int dial68_new_finf(void ** pdata, sc68_dial_f * pcntl);
/* defined in dial_tsel.c */
SC68_EXTERN int dial68_new_tsel(void ** pdata, sc68_dial_f * pcntl);

int dial68(void * data, sc68_dial_f cntl)
{
  int res = -1;
  sc68_dialval_t v;

  if (!cntl) {
    TRACE68(dial_cat,P "%s -- no callback function\n", __FUNCTION__);
    goto exit;
  }

  if (!cntl(data,SC68_DIAL_HELLO,SC68_DIAL_CALL,&v) && v.s) {
    if (!strcmp(v.s,"config"))
      res = dial68_new_conf(&data,&cntl);
    else if (!strcmp(v.s,"fileinfo"))
      res = dial68_new_finf(&data,&cntl);
    else if (!strcmp(v.s,"trackselect"))
      res = dial68_new_tsel(&data,&cntl);
    else
      TRACE68(dial_cat, P "unknow dialog -- \"%s\"\n", v.s);
    if (!res) {
      switch (cntl(data,SC68_DIAL_NEW,SC68_DIAL_CALL,&v)) {
      case 1:  res = 0;   break;
      case 0:  res = v.i; break;
      default: res = -1;  break;
      }
      if (!res)
        res = dial68_frontend(data,cntl);
    }
  }

exit:
  TRACE68(dial_cat,P "%s -- *%d*\n",__FUNCTION__,res);
  return res;
}

#endif
