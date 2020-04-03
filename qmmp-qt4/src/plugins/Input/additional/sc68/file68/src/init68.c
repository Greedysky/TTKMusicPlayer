/*
 * @file    init68.c
 * @brief   library initialization
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
#include "file68.h"
#include "file68_opt.h"
#include "file68_msg.h"
#include "file68_err.h"
#include "file68_reg.h"
#include "file68_vfs_ao.h"
#include "file68_vfs_curl.h"
#include "file68_vfs_fd.h"
#include "file68_vfs_file.h"
#include "file68_vfs_mem.h"
#include "file68_vfs_null.h"
#include "file68_vfs_z.h"
#include "file68_rsc.h"
#include "file68_str.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static volatile int init;

int  option68_init(void);              /* defined in option68.c   */
void option68_shutdown(void);          /* defined in option68.c   */
int  file68_loader_init(void);         /* defined in file68.c     */
void file68_loader_shutdown(void);     /* defined in file68.c     */

static char * mygetenv(const char *name)
{
#ifndef HAVE_GETENV
  return 0;
#else
  return getenv(name);
#endif
}

#ifdef NATIVE_WIN32
/* Get path from registry, converts '\' to '/' and remove trailing '/'.
 *
 * @return pointer to the end of string
 * @retval 0 error
 */
static char * get_reg_path(registry68_key_t key, char * kname,
                           char * buffer, int buflen)
{
  char * e = 0;

  if (registry68_support() &&
      !registry68_gets(key,kname,buffer,buflen)) {
    for (e=buffer; *e; ++e) {
      if (*e == '\\') *e = '/';
    }
    if (e > buffer && e[-1] == '/')
      --e;
    *e = 0;
  }
  if (!e) buffer[0] = 0;
  return e;
}
#endif

static const char dbgcat[] = "debug";
static const char rsccat[] = "files";
static const char prefix[] = "sc68-";


static int ocp(const option68_t * opt, value68_t * value);
static int ocd(const option68_t * opt, value68_t * value);

/* list of options / envvars
 *
 * !!! DON'T CHANGE ORDER OR MODIFY ocp() !!!
 *
 * "debug" is a string to be evaluted as an integer later in the
 * process (once debug features have been defined).
 *
 */
static option68_t opts[] = {
  OPT68_STRG(prefix,"share-path",rsccat,"shared resource path"    ,1,ocp),
  OPT68_STRG(prefix,"user-path" ,rsccat,"user resource path"      ,0,ocp),
  OPT68_STRG(prefix,"music-path",rsccat,"music database path"     ,1,ocp),
  OPT68_STRG(prefix,"remote-uri",rsccat,"online music base URI"   ,1,ocp),
  OPT68_BOOL(prefix,"no-debug"  ,dbgcat,"disable all debug output",0,0),
  OPT68_STRG(prefix,"debug"     ,dbgcat,"set debug features"      ,0,ocd),
};

static int ocp(const option68_t * opt, value68_t * value)
{
  static const char * (*f[4])(const char *) = {
    rsc68_set_share,rsc68_set_user,rsc68_set_music,rsc68_set_remote_music
  };
  int idx = opt - opts;
  assert(idx >= 0 && idx < 4);

  return -!f[idx](value->str);
}

static int ocd(const option68_t * opt, value68_t * value)
{
  static const char delim[] = "+/|-~=";
  int op, j = 0;
  const char * str = value->str;
  char arg[64];

  op = strchr(delim, *str) ? *str++ : '=';
  while (op) {
    int c = *str++;
    if (!c || strchr(delim,c)) {
      if (j > 0) {
        unsigned int bits;
        arg[j] = 0; j = 0;
        if (!strcmp68(arg,"all"))
          bits = -1;
        else if (arg[0] == '#' && isdigit((int)arg[1]))
          bits = 1 << strtol(arg+1,0,0);
        else if (isdigit((int)arg[0]))
          bits = strtol(arg,0,0);
        else {
          int bit = msg68_cat_bit(arg);
          if (bit >= 0)
            bits = 1 << bit;
          else
            bits = 0;
        }
        switch (op) {
        case '=':
          msg68_cat_filter(~0, bits); break;
        case '-': case '~':
          msg68_cat_filter(bits, 0); break;
        case '+': case '/': case '|':
          msg68_cat_filter(0, bits); break;
        }
      }
      op = c;
    } else if (j < sizeof(arg)-1)
      arg[j++] = c;
  }
  return 0;
}


static char * convert_backslash(char * s) {
  int i;
  for (i=0; s[i]; ++i)
    if (s[i] == '\\')
      s[i] = '/';
  return s;
}

int file68_init(int argc, char **argv)
{
  char tmp[1024];
  option68_t * opt;

  if (init)
    return -1;
  init = 3;

  /* Options */
  option68_init();

#if !defined(DEBUG) || defined(NDEBUG)
  opts[4].hide = opts[5].hide = 1;      /* hide debugs option */
#endif

  option68_append(opts,sizeof(opts)/sizeof(*opts));
  argc = option68_parse(argc, argv);

  /* Check for --sc68-no-debug */
  opt = option68_get("no-debug", opt68_ISSET);
  if (opt && opt->val.num) {
    /* Remove all debug messages whatsoever. */
    msg68_set_handler(0);
  }

  /* Zlib */
  vfs68_z_init();

  /* Curl */
  vfs68_curl_init();

  /* Xiph AO */
  vfs68_ao_init();

  /* Memory */
  vfs68_mem_init();

  /* NUll */
  vfs68_null_init();

  /* File descriptor */
  vfs68_fd_init();

  /* File */
  vfs68_file_init();

  /* Resource locator */
  rsc68_init();

  /* Loader */
  file68_loader_init();

#ifdef NATIVE_WIN32
  /* Get share path from registry */
  opt = option68_get("share-path", opt68_ALWAYS);
  if (opt) {
    /* Get data path from registry */
    if (!option68_isset(opt)) {
      if (get_reg_path(0, "LMK:SOFTWARE/sashipa/sc68/Install_Dir",
                       tmp, sizeof(tmp)))
        option68_set(opt,tmp,opt68_ALWAYS,opt68_CFG);
    }
  }
#endif

  /* Get user path  */
  opt = option68_get("user-path", opt68_ALWAYS);
  if (opt) {

#ifdef NATIVE_WIN32
    static const char path[] = "/sc68";

    /* Get user path from registry */
    if (!option68_isset(opt)) {
      char * env;
      env = get_reg_path(0, "CUK:Volatile Environment/APPDATA",
                         tmp, sizeof(tmp));
      if(env && strlen(env)+sizeof(path) < sizeof(tmp)) {
        strcpy(tmp,env);
        strcat(tmp,path);
        option68_set(opt,tmp,opt68_ALWAYS,opt68_CFG);
      }
    }
    if (!option68_isset(opt)) {
      /* Get user from win32 env */
      const char * drv = mygetenv("HOMEDRIVE");
      const char * env = mygetenv("HOMEPATH");
      if (drv && env && strlen(drv)+strlen(env)+sizeof(path) < sizeof(tmp)) {
        strcpy(tmp,drv);
        strcat(tmp,env);
        strcat(tmp,path);
        option68_set(opt,convert_backslash(tmp),opt68_ALWAYS,opt68_CFG);
      }
    }
#endif
    /* Get user path from HOME (usually not defined for win32 platform) */
    if (!option68_isset(opt)) {
      const char path[] = "/.sc68";
      const char * env = mygetenv("HOME");
      if(env && strlen(env)+sizeof(path) < sizeof(tmp)) {
        strcpy(tmp,env);
        strcat(tmp,path);
        option68_set(opt,convert_backslash(tmp),opt68_ALWAYS,opt68_CFG);
      }
    }
  }

  init = 1;
  return argc;
}

void file68_shutdown(void)
{
  if (init == 1) {
    init = 2;

    /* Options */
    option68_shutdown();

    /* Loader */
    file68_loader_shutdown();

    /* Shutdown resource */
    rsc68_shutdown();

    /* Zlib */
    vfs68_z_shutdown();

    /* Curl */
    vfs68_curl_shutdown();

    /* Xiph AO */
    vfs68_ao_shutdown();

    /* Memory */
    vfs68_mem_shutdown();

    /* NUll */
    vfs68_null_shutdown();

    /* File descriptor */
    vfs68_fd_shutdown();

    /* File */
    vfs68_file_shutdown();

    init = 0;
  }
}

int file68_version(void)
{
#ifndef PACKAGE_VERNUM
# define PACKAGE_VERNUM 0
#endif
  return PACKAGE_VERNUM;
}

const char * file68_versionstr()
{
#ifndef PACKAGE_STRING
# define PACKAGE_STRING "init68 n/a"
#endif
  return PACKAGE_STRING;
}
