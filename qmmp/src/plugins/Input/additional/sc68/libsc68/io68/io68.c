/*
 * @file    io68.c
 * @brief   IO chips library
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

#include <assert.h>

#include "io68.h"
#include "sc68/file68_msg.h"

static const struct {
  const char * name;
  int  (* init)    (int *, char **);
  void (* shutdown)(void);
} func[] = {
  { "paula",     paulaio_init,   paulaio_shutdown   },
  { "ym-2149",   ymio_init,      ymio_shutdown      },
  { "microwire", mwio_init,      mwio_shutdown      },
  { "mfp-68901", mfpio_init,     mfpio_shutdown     },
  { "shifter",   shifterio_init, shifterio_shutdown },
};

const int max = sizeof(func) / sizeof(*func);

int io68_init(int * argc, char ** argv)
{
  int i,err;
  for ( err = i = 0; i < max; ++i ) {
    if (func[i].init) {
      err = func[i].init(argc, argv);
      if (err) {
        msg68_error("io68: failed to initialize *%s* IO plugin\n",func[i].name);
        break;
      }
    }
  }
  return err;
}

void io68_shutdown(void)
{
  int i;
  for ( i=0; i < max; ++i ) {
    if (func[i].shutdown)
      func[i].shutdown();
  }
}

void io68_destroy(io68_t * const io)
{
  if (io) {
    assert(!io->next);
    if (io->next)
      msg68_critical("io68: destroying an attached IO <%s>\n",io->name);
    if (io->destroy)
      io->destroy(io);
    else
      emu68_free(io);
  }
}

int io68_reset(io68_t * const io)
{
  return !io
    ? -1
    : io->reset ? io->reset(io) : 0
    ;
}
