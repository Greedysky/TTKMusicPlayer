/*
 * @file    fileinfo.c
 * @brief   sc68 file info dialog
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

static const int magic = ('F'<<24)|('I'<<16)|('N'<<8)|'F';

struct dial_s {
  dial68_t     dial;                    /* base dialog */
  sc68_t      *sc68;                    /* result of "sc68" call */
  sc68_disk_t  disk;                    /* result of "disk" call */
  sc68_minfo_t info;                    /* current track info    */
  char         tstr[4];                 /* Temp storage for track # */
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
int finf(void * data, const char * key, int op, sc68_dialval_t *val)
{
  dial_t * const dial = data;
  int res;

  assert(ismagic(dial));
  assert(key);
  assert(val);

  /* Sanity check */
  if (!key || !val || !ismagic(dial))
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
      dial->sc68 = (sc68_disk_t) val->s;
    if (!dial->dial.cntl(dial->dial.data, "disk", op, val))
      dial->disk = (sc68_disk_t) val->s;
    TRACE68(dial_cat, P "\"%s\" sc68:%p disk:%p\n",
            key, (void*)dial->sc68, (void*)dial->disk);
    val->i = sc68_music_info(dial->sc68,&dial->info,1,dial->disk);
    if (!val->i) {
      TRACE68(dial_cat, P "Got info: %02d - %s - %s\n",
              dial->info.tracks, dial->info.artist, dial->info.album);
    }
  } else if (keyis("track")) {
    int track;
    switch (op) {

    case SC68_DIAL_CNT:
      val->i = dial->info.tracks; /* get count current */
      return 0;

    case SC68_DIAL_GETI:
      val->i = dial->info.trk.track - 1; /* get current */
      return 0;

    case SC68_DIAL_SETI:
      track = (int) val->i + 1;
      if (track <= 0 || track > dial->info.tracks)
        track = dial->info.dsk.track;
      if (track != dial->info.trk.track)
        sc68_music_info(dial->sc68, &dial->info, track, dial->disk);
      val->i = dial->info.trk.track - 1;
      return 0;

    case SC68_DIAL_ENUM:
      track = val->i;
      if (track >= 0 && track < dial->info.tracks) {
        ++track;
        dial->tstr[0] = '0' + (track/10u);
        dial->tstr[1] = '0' + (track%10u);
        dial->tstr[2] = 0;
        val->s = dial->tstr;
      } else {
        TRACE68(dial_cat, P "invalid index \"%s[%d]\"", key,track);
        val->s = "";
        res = -1;
      }
      break;

    default:
      res = -1;
    }
  } else {

    switch (op) {

    case SC68_DIAL_GETS:
      /* if (keyis("uri")) */
      /*   val->s = dial->uri; */
      /* else */
      if (keyis("format"))
        val->s = dial->info.format;
      else if (keyis("genre"))
        val->s = dial->info.genre;
      else if (keyis("title"))
        val->s = dial->info.title;
      else if (keyis("artist"))
        val->s = dial->info.artist;
      else if (keyis("album"))
        val->s = dial->info.album;
      else if (keyis("ripper"))
        val->s = dial->info.ripper;
      else if (keyis("converter"))
        val->s = dial->info.converter;
      else if (keyis("year"))
        val->s = dial->info.year;
      else
        res = 1;
      break;

    case SC68_DIAL_GETI:
      if (keyis("time"))
        val->i = (dial->info.trk.time_ms+500u)/1000u;
      else if (keyis("hw_ym"))
        val->i = dial->info.trk.ym;
      else if (keyis("hw_ste"))
        val->i = dial->info.trk.ste;
      else if (keyis("hw_asid"))
        val->i = dial->info.trk.asid;
      else if (keyis("tag-key"))
        val->i = 0;
      else
        res = 1;
      break;

    case SC68_DIAL_ENUM:
      if (keyis("tag-key") || keyis("tag-val")) {
        const int a = dial->info.dsk.tags;
        const int n = a + dial->info.trk.tags;
        if (val->i >= 0 && val->i<n) {
          val->s = val->i < a
            ? ( key[4] == 'k'
                ? dial->info.dsk.tag[val->i].key
                : dial->info.dsk.tag[val->i].val )
            : ( key[4] == 'k'
                ? dial->info.trk.tag[val->i-a].key
                : dial->info.trk.tag[val->i-a].val )
            ;
        } else
          res = -1;
      } else
        res = 1;
      break;

    default:
      res = 1;
    }
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
int dial68_new_finf(void ** pdata, sc68_dial_f * pcntl)
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
    *pcntl = finf;
    *pdata = dial;
    res = 0;
  }
  TRACE68(dial_cat, P "%s -> %p %d\n", __FUNCTION__, (void*)dial, res);
  return res;
}
