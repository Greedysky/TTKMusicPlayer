//
// Audio Overload
// Emulated music player
//
// (C) 2000-2008 Richard F. Bannister
//

//
// eng_dsf.c
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ao.h"
#include "eng_protos.h"
#include "corlett.h"
#include "dc_hw.h"
#include "cpuintrf.h"
#include "aica.h"
#include "aicadsp.h"

#define DEBUG_LOADER	(0)
#define DK_CORE	(1)

#if DK_CORE
#include "arm7.h"
#else
#include "arm7core.h"
#endif

typedef struct {
    corlett_t	*c;
    char 		psfby[256];
    uint32		decaybegin, decayend, total_samples;
    struct sARM7 *cpu;
    uint8 init_dc_ram[8*1024*1024];
} dsf_synth_t;

void *dsf_start(const char *path, uint8 *buffer, uint32 length)
{
    dsf_synth_t *s = malloc (sizeof (dsf_synth_t));
    memset (s, 0, sizeof (dsf_synth_t));

	uint8 *file = NULL, *lib_decoded = NULL, *lib_raw_file = NULL;
	uint32 offset, lengthMS, fadeMS;
	uint64 file_len, lib_len, lib_raw_length;
	corlett_t *lib;
	char *libfile;
	int i;

	// Decode the current SSF
	if (corlett_decode(buffer, length, &file, &file_len, &s->c) != AO_SUCCESS)
	{
        dsf_stop (s);
		return NULL;
	}

	#if DEBUG_LOADER
	printf("%d bytes decoded\n", file_len);
	#endif

	s->cpu = ARM7_Alloc ();
	// Get the library file, if any
	for (i=0; i<9; i++) {
		libfile = i ? s->c->libaux[i-1] : s->c->lib;
		if (libfile[0] != 0)
		{
			uint64 tmp_length;
			char libpath[PATH_MAX];
            ao_getlibpath (path, s->c->lib, libpath, sizeof (libpath));
	
			#if DEBUG_LOADER	
			printf("Loading library: %s\n", libpath);
			#endif
			if (ao_get_lib(libpath, &lib_raw_file, &tmp_length) != AO_SUCCESS)
			{
                dsf_stop (s);
				return NULL;
			}
			lib_raw_length = tmp_length;
		
			if (corlett_decode(lib_raw_file, lib_raw_length, &lib_decoded, &lib_len, &lib) != AO_SUCCESS)
			{
				free(lib_raw_file);
                dsf_stop (s);
				return NULL;
			}
				
			// Free up raw file
			free(lib_raw_file);

			// patch the file into ram
			offset = lib_decoded[0] | lib_decoded[1]<<8 | lib_decoded[2]<<16 | lib_decoded[3]<<24;
			memcpy(&s->cpu->dc_ram[offset], lib_decoded+4, lib_len-4);

			// Dispose the corlett structure for the lib - we don't use it
			free(lib_decoded);
			free(lib);
		}
	}

	// now patch the file into RAM over the libraries
	offset = file[3]<<24 | file[2]<<16 | file[1]<<8 | file[0];
	memcpy(&s->cpu->dc_ram[offset], file+4, file_len-4);

	free(file);
	
	// Finally, set psfby/ssfby tag
	strcpy(s->psfby, "n/a");
	if (s->c)
	{
		for (i = 0; i < MAX_UNKNOWN_TAGS; i++)
		{
			if ((!strcasecmp(s->c->tag_name[i], "psfby")) || (!strcasecmp(s->c->tag_name[i], "ssfby")))
				strcpy(s->psfby, s->c->tag_data[i]);
		}
	}

	#if DEBUG_LOADER && 0
	{
		FILE *f;

		f = fopen("dcram.bin", "wb");
		fwrite(s->cpu->dc_ram, 2*1024*1024, 1, f);
		fclose(f);
	}
	#endif


    memcpy (s->init_dc_ram, s->cpu->dc_ram, sizeof (s->init_dc_ram));
	#if DK_CORE
	ARM7_Init(s->cpu);
	#else
	arm7_init(cpu, 0, 45000000, NULL, NULL);
	arm7_reset(s->cpu);
	#endif
	dc_hw_init(s->cpu);

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

int32 dsf_gen(void *handle, int16 *buffer, uint32 samples)
{
    dsf_synth_t *s = handle;

	int i;
	int16 output[44100/30], output2[44100/30];
	int16 *stereo[2];
	int16 *outp = buffer;
	int opos;

	opos = 0;
	for (i = 0; i < samples; i++)
	{
		#if DK_CORE
		ARM7_Execute(s->cpu, (33000000 / 60 / 4) / 735);
		#else
		arm7_execute(s->cpu, (33000000 / 60 / 4) / 735);
		#endif
		stereo[0] = &output[opos];
		stereo[1] = &output2[opos];
		AICA_Update(s->cpu->AICA, NULL, NULL, stereo, 1);
		opos++;		
	}

	for (i = 0; i < samples; i++)
	{
		// process the fade tags
		if (s->total_samples >= s->decaybegin)
		{
			if (s->total_samples >= s->decayend)
			{
				// song is done here, signal your player appropriately!
//				ao_song_done = 1;
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

int32 dsf_stop(void *handle)
{
    dsf_synth_t *s = handle;
    if (s->cpu) {
        dc_hw_free (s->cpu);
        ARM7_Free (s->cpu);
    }
    if (s->c) {
        free (s->c);
    }
    free (s);
	return AO_SUCCESS;
}

int32 dsf_command(void *handle, int32 command, int32 parameter)
{
    dsf_synth_t *s = handle;
	switch (command)
	{
		case COMMAND_RESTART:
            dc_hw_free (s->cpu);
            memcpy (s->cpu->dc_ram, s->init_dc_ram, sizeof (s->init_dc_ram));
            ARM7_Init(s->cpu);
            dc_hw_init (s->cpu);
            s->total_samples = 0;
			return AO_SUCCESS;
		
	}
	return AO_FAIL;
}

int32 dsf_fill_info(void *handle, ao_display_info *info)
{
    dsf_synth_t *s = handle;
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
