/*  wave.c - functions to parse and verify WAVE headers
 *  Copyright (C) 2000-2007  Jason Jordan <shnutils@freeshell.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * $Id: wave.c,v 1.13 2007/03/23 05:49:48 jason Exp $
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "shndec.h"

char *shn_format_to_str(uint16_t format)
{
  switch (format) {
    case WAVE_FORMAT_UNKNOWN:
      return "Microsoft Official Unknown";
    case WAVE_FORMAT_PCM:
      return "Microsoft PCM";
    case WAVE_FORMAT_ADPCM:
      return "Microsoft ADPCM";
    case WAVE_FORMAT_IEEE_FLOAT:
      return "IEEE Float";
    case WAVE_FORMAT_ALAW:
      return "Microsoft A-law";
    case WAVE_FORMAT_MULAW:
      return "Microsoft U-law";
    case WAVE_FORMAT_OKI_ADPCM:
      return "OKI ADPCM format";
    case WAVE_FORMAT_IMA_ADPCM:
      return "IMA ADPCM";
    case WAVE_FORMAT_DIGISTD:
      return "Digistd format";
    case WAVE_FORMAT_DIGIFIX:
      return "Digifix format";
    case WAVE_FORMAT_DOLBY_AC2:
      return "Dolby AC2";
    case WAVE_FORMAT_GSM610:
      return "GSM 6.10";
    case WAVE_FORMAT_ROCKWELL_ADPCM:
      return "Rockwell ADPCM";
    case WAVE_FORMAT_ROCKWELL_DIGITALK:
      return "Rockwell DIGITALK";
    case WAVE_FORMAT_G721_ADPCM:
      return "G.721 ADPCM";
    case WAVE_FORMAT_G728_CELP:
      return "G.728 CELP";
    case WAVE_FORMAT_MPEG:
      return "MPEG";
    case WAVE_FORMAT_MPEGLAYER3:
      return "MPEG Layer 3";
    case WAVE_FORMAT_G726_ADPCM:
      return "G.726 ADPCM";
    case WAVE_FORMAT_G722_ADPCM:
      return "G.722 ADPCM";
  }
  return "Unknown";
}

int is_valid_file(shn_file *info)
/* determines whether the given filename (info->filename) is a regular file, and is readable */
{
  struct stat sz;
  FILE *f;

  if (0 != stat(info->wave_header.filename,&sz)) {
    if (errno == ENOENT)
      shn_error("cannot open '%s' because it does not exist",info->wave_header.filename);
    else if (errno == EACCES)
      shn_error("cannot open '%s' due to insufficient permissions",info->wave_header.filename);
    else if (errno == EFAULT)
      shn_error("cannot open '%s' due to bad address",info->wave_header.filename);
    else if (errno == ENOMEM)
      shn_error("cannot open '%s' because the kernel ran out of memory",info->wave_header.filename);
    else if (errno == ENAMETOOLONG)
      shn_error("cannot open '%s' because the file name is too long",info->wave_header.filename);
    else
      shn_error("cannot open '%s' due to an unknown problem",info->wave_header.filename);
    return 0;
  }
  if (0 == S_ISREG(sz.st_mode)) {
    if (S_ISDIR(sz.st_mode))
      shn_error("'%s' is a directory, not a regular file",info->wave_header.filename);
    else if (S_ISCHR(sz.st_mode))
      shn_error("'%s' is a character device, not a regular file",info->wave_header.filename);
    else if (S_ISBLK(sz.st_mode))
      shn_error("'%s' is a block device, not a regular file",info->wave_header.filename);
    else if (S_ISFIFO(sz.st_mode))
      shn_error("'%s' is a fifo, not a regular file",info->wave_header.filename);
#ifndef _WIN32
    else if (S_ISSOCK(sz.st_mode))
      shn_error("'%s' is a socket, not a regular file",info->wave_header.filename);
    else if (S_ISLNK(sz.st_mode))
      shn_error("'%s' is a symbolic link, not a regular file",info->wave_header.filename);
#endif
    return 0;
  }
  info->wave_header.actual_size = (uint32_t)sz.st_size;

  if (NULL == (f = fopen(info->wave_header.filename,"rb"))) {
    shn_error("could not open '%s': %s",info->wave_header.filename,strerror(errno));
    return 0;
  }
  fclose(f);

  return 1;
}

int shn_verify_header(shn_file *this_shn)
{
    uint32_t l;
	int cur = 0;

	if (0 == is_valid_file(this_shn))
	{
		shn_debug("while processing '%s': something went wrong while opening this file, see above",this_shn->wave_header.filename);
		return 0;
	}

	if (this_shn->vars.bytes_in_header < CANONICAL_HEADER_SIZE) {
		shn_debug("while processing '%s': header is only %d bytes (should be at least %d bytes)",
			this_shn->wave_header.filename,this_shn->vars.bytes_in_header,CANONICAL_HEADER_SIZE);
		return 0;
	}

	if (WAVE_RIFF != shn_uchar_to_ulong_le(this_shn->vars.header+cur))
	{
		if (AIFF_FORM == shn_uchar_to_ulong_le(this_shn->vars.header+cur))
			shn_debug("while processing '%s': file contains AIFF data, which is currently not supported",this_shn->wave_header.filename);
		else
			shn_debug("while processing '%s': WAVE header is missing RIFF tag - possible corrupt file",this_shn->wave_header.filename);
		return 0;
	}
	cur += 4;

	this_shn->wave_header.chunk_size = shn_uchar_to_ulong_le(this_shn->vars.header+cur);
	cur += 4;

	if (WAVE_WAVE != shn_uchar_to_ulong_le(this_shn->vars.header+cur))
	{
		shn_debug("while processing '%s': WAVE header is missing WAVE tag",this_shn->wave_header.filename);
		return 0;
	}
	cur += 4;

	for (;;)
	{
		cur += 4;

		l = shn_uchar_to_ulong_le(this_shn->vars.header+cur);
		cur += 4;

		if (WAVE_FMT == shn_uchar_to_ulong_le(this_shn->vars.header+cur-8))
			break;

		cur += l;
	}

	if (l < 16)
	{
		shn_debug("while processing '%s': fmt chunk in WAVE header was too short",this_shn->wave_header.filename);
		return 0;
	}

	this_shn->wave_header.wave_format = shn_uchar_to_ushort_le(this_shn->vars.header+cur);
	cur += 2;

	switch (this_shn->wave_header.wave_format)
	{
		case WAVE_FORMAT_PCM:
			break;
		default:
			shn_debug("while processing '%s': unsupported format 0x%04x (%s) - only PCM data is supported at this time",
				this_shn->wave_header.filename,this_shn->wave_header.wave_format,shn_format_to_str(this_shn->wave_header.wave_format));
                        return 0;
	}

	this_shn->wave_header.channels = shn_uchar_to_ushort_le(this_shn->vars.header+cur);
	cur += 2;
	this_shn->wave_header.samples_per_sec = shn_uchar_to_ulong_le(this_shn->vars.header+cur);
	cur += 4;
	this_shn->wave_header.avg_bytes_per_sec = shn_uchar_to_ulong_le(this_shn->vars.header+cur);
	cur += 4;
	this_shn->wave_header.block_align = shn_uchar_to_ushort_le(this_shn->vars.header+cur);
	cur += 2;
	this_shn->wave_header.bits_per_sample = shn_uchar_to_ushort_le(this_shn->vars.header+cur);
	cur += 2;

	if (this_shn->wave_header.bits_per_sample != 8 && this_shn->wave_header.bits_per_sample != 16)
	{
		shn_debug("while processing '%s': bits per sample is neither 8 nor 16",this_shn->wave_header.filename);
		return 0;
	}

	l -= 16;

	if (l > 0)
		cur += l;

	for (;;)
	{
		cur += 4;

		l = shn_uchar_to_ulong_le(this_shn->vars.header+cur);
		cur += 4;

		if (WAVE_DATA == shn_uchar_to_ulong_le(this_shn->vars.header+cur-8))
			break;

		cur += l;
	}

    this_shn->wave_header.rate = ((uint32_t)this_shn->wave_header.samples_per_sec *
                      (uint32_t)this_shn->wave_header.channels *
                      (uint32_t)this_shn->wave_header.bits_per_sample) / 8;
	this_shn->wave_header.header_size = cur;
	this_shn->wave_header.data_size = l;
	this_shn->wave_header.total_size = this_shn->wave_header.chunk_size + 8;
	this_shn->wave_header.length = this_shn->wave_header.data_size / this_shn->wave_header.rate;
	this_shn->wave_header.exact_length = (double)this_shn->wave_header.data_size / (double)this_shn->wave_header.rate;

	if (this_shn->wave_header.channels == CD_CHANNELS &&
	    this_shn->wave_header.bits_per_sample == CD_BITS_PER_SAMPLE &&
	    this_shn->wave_header.samples_per_sec == CD_SAMPLES_PER_SEC &&
	    this_shn->wave_header.avg_bytes_per_sec == CD_RATE &&
	    this_shn->wave_header.rate == CD_RATE)
	{
		if (this_shn->wave_header.data_size < CD_MIN_BURNABLE_SIZE)
			this_shn->wave_header.problems |= PROBLEM_CD_BUT_TOO_SHORT;
		if (this_shn->wave_header.data_size % CD_BLOCK_SIZE != 0)
			this_shn->wave_header.problems |= PROBLEM_CD_BUT_BAD_BOUND;
	}
	else
		this_shn->wave_header.problems |= PROBLEM_NOT_CD_QUALITY;

	if (this_shn->wave_header.header_size != CANONICAL_HEADER_SIZE)
		this_shn->wave_header.problems |= PROBLEM_HEADER_NOT_CANONICAL;

    if ((uint32_t)this_shn->wave_header.header_size + this_shn->wave_header.data_size > this_shn->wave_header.total_size)
		this_shn->wave_header.problems |= PROBLEM_HEADER_INCONSISTENT;

    if ((uint32_t)this_shn->wave_header.header_size + this_shn->wave_header.data_size < this_shn->wave_header.total_size)
		this_shn->wave_header.problems |= PROBLEM_EXTRA_CHUNKS;

	shn_length_to_str(this_shn);

	/* header looks ok */
	return 1;
}
