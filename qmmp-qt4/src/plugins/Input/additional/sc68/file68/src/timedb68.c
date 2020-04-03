/*
 * @file    timedb68.c
 * @brief   music duration database
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
#include "file68_tdb.h"

#include <stdlib.h>

#define HBIT 32                         /* # of bit for hash     */
#define TBIT 6                          /* # of bit for track    */
#define WBIT 5                          /* # of bit for hardware */
#define FBIT (64-HBIT-TBIT-WBIT)        /* # of bit for frames   */
#define HFIX (32-HBIT)

#define TIMEDB_ENTRY(HASH,TRACK,FRAMES,FLAGS) \
  { 0x##HASH>>HFIX, TRACK, FLAGS, FRAMES }
#define E_EMPTY { 0,0,0,0 }


typedef struct {
  unsigned int hash   : HBIT;           /* hash code              */
  unsigned int track  : TBIT;           /* track number (0-based) */
  unsigned int flags  : WBIT;           /* see enum               */
  unsigned int frames : FBIT;           /* length in frames       */
} dbentry_t;

#ifndef HAVE_TIMEDB_INC_H
# define HAVE_TIMEDB_INC_H 1
#endif

#if HAVE_TIMEDB_INC_H

#define STE TDB_STE
#define YM  ( TDB_TA + TDB_TB + TDB_TC + TDB_TD )
#define TA  -TDB_TA
#define TB  -TDB_TB
#define TC  -TDB_TC
#define TD  -TDB_TD
#define NA  -YM

static dbentry_t db[] = {
# include "timedb.inc.h"
  /* Add a bit of Supplemental empty space */
  E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,
  E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,
  E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,
  E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,
  E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,
  E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,
  E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,
  E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,E_EMPTY,
};
# define DB_MAX   (sizeof(db)/sizeof(*db))
# define DB_COUNT DB_MAX-64
# define DB_SORT  1
#else
# define DB_MAX   10000
# define DB_COUNT 0
# define DB_SORT  1
static dbentry_t db[DB_MAX];
#endif

static int dbcount  = DB_COUNT;      /* entry count                 */
static int dbmax    = DB_MAX;        /* max entry                   */
static int dbsort   = DB_SORT;       /* set if db is sorted         */
static int dbchange = 0;             /* set if db has been modified */

/* static const char * db_uri="sc68://timedb"; */

static int cmp(const void * ea, const void *eb)
{
  const dbentry_t * a = (const dbentry_t *) ea;
  const dbentry_t * b = (const dbentry_t *) eb;
  int v;

  v = a->hash - b->hash;
  if (!v)
    v = a->track - b->track;
  return v;
}

static dbentry_t * search_for(const dbentry_t * key)
{
  if (!dbsort) {
    qsort(db,dbcount,sizeof(dbentry_t), cmp);
    dbsort = 1;
  }
  return (dbentry_t *) bsearch(key, db, dbcount, sizeof(dbentry_t), cmp);
}


int timedb68_add(int hash, int track, unsigned int frames, int flags)
{
  dbentry_t e, *s;

  if ((unsigned)track >= (1u << TBIT) || frames >= (1u << FBIT) )
    return -1;

  e.hash   = hash >> HFIX;
  e.track  = track;
  e.frames = frames;
  e.flags  = flags;

  s = search_for(&e);
  if (!s && dbcount < dbmax) {
    s = db + dbcount++;
    dbsort = 0;
  }
  if (s)
    *s = e;
  return s - db;
}

int timedb68_get(int hash, int track, unsigned int * frames, int * flags)
{
  dbentry_t e, *s;
  e.hash   = hash >> HFIX;
  e.track  = track;
  s = search_for(&e);
  if (s) {
    if (frames) *frames = s->frames;
    if (flags)  *flags  = s->flags;
    return s - db;
  }
  return -1;
}

int timedb68_load(void) {
  return -1;
}

int timedb68_save(void) {
  if (dbchange) {
    dbchange = 0;
  }
  return -1;
}
