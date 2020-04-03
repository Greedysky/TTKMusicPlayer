/*
	Audio Overload SDK - main driver.  for demonstration only, not user friendly!

	Copyright (c) 2007-2009 R. Belmont and Richard Bannister.

	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	* Neither the names of R. Belmont and Richard Bannister nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
	CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stdio_file.h"
#include "ao.h"
#include "eng_protos.h"

static struct 
{ 
	uint32 sig; 
	char *name; 
	void * (*start)(const char *path, uint8 *, uint32); // returns handle
	int32 (*gen)(void *handle, int16 *, uint32); 
	int32 (*stop)(void *handle); 
	int32 (*command)(void *handle, int32, int32); 
	uint32 rate; 
	int32 (*fillinfo)(void *handle, ao_display_info *); 
} types[] = {
	{ 0x50534641, "Capcom QSound (.qsf)", qsf_start, qsf_gen, qsf_stop, qsf_command, 60, qsf_fill_info },
	{ 0x50534611, "Sega Saturn (.ssf)", ssf_start, ssf_gen, ssf_stop, ssf_command, 60, ssf_fill_info },
	{ 0x50534601, "Sony PlayStation (.psf)", psf_start, psf_gen, psf_stop, psf_command, 60, psf_fill_info },
	{ 0x53505500, "Sony PlayStation (.spu)", spu_start, spu_gen, spu_stop, spu_command, 60, spu_fill_info },
	{ 0x50534602, "Sony PlayStation 2 (.psf2)", psf2_start, psf2_gen, psf2_stop, psf2_command, 60, psf2_fill_info },
	{ 0x50534612, "Sega Dreamcast (.dsf)", dsf_start, dsf_gen, dsf_stop, dsf_command, 60, dsf_fill_info },

	{ 0xffffffff, "", NULL, NULL, NULL, NULL, 0, NULL }
};

/* ao_get_lib: called to load secondary files */
int ao_get_lib(char *filename, uint8 **buffer, uint64 *length)
{
	uint8 *filebuf;
	uint32 size;
    FILE *auxfile;

        auxfile = stdio_open(filename);
	if (!auxfile)
	{
		fprintf(stderr, "Unable to find auxiliary file %s\n", filename);
		return AO_FAIL;
	}

        stdio_seek(auxfile, 0, SEEK_END);
        size = stdio_tell(auxfile);
        stdio_seek(auxfile, 0, SEEK_SET);

	filebuf = malloc(size);

	if (!filebuf)
	{
                stdio_close(auxfile);
		printf("ERROR: could not allocate %d bytes of memory\n", size);
		return AO_FAIL;
	}

        stdio_read(filebuf, size, 1, auxfile);
        stdio_close(auxfile);

	*buffer = filebuf;
	*length = (uint64)size;

	return AO_SUCCESS;
}

#if 0
/* file types */
static uint32 type;

static void do_frame(uint32 size, int16 *buffer)
{
	(*types[type].gen)(buffer, size);
}

int main(int argv, char *argc[])
{
	FILE *file;
	uint8 *buffer;
	uint32 size, filesig;

	printf("AOSDK test program v1.0 by R. Belmont [AOSDK release 1.4.8]\nCopyright (c) 2007-2009 R. Belmont and Richard Bannister - please read license.txt for license details\n\n");

	// check if an argument was given
	if (argv < 2)
	{
		printf("Error: must specify a filename!\n");
		return -1;
	}	

	file = fopen(argc[1], "rb");

	if (!file)
	{
		printf("ERROR: could not open file %s\n", argc[1]);
		return -1;
	}

	// get the length of the file by seeking to the end then reading the current position
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	// reset the pointer
	fseek(file, 0, SEEK_SET);

	buffer = malloc(size);

	if (!buffer)
	{
		fclose(file);
		printf("ERROR: could not allocate %d bytes of memory\n", size);
		return -1;
	}

	// read the file
	fread(buffer, size, 1, file);
	fclose(file);

	// now try to identify the file
	type = 0;
	filesig = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
	while (types[type].sig != 0xffffffff)
	{
		if (filesig == types[type].sig)
		{
			break;
		}
		else
		{
			type++;
		}
	}

	// now did we identify it above or just fall through?
	if (types[type].sig != 0xffffffff)
	{
		printf("File identified as %s\n", types[type].name);
	}
	else
	{
		printf("ERROR: File is unknown, signature bytes are %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
		free(buffer);
		return -1;
	}

    void *handle = (*types[type].start)(argc[1], buffer, size);

	if (!handle)
	{
		free(buffer);
		printf("ERROR: Engine rejected file!\n");
		return -1;
	}
	
#if 0
	m1sdr_Init(44100);
	m1sdr_SetCallback(do_frame);
	m1sdr_PlayStart();

	printf("\n\nPlaying.  Press CTRL-C to stop.\n");

	while (1)
	{
		m1sdr_TimeCheck();
	}		
#endif
	free(buffer);

	return 1;
}
#endif

// stub for MAME stuff
int change_pc(int foo)
{
    return 0;
}

int
ao_identify (char *buffer) {
    uint32 filesig;
    uint32 type = 0;

	filesig = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
	while (types[type].sig != 0xffffffff)
	{
		if (filesig == types[type].sig)
		{
			break;
		}
		else
		{
			type++;
		}
	}

	// now did we identify it above or just fall through?
	if (types[type].sig != 0xffffffff)
	{
        printf ("psf: File identified as %s\n", types[type].name);
	}
	else
	{
        printf ("psf: File is unknown, signature bytes are %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
		return -1;
    }
    return type;
}

void *
ao_start (uint32 type, const char *path, uint8 *buffer, uint32 size) {
	return (*types[type].start)(path, buffer, size);
}

int
ao_stop (uint32 type, void *handle) {
	return (*types[type].stop)(handle);
}

int
ao_get_info (uint32 type, void *handle, ao_display_info *info) {
	return (*types[type].fillinfo)(handle, info);
}

int
ao_decode (uint32 type, void *handle, int16 *buffer, uint32 size) {
	(*types[type].gen)(handle, buffer, size);
	return size;
}

int
ao_command (uint32 type, void *handle, int32 command, int32 param) {
	return (*types[type].command)(handle, command, param);
}
