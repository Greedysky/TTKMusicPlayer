/*
 * @file    uri68.c
 * @brief   uri parser and dispatcher
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
#include "file68_uri.h"
#include "file68_rsc.h"
#include "file68_str.h"
#include "file68_msg.h"
#include "file68_vfs_null.h"
#include "file68_vfs_file.h"
#include "file68_vfs_fd.h"
#include "file68_vfs_curl.h"
#include "file68_vfs_ao.h"

#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MYHD "uri68  : "

static scheme68_t * schemes;
static int uri_cat = msg68_DEFAULT;


#define GEN_DELIM ":/?#[]@"
#define SUB_DELIM "!$&'()*+,;="


/* Related documentation:
 *
 * RFC 1630 - Universal Resource Identifiers in WWW
 * RFC 3986 - Uniform Resource Identifier (URI): Generic Syntax
 */

/* URI chars:

   reserved    = gen-delims sub-delims
   gen-delims  = : / ? # [ ] @
   sub-delims  = ! $ & ' ( ) * + , ; =
   unreserved  = alpha-num - _ . ~


   Syntax components (3936/section#3)

   URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

   hier-part = "//" authority path-abempty
             / path-absolute
             / path-rootless
             / path-empty

   scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
   Case: insensitive
   Canonical: lowercase

   authority   = [ userinfo "@" ] host [ ":" port ]
   userinfo    = *( unreserved / pct-encoded / sub-delims / ":" )
   host        = IP-literal / IPv4address / reg-name
   query       = *( pchar / "/" / "?" )
   fragment    = *( pchar / "/" / "?" )

     Preceded by "//".
     Terminated with "/" or "?" or "#" or <EOS>.

   The percent sign

      The percent sign ("%", ASCII 25 hex) is used as the escape
      character in the encoding scheme and is never allowed for
      anything else.

      Percent-encoding

      pct-encoded = "%" HEXDIG HEXDIG
      Case: insensitive
      Canonical: uppercase. Never pct-enc ALPHA,DIGIT,'-','.','_' or '~'



   Hierarchical forms

      The slash ("/", ASCII 2F hex) character reserved for the
      delimiting of substrings whose relationship is hierarchical.

      The significance of the slash between two segments is that the
      segment of the path to the left is more significant than the
      segment of the path to the right.  ("Significance" in this case
      refers solely to closeness to the root of the hierarchical
      structure and makes no value judgement!)

   Hash for fragment identifiers

      The hash ("#", ASCII 23 hex) character is reserved as a
      delimiter to separate the URI of an object from a fragment
      identifier .

   Query strings

      The question mark ("?", ASCII 3F hex) is used to delimit the
      boundary between the URI of a queryable object, and a set of
      words used to express a query on that object.

      Within the query string, the plus sign is reserved as shorthand
      notation for a space.

   Other reserved characters

      The astersik ("*", ASCII 2A hex) and exclamation mark ("!" , ASCII
      21 hex) are reserved for use as having special signifiance within
      specific schemes.

*/

static const char hex[16] =
{ '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

static void check_ascii() {
  /* stupid run-time ascii checks. */
  assert( '%' == 0x25 );
  assert( '/' == 0x2f );
  assert( '#' == 0x23 );
  assert( '?' == 0x3f );
  assert( '*' == 0x2a );
  assert( '!' == 0x21 );
  assert( '0' == 0x30 );
  assert( 'A' == 0x41 );
  assert( 'F' == 0x46 );
  assert( 'a' == 0x61 );
  assert( 'f' == 0x66 );
  assert( '_' == 0x5f );
  assert( '-' == 0x2d );
  assert( '.' == 0x2e );
  assert( '~' == 0x7e );
}

/**
 * @retval [0-16] or -1
 */
static int xdigit(int c) {
  if (c >= '0' && c <= '9')
    c -= '9';
  else if (c >= 'A' && c <= 'F')
    c -= 'A'-10;
  else if (c >= 'a' && c <= 'f')
    c -= 'a'-10;
  else c = -1;
  return c;
}

static int is_uri_reserved(int c)
{
  return strchr("!*'();:@&=+$,/?#[]",c) != 0;
}

static int is_uri_unreserved(int c)
{
  return isalnum(c) || c=='-' || c =='_' || c=='.' || c=='~';
}

static int is_uri_pctenc(int c, const int flags)
{
  return !is_uri_unreserved(c);
}

/**
 * Encode a string with percent-encoding method
 * @return the output string size (in bytes)
 * @retval max on buffer overflow.
 * @retval -1 on error
 */
static int pct_encode(char * dst, const int max,
                      const char * src, const int len,
                      const int flags)
{
  int i,j;

  for (i=j=0; i<len && j<max; ++i) {
    const int c = 255 & src[i];
    if (!c) break;
    if (is_uri_pctenc(c,flags)) {
      dst[j++] = '%';
      if (j >= max) break;
      dst[j++] = hex[c>>4];
      if (j >= max) break;
      dst[j++] = hex[c&15];
    } else
      dst[j++] = c;
  }
  if (j < max)
    dst[j] = 0;
  return j;
}


/**
 * Decode a percent-encoded string.
 * @return the output string size (in bytes)
 * @retval max on buffer overflow.
 * @retval -1 on error
 */
static int pct_decode(char * dst, const int max,
                      const char * src, const int len,
                      const int flags)
{
  int i,j,e;

  for (i=j=e=0; i<len && j<max; ++i) {
    int c = 255 & src[i];
    int h, l;
    if (!c) break;
    if (c == '%' && i+2 < len
        && (h = xdigit(src[i+1])) >= 0
        && (l = xdigit(src[i+2])) >= 0) {
        c = (h << 4) + l;
        i += 2;
    }
    dst[j++] = c;
  }
  if (j < max)
    dst[j] = 0;
  return j;
}


/**
 * @return length of the scheme (including trailing ':')
 * @retval  0  No scheme
 * @retval >0  length of scheme string
 */
static
int parse_scheme(const char * uri)
{
  int i = 0;

  /* First char must be alpha */
  if ( ! isalpha((int)uri[i]))
    return 0;

  /* Others must be alpha, digit, dot `.', plus `+' or hyphen `-' */
  for (i=1;
       isalnum((int)uri[i]) || uri[i] == '+' || uri[i] == '.' || uri[i] == '-';
       ++i);

  /* Must end by a colon `:' */
  return (uri[i] == ':') ? i+1 : 0;
}

int uri68_get_scheme(char * scheme, int max, const char *uri)
{
  int len = -1;

  if (uri) {
    len = parse_scheme(uri);
    if (scheme) {
      if (len == 0 )
        scheme[0] = 0;
      else if (len > 0) {
        if (len >= max)
          return -1;
        memcpy(scheme, uri, len);
        scheme[len] = 0;
      }
    }
  }
  return len;
}

void uri68_unregister(scheme68_t * scheme)
{
  if (scheme) {
    TRACE68(uri_cat, MYHD "unregister scheme -- %s\n", scheme->name);
    if (scheme == schemes)
      schemes = scheme->next;
    else if (schemes) {
      scheme68_t * sch;
      for (sch = schemes; sch->next; sch = sch->next)
        if (sch->next == scheme) {
          sch->next = scheme->next;
          break;
        }
    }
    scheme->next = 0;
  }
}

int uri68_register(scheme68_t * scheme)
{
  if (!scheme)
    return -1;

  assert(!scheme->next);
  scheme->next = schemes;
  schemes = scheme;
  TRACE68(uri_cat, MYHD "registered scheme -- %s\n", scheme->name);

  return 0;
}

vfs68_t * uri68_vfs_va(const char * uri, int mode, int argc, va_list list)
{
  vfs68_t * vfs = 0;
  scheme68_t * scheme;

  for (scheme = schemes; scheme; scheme = scheme->next) {
    int res = scheme->ismine(uri);
    if (!res)
      continue;
    if ( (mode & res & 3) == ( mode & 3 ) )
      break;
  }

  if (scheme)
    vfs = scheme->create(uri, mode, argc, list);

  TRACE68(uri_cat, MYHD
          "create url='%s' %c%c => [%s,'%s']\n",
          strnevernull68(uri),
          (mode&1) ? 'R' : '.',
          (mode&2) ? 'W' : '.',
          strok68(!vfs),
          vfs68_filename(vfs));

  return vfs;
}

vfs68_t * uri68_vfs(const char * uri, int mode, int argc, ...)
{
  vfs68_t * vfs;
  va_list list;

  va_start(list, argc);
  vfs = uri68_vfs_va(uri, mode, argc, list);
  va_end(list);

  return vfs;
}
