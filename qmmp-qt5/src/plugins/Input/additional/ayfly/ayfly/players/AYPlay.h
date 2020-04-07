/*
 * parts of ay read code and memory init are from aylet player:
 * Copyright (C) 2001-2005 Russell Marks and Ian Collier.
 */

static const unsigned char intz[] =
{ 0xf3, /* di */
0xcd, 0, 0, /* call init */
0xed, 0x5e, /* loop: im 2 */
0xfb, /* ei */
0x76, /* halt */
0x18, 0xfa /* jr loop */
};

static const unsigned char intnz[] =
{ 0xf3, /* di */
0xcd, 0, 0, /* call init */
0xed, 0x56, /* loop: im 1 */
0xfb, /* ei */
0x76, /* halt */
0xcd, 0, 0, /* call interrupt */
0x18, 0xf7 /* jr loop */
};

//for .ay format
struct ayTrack
{
    unsigned char *name;
    unsigned char *data;
    unsigned char *data_points, *data_memblocks;
    unsigned long fadestart, fadelen;
};

struct ayData
{
    unsigned char *filedata;
    unsigned long filelen;
    struct ayTrack *tracks;

    unsigned long filever, playerver;
    unsigned char *author, *misc;
    unsigned long num_tracks;
    unsigned long first_track;
};

void AY_initayfmt(AYSongInfo &info, ayData &aydata, unsigned char track)
{
    unsigned long init, ay_1st_block, ourinit, interrupt;
    unsigned char *ptr;
    unsigned long addr, len, ofs;

#undef GET_WORD
#define GET_WORD(x) (((*(x))<<8)|(*(x+1)))

    memset(info.module, 0, info.module_len);

    init = GET_WORD(aydata.tracks[track].data_points+2);
    interrupt = GET_WORD(aydata.tracks[track].data_points+4);
    ay_1st_block = GET_WORD(aydata.tracks[track].data_memblocks);

#ifndef __SYMBIAN32__
	srand(time(0));
    for(unsigned long i = 0x100; i < 0x4000; i++)
    {
        info.module [i] = rand() % 256;
    }
#else
    memset(info.module + 0x0100, 0xff, 0x3f00);
#endif
    memset(info.module + 0x0000, 0xc9, 0x0100);
    memset(info.module + 0x4000, 0x00, 0xc000);
    info.module[0x38] = 0xfb; /* ei */

    /* call first AY block if no init */
    ourinit = (init ? init : ay_1st_block);

    if(!interrupt)
        memcpy(info.module, intz, sizeof(intz));
    else
    {
        memcpy(info.module, intnz, sizeof(intnz));
        info.module[9] = interrupt % 256;
        info.module[10] = interrupt / 256;
    }

    info.module[2] = ourinit % 256;
    info.module[3] = ourinit / 256;

    /* now put the memory blocks in place */
    ptr = aydata.tracks[track].data_memblocks;
    while((addr = GET_WORD(ptr)) != 0)
    {
        len = GET_WORD(ptr + 2);
        ofs = GET_WORD(ptr + 4);
        if(ofs >= 0x8000)
            ofs = -0x10000 + ofs;

        /* range check */
        if(ptr - 4 - aydata.filedata + ofs >= aydata.filelen || ptr - 4 - aydata.filedata + (long)ofs < 0)
        {
            ptr += 6;
            continue;
        }

        /* fix any broken length */
        if(ptr + 4 + ofs + len >= aydata.filedata + aydata.filelen)
            len = aydata.filedata + aydata.filelen - (ptr + 4 + ofs);
        if(addr + len > 0x10000)
            len = 0x10000 - addr;

        memcpy(info.module + addr, ptr + 4 + ofs, len);
        ptr += 6;
    }

    z80ex_set_reg(info.z80ctx, regSP, aydata.tracks[track].data_points[0] * 256 + aydata.tracks[track].data_points[1]);
    z80ex_set_reg(info.z80ctx, regI, 0);
    Z80EX_WORD reg = aydata.tracks[track].data[8] * 256 + aydata.tracks[track].data[9];
    z80ex_set_reg(info.z80ctx, regAF, reg);
    z80ex_set_reg(info.z80ctx, regAF_, reg);
    z80ex_set_reg(info.z80ctx, regBC, reg);
    z80ex_set_reg(info.z80ctx, regBC_, reg);
    z80ex_set_reg(info.z80ctx, regDE, reg);
    z80ex_set_reg(info.z80ctx, regDE_, reg);
    z80ex_set_reg(info.z80ctx, regHL, reg);
    z80ex_set_reg(info.z80ctx, regHL_, reg);
    z80ex_set_reg(info.z80ctx, regIX, reg);
    z80ex_set_reg(info.z80ctx, regIY, reg);

}

void AY_Init(AYSongInfo &info)
{
#undef GET_WORD
#define GET_WORD(x) {(x) = (*ptr++) << 8; (x) |= *ptr++;}
#define GET_PTR(x) {unsigned long tmp; GET_WORD(tmp); if(tmp >= 0x8000) tmp=-0x10000+tmp; (x)=ptr-2+tmp;}
    ayData aydata;
    unsigned char *ptr = info.file_data;
    unsigned char *ptr2;
    if(!ay_sys_initz80(info))
        return;
    if(*ptr == 'Z' && *(ptr + 1) == 'X' && *(ptr + 2) == 'A' && *(ptr + 3) == 'Y' && *(ptr + 4) == 'E' && *(ptr + 5) == 'M' && *(ptr + 6) == 'U' && *(ptr + 7) == 'L')
    {
        ptr += 8;
        aydata.filever = *ptr++;
        aydata.playerver = *ptr++;
        ptr += 2;
        GET_PTR(aydata.author);
        GET_PTR(aydata.misc);
        aydata.num_tracks = 1 + *ptr++;
        aydata.first_track = *ptr++;
        GET_PTR(ptr2);
        ptr = ptr2;
        if((aydata.tracks = new ayTrack[aydata.num_tracks]))
        {
            for(unsigned long i = 0; i < aydata.num_tracks; i++)
            {
                GET_PTR(aydata.tracks[i].name);
                GET_PTR(aydata.tracks[i].data);
            }
            for(unsigned long i = 0; i < aydata.num_tracks; i++)
            {
                ptr = aydata.tracks[i].data + 10;
                GET_PTR(aydata.tracks[i].data_points);
                GET_PTR(aydata.tracks[i].data_memblocks);

                ptr = aydata.tracks[i].data + 4;
                GET_WORD(aydata.tracks[i].fadestart);
                GET_WORD(aydata.tracks[i].fadelen);
            }
            aydata.filelen = info.file_len;
            aydata.filedata = info.file_data;
            AY_initayfmt(info, aydata, 0);
            delete[] aydata.tracks;
            aydata.tracks = 0;
        }
    }
    ay_resetay(&info);
}

void AY_Play(AYSongInfo &info)
{
    do
    {
        z80ex_step(info.z80ctx);
    }
    while(z80ex_get_reg(info.z80ctx, regPC) != 4);
}

void AY_GetInfo(AYSongInfo &info)
{
#undef GET_WORD
#define GET_WORD(x) {(x) = (*ptr++) << 8; (x) |= *ptr++;}
#define GET_PTR(x) {unsigned long tmp; GET_WORD(tmp); if(tmp >= 0x8000) tmp=-0x10000+tmp; (x)=ptr-2+tmp;}
    ayData aydata_loc;
    unsigned char *ptr = info.file_data;
    unsigned char *ptr2;
    if(*ptr == 'Z' && *(ptr + 1) == 'X' && *(ptr + 2) == 'A' && *(ptr + 3) == 'Y' && *(ptr + 4) == 'E' && *(ptr + 5) == 'M' && *(ptr + 6) == 'U' && *(ptr + 7) == 'L')
    {
        ptr += 8;
        aydata_loc.filever = *ptr++;
        aydata_loc.playerver = *ptr++;
        ptr += 2;
        GET_PTR(aydata_loc.author);
        GET_PTR(aydata_loc.misc);
        aydata_loc.num_tracks = 1 + *ptr++;
        aydata_loc.first_track = *ptr++;
        GET_PTR(ptr2);
        ptr = ptr2;
        if((aydata_loc.tracks = new ayTrack[aydata_loc.num_tracks]))
        {
            for(unsigned long i = 0; i < aydata_loc.num_tracks; i++)
            {
                GET_PTR(aydata_loc.tracks [i].name);
                GET_PTR(aydata_loc.tracks [i].data);
            }
            for(unsigned long i = 0; i < aydata_loc.num_tracks; i++)
            {
                ptr = aydata_loc.tracks[i].data + 10;
                GET_PTR(aydata_loc.tracks [i].data_points);
                GET_PTR(aydata_loc.tracks [i].data_memblocks);

                ptr = aydata_loc.tracks[i].data + 4;
                GET_WORD(aydata_loc.tracks [i].fadestart);
                GET_WORD(aydata_loc.tracks [i].fadelen);
            }
            if(aydata_loc.num_tracks)
            {
                info.Length = aydata_loc.tracks[0].fadestart;
                if(!info.Length)
                    info.Length = 3000 * 3;
                info.Name = ay_sys_getstr(aydata_loc.tracks[0].name, strlen((char *)aydata_loc.tracks[0].name));
                info.Author = ay_sys_getstr(aydata_loc.author, strlen((char *)aydata_loc.author));
            }

            delete[] aydata_loc.tracks;
        }

    }
}

void AY_Cleanup(AYSongInfo &info)
{
    ay_sys_shutdownz80(info);
}

bool AY_Detect(unsigned char *module, unsigned long length)
{
    unsigned char *ptr = module;
    if(*ptr == 'Z' && *(ptr + 1) == 'X' && *(ptr + 2) == 'A' && *(ptr + 3) == 'Y' && *(ptr + 4) == 'E' && *(ptr + 5) == 'M' && *(ptr + 6) == 'U' && *(ptr + 7) == 'L')
        return true;
    return false;
}
