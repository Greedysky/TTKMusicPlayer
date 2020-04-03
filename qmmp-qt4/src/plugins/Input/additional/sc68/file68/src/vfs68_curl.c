/*
 * @file    istream68_curl.c
 * @brief   implements istream68 VFS for cURL
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
#include "file68_vfs_curl.h"
#include "file68_msg.h"

/* Define this if you want CURL support. */
#ifdef USE_CURL

#ifndef DEBUG_CURL_O
# define DEBUG_CURL_O 0
#endif
static int curl_cat = msg68_DEFAULT;

static const char curl_schemes[] = CURL_SCHEMES;

#include "file68_vfs_def.h"
#include "file68_uri.h"
#include "file68_str.h"

#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#if defined (HAVE_UNISTD_H) && (defined (HAVE_USLEEP) || defined (HAVE_SLEEP))
/* Need this to have usleep prototype. */
# if defined (HAVE_USLEEP) && !defined(__USE_BSD) && !defined(__USE_XOPEN)
#  define __USE_BSD
# endif
# include <unistd.h>
#endif

#ifndef ISF_VERBOSE
# ifdef _DEBUG
#  define ISF_VERBOSE 1
# else
#  define ISF_VERBOSE 0
# endif
#endif

/* Timeout in ms */
#define IS68_TIMEOUT        10000

#define IS68_CACHE_FIX      8
#define IS68_CACHE_SZ       (1<<IS68_CACHE_FIX)
#define IS68_CACHE_BLKS_FIX 7
#define IS68_CACHE_BLKS     (1<<IS68_CACHE_BLKS_FIX)

typedef struct {
  int top;
  int bytes;
  char buffer[IS68_CACHE_SZ];
} vfs68_cache_t;

/** vfs curl structure. */
typedef struct {
  vfs68_t vfs;                   /**< vfs function.                 */

  CURLM * curm;              /**< CURL "multi" handle               */
  CURL  * curl;              /**< CURL "easy" handle.               */

  CURLcode code;             /**< CURL last "easy" operation code.  */
  CURLMcode mcode;           /**< CURL last "multi" operation code. */

  /* struct { fd_set r,w,e; } sets; */

  unsigned int mode         : 2;  /**< Open modes.                  */
  unsigned int has_hd_read  : 1;  /**< 1: read header has started.  */
  unsigned int has_hd_write : 1;  /**< 1: write header has started. */
  unsigned int has_read     : 1;  /**< 1: read data has started.    */
  unsigned int has_write    : 1;  /**< 1: write data has started.   */

  int pos;                        /**< Current R/W position.        */
  int curl_pos;                   /**< CURL current R/W position.   */
  int length;                     /**< Known length (-1:unknown).   */

  /** Cached data. */
  vfs68_cache_t cache[IS68_CACHE_BLKS];

  /** Curl error message. */
  char error[CURL_ERROR_SIZE];

  /* MUST BE at the end of the structure because supplemental bytes will
   * be allocated to store curlname.
   */
  char name[1];                   /**< url.                         */

} vfs68_curl_t;

static int init = 0;

static int curl_ismine(const char *);
static vfs68_t * curl_create(const char *, int, int, va_list);
static scheme68_t curl_scheme = {
  0, "vfs-curl", curl_ismine, curl_create
};

static int curl_ismine(const char * uri)
{
  const int ok = SCHEME68_ISMINE | SCHEME68_READ;
  const char * dot = strchr(uri,':');

  if (dot) {
    const int len = dot - uri;
    if (len < 16 && uri[len+1] == '/' && uri[len+2] == '/') {
      int l;
      const char * c;
      for (c = curl_schemes; l = strlen(c), l; c += l+1)
        if (l == len &&  !strncmp68(uri,c,l))
          return ok;
    }
  }
  return 0;
}


static int mysleep(unsigned long ms)
{
#ifdef HAVE_USLEEP
  unsigned long us = ms * 1000u;
  usleep(us);
  return (int) ms;
#elif defined(_MSC_VER) || defined(WIN32) || defined(_WIN32)
  Sleep(ms);
  return (int) ms;
#elif defined(HAVE_SLEEP)
  unsigned int sec = (ms + 999u) / 1000u;
  sleep(sec);
  return (int)(sec * 1000u);
#else
  /* $$$ Ooops very bad thing */
#error "Undefined sleep function"
  return ms;
#endif
}

static void isf_reset_cache_blocks(vfs68_curl_t *isf)
{
  int i;

  for (i=0; i<IS68_CACHE_BLKS; ++i) {
    vfs68_cache_t * c = isf->cache+i;
    c->top = -1;
    c->bytes = 0;
  }
}

static const char * isf_name(vfs68_t * vfs)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;

  return (!isf->name[0])
    ? 0
    : isf->name
    ;
}

/* $$$ TODO */
static size_t isf_read_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)stream;
  int bytes, org_bytes/* , pos */;

  bytes = org_bytes = (int)(size * nmemb);
  /* pos = isf->curl_pos; */

  isf->has_read = 1;
  if (!bytes) {
    return 0; /* Avoid 0 divide at ending return */
  }

  return (org_bytes - bytes) / size;
}

static size_t isf_write_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)stream;

  int bytes, org_bytes, pos;
  int off, top, blk;

  bytes = org_bytes = (int)(size * nmemb);
  pos = isf->curl_pos;

  isf->has_write = 1;
  if (!bytes) {
    return 0; /* Avoid 0 divide at ending return */
  }

  off = pos & (IS68_CACHE_SZ-1);
  top = pos - off;
  blk = (pos>>IS68_CACHE_FIX) & (IS68_CACHE_BLKS-1);

  while (bytes > 0) {
    int n;

    if (!off) {
      /* Starting a new cache block */
      isf->cache[blk].top = top;
    } else if (isf->cache[blk].top != top) {
      /* Oops, got a problem here */
      break;
    }

    n = IS68_CACHE_SZ - off;
    if (n > bytes) {
      n = bytes;
    }

    memcpy(isf->cache[blk].buffer+off, ptr, n);
    ptr = (char *)ptr + n;
    isf->cache[blk].bytes = n+off;
    bytes -= n;
    off = 0;
    blk = (blk+1) & (IS68_CACHE_BLKS-1);
    top += IS68_CACHE_SZ;
  }
  org_bytes -= bytes;
  isf->curl_pos = pos + org_bytes;

  return org_bytes / size;
}

#if 0
static int isf_passwd_cb(void *clientp,
                         const char *prompt, char *buffer, int buflen)
{
  /* Produce a CURLE_BAD_PASSWORD_ENTERED error. */
  return -1;
}
#endif

static int isf_prog_cb(void *clientp,
                       double dltotal,double dlnow,
                       double ultotal,double ulnow)
{
  return 0;
}

static int isf_debug_cb(CURL *handle, curl_infotype type,
                        char *data, size_t size,
                        void *userp)
{
#if 0

  vfs68_curl_t * isf = (vfs68_curl_t *)userp;
  const char * typestr;

  switch (type) {
  case CURLINFO_TEXT:
    typestr = "TEXT";
    break;
  case CURLINFO_HEADER_IN:
    typestr = "HEADER_IN";
    break;
  case CURLINFO_HEADER_OUT:
    typestr = "HEADER_OUT";
    break;
  case CURLINFO_DATA_IN:
    typestr = "DATA_IN";
    data = 0;
    break;
  case CURLINFO_DATA_OUT:
    typestr = "DATA_OUT";
    data = 0;
    break;
  case CURLINFO_END:
    typestr = "DATA_END";
    data = 0;
    break;
  default:
    typestr = "UNKNOWN";
    data = 0;
    break;
  }

  TRACE68(curl_cat,"%s:%p:%p >%s\n", isf->name, isf->curm, handle, typestr);
  if (data) {
    int i;
    char tmp[256];
    int max = (size > sizeof(tmp)-1) ? sizeof(tmp)-1 : size;
    for (i=0; i<max; ++i) {
      int c = data[i] & 255;
      if (c==9) c = ' ';
      if (c < 32 || c > 127) break;
      tmp[i] = c;
    }
    tmp[i] = 0;
    if (i>0) {
      TRACE68(curl_cat,"[%s]\n", tmp);
    }
  }
#endif

  return 0;
}

static int match_header(int *v,
                        char * ptr, const int bytes,
                        const char *header, const int hdsize)
{
  if (bytes > hdsize+1 &&
      !memcmp(ptr,header,hdsize-1)) {
    int i, c;

    /* Find first digit */
    for (i=hdsize-1, c=0; i<bytes && (c=ptr[i], (c<'0' || c>'9')); ++i)
      ;
    /* Convert base 10 number */
    if (c>='0' || c<='9') {
      int len = c - '0';
      for (++i; i<bytes && (c=ptr[i], (c>='0' && c<='9')); ++i) {
        len = len * 10 + c - '0';
      }
      *v = len;
/*       TRACE68(curl_cat,"Match header %s-> %d\n", header, len); */
      return 1;
    }
  }
  return 0;
}

static size_t isf_header_cb(void  *ptr, size_t  size,
                            size_t  nmemb,  void  *stream)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)stream;

  static const char ftp_size[] = "213";
/*   static const char ftp_complete[] = "226"; */

  static const char content_length[] = "Content-Length:";
/*   static const char content_type[] = "Content-Type:"; */

  int len;
  int bytes = nmemb*size;

  isf->has_hd_write = 1;

/*   TRACE68(curl_cat,"curl68::header_cb(%s:%p, %d)\n", */
/*           isf->name, isf->curm, size*nmemb); */

  if (match_header(&len, ptr, bytes, content_length, sizeof(content_length))
      || match_header(&len, ptr, bytes, ftp_size, sizeof(ftp_size))) {
    isf->length = len;
/*     TRACE68(curl_cat,"%s::Set length to %d\n", isf->name, len); */
  }

  /* $$$ TODO : Add transfert complete checks */

  return nmemb;
}

static int safe_curl_cleanup(vfs68_curl_t * isf)
{
  int err = 1;
  if (isf) {
    err = 0;
    if (isf->curm) {
      if (isf->curl) {
        err |= ( CURLM_OK != curl_multi_remove_handle(isf->curm, isf->curl) );
        curl_easy_cleanup(isf->curl);
        isf->curl = 0;
      }
      err |= ( CURLM_OK != curl_multi_cleanup(isf->curm) );
      isf->curm = 0;
    } else if (isf->curl) {
      curl_easy_cleanup(isf->curl);
      isf->curl = 0;
    }
  }
  return -err;
}

static int isf_open(vfs68_t * vfs)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;
  const int verbose = ISF_VERBOSE;
  CURLcode code = 0;
  CURLMcode mcode = 0;
  int err;

  if (!isf->name[0] || isf->curm || isf->curl) {
    return -1;
  }

/*   SC68os_pdebug("curl68::open(%s,%c%c)\n",isf->name, */
/*              (isf->mode & VFS68_OPEN_READ)?'R':'r', */
/*              (isf->mode & VFS68_OPEN_WRITE)?'W':'w'); */

  /* Reset info. */
  isf->has_read = isf->has_write = isf->has_hd_read = isf->has_hd_write = 0;
  isf->curl_pos = isf->pos = 0;
  isf->length   = -1;
  isf->error[0] = 0;
  isf->code     = 0;
  isf->mcode    = 0;

  /*
    FD_ZERO(&isf->sets.r);
    FD_ZERO(&isf->sets.w);
    FD_ZERO(&isf->sets.e);
  */

  /* Currently accapts only read only open mode. */
  if (isf->mode != VFS68_OPEN_READ) {
    strcpy(isf->error, "Invalid open mode");
    return -1;
  }

  /* Create curl handle */
  isf->curl = curl_easy_init();
  isf->curm = curl_multi_init();
/*   TRACE68(curl_cat,"%s:%p:%p:open()\n",isf->name, isf->curm, isf->curl); */
  if (!isf->curl || !isf->curm) {
    goto error;
  }

  /* URL, most important thing to setup. */
  err = 0;
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_URL,isf->name)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_ERRORBUFFER,isf->error)));
  /* Set curl internal buffer (just an hint) */
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_BUFFERSIZE,1024)));

  /* Debug options. */
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_VERBOSE,verbose)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_DEBUGFUNCTION,isf_debug_cb)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_DEBUGDATA,isf)));
  /* Our read/write/progress callback. */
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_WRITEFUNCTION,isf_write_cb)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_WRITEDATA,isf)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_READFUNCTION,isf_read_cb)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_READDATA,isf)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_PROGRESSFUNCTION,isf_prog_cb)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_PROGRESSDATA,isf)));

  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_HEADERFUNCTION,isf_header_cb)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_HEADERDATA,isf)));

  /* Password callback (currently raises an error) */
  /* $$$ OBSOLETE */
  /*
    err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_PASSWDFUNCTION,isf_passwd_cb)));
    err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_PASSWDDATA,isf)));
  */
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_NETRC,CURL_NETRC_OPTIONAL)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_FOLLOWLOCATION,1)));
/* MAC-OSX (7.10.2) version does not have it ?? */
#ifdef CURLOPT_UNRESTRICTED_AUTH
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_UNRESTRICTED_AUTH,0)));
#endif
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_TRANSFERTEXT,0)));

  // $$$ FIXME
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_INFILESIZE,0)));
  err = err || (CURLE_OK != (code=curl_easy_setopt(isf->curl,CURLOPT_UPLOAD,0)));

  if (err) {
    goto error;
  }

  isf_reset_cache_blocks(isf);
  mcode = curl_multi_add_handle(isf->curm, isf->curl);
  if (mcode != CURLM_OK && mcode != CURLM_CALL_MULTI_PERFORM) {
    goto error;
  } else {
    const int re_timeout = IS68_TIMEOUT;
    int timeout = re_timeout;

    do {
      int n;
      mcode = curl_multi_perform(isf->curm, &n);
/*       TRACE68(curl_cat,"READ_HEADER: MCODE=[%s]\n", */
/*               mcode == CURLM_OK ? "OK" : */
/*               (mcode == CURLM_CALL_MULTI_PERFORM) ? "MULTI" : "OTHER"); */

      if (mcode == CURLM_OK) {
/*         TRACE68(curl_cat,"OK, timeleft:%d\n",timeout); */
        timeout -= mysleep(200);
        if (timeout < 0) {
          TRACE68(curl_cat,"vfs_curl: read-header -- *%s*\n","TIME-OUT");
          break;
        }
      } else if (mcode != CURLM_CALL_MULTI_PERFORM) {
        goto error;
      } else {
        timeout = re_timeout;
      }
    } while (!(isf->has_read | isf->has_write));
    if ( ! (isf->has_hd_write | isf->has_read | isf->has_write) ) {
      goto error;
    }
  }
  isf->code = code;
  isf->mcode = mcode;
  return 0;

error:
  isf->code  = code;
  isf->mcode = mcode;
  safe_curl_cleanup(isf);

  return -1;
}

static int isf_close(vfs68_t * vfs)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;
  return safe_curl_cleanup(isf);
}

static int isf_need_more(vfs68_curl_t * isf)
{
  int n=0, pos;

  pos = isf->curl_pos;
  isf->mcode = curl_multi_perform(isf->curm, &n);
  n = isf->curl_pos - pos;
  if (n) {
/*     TRACE68(curl_cat,"%s:%p:GETS MORE\n" */
/*             "-> %d (code=%d)\n", */
/*             isf->name, isf->curm, */
/*             n, isf->mcode); */
  } else {
    switch (isf->mcode) {
    case CURLM_OK:
      break;
    case CURLM_CALL_MULTI_PERFORM:
    default:
/*       TRACE68(curl_cat,"%s:%p:GETS MORE DETECTS EOF\n" */
/*               "-> pos:%d length:%d (code=%d)\n", */
/*               isf->name, isf->curm, */
/*               isf->curl_pos, isf->length, isf->mcode); */
      n = -1;
      break;
    }
  }
  return n;
}


static int isf_read(vfs68_t * vfs, void * data, int n)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;
  int pos, off, top, blk, bytes;
  const int re_timeout = IS68_TIMEOUT;
  int timeout = re_timeout;

/*   SC68os_pdebug("%s:%p::read: %d bytes -> %p\n", */
/*              isf->name, isf->curm, n, data); */

  if (!isf->curl) {
    return -1;
  } else if (!n) {
    return 0;
  }

  pos = isf->pos;
  bytes = n;

  while (bytes > 0) {
    char * src = 0;
    int n = -1;

    if (isf->length != -1 && pos >= isf->length) {
/*       TRACE68(curl_cat,"%s:%p, EOF:\n" */
/*               "->pos:%p/%p\n", */
/*               isf->name, isf->curm, */
/*               pos, isf->length); */
      break;
    } else if (timeout < 0) {
/*       TRACE68(curl_cat,"%s:%p, TIME-OUT :\n" */
/*             "->pos:%d/%d (%d ms)\n", */
/*             isf->name, isf->curm, */
/*             pos, isf->length, re_timeout); */
      break;
    }

    off = pos & (IS68_CACHE_SZ-1);
    top = pos - off;
    blk = (pos>>IS68_CACHE_FIX) & (IS68_CACHE_BLKS-1);

    if (top == isf->cache[blk].top) {

/*       TRACE68(curl_cat,"%s:%p, read block:\n" */
/*               "->pos:%x top:%x blk:%x off:%x bytes:%d cache:[%x:%d]\n", */
/*             isf->name, isf->curm, */
/*             pos, top, blk, off, bytes, */
/*             isf->cache[blk].top, isf->cache[blk].bytes); */

      src = isf->cache[blk].buffer + off;
      n = isf->cache[blk].bytes - off;
      if (n > bytes) {
        n = bytes;
      }
    } else {
/*       TRACE68(curl_cat,"NOT MY TOP\n"); */
    }

    if (n <= 0) {
      if (pos < isf->curl_pos) {
        TRACE68(curl_cat,"%s:%p: Can not seek backward from %d to %d\n",
                isf->name, isf->curm, isf->curl_pos, pos);
        break;
      }

      n=isf_need_more(isf);
      if (!n) {
        /* $$$ FIXME : should use fd_set here ! */
        timeout -= mysleep(100);
      } else if (n == -1) {
        break;
      } else {
        timeout = re_timeout;
      }
    } else {
      memcpy(data, src, n);
      data = (char *)data + n;
      pos += n;
      bytes -= n;
    }
  }
  isf->pos = pos;

  if (isf->mcode != CURLM_OK && isf->mcode != CURLM_CALL_MULTI_PERFORM) {
    return -1;
  }
/*   TRACE68(curl_cat,"%s:%p:read -> %d\n", isf->name, isf->curm, n-bytes); */
  return n - bytes;
}

/* $$$ TODO */
static int isf_write(vfs68_t * vfs, const void * data, int n)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;

  return (!isf->curl)
    ? -1
    : 0
    ;
}

/* $$$ TODO */
static int isf_flush(vfs68_t * vfs)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;
  return (!isf->curl)
    ? -1
    : 0
    ;
}

/* We could have store the length value at opening, but this way it handles
 * dynamic changes of curl size.
 */
static int isf_length(vfs68_t * vfs)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;

  return (!isf->curl)
    ? -1
    : isf->length
    ;
}

static int isf_tell(vfs68_t * vfs)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;
  return (!isf->curl)
    ? -1
    : isf->pos
    ;
}

static int isf_seek(vfs68_t * vfs, int offset)
{
  vfs68_curl_t * isf = (vfs68_curl_t *)vfs;
  int pos;

  if (!isf->curl) {
    return -1;
  }

  pos = isf->pos + offset;
  if (pos < 0 || (isf->length != -1 && pos > isf->length)) {
    return -1;
  }
  isf->pos = pos;
  return 0;
}

static void isf_destroy(vfs68_t * vfs)
{
  free(vfs);
}

static const vfs68_t vfs68_curl = {
  isf_name,
  isf_open, isf_close,
  isf_read, isf_write, isf_flush,
  isf_length, isf_tell, isf_seek, isf_seek,
  isf_destroy
};

int vfs68_curl_init(void)
{
  if (curl_cat == msg68_DEFAULT) {
    curl_cat = msg68_cat("curl","cURL VFS",DEBUG_CURL_O);
    curl_cat = curl_cat != -1 ? curl_cat : msg68_DEFAULT;
  }
  if (!init) {
    init = ( curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK ) ? 1 : -1;
    if (init == 1)
      uri68_register(&curl_scheme);
  }
  return -(init != 1);
}

void vfs68_curl_shutdown(void)
{
  if (init == 1) {
    uri68_unregister(&curl_scheme);
    curl_global_cleanup();
    init = 0;
  }
  if (curl_cat != msg68_DEFAULT) {
    msg68_cat_free(curl_cat);
    curl_cat = msg68_DEFAULT;
  }
}

static vfs68_t * curl_create(const char *uri, int mode,
                             int argc, va_list list)
{
  vfs68_curl_t *isf;
  int len;

  if (vfs68_curl_init()) {
    return 0;
  }

  if (!uri || !uri[0]) {
    return 0;
  }

  /* Don't need +1 because 1 byte already allocated in the
   * vfs68_curl_t::uri.
   */
  len = strlen(uri);
  isf = calloc(sizeof(vfs68_curl_t) + len, 1);
  if (!isf) {
    return 0;
  }

  /* Copy vfs functions. */
  memcpy(&isf->vfs, &vfs68_curl, sizeof(vfs68_curl));

  /* Clean curl handle. */
  isf->mode = mode & (VFS68_OPEN_READ|VFS68_OPEN_WRITE);

  strcpy(isf->name, uri);
  return &isf->vfs;
}


#else /* #ifdef USE_CURL */

/* vfs curl must not be include in this package. Anyway the creation
 * still exist but it always returns error.
 */

#include "file68_vfs_curl.h"
#include "file68_vfs_def.h"

int vfs68_curl_init(void) { return 0; }
void vfs68_curl_shutdown(void) {}

#endif
