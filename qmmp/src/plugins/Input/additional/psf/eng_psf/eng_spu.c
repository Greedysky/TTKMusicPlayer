/*
	Audio Overload SDK - SPU file format engine

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

//
// eng_spu.c
//
// Note: support for old-format files is not tested and may not work.  All the rips I could find
//       are in the newer format.  Also, CDDA and XA commands do not work - I've not found a rip using them.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ao.h"
#include "eng_protos.h"
#include "cpuintrf.h"
#include "psx.h"
#include "peops/externals.h"
#include "peops/spu.h"
#include "peops/regs.h"

typedef struct {
    uint8 *start_of_file, *song_ptr;
    uint32 cur_tick, cur_event, num_events, next_tick, end_tick;
    int old_fmt;
    char name[128], song[128], company[128];
    char *spu_pOutput;
    mips_cpu_context *mips_cpu;
} spu_synth_t;


static void spu_update (unsigned char* pSound,long lBytes,void *data)
{
    spu_synth_t *s = data;
	memcpy(s->spu_pOutput, pSound, lBytes);
}


void *spu_start(const char *path, uint8 *buffer, uint32 length)
{
    spu_synth_t *s = malloc (sizeof (spu_synth_t));
    memset (s, 0, sizeof (spu_synth_t));

	int i;
	uint16 reg;

	if (strncmp((char *)buffer, "SPU", 3))
	{
		spu_stop (s);
		return NULL;
	}

	s->start_of_file = buffer;

    s->mips_cpu = mips_alloc ();
	SPUinit(s->mips_cpu, spu_update, s);
	SPUopen(s->mips_cpu);
	setlength(s->mips_cpu->spu, ~0, 0);

	// upload the SPU RAM image
	SPUinjectRAMImage(s->mips_cpu, (unsigned short *)&buffer[0]);

	// apply the register image	
	for (i = 0; i < 512; i += 2)
	{
		reg = buffer[0x80000+i] | buffer[0x80000+i+1]<<8;

		SPUwriteRegister(s->mips_cpu, (i/2)+0x1f801c00, reg);
	}

	s->old_fmt = 1;

	if ((buffer[0x80200] != 0x44) || (buffer[0x80201] != 0xac) || (buffer[0x80202] != 0x00) || (buffer[0x80203] != 0x00))
	{
		s->old_fmt = 0;
	}

	if (s->old_fmt)
	{
		s->num_events = buffer[0x80204] | buffer[0x80205]<<8 | buffer[0x80206]<<16 | buffer[0x80207]<<24;

		if (((s->num_events * 12) + 0x80208) > length)
		{
			s->old_fmt = 0;
		}
		else
		{
			s->cur_tick = 0;
		}
	}

	if (!s->old_fmt)
	{
		s->end_tick = buffer[0x80200] | buffer[0x80201]<<8 | buffer[0x80202]<<16 | buffer[0x80203]<<24; 
		s->cur_tick = buffer[0x80204] | buffer[0x80205]<<8 | buffer[0x80206]<<16 | buffer[0x80207]<<24; 
		s->next_tick = s->cur_tick;
	}

	s->song_ptr = &buffer[0x80208];
	s->cur_event = 0;

	strncpy((char *)&buffer[4], s->name, 128);
	strncpy((char *)&buffer[0x44], s->song, 128);
	strncpy((char *)&buffer[0x84], s->company, 128);

	return s;
}

static void spu_tick(spu_synth_t *s)
{
	uint32 time, reg, size;
	uint16 rdata;
	uint8 opcode;

	if (s->old_fmt)
	{
		time = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24;

		while ((time == s->cur_tick) && (s->cur_event < s->num_events))
		{
			reg = s->song_ptr[4] | s->song_ptr[5]<<8 | s->song_ptr[6]<<16 | s->song_ptr[7]<<24;
			rdata = s->song_ptr[8] | s->song_ptr[9]<<8;

			SPUwriteRegister(s->mips_cpu, reg, rdata);

			s->cur_event++;
			s->song_ptr += 12;

			time = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24;
		}
	}
	else
	{
		if (s->cur_tick < s->end_tick)
		{
			while (s->cur_tick == s->next_tick)
			{
				opcode = s->song_ptr[0];
				s->song_ptr++;

				switch (opcode)
				{
					case 0:	// write register
						reg = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24;
						rdata = s->song_ptr[4] | s->song_ptr[5]<<8;

						SPUwriteRegister(s->mips_cpu, reg, rdata);

						s->next_tick = s->song_ptr[6] | s->song_ptr[7]<<8 | s->song_ptr[8]<<16 | s->song_ptr[9]<<24; 
						s->song_ptr += 10;
						break;

					case 1:	// read register
				 		reg = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24;
						SPUreadRegister(s->mips_cpu, reg);
						s->next_tick = s->song_ptr[4] | s->song_ptr[5]<<8 | s->song_ptr[6]<<16 | s->song_ptr[7]<<24; 
						s->song_ptr += 8;
						break;

					case 2: // dma write
						size = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24; 
						s->song_ptr += (4 + size);
						s->next_tick = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24; 
						s->song_ptr += 4;
						break;

					case 3: // dma read
						s->next_tick = s->song_ptr[4] | s->song_ptr[5]<<8 | s->song_ptr[6]<<16 | s->song_ptr[7]<<24; 
						s->song_ptr += 8;
						break;

					case 4: // xa play
						s->song_ptr += (32 + 16384);
						s->next_tick = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24; 
						s->song_ptr += 4;
						break;

					case 5: // cdda play
						size = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24; 
						s->song_ptr += (4 + size);
						s->next_tick = s->song_ptr[0] | s->song_ptr[1]<<8 | s->song_ptr[2]<<16 | s->song_ptr[3]<<24; 
						s->song_ptr += 4;
						break;

					default:
						printf("Unknown opcode %d\n", opcode);
						exit(-1);
						break;
				}
			}
		}
		else
		{
//			ao_s->song_done = 1;
		}
	}

	s->cur_tick++;
}

int32 spu_gen(void *handle, int16 *buffer, uint32 samples)
{
    spu_synth_t *s = handle;
	int i, run = 1;

	if (s->old_fmt)
	{
		if (s->cur_event >= s->num_events)
		{
			run = 0;
		}
	}
	else
	{
		if (s->cur_tick >= s->end_tick)
		{
			run = 0;
		}
	}

	if (run)
	{
		for (i = 0; i < samples; i++)
		{
		  	spu_tick(s);
			SPUasync(s->mips_cpu, 384);
		}

		s->spu_pOutput = (char *)buffer;
		SPU_flushboot(s->mips_cpu);
	}
	else
	{
		memset(buffer, 0, samples*2*sizeof(int16));
	}

	return AO_SUCCESS;
}

int32 spu_stop(void *handle)
{
    spu_synth_t *s = handle;
    if (s) {
        if (s->mips_cpu) {
            SPUclose(s->mips_cpu);
            free (s->mips_cpu);
        }
        free (s);
    }
	return AO_SUCCESS;
}

int32 spu_command(void *handle, int32 command, int32 parameter)
{
    spu_synth_t *s = handle;
	switch (command)
	{
		case COMMAND_GET_MIN:
		case COMMAND_GET_MAX:
		{
			return 0;
		}
		break;
		
		case COMMAND_HAS_PREV:
		case COMMAND_HAS_NEXT:
		case COMMAND_PREV:
		case COMMAND_NEXT:
		case COMMAND_JUMP:
		{
			return AO_FAIL;
		}
		break;
		
		case COMMAND_RESTART:
		{
            printf ("eng_spu restart\n");
            uint8 *buffer = s->start_of_file;
            int i;
            uint16 reg;

            // apply the register image	
            for (i = 0; i < 512; i += 2)
            {
                reg = buffer[0x80000+i] | buffer[0x80000+i+1]<<8;

                SPUwriteRegister(s->mips_cpu, (i/2)+0x1f801c00, reg);
            }

            if (!s->old_fmt)
            {
                s->end_tick = buffer[0x80200] | buffer[0x80201]<<8 | buffer[0x80202]<<16 | buffer[0x80203]<<24; 
                s->cur_tick = buffer[0x80204] | buffer[0x80205]<<8 | buffer[0x80206]<<16 | buffer[0x80207]<<24; 
                s->next_tick = s->cur_tick;
            }

            s->song_ptr = &buffer[0x80208];
            s->cur_event = 0;
			return AO_SUCCESS;
		}
		break;
		
#if VERBOSE
		default:
			printf("Unknown command executed!\n");
			break;
#endif		
	}
	
	return AO_FAIL;
}

int32 spu_fill_info(void *handle, ao_display_info *info)
{
    spu_synth_t *s = handle;
	strcpy(info->title[1], "Game: ");
	sprintf(info->info[1], "%.128s", s->name);
	strcpy(info->title[2], "Song: ");
	sprintf(info->info[2], "%.128s", s->song);
	strcpy(info->title[3], "Company: ");
	sprintf(info->info[3], "%.128s", s->company);

	return AO_SUCCESS;
}
