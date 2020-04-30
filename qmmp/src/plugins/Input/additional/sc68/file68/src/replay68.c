/*
 * @file    replay68.c
 * @brief   built-in external replay rom
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
#include "file68_str.h"
#include "file68_msg.h"
FILE68_API
int replay68_get(const char * name, const void ** data,
                 int * csize, int * dsize);

#include "replay.inc.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static int cmp(const void * pa, const void * pb)
{
  const char
    * a = ( (const struct replay *) pa ) -> name,
    * b = ( (const struct replay *) pb ) -> name;
  return strcmp68(a, b);
}

int replay68_get(const char * name, const void ** data,
                 int * csize, int * dsize)
{
  const int max = sizeof(replays)/sizeof(*replays);
  struct replay s;
  const struct replay *r;
  s.name = name;

  r = bsearch(&s, replays, max, sizeof(*replays), cmp);
  if (!r) {
    int i;
    /* This should not happen unless the replay-rom is not sorted
     * properly. Anyway it does not cost too much work.
     */
#ifdef DEBUG
    const char * prev = "";
    for (i=0; i<max; ++i) {
      assert ( strcmp68(prev, replays[i].name) < 0);
      prev = replays[i].name;
    }
#endif
    for (i=0; i<max; ++i)
      if (!strcmp68(name, replays[i].name)) {
        r = replays+i;
        break;
      }
  }

  if (r) {
    if (data)
      *data = r->data;
    if (csize)
      *csize = r->csize;
    if (dsize)
      *dsize = r->dsize;
  } else
    msg68_warning("rsc68: can't find built-in replay -- *%s*\n",
                  name);
  return -!r;
}
