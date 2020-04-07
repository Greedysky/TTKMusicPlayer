/* This player module was ported from:
 AY-3-8910/12 Emulator
 Version 3.0 for Windows 95
 Author Sergey Vladimirovich Bulba
 (c)1999-2004 S.V.Bulba
 */

struct PSC_File
{
    signed char PSC_MusicName[69];
    unsigned char PSC_UnknownPointer0, PSC_UnknownPointer1;
    unsigned char PSC_PatternsPointer0, PSC_PatternsPointer1;
    unsigned char PSC_Delay;
    unsigned char PSC_OrnamentsPointer0, PSC_OrnamentsPointer1;
    unsigned char PSC_SamplesPointers0[64];
};
#define PSC_UnknownPointer (header->PSC_UnknownPointer0 | (header->PSC_UnknownPointer1 << 8))
#define PSC_PatternsPointer (header->PSC_PatternsPointer0 | (header->PSC_PatternsPointer1 << 8))
#define PSC_OrnamentsPointer (header->PSC_OrnamentsPointer0 | (header->PSC_OrnamentsPointer1 << 8))
#define PSC_SamplesPointers(x) (header->PSC_SamplesPointers0 [(x) * 2] | ((header->PSC_SamplesPointers0 [((x) * 2) + 1]) << 8))

struct PSC_Channel_Parameters
{
    unsigned char num;
    unsigned short Address_In_Pattern, OrnamentPointer, SamplePointer, Ton;
    short Current_Ton_Sliding, Ton_Accumulator, Addition_To_Ton;
    signed char Initial_Volume, Note_Skip_Counter;
    unsigned char Note, Volume, Amplitude, Volume_Counter, Volume_Counter1, Volume_Counter_Init, Noise_Accumulator, Position_In_Sample, Loop_Sample_Position, Position_In_Ornament, Loop_Ornament_Position;
    bool Enabled, Ornament_Enabled, Envelope_Enabled, Gliss, Ton_Slide_Enabled, Break_Sample_Loop, Break_Ornament_Loop, Volume_Inc;
};

struct PSC_Parameters
{
    unsigned char Delay, DelayCounter, Lines_Counter, Noise_Base;
    unsigned short Positions_Pointer;
};

struct PSC_SongInfo
{
    PSC_Parameters PSC;
    PSC_Channel_Parameters PSC_A, PSC_B, PSC_C;
};

#define PSC_A ((PSC_SongInfo *)info.data)->PSC_A
#define PSC_B ((PSC_SongInfo *)info.data)->PSC_B
#define PSC_C ((PSC_SongInfo *)info.data)->PSC_C
#define PSC ((PSC_SongInfo *)info.data)->PSC

void PSC_Init(AYSongInfo &info)
{
    unsigned char *module = info.module;
    PSC_File *header = (PSC_File *)module;
    if(info.data)
    {
        delete (ASC_SongInfo *)info.data;
        info.data = 0;
    }
    info.data = (void *)new PSC_SongInfo;
    if(!info.data)
        return;

    PSC.DelayCounter = 1;
    PSC.Delay = header->PSC_Delay;
    PSC.Positions_Pointer = PSC_PatternsPointer;
    PSC.Lines_Counter = 1;
    PSC.Noise_Base = 0;

    PSC_A.num = 0;
    PSC_B.num = 1;
    PSC_C.num = 2;

    PSC_A.SamplePointer = PSC_SamplesPointers(0) + 0x4c;
    PSC_B.SamplePointer = PSC_A.SamplePointer;
    PSC_C.SamplePointer = PSC_A.SamplePointer;
    PSC_A.OrnamentPointer = ay_sys_getword(&module[PSC_OrnamentsPointer]) + PSC_OrnamentsPointer;
    PSC_B.OrnamentPointer = PSC_A.OrnamentPointer;
    PSC_C.OrnamentPointer = PSC_A.OrnamentPointer;

    PSC_A.Break_Ornament_Loop = false;
    PSC_A.Ornament_Enabled = false;
    PSC_A.Enabled = false;
    PSC_A.Break_Sample_Loop = false;
    PSC_A.Ton_Slide_Enabled = false;
    PSC_A.Note_Skip_Counter = 1;
    PSC_A.Ton = 0;

    PSC_B.Break_Ornament_Loop = false;
    PSC_B.Ornament_Enabled = false;
    PSC_B.Enabled = false;
    PSC_B.Break_Sample_Loop = false;
    PSC_B.Ton_Slide_Enabled = false;
    PSC_B.Note_Skip_Counter = 1;
    PSC_B.Ton = 0;

    PSC_C.Break_Ornament_Loop = false;
    PSC_C.Ornament_Enabled = false;
    PSC_C.Enabled = false;
    PSC_C.Break_Sample_Loop = false;
    PSC_C.Ton_Slide_Enabled = false;
    PSC_C.Note_Skip_Counter = 1;
    PSC_C.Ton = 0;
    ay_resetay(&info, 0);
}

void PSC_PatternInterpreter(AYSongInfo &info, PSC_Channel_Parameters &chan)
{
    unsigned char *module = info.module;
    PSC_File *header = (PSC_File *)module;
    bool quit;
    bool b1b, b2b, b3b, b4b, b5b, b6b, b7b;
    quit = b1b = b2b = b3b = b4b = b5b = b6b = b7b = false;
    do
    {
        unsigned char val = module[chan.Address_In_Pattern];
        if(val >= 0xc0)
        {
            chan.Note_Skip_Counter = val - 0xbf;
            quit = true;
        }
        else if(val >= 0xa0 && val <= 0xbf)
        {
            //chan.OrnamentPointer = (*(unsigned short *) &module[PSC_OrnamentsPointer + (val - 0xa0) * 2]) +PSC_OrnamentsPointer;
            chan.OrnamentPointer = ay_sys_getword(&module[PSC_OrnamentsPointer + (val - 0xa0) * 2]) + PSC_OrnamentsPointer;
        }
        else if(val >= 0x7e && val <= 0x9f)
        {
            if(val >= 0x80)
                chan.SamplePointer = PSC_SamplesPointers(val - 0x80) + 0x4c;
        }
        else if(val == 0x6b)
        {
            chan.Address_In_Pattern++;
            chan.Addition_To_Ton = module[chan.Address_In_Pattern];
            b5b = true;
        }
        else if(val == 0x6c)
        {
            chan.Address_In_Pattern++;
            chan.Addition_To_Ton = -(signed char)(module[chan.Address_In_Pattern]);
            b5b = true;
        }
        else if(val == 0x6d)
        {
            b4b = true;
            chan.Address_In_Pattern++;
            chan.Addition_To_Ton = module[chan.Address_In_Pattern];
        }
        else if(val == 0x6e)
        {
            chan.Address_In_Pattern++;
            PSC.Delay = module[chan.Address_In_Pattern];
        }
        else if(val == 0x6f)
        {
            b1b = true;
            chan.Address_In_Pattern++;
        }
        else if(val == 0x70)
        {
            b3b = true;
            chan.Address_In_Pattern++;
            chan.Volume_Counter1 = module[chan.Address_In_Pattern];
        }
        else if(val == 0x71)
        {
            chan.Break_Ornament_Loop = true;
            chan.Address_In_Pattern++;
        }
        else if(val == 0x7a)
        {
            chan.Address_In_Pattern++;
            if(chan.num == 1)
            {
                ay_writeay(&info, AY_ENV_SHAPE, module[chan.Address_In_Pattern] & 15);
                ay_writeay(&info, AY_ENV_FINE, module[chan.Address_In_Pattern + 1]);
                ay_writeay(&info, AY_ENV_COARSE, module[chan.Address_In_Pattern + 2]);
                chan.Address_In_Pattern += 2;
            }
        }
        else if(val == 0x7b)
        {
            chan.Address_In_Pattern++;
            if(chan.num == 1)
                PSC.Noise_Base = module[chan.Address_In_Pattern];
        }
        else if(val == 0x7c)
        {
            b1b = b3b = b4b = b5b = b6b = b7b = false;
            b2b = true;
        }
        else if(val == 0x7d)
        {
            chan.Break_Sample_Loop = true;
        }
        else if(val >= 0x58 && val <= 0x66)
        {
            chan.Initial_Volume = val - 0x57;
            chan.Envelope_Enabled = false;
            b6b = true;
        }
        else if(val == 0x57)
        {
            chan.Initial_Volume = 0xf;
            chan.Envelope_Enabled = true;
            b6b = true;
        }
        else if(val <= 0x56)
        {
            chan.Note = val;
            b6b = true;
            b7b = true;
        }
        else
            chan.Address_In_Pattern++;
        chan.Address_In_Pattern++;
    }
    while(!quit);

    if(b7b)
    {
        chan.Break_Ornament_Loop = false;
        chan.Ornament_Enabled = true;
        chan.Enabled = true;
        chan.Break_Sample_Loop = false;
        chan.Ton_Slide_Enabled = false;
        chan.Ton_Accumulator = 0;
        chan.Current_Ton_Sliding = 0;
        chan.Noise_Accumulator = 0;
        chan.Volume_Counter = 0;
        chan.Position_In_Sample = 0;
        chan.Position_In_Ornament = 0;
    }
    if(b6b)
        chan.Volume = chan.Initial_Volume;
    if(b5b)
    {
        chan.Gliss = false;
        chan.Ton_Slide_Enabled = true;
    }
    if(b4b)
    {
        chan.Current_Ton_Sliding = chan.Ton - ASM_Table[chan.Note];
        chan.Gliss = true;
        if(chan.Current_Ton_Sliding >= 0)
            chan.Addition_To_Ton = -chan.Addition_To_Ton;
        chan.Ton_Slide_Enabled = true;
    }
    if(b3b)
    {
        chan.Volume_Counter = chan.Volume_Counter1;
        chan.Volume_Inc = true;
        if((chan.Volume_Counter & 0x40) != 0)
        {
            chan.Volume_Counter = -(signed char)(chan.Volume_Counter | 128);
            chan.Volume_Inc = false;
        }
        chan.Volume_Counter_Init = chan.Volume_Counter;
    }
    if(b2b)
    {
        chan.Break_Ornament_Loop = false;
        chan.Ornament_Enabled = false;
        chan.Enabled = false;
        chan.Break_Sample_Loop = false;
        chan.Ton_Slide_Enabled = false;
    }
    if(b1b)
        chan.Ornament_Enabled = false;
}

void PSC_GetRegisters(AYSongInfo &info, PSC_Channel_Parameters &chan, unsigned char &TempMixer)
{
    unsigned char *module = info.module;
    unsigned char j, b;
    if(chan.Enabled)
    {
        j = chan.Note;
        if(chan.Ornament_Enabled)
        {
            b = module[chan.OrnamentPointer + chan.Position_In_Ornament * 2];
            chan.Noise_Accumulator += b;
            j += module[chan.OrnamentPointer + chan.Position_In_Ornament * 2 + 1];
            if((signed char)j < 0)
                j += 0x56;
            if(j > 0x55)
                j -= 0x56;
            if(j > 0x55)
                j = 0x55;
            if((b & 128) == 0)
                chan.Loop_Ornament_Position = chan.Position_In_Ornament;
            if((b & 64) == 0)
            {
                if(!chan.Break_Ornament_Loop)
                    chan.Position_In_Ornament = chan.Loop_Ornament_Position;
                else
                {
                    chan.Break_Ornament_Loop = false;
                    if((b & 32) == 0)
                        chan.Ornament_Enabled = false;
                    chan.Position_In_Ornament++;
                }
            }
            else
            {
                if((b & 32) == 0)
                    chan.Ornament_Enabled = false;
                chan.Position_In_Ornament++;
            }
        }
        chan.Note = j;
        //chan.Ton = *(unsigned short *) &module[chan.SamplePointer + chan.Position_In_Sample * 6];
        chan.Ton = ay_sys_getword(&module[chan.SamplePointer + chan.Position_In_Sample * 6]);
        chan.Ton_Accumulator += chan.Ton;
        chan.Ton = ASM_Table[j] + chan.Ton_Accumulator;
        if(chan.Ton_Slide_Enabled)
        {
            chan.Current_Ton_Sliding += chan.Addition_To_Ton;
            if(chan.Gliss && (((chan.Current_Ton_Sliding < 0) && (chan.Addition_To_Ton <= 0)) || ((chan.Current_Ton_Sliding >= 0) && (chan.Addition_To_Ton >= 0))))
                chan.Ton_Slide_Enabled = false;
            chan.Ton += chan.Current_Ton_Sliding;
        }
        chan.Ton = chan.Ton & 0xfff;
        b = module[chan.SamplePointer + chan.Position_In_Sample * 6 + 4];
        TempMixer = TempMixer | ((b & 9) << 3);
        j = 0;
        if((b & 2) != 0)
            j++;
        if((b & 4) != 0)
            j--;
        if(chan.Volume_Counter > 0)
        {
            chan.Volume_Counter--;
            if(chan.Volume_Counter == 0)
            {
                if(chan.Volume_Inc)
                    j++;
                else
                    j--;
                chan.Volume_Counter = chan.Volume_Counter_Init;
            }
        }
        chan.Volume += j;
        if((signed char)chan.Volume < 0)
            chan.Volume = 0;
        else if(chan.Volume > 15)
            chan.Volume = 15;
        chan.Amplitude = ((chan.Volume + 1) * (module[chan.SamplePointer + chan.Position_In_Sample * 6 + 3] & 15)) >> 4;
        if(chan.Envelope_Enabled && ((b & 16) == 0))
            chan.Amplitude = chan.Amplitude | 16;
        if(((chan.Amplitude & 16) != 0) & ((b & 8) != 0))
        {
            unsigned short env = ay_readay(&info, AY_ENV_FINE) | (ay_readay(&info, AY_ENV_COARSE) << 8);
            env += (signed char)(module[chan.SamplePointer + chan.Position_In_Sample * 6 + 2]);
            ay_writeay(&info, AY_ENV_FINE, env & 0xff);
            ay_writeay(&info, AY_ENV_COARSE, (env >> 8) & 0xff);
        }
        else
        {
            chan.Noise_Accumulator += module[chan.SamplePointer + chan.Position_In_Sample * 6 + 2];
            if((b & 8) == 0)
                ay_writeay(&info, AY_NOISE_PERIOD, chan.Noise_Accumulator & 31);
        }
        if((b & 128) == 0)
            chan.Loop_Sample_Position = chan.Position_In_Sample;
        if((b & 64) == 0)
        {
            if(!chan.Break_Sample_Loop)
                chan.Position_In_Sample = chan.Loop_Sample_Position;
            else
            {
                chan.Break_Sample_Loop = false;
                if((b & 32) == 0)
                    chan.Enabled = false;
                chan.Position_In_Sample++;
            }
        }
        else
        {
            if((b & 32) == 0)
                chan.Enabled = false;
            chan.Position_In_Sample++;
        }
    }
    else
        chan.Amplitude = 0;
    TempMixer = TempMixer >> 1;
}

void PSC_Play(AYSongInfo &info)
{
    unsigned char *module = info.module;
    unsigned char TempMixer;

    if(--PSC.DelayCounter <= 0)
    {
        if(--PSC.Lines_Counter <= 0)
        {
            if(module[PSC.Positions_Pointer + 1] == 255)
            {
                //PSC.Positions_Pointer = *(unsigned short *) &module[PSC.Positions_Pointer + 2];
                PSC.Positions_Pointer = ay_sys_getword(&module[PSC.Positions_Pointer + 2]);
            }
            PSC.Lines_Counter = module[PSC.Positions_Pointer + 1];
            //PSC_A.Address_In_Pattern = *(unsigned short *) &module[PSC.Positions_Pointer + 2];
            //PSC_B.Address_In_Pattern = *(unsigned short *) &module[PSC.Positions_Pointer + 4];
            //PSC_C.Address_In_Pattern = *(unsigned short *) &module[PSC.Positions_Pointer + 6];
            PSC_A.Address_In_Pattern = ay_sys_getword(&module[PSC.Positions_Pointer + 2]);
            PSC_B.Address_In_Pattern = ay_sys_getword(&module[PSC.Positions_Pointer + 4]);
            PSC_C.Address_In_Pattern = ay_sys_getword(&module[PSC.Positions_Pointer + 6]);
            PSC.Positions_Pointer += 8;
            PSC_A.Note_Skip_Counter = 1;
            PSC_B.Note_Skip_Counter = 1;
            PSC_C.Note_Skip_Counter = 1;
        }
        if(--PSC_A.Note_Skip_Counter == 0)
            PSC_PatternInterpreter(info, PSC_A);
        if(--PSC_B.Note_Skip_Counter == 0)
            PSC_PatternInterpreter(info, PSC_B);
        if(--PSC_C.Note_Skip_Counter == 0)
            PSC_PatternInterpreter(info, PSC_C);
        PSC_A.Noise_Accumulator += PSC.Noise_Base;
        PSC_B.Noise_Accumulator += PSC.Noise_Base;
        PSC_C.Noise_Accumulator += PSC.Noise_Base;
        PSC.DelayCounter = PSC.Delay;
    }
    TempMixer = 0;
    PSC_GetRegisters(info, PSC_A, TempMixer);
    PSC_GetRegisters(info, PSC_B, TempMixer);
    PSC_GetRegisters(info, PSC_C, TempMixer);

    ay_writeay(&info, AY_MIXER, TempMixer);
    ay_writeay(&info, AY_CHNL_A_FINE, PSC_A.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_A_COARSE, (PSC_A.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_B_FINE, PSC_B.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_B_COARSE, (PSC_B.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_C_FINE, PSC_C.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_C_COARSE, (PSC_C.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_A_VOL, PSC_A.Amplitude);
    ay_writeay(&info, AY_CHNL_B_VOL, PSC_B.Amplitude);
    ay_writeay(&info, AY_CHNL_C_VOL, PSC_C.Amplitude);
}

void PSC_GetInfo(AYSongInfo &info)
{
    unsigned char *module = info.file_data;
    PSC_File *header = (PSC_File *)module;
    unsigned char b;
    unsigned long tm = 0;
    int i;
    char a1, a2, a3;
    unsigned long j1, j2, j3;
    unsigned long pptr, cptr;
    bool f71, f72, f73, f61, f62, f63, f41, f42, f43, flg;
    unsigned short j11, j22, j33;
    f71 = f72 = f73 = f61 = f62 = f63 = f41 = f42 = f43 = flg = false;
    j11 = j22 = j33 = 0;

    b = header->PSC_Delay;
    pptr = PSC_PatternsPointer;
    pptr++;
    while(module[pptr] != 255)
    {
        pptr += 8;
        if(pptr >= 65536)
        {
            info.Length = 0;
            return;
        }
    }
    if(pptr >= 65546 - 2)
    {
        info.Length = 0;
    }
    //cptr = *(unsigned short *) &module[pptr + 1];
    cptr = ay_sys_getword(&module[pptr + 1]);
    cptr++;
    pptr = PSC_PatternsPointer;
    pptr++;
    while(module[pptr] != 255)
    {
        if(pptr == cptr)
            info.Loop = tm;
        if(pptr >= 65536 - 6)
        {
            info.Length = 0;
            return;
        }
        //j1 = *(unsigned short *) &module[pptr + 1];
        //j2 = *(unsigned short *) &module[pptr + 3];
        //j3 = *(unsigned short *) &module[pptr + 5];
        j1 = ay_sys_getword(&module[pptr + 1]);
        j2 = ay_sys_getword(&module[pptr + 3]);
        j3 = ay_sys_getword(&module[pptr + 5]);
        pptr += 8;
        if(pptr >= 65536)
        {
            info.Length = 0;
            return;
        }
        a1 = a2 = a3 = 1;
        for(i = 0; i < module[pptr - 8]; i++)
        {
            a1--;
            if(a1 == 0)
            {
                while(true)
                {
                    unsigned char val = module[j1];
                    if(val >= 0xc0)
                    {
                        a1 = val - 0xbf;
                        j1++;
                        break;
                    }
                    else if((val >= 0x67 && val <= 0x6d) || (val >= 0x6f && val <= 0x7b))
                    {
                        j1++;
                    }
                    else if(val == 0x6e)
                    {
                        j1++;
                        b = module[j1];
                    }
                    j1++;
                }

            }

            a2--;
            if(a2 == 0)
            {
                while(true)
                {
                    unsigned char val = module[j2];
                    if(val >= 0xc0)
                    {
                        a2 = val - 0xbf;
                        j2++;
                        break;
                    }
                    else if((val >= 0x67 && val <= 0x6d) || (val >= 0x6f && val <= 0x7b))
                    {
                        j2++;
                    }
                    else if(val == 0x6e)
                    {
                        j2++;
                        b = module[j2];
                    }
                    j2++;
                }
            }

            a3--;
            if(a3 == 0)
            {
                while(true)
                {
                    unsigned char val = module[j3];
                    if(val >= 0xc0)
                    {
                        a3 = val - 0xbf;
                        j3++;
                        break;
                    }
                    else if((val >= 0x67 && val <= 0x6d) || (val >= 0x6f && val <= 0x7b))
                    {
                        j3++;
                    }
                    else if(val == 0x6e)
                    {
                        j3++;
                        b = module[j3];
                    }
                    j3++;
                }
            }
            tm += b;
        }
    }
    info.Length = tm;
    unsigned char *ptr = module + 0x19;
    info.Name = ay_sys_getstr(ptr, 20);
    ptr = module + 0x31;
    info.Author = ay_sys_getstr(ptr, 20);
}

void PSC_Cleanup(AYSongInfo &info)
{
    if(info.data)
    {
        delete (PSC_SongInfo *)info.data;
        info.data = 0;
    }
}

bool PSC_Detect(unsigned char *module, unsigned long length)
{
    int j, j1;
    PSC_File *header = (PSC_File *)module;
    if(length < 0x4c + 2)
        return false;
    if(PSC_OrnamentsPointer >= length)
        return false;
    if(PSC_OrnamentsPointer < 0x4c + 2)
        return false;
    if(PSC_OrnamentsPointer > 64 + 0x4c)
        return false;
    if((PSC_OrnamentsPointer % 2) != 0)
        return false;
    if(PSC_SamplesPointers(0) + 0x4c + 5 > 65534)
        return false;
    if(PSC_SamplesPointers(0) + 0x4c + 5 > length)
        return false;

    j = ay_sys_getword(&module[PSC_OrnamentsPointer]);
    j += PSC_OrnamentsPointer;
    if(j > 65535)
        return false;
    if(j >= length)
        return false;

    j1 = 0;
    j1 = ay_sys_getword(&module[PSC_OrnamentsPointer-2]);
    j1 += 0x4c;
    if(j1 > 65535)
        return false;
    if(j1 >= length)
        return false;
    if(j - j1 < 8)
        return false;
    if(((j - j1) % 6) != 2)
        return false;

    j1 = PSC_SamplesPointers(0) + 0x4c + 4;
    while((j1 < 65536) && (j1 <= length) && ((module[j1] & 32) != 0))
        j1 += 6;
    if(j1 > 65534)
        return false;
    if(j1 > length)
        return false;

    if(PSC_OrnamentsPointer - 0x4c - 2 > 0)
    {
        if(j1 + 3 != PSC_SamplesPointers(1) + 0x4c)
            return false;
    }
    else if(j1 + 4 != j)
        return false;

    if(PSC_PatternsPointer + 11 > 65535)
        return false;
    if(PSC_PatternsPointer + 11 >= length)
        return false;

    j = PSC_PatternsPointer + 1;
    if(module[j] == 255)
        return false;

    while(1)
    {
        j += 8;
        if(((j > 65532) && (j + 2 > length)) || (module[j] == 255))
            break;
    }
    
    if(j > 65532)
        return false;
    if(j + 2 > length)
        return false;

    return true;

}
