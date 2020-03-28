/*  seek.c - functions related to real-time seeking
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
 * $Id: seek.c,v 1.18 2007/03/23 05:49:48 jason Exp $
 */

#include <stdlib.h>
#include "shndec.h"

#define ID3V1_TAG_SIZE 128

shn_seek_entry *shn_seek_entry_search(shn_seek_entry *table, uint32_t goal, uint32_t min, uint32_t max, uint32_t resolution)
{
    uint32_t med = (min + max) / 2;
	shn_seek_entry *middle = table + med;
    uint32_t sample = shn_uchar_to_ulong_le(middle->data);

	shn_debug("Examining seek table entry %lu with sample %lu (min/max = %lu/%lu, goal sample is %lu, resolution is %lu samples)",med,sample,min,max,goal,resolution);

	if (goal < sample)
		return shn_seek_entry_search(table,goal,min,med-1,resolution);
	if (goal > sample + resolution)
		return shn_seek_entry_search(table,goal,med+1,max,resolution);
	return middle;
}

int load_separate_seek_table_generic(char *filename,shn_file *this_shn)
{
	FILE *f;
    int32_t seek_table_len;

	shn_debug("Looking for seek table in separate file: '%s'",filename);

	if (!(f=fopen(filename,"rb")))
	{
		return 0;
	}

	fseek(f,0,SEEK_END);
    seek_table_len = (int32_t)ftell(f) - SEEK_HEADER_SIZE;
	fseek(f,0,SEEK_SET);

	if (fread((void *)this_shn->seek_header.data,1,SEEK_HEADER_SIZE,f) == SEEK_HEADER_SIZE)
	{
        this_shn->seek_header.version = (int32_t)shn_uchar_to_ulong_le(this_shn->seek_header.data+4);
		this_shn->seek_header.shnFileSize = shn_uchar_to_ulong_le(this_shn->seek_header.data+8);
		if (memcmp(this_shn->seek_header.data,SEEK_HEADER_SIGNATURE,strlen(SEEK_HEADER_SIGNATURE)) == 0)
		{
			if (this_shn->seek_header.shnFileSize != this_shn->wave_header.actual_size)
			{
				shn_debug("warning: Seek table expected .shn file size %lu differs from actual .shn file size %lu - seek table might not belong to this file",
					this_shn->seek_header.shnFileSize,this_shn->wave_header.actual_size);
			}

			if ((this_shn->seek_table = malloc(seek_table_len)))
			{
				if (fread((void *)this_shn->seek_table,1,seek_table_len,f) == seek_table_len)
				{
					shn_debug("Successfully loaded seek table in separate file: '%s'",filename);

					this_shn->vars.seek_table_entries = seek_table_len / SEEK_ENTRY_SIZE;

					if (this_shn->vars.seek_table_entries > 1)
						this_shn->vars.seek_resolution = shn_uchar_to_ulong_le(this_shn->seek_table->data+SEEK_ENTRY_SIZE);
					else
						this_shn->vars.seek_resolution = SEEK_RESOLUTION;

					fclose(f);

					return 1;
				}
			}
		}
	}

	fclose(f);
	return 0;
}

int load_appended_seek_table(shn_file *this_shn,const char *filename,long bytes_from_end)
{
	switch (bytes_from_end)
	{
		case 0:
			shn_debug("Looking for seek table appended to file: '%s'",filename);
			break;
		case ID3V1_TAG_SIZE:
			shn_debug("Looking for seek table hidden behind an ID3v1 tag at the end of file: '%s'",filename);
			break;
		default:
			shn_debug("Looking for seek table located %ld bytes from the end of file: '%s'",bytes_from_end,filename);
			break;
	}

    fseek(this_shn->vars.fd,-(SEEK_TRAILER_SIZE+bytes_from_end),SEEK_END);
    if (fread((void *)this_shn->seek_trailer.data,1,SEEK_TRAILER_SIZE,this_shn->vars.fd) == SEEK_TRAILER_SIZE)
	{
		this_shn->seek_trailer.seekTableSize = shn_uchar_to_ulong_le(this_shn->seek_trailer.data);
		if (memcmp(this_shn->seek_trailer.data+4,SEEK_TRAILER_SIGNATURE,strlen(SEEK_TRAILER_SIGNATURE)) == 0)
		{
            fseek(this_shn->vars.fd,-(this_shn->seek_trailer.seekTableSize+bytes_from_end),SEEK_END);
			this_shn->seek_trailer.seekTableSize -= (SEEK_HEADER_SIZE + SEEK_TRAILER_SIZE);
            if (fread((void *)this_shn->seek_header.data,1,SEEK_HEADER_SIZE,this_shn->vars.fd) == SEEK_HEADER_SIZE)
			{
                this_shn->seek_header.version = (int32_t)shn_uchar_to_ulong_le(this_shn->seek_header.data+4);
				this_shn->seek_header.shnFileSize = shn_uchar_to_ulong_le(this_shn->seek_header.data+8);
				if ((this_shn->seek_table = malloc(this_shn->seek_trailer.seekTableSize)))
				{
                    if (fread((void *)this_shn->seek_table,1,this_shn->seek_trailer.seekTableSize,this_shn->vars.fd) == this_shn->seek_trailer.seekTableSize)
					{
						shn_debug("Successfully loaded seek table appended to file: '%s'",filename);

						this_shn->vars.seek_table_entries = this_shn->seek_trailer.seekTableSize / SEEK_ENTRY_SIZE;

						if (this_shn->vars.seek_table_entries > 1)
							this_shn->vars.seek_resolution = shn_uchar_to_ulong_le(this_shn->seek_table->data+SEEK_ENTRY_SIZE);
						else
							this_shn->vars.seek_resolution = SEEK_RESOLUTION;

						return 1;
					}
				}
			}
		}
	}

	return 0;
}

int load_separate_seek_table_samedir(shn_file *this_shn,const char *filename)
{
	char *altfilename,*basefile,*basedir;

	if (!(basefile = shn_get_base_filename(filename)))
	{
		return 0;
	}

	if (!(basedir = shn_get_base_directory(filename)))
	{
		free(basefile);
		return 0;
	}

	if (!(altfilename = malloc(strlen(basedir)+strlen(basefile)+sizeof(SEEK_SUFFIX)+3)))
	{
		shn_debug("Could not allocate memory for same dir filename");
		free(basefile);
		free(basedir);
		return 0;
	}

	sprintf(altfilename,"%s/%s.%s",basedir,basefile,SEEK_SUFFIX);

	free(basefile);
	free(basedir);

	if (load_separate_seek_table_generic(altfilename,this_shn))
	{
		free(altfilename);
		return 1;
	}

	free(altfilename);
	return 0;
}

int load_separate_seek_table_relative(shn_file *this_shn,const char *filename)
{
	char *altfilename,*basefile,*basedir;

	if (!(basefile = shn_get_base_filename(filename)))
	{
		return 0;
	}

	if (!(basedir = shn_get_base_directory(filename)))
	{
		free(basefile);
		return 0;
	}

    if (!(altfilename = malloc(strlen(basedir)+strlen("")+strlen(basefile)+sizeof(SEEK_SUFFIX)+4)))
	{
		shn_debug("Could not allocate memory for absolute filename");
		free(basefile);
		free(basedir);
		return 0;
	}

	free(basefile);
	free(basedir);

	if (load_separate_seek_table_generic(altfilename,this_shn))
	{
		free(altfilename);
		return 1;
	}

	free(altfilename);
	return 0;
}

int load_separate_seek_table_absolute(shn_file *this_shn,const char *filename)
{
	char *altfilename,*basefile;

	if (!(basefile = shn_get_base_filename(filename)))
	{
		return 0;
	}

    if (!(altfilename = malloc(strlen("")+strlen(basefile)+sizeof(SEEK_SUFFIX)+3)))
	{
		shn_debug("Could not allocate memory for same dir filename");
		free(basefile);
		return 0;
	}

	free(basefile);

	if (load_separate_seek_table_generic(altfilename,this_shn))
	{
		free(altfilename);
		return 1;
	}

	free(altfilename);
	return 0;
}

void shn_load_seek_table(shn_file *this_shn,const char *filename)
{
	if (load_appended_seek_table(this_shn,filename,0))
		return;

	if (load_appended_seek_table(this_shn,filename,ID3V1_TAG_SIZE))
		return;

	if (load_separate_seek_table_samedir(this_shn,filename))
		return;

	if (load_separate_seek_table_relative(this_shn,filename))
		return;

	if (load_separate_seek_table_absolute(this_shn,filename))
		return;

	shn_debug("Could not find any seek tables");
}
