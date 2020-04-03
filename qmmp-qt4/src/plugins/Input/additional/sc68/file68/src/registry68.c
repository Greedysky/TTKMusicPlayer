/*
 * @file    registry68.c
 * @brief   ms-Windows registry
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

/* TODO: remove the recursive stuff as it is totally useless. */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "file68_reg.h"

#ifdef USE_REGISTRY68

int registry68_support(void) { return 1; }

#include "file68_msg.h"
#include "file68_str.h"
#include <windows.h>
#include <winreg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef HKEY_INVALID
# define HKEY_INVALID ((HKEY)0)
#endif

/* this table *MUST* match enum registry68_key_e definition. */
static struct reg68_keytable_s {
  const char name[4];
  HKEY hkey;
} keys[REGISTRY68_LST+1] = {
  { /* 00 */ {'C','R','K',0}, HKEY_CLASSES_ROOT     },
  { /* 01 */ {'C','U','K',0}, HKEY_CURRENT_USER     },
  { /* 02 */ {'L','M','K',0}, HKEY_LOCAL_MACHINE    },
  { /* 03 */ {'U','S','K',0}, HKEY_USERS            },
  { /* 04 */ {'P','D','K',0}, HKEY_PERFORMANCE_DATA },
  { /* 05 */ {'C','C','K',0}, HKEY_CURRENT_CONFIG   },
  { /* 06 */ {'D','D','K',0}, HKEY_DYN_DATA         },
  { /* xx */ {'I','N','K',0}, HKEY_INVALID          }
};

static const char * keyhdlname(const registry68_key_t key)
{
  int i;

  for (i=0; i < REGISTRY68_LST && (HKEY)key != keys[i].hkey; ++i)
    ;
  return keys[i].name;
}

#if 0
static const char * keyname(const enum registry68_key_e key)
{
  return keys
    [(unsigned int)key >= (unsigned int)REGISTRY68_LST
     ? REGISTRY68_LST
     : key
      ] . name;
}
#endif

static HKEY keyhandle(const enum registry68_key_e key)
{
  return keys
    [(unsigned int)key >= (unsigned int)REGISTRY68_LST
     ? REGISTRY68_LST
     : key
      ] . hkey;
}

registry68_key_t registry68_rootkey(enum registry68_key_e rootkey)
{
  registry68_key_t key = (registry68_key_t) keyhandle(rootkey);
  return key;
}

static char * SetSystemError(char * str, int max, int err)
{
  int  l;

  *str = 0;
  if (err == ERROR_SUCCESS)
    return str;

  /* if (!str) { */
  /*   msg68_critical("registry68: race condition may occur\n"); */
  /*   str = registry68_errorstr; */
  /*   max = sizeof(registry68_errorstr); */
  /* } */

  FormatMessageA(
    /* source and processing options */
    FORMAT_MESSAGE_FROM_SYSTEM,
    /* pointer to message source */
    0,
    /* requested message identifier */
    err,
    /* language identifier for requested message */
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    /* pointer to message buffer */
    str,
    /* maximum size of message buffer */
    max,
    /* pointer to array of message inserts */
    0);
  str[max-1] = 0;
  l = strlen(str);
  while (--l>=0 && (str[l] == '\n' || str[l] == '\r' ||str[l] == ' '))
    str[l] = 0;

  return str;
}

static int GetOneLevelKey(HKEY hkey, char **kname, HKEY *hkeyres, int secu)
{
  char *s;
  LONG hres;

  /* Find next sub-key */
  s=strchr(*kname,'/');
  if (s) {
    s[0] = 0;
  }

  if (secu & KEY_CREATE_SUB_KEY) {
    hres =
      RegCreateKeyExA(hkey,     /* handle to open key                */
                      *kname,   /* address of name of subkey to open */
                      0,        /* reserved                          */
                      0,        /* user-defined class type           */
                      REG_OPTION_NON_VOLATILE, /* Options            */
                      secu,     /* Access right                      */
                      0,        /* security attributs                */
                      hkeyres,  /* address of handle to open key     */
                      0         /* Disposition                       */
        );
  } else {
    /* Open this sub-key */
    hres =
      RegOpenKeyExA(hkey,     /* handle to open key                */
                    *kname,   /* address of name of subkey to open */
                    0,        /* reserved                          */
                    secu,     /* security access mask              */
                    hkeyres   /* address of handle to open key     */
        );
  }


  /* If next sub-key exist, advance pointer to beginning of key-name */
  if (s) {
    s[0] = '/';
    ++s;
  }

  if (hres != ERROR_SUCCESS) {
    *hkeyres = HKEY_INVALID;
  } else {
    *kname = s;
  }
  return hres;
}

static int rGetRegistryKey(HKEY hkey, char *kname, HKEY *hkeyres, int secu)
{
  HKEY subkey = HKEY_INVALID;
  int err;

  if (!kname) {
    *hkeyres = hkey;
    return 0;
  }

  err = GetOneLevelKey(hkey, &kname, &subkey, secu);
  *hkeyres = subkey;
  if (!err && kname) {
    err = rGetRegistryKey(subkey, kname, hkeyres, secu);
    RegCloseKey(subkey);
  }
  return err;
}

registry68_key_t registry68_open(registry68_key_t hkey,
                                 const char *kname_cst, int mode)
{
  HKEY hdl = HKEY_INVALID;
  int  len, secu, hres;
  char kname[1024], error[256];

  if (!kname_cst) {
    msg68_critical("registry68: open '%s::<null> null pointer\n",
                   keyhdlname(hkey));
    goto error;
  }

  if ( (len = strlen(kname_cst)) >= sizeof(kname) ) {
    msg68_critical("registry68: key name too long '%s::%s'\n",
                   keyhdlname(hkey), kname_cst);
    goto error;
  }
  memcpy(kname, kname_cst, len+1);

  secu = 0;
  if (mode & 1)
    secu |= KEY_READ;
  if (mode & 2)
    secu |= KEY_WRITE;

  hres = rGetRegistryKey((HKEY)hkey, kname, (HKEY *)&hdl, secu);
  if (hres != ERROR_SUCCESS) {
    SetSystemError(error,sizeof(error)-1, hres);
    hdl = HKEY_INVALID;
  }

error:
  msg68_trace("registry68: open '%s::%s' => [%s]\n",
              keyhdlname(hdl), kname_cst, strok68(hdl==HKEY_INVALID));

  return (registry68_key_t) hdl;
}


static void prepare_key(registry68_key_t * p_rootkey,
                        const char ** p_kname_cst, int * p_len)
{
  registry68_key_t rootkey = *p_rootkey;
  const char * kname_cst = *p_kname_cst;
  int len = strlen(kname_cst);

  /* Extract rootkey from path. */
  if (rootkey == HKEY_INVALID &&
      len>=4 && (kname_cst[2]=='K'||kname_cst[2]=='k') && kname_cst[3]==':') {
    int i;
    for (i=0; i<REGISTRY68_LST &&
           (keys[i].name[0] != (kname_cst[0] & ~32) ||
            keys[i].name[1] != (kname_cst[1] & ~32)); ++i)
      ;
    if (i < REGISTRY68_LST) {
      rootkey = keys[i].hkey;
      kname_cst += 4;
      len -= 4;
    }
  }

  *p_len = len;
  *p_rootkey = rootkey;
  *p_kname_cst = kname_cst;
}

static int put_data(registry68_key_t rootkey,
                    const char * kname_cst, const char * kdata, int kdatasz,
                    int type)
{
  HKEY hkey = HKEY_INVALID;
  int len;
  LONG hres;
  char * name, kname[1024]/* , error[256] */;
  const char * kname_save = kname_cst;

  if (!kname_cst|| !kdata || kdatasz <= 0) {
    msg68_critical("registry68: puts -- invalid parameters\n");
    goto error_out;
  }
  prepare_key(&rootkey, &kname_cst, &len);
  if (rootkey == HKEY_INVALID) {
    msg68_critical("registry68: puts '%s' -- invalid rootkey\n", kname_save);
    kdata = 0;
    goto error_out;
  }
  if (len >= sizeof(kname)) {
    msg68_critical("registry68: puts '%s' -- key-path too long\n", kname_save);
    kdata = 0;
    goto error_out;
  }
  memcpy(kname, kname_cst, len+1);
  name = strrchr(kname,'/');
  if (name) {
    *name++ = 0;
  }

  hres = rGetRegistryKey((HKEY)rootkey, kname, &hkey, KEY_WRITE);
  if (hres != ERROR_SUCCESS) {
    /* SetSystemError(error, sizeof(error)-1, hres); */
    kdata = 0;
    hres = ERROR_FILE_NOT_FOUND;
  } else {
    hres =
      /* RegSetValue(hkey, */
      /*             name, */
      /*             type, */
      /*             kdata, */
      /*             kdatasz); */

      RegSetValueExA(hkey,
                     name,
                     0,
                     type,
                     (const BYTE *)kdata,
                     kdatasz);

    if (hres != ERROR_SUCCESS) {
      /* SetSystemError(error, sizeof(error)-1, hres); */
      kdata = 0;
    }
    RegCloseKey(hkey);
  }
  if (name) {
    *--name = '/';
  }
error_out:
  return -!kdata;
}


/* Get a string value from register path
 */
static int get_data(registry68_key_t rootkey,
                    const char * kname_cst, char *kdata, int kdatasz,
                    int type)
{
  DWORD vtype;
  HKEY hkey = HKEY_INVALID;
  int len;
  LONG hres;
  char * name, kname[1024]/* , error[256] */;
  const char * kname_save = kname_cst;

  if (!kname_cst|| !kdata || kdatasz <= 0) {
    msg68_critical("registry68: gets -- invalid parameters\n");
    goto error_out;
  }
  kdata[0]=0;
  prepare_key(&rootkey, &kname_cst, &len);
  if (rootkey == HKEY_INVALID) {
    msg68_critical("registry68: gets '%s' -- invalid rootkey\n", kname_save);
    kdata = 0;
    goto error_out;
  }
  if (len >= sizeof(kname)) {
    msg68_critical("registry68: gets '%s' -- key-path too long\n", kname_save);
    kdata = 0;
    goto error_out;
  }
  memcpy(kname, kname_cst, len+1);
  name = strrchr(kname,'/');
  if (name) {
    *name++ = 0;
  }
  hres = rGetRegistryKey((HKEY)rootkey, kname, &hkey, KEY_READ);
  if (hres != ERROR_SUCCESS) {
    /* SetSystemError(error, sizeof(error)-1, hres); */
    kdata = 0;
    hres = ERROR_FILE_NOT_FOUND;
  } else {
    hres =
      RegQueryValueExA(hkey,             /* handle to key to query            */
                       name,             /* address of name of value to query */
                       NULL,             /* reserved                          */
                       &vtype,           /* address of buffer for value type  */
                       (LPBYTE)kdata,    /* address of data buffer            */
                       (LPDWORD)&kdatasz /* address of data buffer size       */
        );
    if (hres != ERROR_SUCCESS) {
      /* SetSystemError(error, sizeof(error)-1, hres); */
      kdata = 0;
    } else if (vtype != type) {
      msg68_error("registry68: gets '%s' -- unexpected data type\n",
                  kname_save);
      kdata = 0;
    }
    RegCloseKey(hkey);
  }
  if (name) {
    *--name = '/';
  }
error_out:
  return -!kdata;
}

int registry68_geti(registry68_key_t rootkey,
                    const char * kname_cst, int * dword)
{
  DWORD data;
  int err =
    get_data(rootkey, kname_cst, (char*)&data, 4, REG_DWORD);
  if (!err && dword)
    *dword = data;  /* assuming registry is stored in native endian */
  return err;
}

/* Get a string value from register path
 */
int registry68_puti(registry68_key_t rootkey,
                    const char * kname_cst, int kdata)
{
  DWORD dword = kdata;
  return put_data(rootkey, kname_cst, (char*)&dword, 4, REG_DWORD);
}


/* Get a string value from register path
 */
int registry68_gets(registry68_key_t rootkey,
                    const char * kname_cst, char *kdata, int kdatasz)
{
  return get_data(rootkey, kname_cst, kdata, kdatasz, REG_SZ);
}

/* Get a string value from register path
 */
int registry68_puts(registry68_key_t rootkey,
                    const char * kname_cst, const char * kdata)
{
  int l = kdata ? strlen(kdata): 0;
  return put_data(rootkey, kname_cst, kdata, l, REG_SZ);
}

#else

int registry68_support(void) { return 0; }

registry68_key_t registry68_rootkey(enum registry68_key_e rootkey)
{
  return (registry68_key_t)0;
}

registry68_key_t registry68_open(registry68_key_t hkey,
                                 const char *kname, int mode)
{
  return (registry68_key_t)0;
}

int registry68_gets(registry68_key_t rootkey, const char * kname_cst,
                    char *kdata, int kdatasz)
{
  return -1;
}

int registry68_geti(registry68_key_t hkey, const char *kname,int * kdata)
{
  return -1;
}

int registry68_puts(registry68_key_t rootkey, const char * kname_cst,
                    const char * kdata)
{
  return -1;
}

int registry68_puti(registry68_key_t rootkey, const char * kname_cst,
                    int kdata)
{
  return -1;
}

#endif /* #ifdef USE_REGISTRY68 */
