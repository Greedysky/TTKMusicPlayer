/*
 * @file    tracksel.c
 * @brief   sc68 track selector dialog
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

/* generated config header */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* libc */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* sc68 */
#include "sc68/sc68.h"
#include "sc68/file68_opt.h"
#include "sc68/file68_msg.h"
#include "dial68.h"

static const int magic = ('T'<<24)|('S'<<16)|('E'<<8)|'L';

struct dial_s {
  dial68_t     dial;                    /* base dialog */
  sc68_t      *sc68;                    /* result of "sc68" call */
  sc68_disk_t  disk;                    /* result of "disk" call */
  int          track;                   /* 0:all >0 track# */
  int          asid;                    /* 0:off 1:on 2:force */
  sc68_minfo_t info;                    /* store current track info */
  char         tstr[128];               /* used as temporary string */
};
typedef struct dial_s dial_t;

static inline int ismagic(dial_t * dial)
{
  return
    dial &&
    dial->dial.dial == magic &&
    dial->dial.size == sizeof(*dial) &&
    dial->dial.cntl /* && dial->dial.data */
    ;
}

static inline void del_dial(dial_t * dial)
{
  assert(dial);
  assert(!dial->dial.data);
  assert(!dial->dial.cntl);
  assert(!dial->dial.size);
  assert(dial->dial.dial == ~magic);
  free(dial);
}

#define keyis(N) !strcmp(key,N)

static
int tsel(void * data, const char * key, int op, sc68_dialval_t *val)
{
  dial_t * const dial = data;
  int res;

  /* Sanity check */
  if (!key || !ismagic(dial))
    return -1;

  /* Run user control function */
  res = dial->dial.cntl(dial->dial.data, key, op, val);
  TRACE68(dial_cat,P "user-cntl #%02d \"%s\" -> %d\n", op, key, res);

  /* Kill special case. */
  if (op == SC68_DIAL_CALL && keyis(SC68_DIAL_KILL)) {
    dial->dial.cntl = 0; dial->dial.data = 0; dial->dial.size = 0;
    dial->dial.dial = ~dial->dial.dial;
    del_dial(dial);
    res = 0;
  }

  /* User dealt with that message. */
  if (res <= 0)
    goto exit;

  /* Assume no error */
  res = 0;

  if (op == SC68_DIAL_CALL && keyis(SC68_DIAL_NEW)) {
    if (!dial->dial.cntl(dial->dial.data, "sc68", op, val))
      dial->sc68 = (sc68_t *)val->s;
    if (!dial->dial.cntl(dial->dial.data, "disk", op, val))
      dial->disk = (sc68_disk_t) val->s;
    /* abort dialof on error */
    val->i = sc68_music_info(dial->sc68,&dial->info,1,dial->disk);
    if (!val->i) {
      TRACE68(dial_cat, P "Got info: %02d - %s - %s\n",
              dial->info.tracks, dial->info.album, dial->info.title);
    }
    assert(!res);
  }

  else if (keyis("track")) {
    int track;

    switch (op) {
    case SC68_DIAL_CNT:
      val->i = dial->info.tracks+1;
      TRACE68(dial_cat, P "Got %s[#] = %d\n", key, val->i);
      assert(!res);
      break;
    case SC68_DIAL_GETI:
      val->i = dial->track;
      TRACE68(dial_cat, P "Got %s[@] = %d\n", key, val->i);
      assert(!res);
      break;
    case SC68_DIAL_SETI:
      track = (int) val->i;
      if (track < 0 || track > dial->info.tracks)
        track = 0;
      if (track > 0 && track != dial->info.trk.track) {
        sc68_music_info(dial->sc68, &dial->info, track, dial->disk);
        track = dial->info.trk.track;
      }
      val->i = dial->track = track;
      assert(!res);
      break;

    case SC68_DIAL_ENUM:
      if (!val->i)
        snprintf(dial->tstr,sizeof(dial->tstr),"ALL - %s",
                 dial->info.album);
      else if (val->i <= dial->info.tracks) {
        sc68_music_info_t info;
        sc68_music_info(dial->sc68, &info, val->i, dial->disk);
        snprintf(dial->tstr,sizeof(dial->tstr),"%02u - %s",
                 info.trk.track, info.title);
      } else {
        res = -1;
        break;
      }
      dial->tstr[sizeof(dial->tstr)-1] = 0;
      val->s = dial->tstr;
      assert(!res);
      break;

    default:
      res = -1;
    }
  }

  else if (keyis("asid")) {
    static const char * asid[3] = { "Off", "On", "Force" };
    const  int max = sizeof(asid) / sizeof(*asid);
    switch (op) {
    case SC68_DIAL_CNT:
      val->i = max;
      assert(!res);
      break;
    case SC68_DIAL_GETI:
      assert (dial->asid >= 0 && dial->asid < max);
      val->i = dial->asid;
      assert(!res);
      break;
    case SC68_DIAL_SETI:
      assert (val->i >= 0 && val->i < max);
      if (val->i >= 0 && val->i < max) {
        dial->asid = val->i;
        assert(!res);
      } else
        res = -1;
      break;

    case SC68_DIAL_ENUM:
      if (val->i == -1) {
        assert (dial->asid >= 0 && dial->asid < max);
        val->s = asid[dial->asid];
        assert(!res);
      } else if (val->i >= 0 && val->i < max) {
        val->s = asid[val->i];
        assert(!res);
      } else
        res = -1;
      break;

    default:
      res = -1;
    }
  }
  else if (op == SC68_DIAL_GETS && keyis("album")) {
    val->s = dial->info.album;
  }
  else if (op == SC68_DIAL_GETI && keyis("hw_asid")) {
    val->i = dial->info.trk.asid;
  } else {
    res = -1;
  }

exit:
  if (op == SC68_DIAL_CNT) {
    TRACE68(dial_cat, P
            "%s() \"%s\"[#] -> %d (%d)\n",
            __FUNCTION__, key, val->i, res);
  }

  if (op == SC68_DIAL_GETI) {
    TRACE68(dial_cat, P
            "%s() \"%s\" -> %d (%d)\n",
            __FUNCTION__, key, val->i, res);
  }

  if (op == SC68_DIAL_GETS || op == SC68_DIAL_ENUM) {
    TRACE68(dial_cat, P
            "%s() \"%s\" -> \"%s\" (%d)\n",
            __FUNCTION__, key, !res?val->s:"<not-set>", res);
  }

  TRACE68(dial_cat, P
          "%s() #%02d \"%s\" -> %d\n",
          __FUNCTION__, op, key, res);
  return res;
}


SC68_EXTERN
int dial68_new_tsel(void ** pdata, sc68_dial_f * pcntl)
{
  int res = -1;
  const int size = sizeof(dial_t);
  dial_t * dial = (dial_t *) malloc(size);

  if (dial) {
    memset(dial,0,size);
    dial->dial.dial = magic;
    dial->dial.size = size;
    dial->dial.data = *pdata;
    dial->dial.cntl = **pcntl;
    *pcntl = tsel;
    *pdata = dial;
    res = 0;
  }
  TRACE68(dial_cat, P "%s -> %p %d\n", __FUNCTION__, (void*)dial, res);
  return res;
}
