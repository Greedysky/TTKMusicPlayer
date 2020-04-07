/***************************************************************************
 *   Copyright (C) 2008 by Deryabin Andrew                                 *
 *   andrew@it-optima.ru                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "ayfly.h"

inline Z80EX_BYTE readMemory(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, int m1_state, void *user_data)
{
    unsigned char *mem = ((AYSongInfo *)user_data)->module;
    return mem[addr];
}

inline void writeMemory(Z80EX_CONTEXT *cpu, Z80EX_WORD addr, Z80EX_BYTE value, void *user_data)
{
    unsigned char *mem = ((AYSongInfo *)user_data)->module;
    mem[addr] = value;

}
inline Z80EX_BYTE readPort(Z80EX_CONTEXT *cpu, Z80EX_WORD port, void *user_data)
{
    /*AYSongInfo *info = (AYSongInfo *)user_data;
     if((port == 0xfffd) || ((port & 0xc000) == 0xc000)) //ay control port
     {
     if(info->ay_reg == 16)
     {
     if(info->chip_type == 0)
     return 0x10;
     else
     return 0xff;
     }
     else
     return info->ay8910[0].ayRead(info->ay_reg);
     }
     unsigned char *io = ((AYSongInfo *)user_data)->z80IO;
     return io[port];*/
    return 0xff;
}

inline void writePort(Z80EX_CONTEXT *cpu, Z80EX_WORD port, Z80EX_BYTE value, void *user_data)
{

    AYSongInfo *info = (AYSongInfo *)user_data;
    //unsigned char *io = info->z80IO;
    /*if((port & 0xfe) == 0xfe)
    {
        info->ay8910 [0].ayBeeper(value & 0x10);
    }
    
    if((port == 0xfffd) || ((port & 0xc000) == 0xc000)) //ay control port
    {
        info->ay_reg = value;
    }
    else if((port == 0xbffd) || ((port & 0xc000) == 0x8000)) // ay data port
    {
        info->ay8910[0].ayWrite(info->ay_reg, value);
        //io[65533] = value;
    }*/
    unsigned char l = port & 0xff;
    unsigned char h = (port >> 8) & 0xff; 
    
    switch(l)
    {
        case 0xfd:
            switch(h)
            {
                case 0xff:
                    write_reg: info->ay_reg = value;
                    break;
                case 0xbf:
                    write_dat: info->ay8910[0].ayWrite(info->ay_reg, value);
                    break;
                default:
                    /* ok, since we do at least have low byte=FDh,
                     * do bitmask for top byte to allow for
                     * crappy .ay conversions. But don't disable
                     * CPC autodetect, just in case.
                     */
                    if((h & 0xc0) == 0xc0)
                        goto write_reg;
                    if((h & 0xc0) == 0x80)
                        goto write_dat;
            }
            break;

        case 0xfe:
            info->ay8910[0].ayBeeper(value & 0x10);
            break;
    }

}

inline Z80EX_BYTE readInt(Z80EX_CONTEXT *cpu, void *user_data)
{
    return 0xff;
}

bool ay_sys_initz80(AYSongInfo &info)
{
    if(info.z80ctx)
    {
        z80ex_destroy(info.z80ctx);
        info.z80ctx = 0;
    }
    info.z80ctx = z80ex_create(readMemory, &info, writeMemory, &info, readPort, &info, writePort, &info, readInt, 0);
    if(!info.z80ctx)
        return false;
    z80ex_reset(info.z80ctx);
    info.ay_reg = 0xff;
    return true;
}

void ay_sys_resetz80(AYSongInfo &info)
{
    z80ex_reset(info.z80ctx);
}

void ay_sys_shutdownz80(AYSongInfo &info)
{
    if(info.z80ctx)
    {
        ay_sys_resetz80(info);
        z80ex_destroy(info.z80ctx);
        info.z80ctx = 0;
    }
    if(info.z80IO)
        memset(info.z80IO, 0, 65536);
}
