/*
	Audio Overload SDK - SSF file format engine

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

/*

Sega driver commands:

00 - NOP
01 - SEQUENCE_START
02 - SEQUENCE_STOP
03 - SEQUENCE_PAUSE
04 - SEQUENCE_CONTINUE
05 - SEQUENCE_VOLUME
06 - SEQUENCE_ALLSTOP
07 - SEQUENCE_TEMPO
08 - SEQUENCE_MAP
09 - HOST_MIDI
0A - VOLUME_ANALYZE_START
0B - VOLUME_ANALYZE_STOP
0C - DSP CLEAR
0D - ALL OFF
0E - SEQUENCE PAN
0F - N/A
10 - SOUND INITIALIZE
11 - Yamaha 3D check (8C)
12 - QSound check (8B)
13 - Yamaha 3D init (8D)
80 - CD level
81 - CD pan
82 - MASTER VOLUME
83 - EFFECT_CHANGE
84 - NOP
85 - PCM stream play start
86 - PCM stream play end
87 - MIXER_CHANGE
88 - Mixer parameter change
89 - Hardware check
8A - PCM parameter change
8B - QSound check
8C - Yamaha 3D check
8D - Yamaha 3D init

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ao.h"
#include "eng_protos.h"
#include "corlett.h"
#include "sat_hw.h"
#include "scsp.h"
#include "m68kcpu.h"

#define DEBUG_LOADER	(0)

typedef struct {
    corlett_t	*c;
    char 		psfby[256];
    uint32		decaybegin, decayend, total_samples;
    uint8 init_sat_ram[512*1024];
    m68ki_cpu_core *cpu;
} ssf_synth_t;

void *ssf_start(const char *path, uint8 *buffer, uint32 length)
{
    ssf_synth_t *s = malloc (sizeof (ssf_synth_t));
    memset (s, 0, sizeof (ssf_synth_t));
	uint8 *file, *lib_decoded, *lib_raw_file;
	uint32 offset, lengthMS, fadeMS;
	uint64 file_len, lib_len, lib_raw_length;
	corlett_t *lib;
	char *libfile;
	int i;

    s->cpu = m68k_init ();
//	// clear Saturn work RAM before we start scribbling in it
//	memset(cpu->sat_ram, 0, 512*1024);

	// Decode the current SSF
	if (corlett_decode(buffer, length, &file, &file_len, &s->c) != AO_SUCCESS)
	{
		ssf_stop (s);
		return NULL;
	}

	#if DEBUG_LOADER
	printf("%d bytes decoded\n", file_len);
	#endif

	// Get the library file, if any
	for (i=0; i<9; i++) 
	{
		libfile = i ? s->c->libaux[i-1] : s->c->lib;
		if (libfile[0] != 0)
		{
			uint64 tmp_length;
	
            char libpath[PATH_MAX];
            ao_getlibpath (path, s->c->lib, libpath, sizeof (libpath));
			#if DEBUG_LOADER	
			printf("Loading library: %s\n", c->lib);
			#endif
			if (ao_get_lib(libfile, &lib_raw_file, &tmp_length) != AO_SUCCESS)
			{
                ssf_stop (s);
                return NULL;
			}
			lib_raw_length = tmp_length;
		
			if (corlett_decode(lib_raw_file, lib_raw_length, &lib_decoded, &lib_len, &lib) != AO_SUCCESS)
			{
				free(lib_raw_file);
                ssf_stop (s);
                return NULL;
			}
				
			// Free up raw file
			free(lib_raw_file);

			// patch the file into ram
			offset = lib_decoded[0] | lib_decoded[1]<<8 | lib_decoded[2]<<16 | lib_decoded[3]<<24;

			// guard against invalid data
			if ((offset + (lib_len-4)) > 0x7ffff)
			{
				lib_len = 0x80000-offset+4;
			}
			memcpy(&s->cpu->sat_ram[offset], lib_decoded+4, lib_len-4);

			// Dispose the corlett structure for the lib - we don't use it
			free(lib);
		}
	}

	// now patch the file into RAM over the libraries
	offset = file[3]<<24 | file[2]<<16 | file[1]<<8 | file[0];

	// guard against invalid data
	if ((offset + (file_len-4)) > 0x7ffff)
	{
		file_len = 0x80000-offset+4;
	}

	memcpy(&s->cpu->sat_ram[offset], file+4, file_len-4);

	free(file);
	
	// Finally, set psfby tag
	strcpy(s->psfby, "n/a");
	if (s->c)
	{
		for (i = 0; i < MAX_UNKNOWN_TAGS; i++)
		{
			if (!strcasecmp(s->c->tag_name[i], "psfby"))
				strcpy(s->psfby, s->c->tag_data[i]);
		}
	}

	#if DEBUG_LOADER
	{
		FILE *f;

		f = fopen("satram.bin", "wb");
		fwrite(cpu->sat_ram, 512*1024, 1, f);
		fclose(f);
	}
	#endif

	// now flip everything (this makes sense because he's using starscream)
	for (i = 0; i < 512*1024; i+=2)
	{
		uint8 temp;

		temp = s->cpu->sat_ram[i];
		s->cpu->sat_ram[i] = s->cpu->sat_ram[i+1];
		s->cpu->sat_ram[i+1] = temp;
	}

	// backup for fast restarting
	memcpy (s->init_sat_ram, s->cpu->sat_ram, sizeof (s->init_sat_ram));

	sat_hw_init(s->cpu);

	// now figure out the time in samples for the length/fade
	lengthMS = psfTimeToMS(s->c->inf_length);
	fadeMS = psfTimeToMS(s->c->inf_fade);
	s->total_samples = 0;

	if (lengthMS == 0)
	{
		lengthMS = ~0;
	}

	if (lengthMS == ~0)
	{
		s->decaybegin = lengthMS;
	}
	else
	{
		lengthMS = (lengthMS * 441) / 10;
		fadeMS = (fadeMS * 441) / 10;

		s->decaybegin = lengthMS;
		s->decayend = lengthMS + fadeMS;
	}

	return s;
}

int32 ssf_gen(void *handle, int16 *buffer, uint32 samples)
{	
    ssf_synth_t *s = handle;
	int i;
	int16 output[samples], output2[samples];
	int16 *stereo[2];
	int16 *outp = buffer;

	for (i = 0; i < samples; i++)
	{
		m68k_execute(s->cpu, (11300000/60)/735);
		stereo[0] = &output[i];
		stereo[1] = &output2[i];
		SCSP_Update(s->cpu->SCSP, NULL, stereo, 1);
	}

	for (i = 0; i < samples; i++)
	{
		// process the fade tags
		if (s->total_samples >= s->decaybegin)
		{
			if (s->total_samples >= s->decayend)
			{
				// song is done here, call out as necessary to make your player stop
				output[i] = 0;
				output2[i] = 0;
			}
			else
			{
				int32 fader = 256 - (256*(s->total_samples - s->decaybegin)/(s->decayend-s->decaybegin));
				output[i] = (output[i] * fader)>>8;
				output2[i] = (output2[i] * fader)>>8;

				s->total_samples++;
			}
		}
		else
		{
			s->total_samples++;
		}

		*outp++ = output[i];
		*outp++ = output2[i];
	}

	return AO_SUCCESS;
}

int32 ssf_stop(void *handle)
{
    ssf_synth_t *s = handle;
    if (s) {
        if (s->c) {
            free (s->c);
        }
        if (s->cpu) {
            sat_hw_free (s->cpu);
            free (s->cpu);
        }
        free (s);
    }
    return AO_SUCCESS;
}

int32 ssf_command(void *handle, int32 command, int32 parameter)

{
    ssf_synth_t *s = handle;
	switch (command)
	{
		case COMMAND_RESTART:
            sat_hw_free (s->cpu);
            memcpy (s->cpu->sat_ram, s->init_sat_ram, sizeof (s->init_sat_ram));
            sat_hw_init (s->cpu);
            s->total_samples = 0;
			return AO_SUCCESS;
		
	}
	return AO_FAIL;
}

int32 ssf_fill_info(void *handle, ao_display_info *info)
{
    ssf_synth_t *s = handle;
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
