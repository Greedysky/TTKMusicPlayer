/**
 * @ingroup  lib_file68
 * @file     sc68/file68_str.h
 * @brief    String manipulation header.
 * @author   Benjamin Gerard
 * @date     2003-08-11
 */

/* Copyright (c) 1998-2015 Benjamin Gerard */

#ifndef FILE68_STR_H
#define FILE68_STR_H

#ifndef FILE68_API
# include "file68_api.h"
#endif

/**
 * @defgroup  lib_file68_str  String manipulation
 * @ingroup   lib_file68
 *
 *   Provides string manipulation functions.
 *
 * @{
 */

FILE68_API
/**
 * Compare two string (case insensitive).
 *
 *   The strcmp68() function compares the two strings a and b,
 *   ignoring the case of the characters. It returns an integer less
 *   than, equal to, or greater than zero if a is found, respectively,
 *   to be less than, to match, or be greater than b.
 *
 * @param  a  First string to compare
 * @param  b  String to compare with
 *
 * @return  Integer result of the two string compare. The difference
 *           between last tested characters of a and b.
 * @retval  0   a and b are equal
 * @retval  <0  a is less than b
 * @retval  >0  a is greater than b
 */
int strcmp68(const char *a, const char *b);

FILE68_API
/**
 * Compare two string (case insensitive, size limited).
 *
 *   The strcmp68() function compares the two strings a and b,
 *   ignoring the case of the characters. It returns an integer less
 *   than, equal to, or greater than zero if a is found, respectively,
 *   to be less than, to match, or be greater than b.
 *
 * @param  a  First string to compare
 * @param  b  String to compare with
 * @param max Strings max length
 *
 * @return  Integer result of the two string compare. The difference
 *          between last tested characters of a and b.
 * @retval  0   a and b are equal
 * @retval  <0  a is less than b
 * @retval  >0  a is greater than b
 */
int strncmp68(const char *a, const char *b, int max);


FILE68_API
/**
 * Concatenate two strings.
 *
 *   The strcat68() function appends the b string to the a string
 *   overwriting the 0 character at the end of dest, and then adds a
 *   terminating 0 character. The strings may not overlap. Destination
 *   string has a maximum size of l characters. On overflow, the
 *   trailing 0 is omitted.
 *
 * @param  a  Destination string.
 * @param  b  String to append.
 * @param  l  Destination maximum size (including trailing 0)
 *
 * @return  Destination string
 * @retval  a
 */
char * strcat68(char * a, const char * b, int l);

FILE68_API
/**
 * Duplicate a string.
 *
 *   The strdup68() function returns a pointer to a new string which
 *   is a duplicate of the string s. Memory for the new string is
 *   obtained with SC68alloc(), and can be freed with SC68free().
 *
 * @param  s        String to duplicate.
 *
 * @return  Duplicated string
 * @return  0 error
 */
char * strdup68(const char * s);

FILE68_API
/**
 * Concat two strings in a duplicate buffer.
 *
 *   The strcatdup68() function returns a pointer to a new string
 *   which is a duplicate of the string a+b. Memory for the new string
 *   is obtained with SC68alloc(), and can be freed with
 *   SC68free(). If either a or b is null the function does not failed
 *   but replace it by an empty string. If both a and b are null the
 *   function returns 0.
 *
 * @param   a       Left string.
 * @param   b       Right string.
 *
 * @return  Duplicated string a+b
 * @return  0 error
 */
char * strcatdup68(const char * a, const char * b);

FILE68_API
/**
 * Make a track and time information string.
 *
 *   The strtime68() function formats a string with track time info.
 *   The general format is "TK MN:SS" where:
 *    - TK is track_num or "--" if track_num < 0 or track_num > 99
 *    - MN:SS is time minutes and seconds or "--:--" if seconds < 0
 *
 * @param  buffer     Destination buffer (0 use default static buffer).
 * @param  track_num  Track number from 00 to 99, minus values disable.
 * @param  seconds    Time to display in seconds [00..5999], other values
 *                     disable.
 *
 * @return  Pointer to result formatted string in a static buffer.
 *
 * @warning  The function returns a static buffer. Do try to modify it.
 * @warning  Not thread safe.
 */
char * strtime68(char * buffer, int track_num, int seconds);

FILE68_API
/**
 * Convert time (in second) to string.
 *
 *   The strlongtime68() function converts a time in seconds to days,
 *   hours, minutes and second string. Day and hour unit are removed
 *   if they are null (not signifiant). The output string looks like :
 *   [D days, ][H h, ] MN' SS"
 *
 * @param  buffer  Destination buffer (0 use default static buffer).
 * @param  time    Time in second to convert to string.
 *
 * @return  pointer to result time string (given buffer or static buffer).
 *
 * @warning  Not thread safe when using static buffer.
 *
 */
char * strlongtime68(char * buffer, int time);

#define strnevernull68 strnevernull68 /**< @see strnevernull68 */
#ifdef strnevernull68
static inline
const char * strnevernull68(const char *s)
{
  return s ? s : "<nul>";
}
#else
FILE68_API
/**
 * Returns a never null string.
 *
 * @param  s  String to test
 *
 * @return  a never null string
 * @retval  s        if s was not null
 * @retval  "<nul>"  if s was null
 */
const char * strnevernull68(const char *s);
#endif

#define strok68(V) strok68(V)           /**< inlined version of strok68()  */
#ifdef strok68
static inline
const char * strok68(const int v)
{
  static const char r[2][8] = { "failure","success" };
  return r[!v];
}
#else
FILE68_API
/**
 * Returns success string.
 *
 * @param  v  Value to test
 *
 * @return  a success string
 * @retval  "success"  if v == 0
 * @retval  "failure"  if v != 0
 */
const char * strok68(const int v);
#endif

/**
 * @}
 */

#endif
