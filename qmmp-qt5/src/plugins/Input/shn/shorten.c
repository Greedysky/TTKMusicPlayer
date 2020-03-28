/******************************************************************************
*                                                                             *
*  Copyright (C) 1992-1995 Tony Robinson                                      *
*                                                                             *
*  See the file doc/LICENSE.shorten for conditions on distribution and usage  *
*                                                                             *
******************************************************************************/

/*
 * $Id: shorten.c,v 1.7 2003/08/26 05:34:04 jason Exp $
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include "shorten.h"

void init_offset(int32_t **offset,int nchan,int nblock,int ftype)
{
  int32_t mean = 0;
  int  chan, i;

  /* initialise offset */
  switch(ftype)
  {
  	case TYPE_AU1:
  	case TYPE_S8:
  	case TYPE_S16HL:
  	case TYPE_S16LH:
  	case TYPE_ULAW:
  	case TYPE_AU2:
  	case TYPE_AU3:
  	case TYPE_ALAW:
    	mean = 0;
    	break;
  	case TYPE_U8:
    	mean = 0x80;
    	break;
  	case TYPE_U16HL:
  	case TYPE_U16LH:
    	mean = 0x8000;
    	break;
  	default:
        shn_debug("Unknown file type: %d", ftype);
  }

  for(chan = 0; chan < nchan; chan++)
    for(i = 0; i < nblock; i++)
      offset[chan][i] = mean;
}
