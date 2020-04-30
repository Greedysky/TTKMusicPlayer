/*
 * @file    istream68_ao.c
 * @brief   implements istream68 for libao
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
#include "file68_vfs_ao.h"
#include "file68_msg.h"

#define AOHD "libao68: "

#ifndef DEBUG_AO68_O
# define DEBUG_AO68_O 0
#endif

/* Define this if you want xiph libao support. */
#ifdef USE_AO

static int ao68_cat = msg68_DEFAULT;

#include "file68_vfs_def.h"
#include "file68_str.h"
#include "file68_uri.h"
#include "file68_features.h"

#include <ao/ao.h>
#include <string.h>
#include <stdlib.h>

struct ao68_info_s {
  int                default_id;
  ao_device        * default_device;
  int                driver_id;
  ao_device        * device;
  ao_sample_format   format;
  ao_option        * options;
};

typedef struct ao68_info_s ao68_info_t;

/** vfs ao structure. */
typedef struct {
  vfs68_t vfs;                /**< vfs function.                    */
  unsigned int count;         /**< current position.                */
  ao68_info_t ao;             /**< ao specific struct.              */
  int  no_overwrite;          /**< overwriting file output.         */
  char *outname;              /**< part of name[] for file driver.  */
  char defoutname[16];        /**< default outname for file driver. */
  char name[1];               /**< filename.                        */
} vfs68_ao_t;

enum {
  AO_SPR_MIN = FILE68_SPR_MIN,
  AO_SPR_MAX = FILE68_SPR_MAX,
  AO_SPR_DEF = FILE68_SPR_DEF
};

static volatile int init;
static unsigned int vfs68_ao_defaut_rate = AO_SPR_DEF;
static vfs68_ao_t * ao_unic_vfs;
static int ao_ismine(const char *);
static vfs68_t * ao_create(const char *, int, int, va_list);
static scheme68_t ao_scheme = {
  0, "vfs-ao", ao_ismine, ao_create
};

static unsigned int spr_in_range(unsigned int rate)
{
  if (rate > (unsigned)AO_SPR_MAX) {
    rate = AO_SPR_MAX;
  } else if (rate < (unsigned)AO_SPR_MIN) {
    rate = AO_SPR_MIN;
  }
  return rate;
}

int vfs68_ao_sampling(vfs68_t * vfs)
{
  vfs68_ao_t * is = (vfs68_ao_t *)vfs;
  if (!is || !is->ao.format.rate)
    return vfs68_ao_defaut_rate;
  return is->ao.format.rate;
}

/* Init does not actually initialize aolib. That task will be
 * performed on the fly only if needed.
 */
int vfs68_ao_init(void)
{
  if (ao68_cat == msg68_DEFAULT) {
    ao68_cat =
      msg68_cat("audio","Xiph libao audio VFS",DEBUG_AO68_O);
    if (ao68_cat == -1)
      ao68_cat = msg68_DEFAULT;
  }
  uri68_register(&ao_scheme);
  return 0;
}


static int init_aolib(void)
{
  int err = -1;

  if (init) {
    msg68_critical(AOHD "*%s*\n","already done");
  } else {
    ao_initialize();
    err = 0;
    init = 1;
    TRACE68(ao68_cat,AOHD "*%s*\n","initialized");
  }
  return err;
}

static void shutdown_aolib(void)
{
  if (init) {
    init = 0;
    ao_shutdown();
    TRACE68(ao68_cat,
            AOHD "*%s*\n","shutdowned");
  }
}

void vfs68_ao_shutdown(void)
{
  shutdown_aolib();
  if (ao68_cat != msg68_DEFAULT) {
    msg68_cat_free(ao68_cat);
    ao68_cat = msg68_DEFAULT;
  }
}

static int ao_ismine(const char * uri)
{
  if (!strncmp68(uri, "audio:", 6))
    return SCHEME68_ISMINE | SCHEME68_WRITE;
  return 0;
}

static const char * isao_name(vfs68_t * vfs)
{
  vfs68_ao_t * ism = (vfs68_ao_t *)vfs;

  return (!ism || !ism->name[0])
    ? 0
    : ism->name
    ;
}

static void dump_ao_info(const int id, const int cat,
                         const ao_info * info, const int full)
{
  if (info) {
    msg68(
      cat,
      "\n"
      AOHD "ao_driver #%02d\n"
      "         ,------------\n"
      "         | type    : %s\n"
      "         | name    : %s\n"
      "         | short   : %s\n"
      "         | author  : %s\n"
      "         | comment : %s\n"
      "         | prio    : %d\n",
      id, info->type==AO_TYPE_LIVE?"live":"file",
      strnevernull68(info->name),
      strnevernull68(info->short_name),
      strnevernull68(info->author),
      strnevernull68(info->comment),info->priority);
    if (full) {
      int i;
      msg68(cat, "         | options : %d --", info->option_count);
      for (i=0; i<info->option_count; ++i)
        msg68(cat, " %s",info->options[i]);
      msg68(cat, "%c",'\n');
    }
  }
}

static void dump_ao_format(const ao_sample_format * fmt)
{
  TRACE68(ao68_cat,
          AOHD "sample format -- %d-bits %d-hz %d-chan %c-endian %s\n",
          fmt->bits, fmt->rate, fmt->channels,
          fmt->byte_format == AO_FMT_LITTLE ? 'L' :
          (fmt->byte_format == AO_FMT_BIG ? 'B' : 'N'),
          fmt->matrix?fmt->matrix:"no-matrix");
}

static int isao_open(vfs68_t * vfs)
{
  vfs68_ao_t * is = (vfs68_ao_t *)vfs;
  int err = -1;
  ao_info * info = 0;
  char * uri;

  TRACE68(ao68_cat,AOHD "open -- '%s'\n", vfs68_filename(vfs));
  if (!is || is->ao.device) {
    goto error;
  }

  uri = is->name+6;                     /* skip "audio:" */
  do {
    char * s = strchr(uri,'/'), *s2, *key = uri, *val = 0;
    if (s == uri) {
      ++uri; continue;                  /* skip "//" instance */
    }
    if (s) *s = 0;
    s2 = strchr(uri,'=');
    if (s2) {
      *s2 = 0;
      val = s2+1;
    }

    TRACE68(ao68_cat,
            AOHD "open -- KEY[%s]='%s'\n", key, val?val:"");
    if (!*key) {
      /* nothing to do */
    } else if (!strcmp68(key,"driver")) {
      /* key: driver=list|default|name */
      if (!strcmp68(val,"list") ) {
        /* List all drivers in debug */
        int id,n;
        ao_info ** infolist = ao_driver_info_list(&n);
        if (infolist) {
          for (id=0; id<n; ++id) {
            dump_ao_info(id, msg68_INFO, infolist[id], 1);
          }
        }
      } else {
        int id = !strcmp68(val,"default")
          ? ao_default_driver_id()
          : ao_driver_id(val);
        info = ao_driver_info(id);
        if (info) {
          is->ao.driver_id = id;
          TRACE68(ao68_cat,AOHD "open -- *DRIVER* #%d (%s) %s\n",
                  id, info->short_name, info->name);
        }
      }
    } else if (!strcmp68(key,"rate")) {
      /* key: rate=[0-9]+ */
      int frq = 0;
      while (*val>='0' && *val<='9') frq = frq*10 + *val++ - '0';
      if (frq) {
        frq = spr_in_range(frq);
        TRACE68(ao68_cat,AOHD "open -- *SAMPLING-RATE* %d\n",frq);
        is->ao.format.rate = frq;
      }
    } else if (!strcmp68(key,"format")) {
      /* key: format=[-+nlb12WBF] */
      while(*val) {
        switch (*val++) {

          /* ENDIANESS */
        case 'n': /* native (same as cpu) */
          is->ao.format.byte_format = AO_FMT_NATIVE;
          break;
        case 'l': /* little */
          is->ao.format.byte_format = AO_FMT_LITTLE;
          break;
        case 'b': /* big    */
          is->ao.format.byte_format = AO_FMT_BIG;
          break;

          /* SIGN */
        case '+': /* unsigned */
          msg68_warning(AOHD "ignoring -- *%s* request\n", "UNSIGNED");
        case '-': /*   signed */
          break;

          /* CHANNELS */
        case '1': /* mono, 1 channel */
          is->ao.format.channels = 1;
          break;
        case '2': /* stereo, 2 channels */
          is->ao.format.channels = 2;
          break;

          /* FORMAT */
        case 'W': /* 16 bit */
          is->ao.format.bits = 16;
          break;
        case 'B': /*  8 bit */
          is->ao.format.bits = 8;
          break;
        case 'F': /* float  */
          msg68_warning(AOHD "open -- ignoring *%s* request\n","FLOAT");
          break;
        } /* switch */
      } /* while */
    } else  if (!strcmp68(key,"output")) {
      /* key: output=.* */
      if (s) *s = '/';          /* restore removed '/' in the URI */
      s = 0;                    /* trick to end the options parsing */
      is->outname = val;
      TRACE68(ao68_cat, AOHD "open -- OUT='%s'\n", is->outname);
    } else {
      /* int i = 0; */
      /* anything else is sent to ao driver as option */
      if (!val)
        val = (*key == '!' || *key == '~') ? "0" : "1";
      /* removed all test for options, juut add */
      ao_append_option(&is->ao.options, key, val);

      /* if (!info) */
      /*   msg68_warning(AOHD "choose driver before options -- [%s]='%s'\n", */
      /*                 key, val); */
      /* else */
      /*   for (; i<info->option_count && strcmp68(info->options[i],key); ++i) */
      /*     ; */
      /* if (!info || i<info->option_count) { */
      /*   int res = ao_append_option(&is->ao.options, key, val); */
      /*   res = res; */
      /*   TRACE68(ao68_cat, AOHD "add options [%s]='%s' -- *%s*\n", */
      /*           key, val, strok68(!res)); */
      /* } else */
      /*   msg68_warning(AOHD "ignore option for driver '%s' -- [%s]='%s'\n", */
      /*                 info->short_name, key, val); */
    }
    if (s2) *s2 = '=';                /* restore '=' */
    uri = 0;
    if (s) {
      *s = '/';                       /* restore '/' */
      uri = s+1;                      /* next */
    }
  } while (uri);

  info = ao_driver_info(is->ao.driver_id);
  if (!info) {
    goto error;
  }

  dump_ao_info(is->ao.driver_id, ao68_cat, info, 1);

  if (!is->outname) {
#ifdef HAVE_AO_FILE_EXTENSION
    char * ext = ao_file_extension(is->ao.driver_id);
#else
    const char * ext = ".out";
    static const char * exts[] = {
      ".wav", ".iff", ".raw", ".au", ".mp3", ".ogg"
    };
    int e;
    for (e = 0; e < sizeof(exts)/sizeof(*exts); ++e)
      if (!strcmp68(info->short_name,exts[e]+1)) {
        ext = exts[e];
        break;
      }

#endif
    strcpy(is->defoutname,"sc68");
    strcat68(is->defoutname,ext,sizeof(is->defoutname)-1);
    is->outname = is->defoutname;
    TRACE68(ao68_cat,
            AOHD "open -- default *FILENAME* '%s'\n", is->outname);
  }

  if (is->ao.options) {
    int i;
    ao_option * o;
    TRACE68(ao68_cat,"%s\n", AOHD "options summary:");
    for (i=1, o=is->ao.options; o; o=o->next, ++i) {
      TRACE68(ao68_cat,
              AOHD " - #%d %-12s = '%s'\n", i, o->key, o->value);
    }
  }

  if (!is->ao.format.rate) {
    is->ao.format.rate = vfs68_ao_defaut_rate;
    TRACE68(ao68_cat,AOHD "%s\n", "using default sampling rate");
  }
  dump_ao_format(&is->ao.format);
  is->ao.device =
    (info->type==AO_TYPE_LIVE)
    ? ao_open_live(is->ao.driver_id, &is->ao.format, is->ao.options)
    : ao_open_file(is->ao.driver_id, is->outname, !is->no_overwrite,
                   &is->ao.format, is->ao.options);
  if (!is->ao.device) {
    goto error;
  }

  if (info->type == AO_TYPE_LIVE) {
    msg68_notice(AOHD "live driver -- *%s*\n", info->short_name);
  } else {
    msg68_notice(AOHD "file driver -- *%s* -- '%s''\n",
                 info->short_name, is->outname);
  }
  dump_ao_format(&is->ao.format);

  is->count = 0;
  err = 0;
error:
  TRACE68(ao68_cat, AOHD "open -- *%s*\n", strok68(err));
  return err;
}

static int isao_close(vfs68_t * vfs)
{
  vfs68_ao_t * is = (vfs68_ao_t *)vfs;
  int err = -1;

  if (is->ao.options) {
    ao_free_options(is->ao.options);
    is->ao.options = 0;
  }
  if (is->ao.device) {
    ao_close(is->ao.device);
    is->ao.device = 0;
    err = 0;
  }
  TRACE68(ao68_cat, AOHD "close -- *%s*\n", strok68(err));
  return err;
}

static int isao_read(vfs68_t * vfs, void * data, int n)
{
  return -1;
}

static int isao_write(vfs68_t * vfs, const void * data, int n)
{
  vfs68_ao_t * isao = (vfs68_ao_t *)vfs;

  if (!isao || !isao->ao.device) {
    return -1;
  }
  if (!n) {
    return 0;
  }
  if (n<0) {
    return 0;
  }
  if (ao_play(isao->ao.device, (void *)data, n)) {
    isao->count += n;
    return n;
  } else {
    return -1;
  }
}

static int isao_flush(vfs68_t * vfs)
{
  vfs68_ao_t * isao = (vfs68_ao_t *)vfs;
  return !isao->ao.device
    ? -1
    : 0
    ;
}

static int isao_length(vfs68_t * vfs)
{
  vfs68_ao_t * isao = (vfs68_ao_t *)vfs;
  return !isao->ao.device
    ? -1
    : isao->count
    ;
}

static int isao_tell(vfs68_t * vfs)
{
  return isao_length(vfs);
}

static int isao_seek(vfs68_t * vfs, int offset)
{
  return -1;
}

static void isao_destroy(vfs68_t * vfs)
{
  TRACE68(ao68_cat, AOHD "destroy -- '%s'\n", vfs68_filename(vfs));
  if (&ao_unic_vfs->vfs == vfs)
    ao_unic_vfs = 0;
  vfs68_ao_shutdown();
  free(vfs);
}

static const vfs68_t vfs68_ao = {
  isao_name,
  isao_open, isao_close,
  isao_read, isao_write, isao_flush,
  isao_length, isao_tell, isao_seek, isao_seek,
  isao_destroy
};

/* ao stream URI is composed by /key=value pairs.
 *
 * Specific keys are:
 *
 *  - driver=list
 *  - driver=[null|alsa|pulse|oss|esd|au|raw|wav ...]
 *  - rate=hz
 *  - format=[endianess][sign][channels][format]
 *    with (first value is the default)
 *        - endian   := [n|l|b]
 *        - sign     := [-|+]
 *        - channels := [1|2]
 *        - format   := [W|B|F]
 *
 * Other keys will be used as ao driver options.
 *
 * First non-key component (without '=') is used as the output name
 * (for file drivers).
 *
 */
vfs68_t * vfs68_ao_create(const char * uri, int mode, int spr)
{
  vfs68_ao_t *isf=0;
  int len;
  ao68_info_t ao;

  TRACE68(ao68_cat,
          AOHD "create -- '%s' (%d) %dhz\n",
          strnevernull68(uri), mode, spr);

  if (!init) {
    if (init_aolib() || !init) {
      msg68_critical(AOHD "create error -- *libao*\n");
      goto error;
    }
  }

  if (!uri || !ao_ismine(uri)) {
    msg68_error(AOHD "create error -- *parameter*\n");
    goto error;
  }

  if (mode != VFS68_OPEN_WRITE) {
    msg68_error(AOHD "create error -- *mode*\n");
    goto error;
  }

  len = strlen(uri) + 8;           /* Add a bit for file extension. */
  isf = calloc(sizeof(vfs68_ao_t) + len, 1);
  if (!isf) {
    goto error;
  }

  /* Setup for default driver */
  memset(&ao,0,sizeof(ao));
  ao.default_id         = ao_default_driver_id();
  if (ao.default_id < 0) {
    /* Could not find a default driver, that's probably bad not fatal
     * yet.
     *
     * Following code tries to set libao null driver as the default
     * driver but that's probably not such a good idea. We'd rather
     * know if for some reason there is no working driver.
     */
#if 0
    ao.default_id = ao_driver_id("null");
    if (ao.default_id < 0)
      msg68_warning(AOHD "could not find a default driver\n");
    else
      msg68_warning(AOHD "using *null* driver as default\n");
#endif
  }
  ao.driver_id          = ao.default_id;
  ao.format.bits        = 16;
  ao.format.channels    = 2;
  ao.format.rate        = spr ? spr_in_range(spr) : 0;
  ao.format.byte_format = AO_FMT_NATIVE;

  /* Copy vfs functions. */
  memcpy(&isf->vfs, &vfs68_ao, sizeof(vfs68_ao));
  isf->ao   = ao;
  strcpy(isf->name, uri);

error:
  if (!ao_unic_vfs)
    ao_unic_vfs = isf;
  uri = vfs68_filename(&isf->vfs);
  TRACE68(ao68_cat,
          AOHD "create -- *%s* -- '%s'\n",
          strok68(!isf),strnevernull68(uri));
  return isf ? &isf->vfs : 0;
}

static vfs68_t * ao_create(const char * uri, int mode, int argc, va_list list)
{
  unsigned int rate = 0;
  if (argc == 1)
    rate = va_arg(list,unsigned int);
  return vfs68_ao_create(uri, mode, rate);
}

#else /* #ifdef USE_AO */

/* vfs ao must not be include in this package. Anyway the creation
 * still exist but it always returns error.
 */

#include "file68_vfs_ao.h"
#include "file68_vfs_def.h"

int vfs68_ao_init(void) { return 0; }
void vfs68_ao_shutdown(void) { }

#endif
