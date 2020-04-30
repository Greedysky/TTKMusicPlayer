/*
	Audio Overload SDK - PSF file format engine

	Copyright (c) 2007 R. Belmont and Richard Bannister.

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
#include <string.h>
#include <stdlib.h>

#include "ao.h"
#include "eng_protos.h"
#include "cpuintrf.h"
#include "psx.h"

#include "peops/stdafx.h"
#include "peops/externals.h"
#include "peops/regs.h"
#include "peops/registers.h"
#include "peops/spu.h"


#include "corlett.h"

#define DEBUG_LOADER	(0)

#define trace(...) { fprintf(stderr, __VA_ARGS__); }
//#define trace(fmt,...)

typedef struct {
    corlett_t	*c;
    char psfby[256];
    mips_cpu_context *mips_cpu;
    char *spu_pOutput;
    uint32 initialPC, initialGP, initialSP;
} psf_synth_t;


static void spu_update (unsigned char* pSound,long lBytes,void *data)
{
    psf_synth_t *s = data;
	memcpy(s->spu_pOutput, pSound, lBytes);
}

void
ao_getlibpath (const char *path, const char *libname, char *libpath, int size) {
    const char *e = strrchr (path, ':');
    if (!e) {
        e = strrchr (path, '/');
    }
    if (e) {
        e++;
        memcpy (libpath, path, e-path);
        libpath[e-path] = 0;
        strcat (libpath, libname);
    }
    else {
        strcpy (libpath, libname);
    }
}

void *psf_start(const char *path, uint8 *buffer, uint32 length)
{
    psf_synth_t *s = malloc (sizeof (psf_synth_t));
    memset (s, 0, sizeof (psf_synth_t));

	uint8 *file = NULL, *lib_decoded = NULL, *lib_raw_file = NULL, *alib_decoded = NULL;
	uint32 offset, plength, PC, SP, GP, lengthMS, fadeMS;
	uint64 file_len, lib_len, lib_raw_length, alib_len;
	corlett_t *lib = NULL;
	int i;
	union cpuinfo mipsinfo;

//	printf("Length = %d\n", length);

	// Decode the current GSF
	if (corlett_decode(buffer, length, &file, &file_len, &s->c) != AO_SUCCESS)
	{
        psf_stop (s);
		return NULL;
	}

//	printf("file_len %d reserve %d\n", file_len, c->res_size);

	// check for PSX EXE signature
	if (strncmp((char *)file, "PS-X EXE", 8))
	{
        psf_stop (s);
		return NULL;
	}

	#if DEBUG_LOADER
	offset = file[0x18] | file[0x19]<<8 | file[0x1a]<<16 | file[0x1b]<<24;
	printf("Text section start: %x\n", offset);
	offset = file[0x1c] | file[0x1d]<<8 | file[0x1e]<<16 | file[0x1f]<<24;
	printf("Text section size: %x\n", offset);
	printf("Region: [%s]\n", &file[0x4c]);
	printf("refresh: [%s]\n", c->inf_refresh);			
	#endif

	PC = file[0x10] | file[0x11]<<8 | file[0x12]<<16 | file[0x13]<<24;
	GP = file[0x14] | file[0x15]<<8 | file[0x16]<<16 | file[0x17]<<24;
	SP = file[0x30] | file[0x31]<<8 | file[0x32]<<16 | file[0x33]<<24;

	#if DEBUG_LOADER
	printf("Top level: PC %x GP %x SP %x\n", PC, GP, SP);
	#endif

	s->mips_cpu = mips_alloc();
    s->mips_cpu->psf_refresh = -1;
	if (s->c->inf_refresh[0] == '5')
	{
		s->mips_cpu->psf_refresh = 50;
	}
	if (s->c->inf_refresh[0] == '6')
	{
		s->mips_cpu->psf_refresh = 60;
	}

	// Get the library file, if any
	if (s->c->lib[0] != 0)
	{
		uint64 tmp_length;
        char libpath[PATH_MAX];
        ao_getlibpath (path, s->c->lib, libpath, sizeof (libpath));
	
		if (ao_get_lib(libpath, &lib_raw_file, &tmp_length) != AO_SUCCESS)
		{
            psf_stop (s);
            return NULL;
		}
		lib_raw_length = tmp_length;
		
		if (corlett_decode(lib_raw_file, lib_raw_length, &lib_decoded, &lib_len, &lib) != AO_SUCCESS)
		{
			free(lib_raw_file);
            psf_stop (s);
            return NULL;
		}
				
		// Free up raw file
		free(lib_raw_file);

		if (strncmp((char *)lib_decoded, "PS-X EXE", 8))
		{
			printf("Major error!  PSF was OK, but referenced library is not!\n");
			free(lib);
            psf_stop (s);
            return NULL;
		}

		#if DEBUG_LOADER	
		offset = lib_decoded[0x18] | lib_decoded[0x19]<<8 | lib_decoded[0x1a]<<16 | lib_decoded[0x1b]<<24;
		printf("Text section start: %x\n", offset);
		offset = lib_decoded[0x1c] | lib_decoded[0x1d]<<8 | lib_decoded[0x1e]<<16 | lib_decoded[0x1f]<<24;
		printf("Text section size: %x\n", offset);
		printf("Region: [%s]\n", &lib_decoded[0x4c]);
		printf("refresh: [%s]\n", lib->inf_refresh);			
		#endif

		// if the original file had no refresh tag, give the lib a shot
		if (s->mips_cpu->psf_refresh == -1)
		{
			if (lib->inf_refresh[0] == '5')
			{
				s->mips_cpu->psf_refresh = 50;
			}
			if (lib->inf_refresh[0] == '6')
			{
				s->mips_cpu->psf_refresh = 60;
			}
		}

		PC = lib_decoded[0x10] | lib_decoded[0x11]<<8 | lib_decoded[0x12]<<16 | lib_decoded[0x13]<<24;
		GP = lib_decoded[0x14] | lib_decoded[0x15]<<8 | lib_decoded[0x16]<<16 | lib_decoded[0x17]<<24;
		SP = lib_decoded[0x30] | lib_decoded[0x31]<<8 | lib_decoded[0x32]<<16 | lib_decoded[0x33]<<24;

		#if DEBUG_LOADER
		printf("Library: PC %x GP %x SP %x\n", PC, GP, SP);
		#endif

		// now patch the file into RAM
		offset = lib_decoded[0x18] | lib_decoded[0x19]<<8 | lib_decoded[0x1a]<<16 | lib_decoded[0x1b]<<24;
		offset &= 0x3fffffff;	// kill any MIPS cache segment indicators
		plength = lib_decoded[0x1c] | lib_decoded[0x1d]<<8 | lib_decoded[0x1e]<<16 | lib_decoded[0x1f]<<24;
		#if DEBUG_LOADER
		printf("library offset: %x plength: %d\n", offset, plength);
		#endif
		memcpy(&s->mips_cpu->psx_ram[offset/4], lib_decoded+2048, plength);
		
		// Dispose the corlett structure for the lib - we don't use it
		free(lib);
		lib = NULL;
	}

	// now patch the main file into RAM OVER the libraries (but not the aux lib)
	offset = file[0x18] | file[0x19]<<8 | file[0x1a]<<16 | file[0x1b]<<24;
	offset &= 0x3fffffff;	// kill any MIPS cache segment indicators
	plength = file[0x1c] | file[0x1d]<<8 | file[0x1e]<<16 | file[0x1f]<<24;

	// Philosoma has an illegal "plength".  *sigh*
	if (plength > (file_len-2048))
	{
		plength = file_len-2048;
	}
	memcpy(&s->mips_cpu->psx_ram[offset/4], file+2048, plength);

	// load any auxiliary libraries now
	for (i = 0; i < 8; i++)
	{
		if (s->c->libaux[i][0] != 0)
		{
			uint64 tmp_length;
            char libpath[PATH_MAX];
            ao_getlibpath (path, s->c->libaux[i], libpath, sizeof (libpath));
		
			if (ao_get_lib(libpath, &lib_raw_file, &tmp_length) != AO_SUCCESS)
			{
                psf_stop (s);
                return NULL;
			}
			lib_raw_length = tmp_length;
		
			if (corlett_decode(lib_raw_file, lib_raw_length, &alib_decoded, &alib_len, &lib) != AO_SUCCESS)
			{
				free(lib_raw_file);
                psf_stop (s);
                return NULL;
			}
				
			// Free up raw file
			free(lib_raw_file);

			if (strncmp((char *)alib_decoded, "PS-X EXE", 8))
			{
				printf("Major error!  PSF was OK, but referenced library is not!\n");
				free(lib);
                psf_stop (s);
                return NULL;
			}

			#if DEBUG_LOADER	
			offset = alib_decoded[0x18] | alib_decoded[0x19]<<8 | alib_decoded[0x1a]<<16 | alib_decoded[0x1b]<<24;
			printf("Text section start: %x\n", offset);
			offset = alib_decoded[0x1c] | alib_decoded[0x1d]<<8 | alib_decoded[0x1e]<<16 | alib_decoded[0x1f]<<24;
			printf("Text section size: %x\n", offset);
			printf("Region: [%s]\n", &alib_decoded[0x4c]);
			#endif

			// now patch the file into RAM
			offset = alib_decoded[0x18] | alib_decoded[0x19]<<8 | alib_decoded[0x1a]<<16 | alib_decoded[0x1b]<<24;
			offset &= 0x3fffffff;	// kill any MIPS cache segment indicators
			plength = alib_decoded[0x1c] | alib_decoded[0x1d]<<8 | alib_decoded[0x1e]<<16 | alib_decoded[0x1f]<<24;
			memcpy(&s->mips_cpu->psx_ram[offset/4], alib_decoded+2048, plength);
		
			// Dispose the corlett structure for the lib - we don't use it
			free(lib);
			lib = NULL;
			free (alib_decoded);
			alib_decoded = NULL;
		}
	}

	free(file);
	file = NULL;
	free(lib_decoded);
	lib_decoded = NULL;
	
	// Finally, set psfby tag
	strcpy(s->psfby, "n/a");
	if (s->c)
	{
		int i;
		for (i = 0; i < MAX_UNKNOWN_TAGS; i++)
		{
			if (!strcasecmp(s->c->tag_name[i], "psfby"))
				strcpy(s->psfby, s->c->tag_data[i]);
		}
	}

	mips_init (s->mips_cpu);
	mips_reset(s->mips_cpu, NULL);
//	// clear PSX work RAM before we start scribbling in it
//	memset(s->mips_cpu->psx_ram, 0, 2*1024*1024);


	// set the initial PC, SP, GP
	#if DEBUG_LOADER	
	printf("Initial PC %x, GP %x, SP %x\n", PC, GP, SP);
	printf("Refresh = %d\n", psf_refresh);
	#endif
	mipsinfo.i = PC;
	mips_set_info(s->mips_cpu, CPUINFO_INT_PC, &mipsinfo);

	// set some reasonable default for the stack
	if (SP == 0)
	{
		SP = 0x801fff00;
	}

	mipsinfo.i = SP;
	mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R29, &mipsinfo);
	mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R30, &mipsinfo);

	mipsinfo.i = GP;
	mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R28, &mipsinfo);

	#if DEBUG_LOADER && 1
	{
		FILE *f;

		f = fopen("psxram.bin", "wb");
		fwrite(psx_ram, 2*1024*1024, 1, f);
		fclose(f);
	}
	#endif

	psx_hw_init(s->mips_cpu);
	SPUinit(s->mips_cpu, spu_update, s);
	SPUopen(s->mips_cpu);

	lengthMS = psfTimeToMS(s->c->inf_length);
	fadeMS = psfTimeToMS(s->c->inf_fade);

	#if DEBUG_LOADER
	printf("length %d fade %d\n", lengthMS, fadeMS);
	#endif

	if (lengthMS == 0) 
	{
		lengthMS = ~0;
	}

	setlength(s->mips_cpu->spu, lengthMS, fadeMS);

	// patch illegal Chocobo Dungeon 2 code - CaitSith2 put a jump in the delay slot from a BNE
	// and rely on Highly Experimental's buggy-ass CPU to rescue them.  Verified on real hardware
	// that the initial code is wrong.
	if (s->c->inf_game)
	{
		if (!strcmp(s->c->inf_game, "Chocobo Dungeon 2"))
		{
			if (s->mips_cpu->psx_ram[0xbc090/4] == LE32(0x0802f040))
			{
		 		s->mips_cpu->psx_ram[0xbc090/4] = LE32(0);
				s->mips_cpu->psx_ram[0xbc094/4] = LE32(0x0802f040);
				s->mips_cpu->psx_ram[0xbc098/4] = LE32(0);
			}
		}
	}

//	psx_ram[0x118b8/4] = LE32(0);	// crash 2 hack

	// backup the initial state for restart
	memcpy(s->mips_cpu->initial_ram, s->mips_cpu->psx_ram, 2*1024*1024);
	memcpy(s->mips_cpu->initial_scratch, s->mips_cpu->psx_scratch, 0x400);
	s->initialPC = PC;
	s->initialGP = GP;
	s->initialSP = SP;

	mips_execute(s->mips_cpu, 5000);
	
	return s;
}

int32 psf_gen(void *handle, int16 *buffer, uint32 samples)
{	
    psf_synth_t *s = handle;
	int i;

	for (i = 0; i < samples; i++)
	{
		psx_hw_slice(s->mips_cpu);
		SPUasync(s->mips_cpu, 384);
	}

    s->spu_pOutput = (char *)buffer;
	SPU_flushboot(s->mips_cpu);

	psx_hw_frame(s->mips_cpu);

	return AO_SUCCESS;
}

int32 psf_stop(void *handle)
{
    psf_synth_t *s = handle;
    if (s->mips_cpu) {
        SPUclose(s->mips_cpu);
        mips_exit (s->mips_cpu);
    }
	free(s->c);
	free (s);

	return AO_SUCCESS;
}

int32 psf_command(void *handle, int32 command, int32 parameter)
{
    psf_synth_t *s = handle;
	union cpuinfo mipsinfo;
	uint32 lengthMS, fadeMS;

	switch (command)
	{
		case COMMAND_RESTART:
			SPUclose(s->mips_cpu);

			memcpy(s->mips_cpu->psx_ram, s->mips_cpu->initial_ram, 2*1024*1024);
			memcpy(s->mips_cpu->psx_scratch, s->mips_cpu->initial_scratch, 0x400);

//			mips_init();
			mips_reset(s->mips_cpu, NULL);
			psx_hw_init(s->mips_cpu);
            SPUinit(s->mips_cpu, spu_update, s);
            SPUopen(s->mips_cpu);

			lengthMS = psfTimeToMS(s->c->inf_length);
			fadeMS = psfTimeToMS(s->c->inf_fade);

			if (lengthMS == 0) 
			{
				lengthMS = ~0;
			}
			setlength(s->mips_cpu->spu, lengthMS, fadeMS);

			mipsinfo.i = s->initialPC;
			mips_set_info(s->mips_cpu, CPUINFO_INT_PC, &mipsinfo);
			mipsinfo.i = s->initialSP;
			mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R29, &mipsinfo);
			mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R30, &mipsinfo);
			mipsinfo.i = s->initialGP;
			mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R28, &mipsinfo);

			mips_execute(s->mips_cpu, 5000);

			return AO_SUCCESS;
		
	}
	return AO_FAIL;
}

int32 psf_fill_info(void *handle, ao_display_info *info)
{
    psf_synth_t *s = handle;
	if (s->c == NULL)
		return AO_FAIL;
		
	strcpy(info->title[1], "Name: ");
	sprintf(info->info[1], "%s", s->c->inf_title);

	strcpy(info->title[2], "Game: ");
	sprintf(info->info[2], "%s", s->c->inf_game);
	
	strcpy(info->title[3], "Artist: ");
	sprintf(info->info[3], "%s", s->c->inf_artist);

	strcpy(info->title[4], "Copyright: ");
	sprintf(info->info[4], "%s", s->c->inf_copy);

	strcpy(info->title[5], "Year: ");
	sprintf(info->info[5], "%s", s->c->inf_year);

	strcpy(info->title[6], "Length: ");
	sprintf(info->info[6], "%s", s->c->inf_length);

	strcpy(info->title[7], "Fade: ");
	sprintf(info->info[7], "%s", s->c->inf_fade);

	strcpy(info->title[8], "Ripper: ");
	sprintf(info->info[8], "%s", s->psfby);

	return AO_SUCCESS;
}
