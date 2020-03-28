/******************************************************************************
*                                                                             *
*  Copyright (C) 1992-1995 Tony Robinson                                      *
*                                                                             *
*  See the file doc/LICENSE.shorten for conditions on distribution and usage  *
*                                                                             *
******************************************************************************/

/*
 * $Id: array.c,v 1.7 2003/08/26 05:34:04 jason Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "shorten.h"
#include "shndec.h"

void *pmalloc(uint32_t size, shn_file *this_shn) {
  void *ptr;

  ptr = malloc(size);

  if(ptr == NULL)
    shn_error_fatal(this_shn,"Call to malloc(%ld) failed in pmalloc() -\nyour system may be low on memory", size);

  return(ptr);
}

uint32_t **long2d(uint32_t n0, uint32_t n1, shn_file *this_shn) {
  uint32_t **array0 = NULL;

  if((array0 = (int32_t**) pmalloc((uint32_t) (n0 * sizeof(int32_t*) +
                     n0 * n1 * sizeof(int32_t)),this_shn)) != NULL ) {
    uint32_t *array1 = (int32_t*) (array0 + n0);
    int i;

    for(i = 0; i < n0; i++)
      array0[i] = array1 + i * n1;
  }
  return(array0);
}
