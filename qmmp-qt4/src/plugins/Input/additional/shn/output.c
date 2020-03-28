/*  output.c - functions for message and error output
 *  Copyright (C) 2000-2007  Jason Jordan <shnutils@freeshell.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * $Id: output.c,v 1.11 2007/03/23 05:49:48 jason Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "shndec.h"

void print_lines(char *prefix,char *message)
{
  char *head, *tail;

  head = tail = message;
  while (*head != '\0') {
    if (*head == '\n') {
      *head = '\0';
      fprintf(stderr,"%s%s\n",prefix,tail);
      tail = head + 1;
    }
    head++;
  }
  fprintf(stderr,"%s%s\n",prefix,tail);
}

void shn_error(char *msg, ...)
{
  va_list args;
  char msgbuf[BUF_SIZE];

  va_start(args,msg);

  shn_vsnprintf(msgbuf,BUF_SIZE,msg,args);

  va_end(args);
}

void shn_debug(char *msg, ...)
{
  va_list args;
  char msgbuf[BUF_SIZE];

  va_start(args,msg);

  shn_vsnprintf(msgbuf,BUF_SIZE,msg,args);

  va_end(args);
}

void shn_error_fatal(shn_file *this_shn,char *complaint, ...)
{
  va_list args;

  va_start(args,complaint);

  if (NULL != this_shn) {
    if (0 == this_shn->vars.fatal_error) {
      this_shn->vars.fatal_error = 1;
      this_shn->vars.going = 0;
      shn_vsnprintf(this_shn->vars.fatal_error_msg,BUF_SIZE,complaint,args);
    }
  }

  va_end(args);
}
