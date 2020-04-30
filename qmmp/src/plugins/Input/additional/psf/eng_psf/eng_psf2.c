/*
	Audio Overload SDK - PSF2 file format engine

	Copyright (c) 2007-2008 R. Belmont and Richard Bannister.

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
// Audio Overload
// Emulated music player
//
// (C) 2000-2008 Richard F. Bannister
//

//
// eng_psf2.c
//
// References:
// psf_format.txt v1.6 by Neill Corlett (s->filesystem and decompression info)
// Intel ELF format specs ELF.PS (general ELF parsing info)
// http://ps2dev.org/kb.x?T=457 (IRX relocation and inter-module call info)
// http://ps2dev.org/ (the whole site - lots of IOP info)
// spu2regs.txt (comes with SexyPSF source: IOP hardware info)
// 64-bit ELF Object File Specification: http://techpubs.sgi.com/library/manuals/4000/007-4658-001/pdf/007-4658-001.pdf (MIPS ELF relocation types)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

#include "ao.h"
#include "eng_protos.h"
#include "cpuintrf.h"
#include "psx.h"

#include "peops2/stdafx.h"
#include "peops2/externals.h"
#include "peops2/regs.h"
#include "peops2/registers.h"
#include "peops2/spu.h"

#include "corlett.h"

#define DEBUG_LOADER	(0)
#define MAX_FS		(32)	// maximum # of s->filesystems (libs and subdirectories)

// ELF relocation helpers
#define ELF32_R_SYM(val)                ((val) >> 8)
#define ELF32_R_TYPE(val)               ((val) & 0xff)

// main RAM
static uint32 loadAddr, lengthMS, fadeMS;
static uint8 *filesys[MAX_FS];
static uint32 fssize[MAX_FS];
static int num_fs;

typedef struct {
    corlett_t	*c;
    char 		psfby[256];
    uint32 initialPC, initialSP;

    uint8 *lib_raw_file;
    mips_cpu_context *mips_cpu;
    char		*spu_pOutput;
} psf2_synth_t;

void ps2_update(unsigned char *pSound, long lBytes, void *data)
{
    psf2_synth_t *s = data;
	memcpy(s->spu_pOutput, pSound, lBytes);	// (for direct 44.1kHz output)
}

#if 0
static uint32 secname(uint8 *start, uint32 strndx, uint32 shoff, uint32 shentsize, uint32 name)
{
	uint32 offset, shent;

	// get string table section
	shent = shoff + (shentsize * strndx);

	// find the offset to the section
	offset = start[shent+16] | start[shent+17]<<8 | start[shent+18]<<16 | start[shent+19]<<24; 

	offset += name;
	
	return offset;	
}
#endif

static void do_iopmod(uint8 *start, uint32 offset)
{
	uint32 nameoffs, saddr, heap, tsize, dsize, bsize, vers2;

	nameoffs = start[offset] | start[offset+1]<<8 | start[offset+2]<<16 | start[offset+3]<<24;

	saddr = start[offset+4] | start[offset+5]<<8 | start[offset+6]<<16 | start[offset+7]<<24;
	heap = start[offset+8] | start[offset+9]<<8 | start[offset+10]<<16 | start[offset+11]<<24;
	tsize = start[offset+12] | start[offset+13]<<8 | start[offset+14]<<16 | start[offset+15]<<24; 
	dsize = start[offset+16] | start[offset+17]<<8 | start[offset+18]<<16 | start[offset+19]<<24; 
	bsize = start[offset+20] | start[offset+21]<<8 | start[offset+22]<<16 | start[offset+23]<<24; 
	vers2 = start[offset+24] | start[offset+25]<<8;

//	printf("nameoffs %08x saddr %08x heap %08x tsize %08x dsize %08x bsize %08x\n", nameoffs, saddr, heap, tsize, dsize, bsize);
	#if DEBUG_LOADER
	printf("vers: %04x name [%s]\n", vers2, &start[offset+26]);
	#endif
}

uint32 psf2_load_elf(mips_cpu_context *cpu, uint8 *start, uint32 len)
{
	uint32 entry, phoff, shoff, phentsize, shentsize, phnum, shnum, shstrndx;
	uint32 name, type, flags, addr, offset, size, shent;
	uint32 totallen;
	int i, rec;
//	FILE *f;

	if (loadAddr & 3)
	{
		loadAddr &= ~3;
		loadAddr += 4;
	}

	#if DEBUG_LOADER
	printf("psf2_load_elf: starting at %08x\n", loadAddr | 0x80000000);
	#endif

	if ((start[0] != 0x7f) || (start[1] != 'E') || (start[2] != 'L') || (start[3] != 'F'))
	{
		printf("Not an ELF file\n");
		return 0xffffffff;
	}

	entry = start[24] | start[25]<<8 | start[26]<<16 | start[27]<<24;	// 0x18
	phoff = start[28] | start[29]<<8 | start[30]<<16 | start[31]<<24; 	// 0x1c
	shoff = start[32] | start[33]<<8 | start[34]<<16 | start[35]<<24; 	// 0x20
		
//	printf("Entry: %08x phoff %08x shoff %08x\n", entry, phoff, shoff);

	phentsize = start[42] | start[43]<<8;			// 0x2a
	phnum = start[44] | start[45]<<8;			// 0x2c
	shentsize = start[46] | start[47]<<8;			// 0x2e
	shnum = start[48] | start[49]<<8;			// 0x30
	shstrndx = start[50] | start[51]<<8;			// 0x32

//	printf("phentsize %08x phnum %d shentsize %08x shnum %d shstrndx %d\n", phentsize, phnum, shentsize, shnum, shstrndx);	

	// process ELF sections
	shent = shoff;
	totallen = 0;
	for (i = 0; i < shnum; i++)
	{
		name = start[shent] | start[shent+1]<<8 | start[shent+2]<<16 | start[shent+3]<<24;
		type = start[shent+4] | start[shent+5]<<8 | start[shent+6]<<16 | start[shent+7]<<24;
		flags = start[shent+8] | start[shent+9]<<8 | start[shent+10]<<16 | start[shent+11]<<24;
		addr = start[shent+12] | start[shent+13]<<8 | start[shent+14]<<16 | start[shent+15]<<24;
		offset = start[shent+16] | start[shent+17]<<8 | start[shent+18]<<16 | start[shent+19]<<24;
		size = start[shent+20] | start[shent+21]<<8 | start[shent+22]<<16 | start[shent+23]<<24;

//		printf("Section %02d: name %08x [%s] type %08x flags %08x addr %08x offset %08x size %08x\n", i, name, &start[secname(start, shstrndx, shoff, shentsize, name)], type, flags, addr, offset, size);

		switch (type)
		{
			case 0:			// section table header - do nothing
				break;

			case 1:			// PROGBITS: copy data to destination
				memcpy(&cpu->psx_ram[(loadAddr + addr)/4], &start[offset], size);
				totallen += size;
				break;

			case 2:			// SYMTAB: ignore
				break;

			case 3:			// STRTAB: ignore
				break;

			case 8:			// NOBITS: BSS region, zero out destination
				memset(&cpu->psx_ram[(loadAddr + addr)/4], 0, size);
				totallen += size;
				break;

			case 9:			// REL: short relocation data
		  		for (rec = 0; rec < (size/8); rec++)
				{
					uint32 offs, info, target, temp, val, vallo;
					static uint32 hi16offs = 0, hi16target = 0;

					offs = start[offset+(rec*8)] | start[offset+1+(rec*8)]<<8 | start[offset+2+(rec*8)]<<16 | start[offset+3+(rec*8)]<<24;
					info = start[offset+4+(rec*8)] | start[offset+5+(rec*8)]<<8 | start[offset+6+(rec*8)]<<16 | start[offset+7+(rec*8)]<<24;
					target = LE32(cpu->psx_ram[(loadAddr+offs)/4]);
					
//					printf("[%04d] offs %08x type %02x info %08x => %08x\n", rec, offs, ELF32_R_TYPE(info), ELF32_R_SYM(info), target);

					switch (ELF32_R_TYPE(info))
					{
						case 2:	      	// R_MIPS_32
							target += loadAddr;
//							target |= 0x80000000;
							break;

						case 4:		// R_MIPS_26
							temp = (target & 0x03ffffff);
							target &= 0xfc000000;
							temp += (loadAddr>>2);
							target |= temp;
							break;

						case 5:		// R_MIPS_HI16
							hi16offs = offs;
							hi16target = target;
							break;

						case 6:		// R_MIPS_LO16
							vallo = ((target & 0xffff) ^ 0x8000) - 0x8000;

							val = ((hi16target & 0xffff) << 16) +	vallo;
							val += loadAddr;
//							val |= 0x80000000;

							/* Account for the sign extension that will happen in the low bits.  */
							val = ((val >> 16) + ((val & 0x8000) != 0)) & 0xffff;

							hi16target = (hi16target & ~0xffff) | val;

							/* Ok, we're done with the HI16 relocs.  Now deal with the LO16.  */
							val = loadAddr + vallo;
							target = (target & ~0xffff) | (val & 0xffff);

							cpu->psx_ram[(loadAddr+hi16offs)/4] = LE32(hi16target);
							break;

						default:
							printf("FATAL: Unknown MIPS ELF relocation!\n");
							return 0xffffffff;
							break;
					}

					cpu->psx_ram[(loadAddr+offs)/4] = LE32(target);
				}						
				break;

			case 0x70000080:	// .iopmod
				do_iopmod(start, offset);
				break;

			default:
				#if DEBUG_LOADER
				printf("Unhandled ELF section type %d\n", type);
				#endif
				break;
		}

		shent += shentsize;
	}	

	entry += loadAddr;
	entry |= 0x80000000;
	loadAddr += totallen;

	#if DEBUG_LOADER
	printf("psf2_load_elf: entry PC %08x\n", entry);
	#endif
	return entry;
}

static uint32 load_file_ex(uint8 *top, uint8 *start, uint32 len, char *file, uint8 *buf, uint32 buflen)
{
	int32 numfiles, i, j;
	uint8 *cptr;
	uint32 offs, uncomp, bsize, cofs, uofs;
	uint32 X;
	uLongf dlength;
	int uerr;
	char matchname[512], *remainder;

	// strip out to only the directory name
	i = 0;
	while ((file[i] != '/') && (file[i] != '\\') && (file[i] != '\0'))
	{
		matchname[i] = file[i];
		i++;
	}
	matchname[i] = '\0';
	remainder = &file[i+1];
	
	cptr = start + 4; 

	numfiles = start[0] | start[1]<<8 | start[2]<<16 | start[3]<<24;

	for (i = 0; i < numfiles; i++)
	{
		offs = cptr[36] | cptr[37]<<8 | cptr[38]<<16 | cptr[39]<<24;
		uncomp = cptr[40] | cptr[41]<<8 | cptr[42]<<16 | cptr[43]<<24;
		bsize = cptr[44] | cptr[45]<<8 | cptr[46]<<16 | cptr[47]<<24;

		#if DEBUG_LOADER
		printf("[%s vs %s]: ofs %08x uncomp %08x bsize %08x\n", cptr, matchname, offs, uncomp, bsize);
		#endif
		
		if (!strcasecmp((char *)cptr, matchname))
		{
			if ((uncomp == 0) && (bsize == 0))
			{
				#if DEBUG_LOADER
				printf("Drilling into subdirectory [%s] with [%s] at offset %x\n", matchname, remainder, offs);
				#endif
				return load_file_ex(top, &top[offs], len-offs, remainder, buf, buflen);
			}

			X = (uncomp + bsize - 1) / bsize;
			
			cofs = offs + (X*4);
			uofs = 0;
			for (j = 0; j < X; j++)
			{
				uint32 usize;

				usize = top[offs+(j*4)] | top[offs+1+(j*4)]<<8 | top[offs+2+(j*4)]<<16 | top[offs+3+(j*4)]<<24;

				dlength = buflen - uofs;
		
				uerr = uncompress(&buf[uofs], &dlength, &top[cofs], usize);
				if (uerr != Z_OK)
				{
					printf("Decompress fail: %x %d!\n", (unsigned int)dlength, uerr);
					return 0xffffffff;
				}

				cofs += usize;
				uofs += dlength;
			}

			return uncomp;
		}
		else
		{
			cptr += 48;
		}
	}

	return 0xffffffff;
}

static uint32 load_file(int fs, char *file, uint8 *buf, uint32 buflen)
{
	return load_file_ex(filesys[fs], filesys[fs], fssize[fs], file, buf, buflen);
}

#if 0
static dump_files(int fs, uint8 *buf, uint32 buflen)
{
	int32 numfiles, i, j;
	uint8 *cptr;
	uint32 offs, uncomp, bsize, cofs, uofs;
	uint32 X;
	uLongf dlength;
	int uerr;
	uint8 *start;
	uint32 len;
	FILE *f;
	char tfn[128];

	printf("Dumping FS %d\n", fs);
		
	start = s->filesys[fs];
	len = s->fssize[fs];

	cptr = start + 4; 

	numfiles = start[0] | start[1]<<8 | start[2]<<16 | start[3]<<24;

	for (i = 0; i < numfiles; i++)
	{
		offs = cptr[36] | cptr[37]<<8 | cptr[38]<<16 | cptr[39]<<24;
		uncomp = cptr[40] | cptr[41]<<8 | cptr[42]<<16 | cptr[43]<<24;
		bsize = cptr[44] | cptr[45]<<8 | cptr[46]<<16 | cptr[47]<<24;

		if (bsize > 0)		
		{
			X = (uncomp + bsize - 1) / bsize;

			printf("[dump %s]: ofs %08x uncomp %08x bsize %08x\n", cptr, offs, uncomp, bsize);

			cofs = offs + (X*4);
			uofs = 0;
			for (j = 0; j < X; j++)
			{
				uint32 usize;

				usize = start[offs+(j*4)] | start[offs+1+(j*4)]<<8 | start[offs+2+(j*4)]<<16 | start[offs+3+(j*4)]<<24;

				dlength = buflen - uofs;
		
				uerr = uncompress(&buf[uofs], &dlength, &start[cofs], usize);
				if (uerr != Z_OK)
				{
					printf("Decompress fail: %x %d!\n", dlength, uerr);
					return 0xffffffff;
				}

				cofs += usize;
				uofs += dlength;
			}

			sprintf(tfn, "iopfiles/%s", cptr);
			f = fopen(tfn, "wb");
			fwrite(buf, uncomp, 1, f);
			fclose(f);
		}
		else
		{
			printf("[subdir %s]: ofs %08x uncomp %08x bsize %08x\n", cptr, offs, uncomp, bsize);
		}

		cptr += 48;
	}

	return 0xffffffff;
}
#endif

// find a file on our filesystems
uint32 psf2_load_file(mips_cpu_context *cpu, char *file, uint8 *buf, uint32 buflen)
{
	int i;
	uint32 flen;

	for (i = 0; i < num_fs; i++)
	{
		flen = load_file(i, file, buf, buflen);
		if (flen != 0xffffffff)
		{
			return flen;
		}
	}

	return 0xffffffff;
}

void *psf2_start(const char *path, uint8 *buffer, uint32 length)
{
    psf2_synth_t *s = malloc (sizeof (psf2_synth_t));
    memset (s, 0, sizeof (psf2_synth_t));

	uint8 *file = NULL, *lib_decoded;
	uint32 irx_len;
	uint64 file_len, lib_raw_length, lib_len;
	uint8 *buf;
	union cpuinfo mipsinfo;
	corlett_t *lib = NULL;

	loadAddr = 0x23f00;	// this value makes allocations work out similarly to how they would 
				// in Highly Experimental (as per Shadow Hearts' hard-coded assumptions)

	// Decode the current PSF2
	if (corlett_decode(buffer, length, &file, &file_len, &s->c) != AO_SUCCESS)
	{
        free (s);
		return NULL;
	}
	if (file) {
        free (file);
        file = NULL;
    }

	if (file_len > 0) printf("ERROR: PSF2 can't have a program section!  ps %08x\n", (unsigned int)file_len);

	#if DEBUG_LOADER
	printf("FS section: size %x\n", s->c->res_size);
	#endif

	num_fs = 1;
	filesys[0] = (uint8 *)s->c->res_section;
	fssize[0] = s->c->res_size;

	// Get the library file, if any
	if (s->c->lib[0] != 0)
	{
		uint64 tmp_length;
        char libpath[PATH_MAX];
        ao_getlibpath (path, s->c->lib, libpath, sizeof (libpath));
	
		#if DEBUG_LOADER	
		printf("Loading library: %s\n", s->c->lib);
		#endif
		if (ao_get_lib(libpath, &s->lib_raw_file, &tmp_length) != AO_SUCCESS)
		{
            free (s);
            return NULL;
		}
		lib_raw_length = tmp_length;

		if (corlett_decode(s->lib_raw_file, lib_raw_length, &lib_decoded, &lib_len, &lib) != AO_SUCCESS)
		{
			free(s->lib_raw_file);
            free (s);
            return NULL;
		}

		#if DEBUG_LOADER
		printf("Lib FS section: size %x bytes\n", lib->res_size);
		#endif

		num_fs++;
		filesys[1] = (uint8 *)lib->res_section;
 		fssize[1] = lib->res_size;
 		free (lib);
 		lib = NULL;
	}

	// dump all files
	#if 0
	buf = (uint8 *)malloc(16*1024*1024);
	dump_files(0, buf, 16*1024*1024);
	if (s->c->lib[0] != 0)
		dump_files(1, buf, 16*1024*1024);
	free(buf);
	#endif

	s->mips_cpu = mips_alloc ();
	mips_init(s->mips_cpu);
	mips_reset(s->mips_cpu, NULL);

	// load psf2.irx, which kicks everything off
	buf = (uint8 *)malloc(512*1024);
	irx_len = psf2_load_file(s->mips_cpu, "psf2.irx", buf, 512*1024);

	if (irx_len != 0xffffffff)
	{
		s->initialPC = psf2_load_elf(s->mips_cpu, buf, irx_len);
		s->initialSP = 0x801ffff0;
	}
	free(buf);

	if (s->initialPC == 0xffffffff)
	{
        free (s);
        return NULL;
	}

	lengthMS = psfTimeToMS(s->c->inf_length);
	fadeMS = psfTimeToMS(s->c->inf_fade);
	if (lengthMS == 0) 
	{
		lengthMS = ~0;
	}

	mipsinfo.i = s->initialPC;
	mips_set_info(s->mips_cpu, CPUINFO_INT_PC, &mipsinfo);

	mipsinfo.i = s->initialSP;
	mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R29, &mipsinfo);
	mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R30, &mipsinfo);

	// set RA
	mipsinfo.i = 0x80000000;
	mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R31, &mipsinfo);

	// set A0 & A1 to point to "aofile:/"
	mipsinfo.i = 2;	// argc
	mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R4, &mipsinfo);

	mipsinfo.i = 0x80000004;	// argv
	mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R5, &mipsinfo);
	s->mips_cpu->psx_ram[1] = LE32(0x80000008);
	
	buf = (uint8 *)(&s->mips_cpu->psx_ram[2]);
	strcpy((char *)buf, "aofile:/");		

	s->mips_cpu->psx_ram[0] = LE32(FUNCT_HLECALL);

	// back up initial RAM image to quickly restart songs
	memcpy(s->mips_cpu->initial_ram, s->mips_cpu->psx_ram, 2*1024*1024);

	psx_hw_init(s->mips_cpu);
	SPU2init(s->mips_cpu, ps2_update, s);
	SPU2open(s->mips_cpu, NULL);
	setlength2(s->mips_cpu->spu2, lengthMS, fadeMS);

	return s;
}

int32 psf2_gen(void *handle, int16 *buffer, uint32 samples)
{	
	int i;
	psf2_synth_t *s = handle;

//	memset (buffer, 0, samples * 4);
//	return AO_SUCCESS;
//
	s->spu_pOutput = (char *)buffer;

	for (i = 0; i < samples; i++)
	{
		SPU2async(s->mips_cpu, 1);
		ps2_hw_slice(s->mips_cpu);
	}

	ps2_hw_frame(s->mips_cpu);
	
	return AO_SUCCESS;
}

int32 psf2_stop(void *handle)
{
    psf2_synth_t *s = handle;
	SPU2close(s->mips_cpu);
	SPU2free(s->mips_cpu);
	if (s->c->lib[0] != 0)
	{
		free(s->lib_raw_file);
	}
	free(s->c);
	if (s->mips_cpu) {
        mips_exit (s->mips_cpu);
    }
	free (s);

	return AO_SUCCESS;
}

int32 psf2_command(void *handle, int32 command, int32 parameter)
{
    psf2_synth_t *s = handle;
	union cpuinfo mipsinfo;
	uint32 lengthMS, fadeMS;

	switch (command)
	{
		case COMMAND_RESTART:
			SPU2close(s->mips_cpu);

			memcpy(s->mips_cpu->psx_ram, s->mips_cpu->initial_ram, 2*1024*1024);

			mips_init(s->mips_cpu);
			mips_reset(s->mips_cpu, NULL);
			psx_hw_init(s->mips_cpu);
			SPU2init(s->mips_cpu, ps2_update, s);
			SPU2open(s->mips_cpu, NULL);

			mipsinfo.i = s->initialPC;
			mips_set_info(s->mips_cpu, CPUINFO_INT_PC, &mipsinfo);

			mipsinfo.i = s->initialSP;
			mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R29, &mipsinfo);
			mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R30, &mipsinfo);

			// set RA
			mipsinfo.i = 0x80000000;
			mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R31, &mipsinfo);

			// set A0 & A1 to point to "aofile:/"
			mipsinfo.i = 2;	// argc
			mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R4, &mipsinfo);

			mipsinfo.i = 0x80000004;	// argv
			mips_set_info(s->mips_cpu, CPUINFO_INT_REGISTER + MIPS_R5, &mipsinfo);

			psx_hw_init(s->mips_cpu);

			lengthMS = psfTimeToMS(s->c->inf_length);
			fadeMS = psfTimeToMS(s->c->inf_fade);
			if (lengthMS == 0) 
			{
				lengthMS = ~0;
			}
			setlength2(s->mips_cpu->spu2, lengthMS, fadeMS);

			return AO_SUCCESS;
		
	}
	return AO_FAIL;
}

int32 psf2_fill_info(void *handle, ao_display_info *info)
{
    psf2_synth_t *s = handle;
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

uint32 psf2_get_loadaddr(void)
{
	return loadAddr;
}

void psf2_set_loadaddr(uint32 new)
{
	loadAddr = new;
}
