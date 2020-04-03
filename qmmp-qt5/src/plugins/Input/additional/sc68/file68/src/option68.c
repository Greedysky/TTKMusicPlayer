/*
 * @file    option68.c
 * @brief   command line options
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
#include "file68_opt.h"
#include "file68_str.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

static option68_t * opts;
static char empty[] = "";

#define FOREACH_OPT(opt) for (opt=opts; opt; opt = opt->next)

static inline int in_range(const option68_t * const opt, const int val)
{
  return opt->min == opt->max || (val >= opt->min && val <= opt->max);
}

static inline int opt_isset(const option68_t * const opt)
{
  return opt->org != opt68_UDF;
}

static inline int opt_type(const option68_t * const opt)
{
  return opt->type;
}

static inline int opt_org(const option68_t * const opt)
{
  return opt->org;
}

static int opt_policy(const option68_t * const opt, int set, int org)
{
  switch (set) {
  case opt68_PRIO:   set = org >= (int)opt->org; break;
  case opt68_NOTSET: set = !opt_isset(opt); break;
  case opt68_ISSET:  set =  opt_isset(opt); break;
  }
  return set;
}

static inline void opt_free_str(option68_t * opt)
{
  if (opt->type == opt68_STR) {
    if (opt->val.str != empty) {
      free((void *) opt->val.str);
      opt->val.str = empty;
    }
  }
}

static inline void opt_unset(option68_t * opt)
{
  opt_free_str(opt);
  opt->org = opt68_UDF;
}

static inline void opt_set_int(option68_t * opt, int org, int val)
{
  value68_t value;

  assert(opt->type == opt68_INT ||
         opt->type == opt68_BOL ||
         opt->type == opt68_ENU);

  value.num = val;
  if (!opt->onchange || !opt->onchange(opt, &value)) {
    opt->org = org;
    opt->val.num = value.num;
  }
  /* return value.num; */
}

static inline void opt_set_str(option68_t * opt, int org, const char * val)
{
  value68_t value;

  assert(opt->type == opt68_STR);
  value.str = val;
  if (!opt->onchange || !opt->onchange(opt, &value)) {
    char * s;
    s = strdup68(value.str);
    if (s) {
      opt_free_str(opt);
      opt->val.str = s;
      opt->org = org;
    }
  }
}

static int in_enum(const char * str, const char * enums[], const int n)
{
  int i;
  for (i=0; i<n; ++i)
    if (!strcmp68(str,enums[i]))
      return i;
  return -1;
}

const char * f_true[] = { "true", "yes", "on", "1" };
const char * f_false[] = { "false", "no", "off", "0" };

static void opt_set_strtol(option68_t * opt, int org, const char * val)
{
  int res = 0, ok = 0;
  int type = opt_type(opt);

  switch (type) {
  case opt68_BOL:
    ok = 1;
    if (!val || !*val)
      res = -1;
    else if (in_enum(val,f_true,sizeof(f_true)/sizeof(*f_true)) >= 0)
      res = -1;
    else if (in_enum(val,f_false,sizeof(f_false)/sizeof(*f_false)) >= 0)
      res = 0;
    else
      ok = 0;
    break;

  case opt68_ENU:
    res = in_enum(val, (const char **)opt->set, opt->sets);
    ok  = res >= 0;
    if (ok)
      break;

  case opt68_INT:
    ok = val[*val=='-' || *val=='+'];
    ok = ok >= '0' && ok <= '9';
    if (ok) {
      res = strtol(val,0,0);
      ok = in_range(opt,res);
    }
    break;

  default:
    assert(!"invalid option type");
  }

  /* if (!ok) { */
  /*   res = opt->val.num; */
  /* } else { */
  if (ok)
    opt_set_int(opt, org, res);
}

static option68_t * opt_of(const char * key)
{
  option68_t * opt;
  FOREACH_OPT(opt) if (!strcmp68(key, opt->name)) break;
  return opt;
}

int option68_type(const option68_t * opt)
{
  return opt ? opt_type(opt) : -1;
}

int option68_unset(option68_t * opt)
{
  int err = -1;
  if (opt) {
    opt_unset(opt);
    err = 0;
  }
  return err;
}

void option68_unset_all(void)
{
  option68_t * opt;
  FOREACH_OPT(opt) opt_unset(opt);
}

int option68_set(option68_t * opt, const char * str, int set, int org)
{
  int err = -1;
  if (opt) {
    if (opt_policy(opt, set, org)) {
      err = 0;
      switch (opt_type(opt)) {
      case opt68_STR:
        opt_set_str(opt,org,str); break;
      case opt68_BOL: case opt68_INT: case opt68_ENU:
        opt_set_strtol(opt,org,str); break;
      default:
        err = -1;
      }
    }
  }
  return err;
}

int option68_iset(option68_t * opt, int val, int set, int org)
{
  int err = -1;
  if (opt) {
    if (opt_policy(opt, set, org)) {
      err = 0;
      switch (opt_type(opt)) {
      case opt68_BOL:
        val = -!!val;
      case opt68_ENU: case opt68_INT:
        if (!in_range(opt,val))
          err = -1;
        else
          /* err =  */opt_set_int(opt,org,val);
        break;

      case opt68_STR: {
        char tmp[64];
        snprintf(tmp,sizeof(tmp),"%d",val);
        tmp[sizeof(tmp)-1] = 0;
        /* err =  */opt_set_str(opt,org,tmp);
        msg68_warning("option68: %s\n", "setting string option with integer");
      } break;
      default:
        err = -1;
      }
    }
  }
  return err;
}

int option68_parse(int argc, char ** argv)
{
  int i,n;
  option68_t * opt;

  /* Parse arguments */
  for (i=n=1; i<argc; ++i) {
    int negate = 0;
    char * arg = argv[i], * rearg;

    /* Check for `--' prefix */
    if (arg[0] != '-' || arg[1] != '-') {
      goto keep_it;             /* Not an option; keep it */
    }

    /* '--' breaks options parsing */
    if (!arg[2]) {
      argv[n++] = argv[i++];
      break;
    }
    arg += 2;

    /* Checking for sc68 prefixed options (--sc68- or --no-sc68-) */
    if (arg[0]=='n' && arg[1] == 'o' && arg[2] == '-') {
      negate = 1;
      arg += 3;
    }

    rearg = arg;
    FOREACH_OPT(opt) {
      const int opttype = opt_type(opt);
      arg = rearg;

      if (opt->prefix) {
        if (strncmp(arg,opt->prefix,opt->prefix_len)) {
          continue;             /* prefix does not match */
        }
        arg += opt->prefix_len;
      }

      if (strncmp(arg,opt->name,opt->name_len)) {
        continue;               /* name does not match */
      }

      arg += opt->name_len;
      if (*arg != 0 && *arg != '=') {
        continue;               /* name does not match (incomplet) */
      }

      if (0 == *arg) {
        if (opttype == opt68_BOL) {
          opt_set_int(opt,opt68_CLI,-!negate);
          /* No arg required, set the option */
          break;
        }
        if (i+1 >= argc) {
          break;                /* $$$ should trigger an error */
        }
        arg = argv[++i];        /* Get next arg */
      } else {
        ++arg;
      }

      if (opttype == opt68_STR) {
        /* string option; ``negate'' does not have much meaning. */
        opt_set_str(opt, opt68_CLI, arg);
      } else {
        opt_set_strtol(opt, opt68_CLI, arg);
        if (negate) opt_set_int(opt, opt68_CLI, ~opt->val.num);
      }
      break;
    }

    if (opt) continue;

    /* Not our option; keep it */
 keep_it:
    argv[n++] = argv[i];

  }

  /* Keep remaining arguments */
  for (; i<argc; ++i) {
    argv[n++] = argv[i];
  }
  argc = n;

  return argc;
}

int option68_init(void)
{
  assert(!opts);
  opts = 0;
  return 0;
}

void option68_shutdown(void)
{
  option68_t * opt, * nxt;

  option68_unset_all();
  for (nxt=opts; (opt=nxt); ) {
    nxt = opt->next;
    opt->next = 0;
  }
  opts = 0;
}

int option68_append(option68_t * options, int n)
{
  int i;

  if (! (options && n > 0) )
    *(int*)0 = 0;
  assert(options && n > 0);

  for (i=0; i<n; ++i) {
    option68_t * const opt = options+i;

    assert(opt->name && *opt->name); /* valid name     */
    assert(!opt->next);              /* not attached   */
    assert(opt->org == opt68_UDF);   /* is not defined */
    /* Either there is a set with at least 1 element or not */
    assert( (opt->set && opt->sets > 0) || (!opt->set && !opt->sets) );
    /* Either no limit (min=max=0) or valid limit (min < max) */
    assert( (opt->min == opt->max && !opt->min) || opt->min < opt->max );

    switch (opt->type) {
    case opt68_STR:
      opt->val.str = empty; break;
    case opt68_BOL:
      assert(opt->min == -1 && !opt->max); break;
    case opt68_ENU:
      assert(opt->set && !opt->min && opt->max == opt->sets-1); break;
    case opt68_INT:
      break;
    default:
      assert(!"invalid option type"); break;
    }

    opt->prefix_len = opt->prefix ? strlen(opt->prefix) : 0;
    opt->name_len   = strlen(opt->name);
    opt->next       = opts;
    opts = opt;

    option68_getenv(opt, opt68_ALWAYS);
  }
  return 0;
}

option68_t * option68_get(const char * key, int set)
{
  option68_t * opt = 0;

  if (key && (opt = opt_of(key)) && !opt_policy(opt,set,opt->org))
    opt = 0;
  return opt;
}

int option68_isset(const option68_t * option)
{
  return option
    ? opt_isset(option)
    : 0
    ;
}

/* Convert option name to envvar name */
static
char * opt2env(char * tmp, const int max,
               const char * prefix, const char * name)
{
  int i = 0,c;
  if (!prefix) prefix = "sc68-";
  for (; i<max && (c=*prefix++); )
    tmp[i++] = (c=='-')
      ? '_'
      : (c>'9') ? c+'A'-'a' : c
      ;
  for (; i<max && (c=*name++); )
    tmp[i++] = (c=='-')
      ? '_'
      : (c>'9') ? c+'A'-'a' : c
      ;
  tmp[i] = 0;
  return tmp;
}

/* Get environment variable */
static
char * mygetenv(const option68_t * opt)
{
#ifndef HAVE_GETENV
  return 0;
#else
  char tmp[64];
  opt2env(tmp,sizeof(tmp)-1,opt->prefix,opt->name);
  return getenv(tmp);
#endif
}

const char * option68_getenv(option68_t * opt, int set)
{
  const char * val = opt ? mygetenv(opt) : 0;

  if (val && opt_policy(opt,set,opt68_ENV)) {
    switch (opt_type(opt)) {
    case opt68_STR: opt_set_str(opt,opt68_ENV,val); break;
    case opt68_BOL:
    case opt68_ENU:
    case opt68_INT: opt_set_strtol(opt,opt68_ENV,val); break;
    }
  }

  return val;
}

void option68_help(void * cookie, option68_help_t fct, int flags)
{
  option68_t * opt;
  char option[64], envvar[64], values[256];
  const int omax = sizeof(option)-1;
  const int emax = sizeof(envvar)-1;
  const int vmax = sizeof(values)-1;
  option[omax] = envvar[emax] = values[vmax] = 0;

  assert(fct);

  FOREACH_OPT(opt) {
    int l, j;

    /* Ignore hidden options unless specified by bit#0 of flags */
    if (opt->hide && !(flags&1))
      continue;

    /* Build envvar name */
    /* snprintf(option, omax,"%s%s", */
    /*          opt->prefix ? opt->prefix : "", opt->name); */
    opt2env(envvar, emax, opt->prefix, opt->name);

    /* Build option name */
    l = snprintf(option,omax,"--%s%s%s",
                 opt_type(opt) == opt68_BOL ? "(no-)" : "",
                 opt->prefix ? opt->prefix : "",
                 opt->name);

    /* Build option values */
    switch (opt_type(opt)) {
    case opt68_BOL:
      *values = 0;
      break;
    case opt68_INT:
      if (!opt->sets) {
        if (opt->min < opt->max)
          snprintf(values,vmax,"[%d..%d]",opt->min,opt->max);
        else
          strncpy(values,"<int>",vmax);
      } else {
        values[0] = '['; values[1] = 0; l = 1;
        for (j=0; j<(int)opt->sets; ++j) {
          const int * set = (const int*)opt->set;
          const char * me
            = opt_isset(opt) && opt->val.num == set[j] ? "*" : "";
          l += snprintf(values+l, vmax-l,"%d%s%c",
                        set[j], me, j+1 == opt->sets ? ']' : '|');
        }
      }
      break;
    case opt68_STR:
      if (!opt->sets) {
        strncpy(values,"<str>",vmax);
        break;
      }
    case opt68_ENU:
      values[0] = '['; values[1] = 0; l = 1;
      for (j=0; j<(int)opt->sets; ++j) {
        const char ** set = (const char **)opt->set;
        const char * me = opt_isset(opt) &&
          !strcmp68(set[j],(opt->type==opt68_STR)
                    ? opt->val.str
                    : set[opt->val.num])
          ? "*" : "";
        l += snprintf(values+l, vmax, "%s%s%c",
                      set[j], me, j+1 == opt->sets ? ']' : '|');
      }
      break;

    default:
      assert(!"invalid option type");
      break;
    }

    fct (cookie, option, envvar, values, opt->desc);
  }
}

option68_t * option68_enum(int idx)
{
  option68_t * opt;

  FOREACH_OPT(opt) {
    if (!idx--)
      return opt;
  }
  return 0;
}
