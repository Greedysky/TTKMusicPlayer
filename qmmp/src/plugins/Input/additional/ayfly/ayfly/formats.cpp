/***************************************************************************
 *   Copyright (C) 2008 by Deryabin Andrew   				               *
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

unsigned short ay_sys_getword(unsigned char *p)
{
    return *p | ((*(p + 1)) << 8);
}

unsigned long ay_sys_getdword(unsigned char *p)
{
    return *p | ((*(p + 1)) << 8) | ((*(p + 2)) << 16) | ((*(p + 3)) << 24);
}

void ay_sys_writeword(unsigned char *p, unsigned short val)
{
    *p = (unsigned char)(val & 0xff);
    *(p + 1) = (unsigned char)((val >> 8) & 0xff);
}
#ifdef __SYMBIAN32__
TFileName ay_sys_getstr(const unsigned char *str, unsigned long length)
{
    CCnvCharacterSetConverter *aCharacterSetConverter = CCnvCharacterSetConverter::NewL();
    RFs aFileServerSession;
    TUint aForeignCharacterSet = KCharacterSetIdentifierAscii;
    // check to see if the character set is supported - if not then leave
    if (aCharacterSetConverter->PrepareToConvertToOrFromL(aForeignCharacterSet,
                    aFileServerSession) != CCnvCharacterSetConverter::EAvailable)
    User::Leave(KErrNotSupported);
    // Create a small output buffer 
    TBuf16<20> outputBuffer;
    TFileName endBuffer;
    // Create a buffer for the unconverted text - initialised with the input descriptor
    TPtrC8 remainderOfForeignText(str, length);
    // Create a "state" variable and initialise it with CCnvCharacterSetConverter::KStateDefault
    // After initialisation the state variable must not be tampered with.
    // Simply pass into each subsequent call of ConvertToUnicode()
    TInt state=CCnvCharacterSetConverter::KStateDefault;
    for(;;) // conversion loop

    {
        // Start conversion. When the output buffer is full, return the number
        // of characters that were not converted
        const TInt returnValue=aCharacterSetConverter->ConvertToUnicode(outputBuffer, remainderOfForeignText, state);
        // check to see that the descriptor isn�t corrupt - leave if it is
        if (returnValue==CCnvCharacterSetConverter::EErrorIllFormedInput)
        User::Leave(KErrCorrupt);
        else if (returnValue<0) // future-proof against "TError" expanding
        User::Leave(KErrGeneral);

        // Do something here to store the contents of the output buffer.
        // Finish conversion if there are no unconverted 
        // characters in the remainder buffer
        endBuffer += outputBuffer;
        if (returnValue==0)
        break;
        
        // Remove converted source text from the remainder buffer.
        // The remainder buffer is then fed back into loop
        remainderOfForeignText.Set(remainderOfForeignText.Right(returnValue));
    }
    delete aCharacterSetConverter;
    return endBuffer;
}
    
#else
CayflyString ay_sys_getstr(const unsigned char *str, unsigned long length)
{
    char *str_new = new char [length + 1];
    if(!str_new)
        return CayflyString(TXT(""));
    memset(str_new, 0, length + 1);
    memcpy(str_new, str, length);        
    CayflyString cstr(str_new);
    delete[] str_new;
    return cstr;
}
#endif

void ay_sys_initz80module(AYSongInfo &info, unsigned long player_base, const unsigned char *player_ptr, unsigned long player_length, unsigned long player_init_proc, unsigned long player_play_proc);

#include "players/AYPlay.h"
#include "players/ASCPlay.h"
#include "players/PT1Play.h"
#include "players/PT2Play.h"
#include "players/PT3Play.h"
#include "players/STCPlay.h"
#include "players/STPPlay.h"
#include "players/PSCPlay.h"
#include "players/SQTPlay.h"
#include "players/PSGPlay.h"
#include "players/VTXPlay.h"
#include "players/YMPlay.h"

typedef void (*GETINFO_CALLBACK)(AYSongInfo &info);

struct _Players
{
#ifndef __SYMBIAN32__
    AY_TXT_TYPE ext;
#else
    const TText *ext;
#endif
    PLAYER_INIT_PROC soft_init_proc;
    PLAYER_PLAY_PROC soft_play_proc;
    PLAYER_CLEANUP_PROC soft_cleanup_proc;
    GETINFO_CALLBACK getInfo;
    PLAYER_DETECT_PROC detect;
    bool is_z80;
};

static const _Players Players[] =
{
{ TXT(".ay"), AY_Init, AY_Play, AY_Cleanup, AY_GetInfo, AY_Detect, true },
{ TXT(".vtx"), VTX_Init, VTX_Play, VTX_Cleanup, VTX_GetInfo, VTX_Detect, false },
{ TXT(".ym"), YM_Init, YM6_Play, YM_Cleanup, YM_GetInfo, YM_Detect, false },
{ TXT(".psg"), PSG_Init, PSG_Play, PSG_Cleanup, PSG_GetInfo, PSG_Detect, false },
{ TXT(".asc"), ASC_Init, ASC_Play, ASC_Cleanup, ASC_GetInfo, ASC_Detect, false },
{ TXT(".pt2"), PT2_Init, PT2_Play, PT2_Cleanup, PT2_GetInfo, PT2_Detect, false },
{ TXT(".pt3"), PT3_Init, PT3_Play, PT3_Cleanup, PT3_GetInfo, PT3_Detect, false },
{ TXT(".stc"), STC_Init, STC_Play, STC_Cleanup, STC_GetInfo, STC_Detect, false },
{ TXT(".stp"), STP_Init, STP_Play, STP_Cleanup, STP_GetInfo, STP_Detect, false },
{ TXT(".psc"), PSC_Init, PSC_Play, PSC_Cleanup, PSC_GetInfo, PSC_Detect, false },
{ TXT(".sqt"), SQT_Init, SQT_Play, SQT_Cleanup, SQT_GetInfo, SQT_Detect, false },
{ TXT(".pt1"), PT1_Init, PT1_Play, PT1_Cleanup, PT1_GetInfo, PT1_Detect, false } };

#ifndef __SYMBIAN32__
bool ay_sys_format_supported(AY_TXT_TYPE filePath)
#else
bool ay_sys_format_supported(const TFileName filePath)
#endif
{
#ifndef __SYMBIAN32__
    AY_TXT_TYPE cfp = filePath;
    cfp.toLower();
    for(unsigned long player = 0; player < sizeof_array(Players); player++)
    {
        if(cfp.rcompare(Players[player].ext) == 0)
        {
            return true;
        }
    }
#else
    TFileName cfp = filePath;
    cfp.LowerCase();
    TParse parse;
    parse.Set(cfp, NULL, NULL);
    for (unsigned long player = 0; player < sizeof_array(Players); player++)
    {
        TPtrC ext = parse.Ext();
        TPtrC ext_cur = Players [player].ext;
        if (ext.Compare(ext_cur) == 0)
        {
            return true;
        }
    }
#endif
    return false;
}

#ifndef __SYMBIAN32__
unsigned char *osRead(AY_TXT_TYPE filePath, unsigned long *data_len)
#else
unsigned char *osRead(const TFileName filePath, unsigned long *data_len)
#endif
{
    unsigned char *fileData = 0;
#ifndef __SYMBIAN32__
    FILE *f;
#if !defined(WINDOWS) && defined(AYFLY_UNICODE)
    size_t len = filePath.length() * 6;
    char *mb_str = new char[len + 1];
    if(!mb_str)
    {
        *data_len = 0;
        delete[] fileData;
        return 0;
    }
    mbstate_t mbstate;
    ::memset((void*)&mbstate, 0, sizeof(mbstate));
    const wchar_t *wc_str = filePath.c_str();
    size_t cnt_conv = wcsrtombs(mb_str, &wc_str, len, &mbstate);
    mb_str[cnt_conv] = 0;
    f = fopen(mb_str, "rb");
    delete[] mb_str;
#else
#if defined(WINDOWS) && defined(AYFLY_UNICODE)
    f = _wfopen(filePath.c_str(), TXT("rb"));
#else
    f = fopen(filePath.c_str(), "rb");
#endif
#endif
    if(f)
    {
        fseek(f, 0, SEEK_END);
        *data_len = ftell(f);
        fseek(f, 0, SEEK_SET);

        unsigned long to_allocate = *data_len < 65536 ? 65536 : *data_len;
        fileData = new unsigned char[to_allocate];
        if(!fileData)
        {
            fclose(f);
            return 0;
        }
        memset(fileData, 0, to_allocate);
        fread((char *)fileData, 1, *data_len, f);
        if(ferror(f))
            *data_len = 0;
        fclose(f);
    }
    else
        *data_len = 0;

#else
    RFs fsSession;// = CCoeEnv::Static()->FsSession();
    fsSession.Connect();
    RFileReadStream readStream;
    TEntry entry;
    TInt err = readStream.Open(fsSession, filePath, EFileRead);
    if (err == KErrNone)
    {
        fsSession.Entry(filePath, entry);
        *data_len = (TUint)entry.iSize;
        unsigned long to_allocate = *data_len < 65536 ? 65536 : *data_len;
        fileData = new unsigned char[to_allocate];
        if(!fileData)
        {
            readStream.Close();
            return 0;
        }
        memset(fileData, 0, to_allocate);
        readStream.ReadL((TUint8 *)fileData, *data_len);
        readStream.Close();
    }
    else
    {
        *data_len = 0;
    }

#endif
    if(!*data_len)
    {
        if(fileData)
            delete[] fileData;
        fileData = 0;
    }
    return fileData;
}

long ay_sys_detect(AYSongInfo &info)
{
    long player = -1;

	// Avoid parsing the Picatune2 (PT2) XML format
	if(info.file_data[0] == '<')
		return -1;

    unsigned char *tmp_module = new unsigned char[info.file_len];
    if(!tmp_module)
        return -1;
    memcpy(tmp_module, info.file_data, info.file_len);
    for(player = 0; player < sizeof_array(Players); player++)
    {
        if(Players[player].detect != 0)
        {
            if(Players[player].detect(tmp_module, info.file_len))
                break;
        }
    }
    delete[] tmp_module;

    if(player >= sizeof_array(Players))
    {
#ifndef __SYMBIAN32__
        AY_TXT_TYPE cfp = info.FilePath;
        cfp.toLower();
        for(player = 0; player < sizeof_array(Players); player++)
        {
            if(cfp.rcompare(Players[player].ext) == 0)
            {
                break;
            }
        }
#else
        TFileName cfp = info.FilePath;
        cfp.LowerCase();
        TParse parse;
        parse.Set(cfp, NULL, NULL);
        for (player = 0; player < sizeof_array(Players); player++)
        {
            TPtrC ext = parse.Ext();
            TPtrC ext_cur = Players [player].ext;
            if (ext.Compare(ext_cur) == 0)
            {
                break;
            }
        }
#endif
    }
    if(player >= sizeof_array(Players))
        player = -1;
    else
    {
        info.is_z80 = Players[player].is_z80;
        for(unsigned char i = 0; i < NUMBER_OF_AYS; i++)
        {
            info.ay8910[i].SetParameters(&info);
        }
    }
    return player;
}

bool ay_sys_initsong(AYSongInfo &info)
{
    info.player_num = ay_sys_detect(info);

    if(info.player_num < 0)
        return false;

    memset(info.module, 0, info.file_len);
    memcpy(info.module, info.file_data, info.file_len);
    info.init_proc = Players[info.player_num].soft_init_proc;
    info.play_proc = Players[info.player_num].soft_play_proc;
    info.cleanup_proc = Players[info.player_num].soft_cleanup_proc;
    return true;
}

void ay_sys_initz80module(AYSongInfo &info, unsigned long player_base, const unsigned char *player_ptr, unsigned long player_length, unsigned long player_init_proc, unsigned long player_play_proc)
{
    //fill z80 memory
    memset(info.module + 0x0000, 0xc9, 0x0100);
    memset(info.module + 0x0100, 0xff, 0x3f00);
    memset(info.module + 0x4000, 0x00, 0xc000);
    info.module[0x38] = 0xfb; /* ei */
    //copy player to 0xc000 of z80 memory
    memcpy(info.module + player_base, player_ptr, player_length);

    //copy module right after the player
    memcpy(info.module + player_base + player_length, info.file_data, info.file_len);
    //copy im1 loop to 0x0 of z80 memory
    memcpy(info.module, intnz, sizeof(intnz));

    info.module[2] = player_init_proc % 256;
    info.module[3] = player_init_proc / 256;
    info.module[9] = player_play_proc % 256;
    info.module[10] = player_play_proc / 256;
    z80ex_set_reg(info.z80ctx, regSP, 0xc000);
}

bool ay_sys_readfromfile(AYSongInfo &info)
{
    unsigned long data_len = 65536;
    info.timeElapsed = 0;
    info.Length = 0;
    info.init_proc = 0;
    info.play_proc = 0;
    if(info.file_data)
    {
        delete[] info.file_data;
        info.file_data = 0;
    }
    if(info.module)
    {
        delete[] info.module;
        info.module = 0;
    }
    info.file_data = osRead(info.FilePath, &data_len);
    if(!info.file_data)
        return false;

    info.file_len = data_len;
    info.module_len = data_len;

    unsigned long to_allocate = info.file_len < 65536 ? 65536 : info.file_len;
    info.module = new unsigned char[to_allocate];
    if(!info.module)
    {
        delete[] info.file_data;
        info.file_data = 0;
        return false;
    }
    memset(info.module, 0, to_allocate);

    return true;
}

bool ay_sys_getsonginfoindirect(AYSongInfo &info)
{
    info.Length = 0;
    info.Loop = 0;
    info.Name = TXT("");
    info.Author = TXT("");
    info.player_num = ay_sys_detect(info);
    if(info.player_num < 0)
        return false;
    if(Players[info.player_num].getInfo)
    {
        Players[info.player_num].getInfo(info);
        return true;
    }
    return false;
}
bool ay_sys_getsonginfo(AYSongInfo &info)
{
    if(!ay_sys_readfromfile(info))
        return false;
    return ay_sys_getsonginfoindirect(info);
}

void ay_sys_rewindsong(AYSongInfo &info, long new_position)
{
    bool started = false;
    if(info.player && info.player->Started())
    {
        info.player->Stop();
        started = true;
    }

    if(info.timeElapsed > new_position)
    {
        info.timeElapsed = 0;
        ay_resetsong(&info);
    }

    if(info.is_z80)
    {
        int z80_per_sample_counter = 0;
        int int_per_z80_counter = 0;
        float int_per_z80_f = (float)info.z80_freq / (float)info.int_freq;
        unsigned long int_per_z80 = int_per_z80_f;
        if((int_per_z80_f - int_per_z80) >= 0.5)
            int_per_z80++;
        while(info.timeElapsed != new_position)
        {
            int tstates = z80ex_step(info.z80ctx);
            z80_per_sample_counter += tstates;
            int_per_z80_counter += tstates;
            if(int_per_z80_counter > int_per_z80)
            {
                tstates = z80ex_int(info.z80ctx);
                z80_per_sample_counter += tstates;
                int_per_z80_counter = tstates;
                info.timeElapsed++;
            }
        }
    }
    else
    {
        while(info.timeElapsed != new_position)
        {
            info.play_proc(info);
            info.timeElapsed++;
        }
    }
    
    if(started)
        info.player->Start();
        
}

