/* This player module was ported from:
 AY-3-8910/12 Emulator
 Version 3.0 for Windows 95
 Author Sergey Vladimirovich Bulba
 (c)1999-2004 S.V.Bulba
 */
const unsigned short SQT_Table[] =
{ 0xd5d, 0xc9c, 0xbe7, 0xb3c, 0xa9b, 0xa02, 0x973, 0x8eb, 0x86b, 0x7f2, 0x780, 0x714, 0x6ae, 0x64e, 0x5f4, 0x59e, 0x54f, 0x501, 0x4b9, 0x475, 0x435, 0x3f9, 0x3c0, 0x38a, 0x357, 0x327, 0x2fa, 0x2cf, 0x2a7, 0x281, 0x25d, 0x23b, 0x21b, 0x1fc, 0x1e0, 0x1c5, 0x1ac, 0x194, 0x17d, 0x168, 0x153, 0x140, 0x12e, 0x11d, 0x10d, 0xfe, 0xf0, 0xe2, 0xd6, 0xca, 0xbe, 0xb4, 0xaa, 0xa0, 0x97, 0x8f, 0x87, 0x7f, 0x78, 0x71, 0x6b, 0x65, 0x5f, 0x5a, 0x55, 0x50, 0x4c, 0x47, 0x43, 0x40, 0x3c, 0x39, 0x35, 0x32, 0x30, 0x2d, 0x2a, 0x28, 0x26, 0x24, 0x22, 0x20, 0x1e, 0x1c, 0x1b, 0x19, 0x18, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0xf, 0xe };

struct SQT_File
{
    unsigned char SQT_Size0, SQT_Size1;
    unsigned char SQT_SamplesPointer0, SQT_SamplesPointer1;
    unsigned char SQT_OrnamentsPointer0, SQT_OrnamentsPointer1;
    unsigned char SQT_PatternsPointer0, SQT_PatternsPointer1;
    unsigned char SQT_PositionsPointer0, SQT_PositionsPointer1;
    unsigned char SQT_LoopPointer0, SQT_LoopPointer1;
};

#define SQT_Size0 (header->SQT_Size0 | (header->SQT_Size0 << 8))
#define SQT_SamplesPointer (header->SQT_SamplesPointer0 | (header->SQT_SamplesPointer1 << 8))
#define SQT_OrnamentsPointer (header->SQT_OrnamentsPointer0 | (header->SQT_OrnamentsPointer1 << 8))
#define SQT_PatternsPointer (header->SQT_PatternsPointer0 | (header->SQT_PatternsPointer1 << 8))
#define SQT_PositionsPointer (header->SQT_PositionsPointer0 | (header->SQT_PositionsPointer1 << 8))
#define SQT_LoopPointer (header->SQT_LoopPointer0 | (header->SQT_LoopPointer1 << 8))

#define MAKE_PWORD(x) (unsigned short *)(x)

struct SQT_Channel_Parameters
{
    unsigned short Address_In_Pattern, SamplePointer, Point_In_Sample, OrnamentPointer, Point_In_Ornament, Ton, ix27;
    unsigned char Volume, Amplitude, Note, ix21;
    short Ton_Slide_Step, Current_Ton_Sliding;
    char Sample_Tik_Counter, Ornament_Tik_Counter, Transposit;
    bool Enabled, Envelope_Enabled, Ornament_Enabled, Gliss, MixNoise, MixTon, b4ix0, b6ix0, b7ix0;
};

struct SQT_Parameters
{
    unsigned char Delay, DelayCounter, Lines_Counter;
    unsigned short Positions_Pointer;
};

struct SQT_SongInfo
{
    SQT_Parameters SQT;
    SQT_Channel_Parameters SQT_A, SQT_B, SQT_C;
};

#define SQT_A ((SQT_SongInfo *)info.data)->SQT_A
#define SQT_B ((SQT_SongInfo *)info.data)->SQT_B
#define SQT_C ((SQT_SongInfo *)info.data)->SQT_C
#define SQT ((SQT_SongInfo *)info.data)->SQT

bool SQT_PreInit(AYSongInfo &info)
{
    unsigned char *module = info.module;
    SQT_File *header = (SQT_File *)module;
    int i, i1, i2;
    unsigned long j2;
    unsigned short *pwrd;
    i = SQT_SamplesPointer - 10;
    if(i < 0)
        return false;
    i1 = 0;
    i2 = SQT_PositionsPointer - i;
    if(i2 < 0)
        return false;
    while(module[i2] != 0)
    {
        if(i2 > 65536 - 8)
            return false;
        if(i1 < (module[i2] & 0x7f))
            i1 = module[i2] & 0x7f;
        i2 += 2;
        if(i1 < (module[i2] & 0x7f))
            i1 = module[i2] & 0x7f;
        i2 += 2;
        if(i1 < (module[i2] & 0x7f))
            i1 = module[i2] & 0x7f;
        i2 += 3;
    }
    j2 = (uint64_t)(&module[65535]);
    pwrd = MAKE_PWORD(&header->SQT_SamplesPointer0);
    i1 = (SQT_PatternsPointer - i + i1 * 2) / 2;
    if(i1 < 1)
        return false;
    for(i2 = 1; i2 <= i1; i2++)
    {
        if((uint64_t)(pwrd) >= j2)
            return false;
        if(*pwrd < i)
            return false;
        *pwrd -= i;
        pwrd++;
    }
    return true;
}

void SQT_Init(AYSongInfo &info)
{
    unsigned char *module = info.module;
    SQT_File *header = (SQT_File *)module;

    if(!SQT_PreInit(info))
        return;

    if(info.data)
    {
        delete (SQT_SongInfo *)info.data;
        info.data = 0;
    }
    info.data = (void *)new SQT_SongInfo;
    if(!info.data)
        return;

    SQT_A.Ton = 0;
    SQT_A.Envelope_Enabled = false;
    SQT_A.Ornament_Enabled = false;
    SQT_A.Gliss = false;
    SQT_A.Enabled = false;

    SQT_B.Ton = 0;
    SQT_B.Envelope_Enabled = false;
    SQT_B.Ornament_Enabled = false;
    SQT_B.Gliss = false;
    SQT_B.Enabled = false;

    SQT_C.Ton = 0;
    SQT_C.Envelope_Enabled = false;
    SQT_C.Ornament_Enabled = false;
    SQT_C.Gliss = false;
    SQT_C.Enabled = false;

    SQT.DelayCounter = 1;
    SQT.Delay = 1;
    SQT.Lines_Counter = 1;
    SQT.Positions_Pointer = SQT_PositionsPointer;

    ay_resetay(&info, 0);
}

void SQT_Call_LC1D1(AYSongInfo &info, SQT_Channel_Parameters &chan, unsigned short &Ptr, unsigned char a)
{
    unsigned char *module = info.module;
    Ptr++;
    if(chan.b6ix0)
    {
        chan.Address_In_Pattern = Ptr + 1;
        chan.b6ix0 = false;
    }
    switch(a - 1)
    {
        case 0:
            if(chan.b4ix0)
                chan.Volume = module[Ptr] & 15;
            break;
        case 1:
            if(chan.b4ix0)
                chan.Volume = (chan.Volume + module[Ptr]) & 15;
            break;
        case 2:
            if(chan.b4ix0)
            {
                SQT_A.Volume = module[Ptr];
                SQT_B.Volume = module[Ptr];
                SQT_C.Volume = module[Ptr];
            }
            break;
        case 3:
            if(chan.b4ix0)
            {
                SQT_A.Volume = (SQT_A.Volume + module[Ptr]) & 15;
                SQT_B.Volume = (SQT_B.Volume + module[Ptr]) & 15;
                SQT_C.Volume = (SQT_C.Volume + module[Ptr]) & 15;
            }
            break;
        case 4:
            if(chan.b4ix0)
            {
                SQT.DelayCounter = module[Ptr] & 31;
                if(SQT.DelayCounter == 0)
                    SQT.DelayCounter = 32;
                SQT.Delay = SQT.DelayCounter;
            }
            break;
        case 5:
            if(chan.b4ix0)
            {
                SQT.DelayCounter = (SQT.DelayCounter + module[Ptr]) & 31;
                if(SQT.DelayCounter == 0)
                    SQT.DelayCounter = 32;
                SQT.Delay = SQT.DelayCounter;
            }
            break;
        case 6:
            chan.Current_Ton_Sliding = 0;
            chan.Gliss = true;
            chan.Ton_Slide_Step = -module[Ptr];
            break;
        case 7:
            chan.Current_Ton_Sliding = 0;
            chan.Gliss = true;
            chan.Ton_Slide_Step = module[Ptr];
            break;
        default:
            chan.Envelope_Enabled = true;
            ay_writeay(&info, AY_ENV_SHAPE, (a - 1) & 15);
            ay_writeay(&info, AY_ENV_FINE, module[Ptr]);
            break;
    }
}

void SQT_Call_LC2A8(AYSongInfo &info, SQT_Channel_Parameters &chan, unsigned char a)
{
    unsigned char *module = info.module;
    SQT_File *header = (SQT_File *)module;
    chan.Envelope_Enabled = false;
    chan.Ornament_Enabled = false;
    chan.Gliss = false;
    chan.Enabled = true;
    chan.SamplePointer = ay_sys_getword(&module[a * 2 + SQT_SamplesPointer]);
    chan.Point_In_Sample = chan.SamplePointer + 2;
    chan.Sample_Tik_Counter = 32;
    chan.MixNoise = true;
    chan.MixTon = true;
}

void SQT_Call_LC2D9(AYSongInfo &info, SQT_Channel_Parameters &chan, unsigned char a)
{
    unsigned char *module = info.module;
    SQT_File *header = (SQT_File *)module;
    chan.OrnamentPointer = ay_sys_getword(&module[a * 2 + SQT_OrnamentsPointer]);
    chan.Point_In_Ornament = chan.OrnamentPointer + 2;
    chan.Ornament_Tik_Counter = 32;
    chan.Ornament_Enabled = true;
}

void SQT_Call_LC283(AYSongInfo &info, SQT_Channel_Parameters &chan, unsigned short &Ptr)
{
    unsigned char *module = info.module;
    unsigned char val = module[Ptr];
    if(val <= 0x7f)
    {
        SQT_Call_LC1D1(info, chan, Ptr, val);
    }
    else if(val >= 0x80)
    {
        if(((val >> 1) & 31) != 0)
            SQT_Call_LC2A8(info, chan, (val >> 1) & 31);
        if((val & 64) != 0)
        {
            int Temp = module[Ptr + 1] >> 4;
            if((val & 1) != 0)
                Temp = Temp | 16;
            if(Temp != 0)
                SQT_Call_LC2D9(info, chan, Temp);
            Ptr++;
            if((module[Ptr] & 15) != 0)
                SQT_Call_LC1D1(info, chan, Ptr, module[Ptr] & 15);
        }
    }
    Ptr++;
}

void SQT_Call_LC191(AYSongInfo &info, SQT_Channel_Parameters &chan, unsigned short &Ptr)
{
    unsigned char *module = info.module;
    Ptr = chan.ix27;
    chan.b6ix0 = false;
    if(module[Ptr] <= 0x7f)
    {
        Ptr++;
        SQT_Call_LC283(info, chan, Ptr);
    }
    else if(module[Ptr] >= 0x80)
    {
        SQT_Call_LC2A8(info, chan, module[Ptr] & 31);
    }

}

void SQT_PatternInterpreter(AYSongInfo &info, SQT_Channel_Parameters &chan)
{
    unsigned char *module = info.module;
    unsigned short Ptr = 0;
    if(chan.ix21 != 0)
    {
        chan.ix21--;
        if(chan.b7ix0)
            SQT_Call_LC191(info, chan, Ptr);
        return;
    }
    Ptr = chan.Address_In_Pattern;
    chan.b6ix0 = true;
    chan.b7ix0 = false;
    while(true)
    {
        unsigned char val = module[Ptr];
        if(val <= 0x5f)
        {
            chan.Note = module[Ptr];
            chan.ix27 = Ptr;
            Ptr++;
            SQT_Call_LC283(info, chan, Ptr);
            if(chan.b6ix0)
                chan.Address_In_Pattern = Ptr;
            break;
        }
        else if(val >= 0x60 && val <= 0x6e)
        {
            SQT_Call_LC1D1(info, chan, Ptr, module[Ptr] - 0x60);
            break;
        }
        else if(val >= 0x6f && val <= 0x7f)
        {
            chan.MixNoise = false;
            chan.MixTon = false;
            chan.Enabled = false;
            if(val != 0x6f)
                SQT_Call_LC1D1(info, chan, Ptr, module[Ptr] - 0x6f);
            else
                chan.Address_In_Pattern = Ptr + 1;
            break;
        }
        else if(val >= 0x80 && val <= 0xbf)
        {
            chan.Address_In_Pattern = Ptr + 1;
            if(val <= 0x9f)
            {
                if((val & 16) == 0)
                    chan.Note += val & 15;
                else
                    chan.Note -= val & 15;
            }
            else
            {
                chan.ix21 = val & 15;
                if((val & 16) == 0)
                    break;
                if(chan.ix21 != 0)
                    chan.b7ix0 = true;
            }
            SQT_Call_LC191(info, chan, Ptr);
            break;
        }
        else if(val >= 0xc0)
        {
            chan.Address_In_Pattern = Ptr + 1;
            chan.ix27 = Ptr;
            SQT_Call_LC2A8(info, chan, val & 31);
            break;
        }
    }
}

void SQT_GetRegisters(AYSongInfo &info, SQT_Channel_Parameters &chan, unsigned char &TempMixer)
{
    unsigned char *module = info.module;
    unsigned char j, b0, b1;
    TempMixer = TempMixer << 1;
    if(chan.Enabled)
    {
        b0 = module[chan.Point_In_Sample];
        chan.Amplitude = b0 & 15;
        if(chan.Amplitude != 0)
        {
            chan.Amplitude -= chan.Volume;
            if((signed char)(chan.Amplitude) < 0)
                chan.Amplitude = 0;
        }
        else if(chan.Envelope_Enabled)
            chan.Amplitude = 16;
        b1 = module[chan.Point_In_Sample + 1];
        if((b1 & 32) != 0)
        {
            TempMixer |= 8;
            unsigned char noise = (b0 & 0xf0) >> 3;
            if((signed char)(b1) < 0)
                noise++;
            ay_writeay(&info, AY_NOISE_PERIOD, noise);
        }
        if((b1 & 64) != 0)
        {
            TempMixer |= 1;
        }
        j = chan.Note;
        if(chan.Ornament_Enabled)
        {
            j += module[chan.Point_In_Ornament];
            chan.Ornament_Tik_Counter--;
            if(chan.Ornament_Tik_Counter == 0)
            {
                if(module[chan.OrnamentPointer] != 32)
                {
                    chan.Ornament_Tik_Counter = module[chan.OrnamentPointer + 1];
                    chan.Point_In_Ornament = chan.OrnamentPointer + 2 + module[chan.OrnamentPointer];
                }
                else
                {
                    chan.Ornament_Tik_Counter = module[chan.SamplePointer + 1];
                    chan.Point_In_Ornament = chan.OrnamentPointer + 2 + module[chan.SamplePointer];
                }
            }
            else
                chan.Point_In_Ornament++;
        }
        j += chan.Transposit;
        if(j > 0x5f)
            j = 0x5f;
        if((b1 & 16) == 0)
            chan.Ton = SQT_Table[j] - (((unsigned short)(b1 & 15) << 8) + module[chan.Point_In_Sample + 2]);
        else
            chan.Ton = SQT_Table[j] + (((unsigned short)(b1 & 15) << 8) + module[chan.Point_In_Sample + 2]);
        chan.Sample_Tik_Counter--;
        if(chan.Sample_Tik_Counter == 0)
        {
            chan.Sample_Tik_Counter = module[chan.SamplePointer + 1];
            if(module[chan.SamplePointer] == 32)
            {
                chan.Enabled = false;
                chan.Ornament_Enabled = false;
            }
            chan.Point_In_Sample = chan.SamplePointer + 2 + module[chan.SamplePointer] * 3;
        }
        else
            chan.Point_In_Sample += 3;
        if(chan.Gliss)
        {
            chan.Ton += chan.Current_Ton_Sliding;
            chan.Current_Ton_Sliding += chan.Ton_Slide_Step;
        }
        chan.Ton = chan.Ton & 0xfff;
    }
    else
        chan.Amplitude = 0;
}

void SQT_Play(AYSongInfo &info)
{
    unsigned char *module = info.module;
    SQT_File *header = (SQT_File *)module;
    unsigned char TempMixer;

    if(--SQT.DelayCounter == 0)
    {
        SQT.DelayCounter = SQT.Delay;
        if(--SQT.Lines_Counter == 0)
        {
            if(module[SQT.Positions_Pointer] == 0)
                SQT.Positions_Pointer = SQT_LoopPointer;
            if((signed char)(module[SQT.Positions_Pointer]) < 0)
                SQT_C.b4ix0 = true;
            else
                SQT_C.b4ix0 = false;
            SQT_C.Address_In_Pattern = ay_sys_getword(&module[(unsigned char)(module[SQT.Positions_Pointer] * 2) + SQT_PatternsPointer]);
            SQT.Lines_Counter = module[SQT_C.Address_In_Pattern];
            SQT_C.Address_In_Pattern++;
            SQT.Positions_Pointer++;
            SQT_C.Volume = module[SQT.Positions_Pointer] & 15;
            if((module[SQT.Positions_Pointer] >> 4) < 9)
                SQT_C.Transposit = module[SQT.Positions_Pointer] >> 4;
            else
                SQT_C.Transposit = -((module[SQT.Positions_Pointer] >> 4) - 9) - 1;
            SQT.Positions_Pointer++;
            SQT_C.ix21 = 0;

            if(module[SQT.Positions_Pointer] == 0)
                SQT.Positions_Pointer = SQT_LoopPointer;
            if((signed char)(module[SQT.Positions_Pointer]) < 0)
                SQT_B.b4ix0 = true;
            else
                SQT_B.b4ix0 = false;
            SQT_B.Address_In_Pattern = ay_sys_getword(&module[(unsigned char)(module[SQT.Positions_Pointer] * 2) + SQT_PatternsPointer]) + 1;
            SQT.Positions_Pointer++;
            SQT_B.Volume = module[SQT.Positions_Pointer] & 15;
            if((module[SQT.Positions_Pointer] >> 4) < 9)
                SQT_B.Transposit = module[SQT.Positions_Pointer] >> 4;
            else
                SQT_B.Transposit = -((module[SQT.Positions_Pointer] >> 4) - 9) - 1;
            SQT.Positions_Pointer++;
            SQT_B.ix21 = 0;

            if(module[SQT.Positions_Pointer] == 0)
                SQT.Positions_Pointer = SQT_LoopPointer;
            if((signed char)(module[SQT.Positions_Pointer]) < 0)
                SQT_A.b4ix0 = true;
            else
                SQT_A.b4ix0 = false;
            SQT_A.Address_In_Pattern = ay_sys_getword(&module[(unsigned char)(module[SQT.Positions_Pointer] * 2) + SQT_PatternsPointer]) + 1;
            SQT.Positions_Pointer++;
            SQT_A.Volume = module[SQT.Positions_Pointer] & 15;
            if((module[SQT.Positions_Pointer] >> 4) < 9)
                SQT_A.Transposit = module[SQT.Positions_Pointer] >> 4;
            else
                SQT_A.Transposit = -((module[SQT.Positions_Pointer] >> 4) - 9) - 1;
            SQT.Positions_Pointer++;
            SQT_A.ix21 = 0;

            SQT.Delay = module[SQT.Positions_Pointer];
            SQT.DelayCounter = SQT.Delay;
            SQT.Positions_Pointer++;
        }
        SQT_PatternInterpreter(info, SQT_C);
        SQT_PatternInterpreter(info, SQT_B);
        SQT_PatternInterpreter(info, SQT_A);
    }

    TempMixer = 0;
    SQT_GetRegisters(info, SQT_C, TempMixer);
    SQT_GetRegisters(info, SQT_B, TempMixer);
    SQT_GetRegisters(info, SQT_A, TempMixer);
    TempMixer = (-(TempMixer + 1)) & 0x3f;

    if(!SQT_A.MixNoise)
        TempMixer |= 8;
    if(!SQT_A.MixTon)
        TempMixer |= 1;

    if(!SQT_B.MixNoise)
        TempMixer |= 16;
    if(!SQT_B.MixTon)
        TempMixer |= 2;

    if(!SQT_C.MixNoise)
        TempMixer |= 32;
    if(!SQT_C.MixTon)
        TempMixer |= 4;

    ay_writeay(&info, AY_MIXER, TempMixer);
    ay_writeay(&info, AY_CHNL_A_FINE, SQT_A.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_A_COARSE, (SQT_A.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_B_FINE, SQT_B.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_B_COARSE, (SQT_B.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_C_FINE, SQT_C.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_C_COARSE, (SQT_C.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_A_VOL, SQT_A.Amplitude);
    ay_writeay(&info, AY_CHNL_B_VOL, SQT_B.Amplitude);
    ay_writeay(&info, AY_CHNL_C_VOL, SQT_C.Amplitude);
}

void SQT_GetChannelInfo(AYSongInfo &info, unsigned char &b, unsigned long &tm, char &a1, unsigned short &j1, unsigned short &pptr, unsigned short &cptr, bool &f71, bool &f61, bool &f41, unsigned short &j11, unsigned char chnl_num)
{
    unsigned char *module = info.file_data;
    if(a1 != 0)
    {
        a1--;
        if(f71)
        {
            cptr = j11;
            f61 = false;
            if(module[cptr] <= 0x7f)
            {
                cptr++;
                unsigned char val = module[cptr];
                if(val <= 0x7f)
                {
                    cptr++;
                    if(f61)
                        j1 = cptr + 1;
                    switch(module[cptr - 1] - 1)
                    {
                        case 4:
                            if(f41)
                            {
                                b = module[cptr] & 31;
                                if(b == 0)
                                    b = 32;
                            }
                            break;
                        case 5:
                            if(f41)
                            {
                                b = (b + module[cptr]) & 31;
                                if(b == 0)
                                    b = 32;
                            }
                            break;
                        default:
                            break;
                    }
                }
                else if(val >= 0x80)
                {
                    if((val & 64) != 0)
                    {
                        cptr++;
                        if((module[cptr] & 15) != 0)
                        {
                            cptr++;
                            if(f61)
                                j1 = cptr + 1;
                            switch((module[cptr - 1] & 15) - 1)
                            {
                                case 4:
                                    if(f41)
                                    {
                                        b = module[cptr] & 31;
                                        if(b == 0)
                                            b = 32;
                                    }
                                    break;
                                case 5:
                                    if(f41)
                                    {
                                        b = (b + module[cptr]) & 31;
                                        if(b == 0)
                                            b = 32;
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        cptr = j1;
        f61 = true;
        f71 = false;
        while(true)
        {
            unsigned char val = module[cptr];
            if(val <= 0x5f)
            {
                j11 = cptr;
                cptr++;
                unsigned char val = module[cptr];
                if(val <= 0x7f)
                {
                    cptr++;
                    if(f61)
                    {
                        j1 = cptr + 1;
                        f61 = false;
                    }
                    switch(module[cptr - 1] - 1)
                    {
                        case 4:
                            if(f41)
                            {
                                b = module[cptr] & 31;
                                if(b == 0)
                                    b = 32;
                            }
                            break;
                        case 5:
                            if(f41)
                            {
                                b = (b + module[cptr]) & 31;
                                if(b == 0)
                                    b = 32;
                            }
                            break;
                        default:
                            break;
                    }
                }
                else if(val >= 0x80)
                {
                    if((module[cptr] & 64) != 0)
                    {
                        cptr++;
                        if((module[cptr] & 15) != 0)
                        {
                            cptr++;
                            if(f61)
                            {
                                j1 = cptr + 1;
                                f61 = false;
                            }
                            switch((module[cptr - 1] & 15) - 1)
                            {
                                case 4:
                                    if(f41)
                                    {
                                        b = module[cptr] & 31;
                                        if(b == 0)
                                            b = 32;
                                    }
                                    break;
                                case 5:
                                    if(f41)
                                    {
                                        b = (b + module[cptr]) & 31;
                                        if(b == 0)
                                            b = 32;
                                    }
                                    break;
                                default:
                                    break;
                            }

                        }
                    }
                }
                cptr++;
                if(f61)
                    j1 = cptr;
                break;
            }
            else if(val >= 0x60 && val <= 0x6e)
            {
                cptr++;
                if(f61)
                    j1 = cptr + 1;
                switch(module[cptr - 1] - 0x60 - 1)
                {
                    case 4:
                        if(f41)
                        {
                            b = module[cptr] & 31;
                            if(b == 0)
                                b = 32;
                        }
                        break;
                    case 5:
                        if(f41)
                        {
                            b = (b + module[cptr]) & 31;
                            if(b == 0)
                                b = 32;
                        }
                        break;
                    default:
                        break;
                }
                break;
            }
            else if(val >= 0x6f && val <= 0x7f)
            {
                if(module[cptr] != 0x6f)
                {
                    cptr++;
                    if(f61)
                        j1 = cptr + 1;
                    switch(module[cptr - 1] - 0x6f - 1)
                    {
                        case 4:
                            if(f41)
                            {
                                b = module[cptr] & 31;
                                if(b == 0)
                                    b = 32;
                            }
                            break;
                        case 5:
                            if(f41)
                            {
                                b = (b + module[cptr]) & 31;
                                if(b == 0)
                                    b = 32;
                            }
                            break;
                        default:
                            break;
                    }
                }
                else
                    j1 = cptr + 1;
                break;
            }
            else if(val >= 0x80 && val <= 0xbf)
            {
                j1 = cptr + 1;
                if(chnl_num == 1)
                {
                    if(val >= 0xa0)
                    {
                        a1 = module[cptr] & 15;
                        if((module[cptr] & 16) == 0)
                            break;
                        if(a1 != 0)
                            f71 = true;
                    }
                }
                else if(chnl_num == 2 || chnl_num == 3)
                {
                    if(val > 0x9f)
                    {
                        a1 = module[cptr] & 15;
                        if((module[cptr] & 16) == 0)
                            break;
                        if(a1 != 0)
                            f71 = true;
                    }
                }
                cptr = j11;
                f61 = false;
                if(module[cptr] <= 0x7f)
                {
                    cptr++;
                    unsigned char val = module[cptr];
                    if(val <= 0x7f)
                    {
                        cptr++;
                        if(f61)
                            j1 = cptr + 1;
                        switch(module[cptr - 1] - 1)
                        {
                            case 4:
                                if(f41)
                                {
                                    b = module[cptr] & 31;
                                    if(b == 0)
                                        b = 32;
                                }
                                break;
                            case 5:
                                if(f41)
                                {
                                    b = (b + module[cptr]) & 31;
                                    if(b == 0)
                                        b = 32;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if(val >= 0x80)
                    {
                        if((module[cptr] & 64) != 0)
                        {
                            cptr++;
                            if((module[cptr] & 15) != 0)
                            {
                                cptr++;
                                if(f61)
                                    j1 = cptr + 1;
                                switch((module[cptr - 1] & 15) - 1)
                                {
                                    case 4:
                                        if(f41)
                                        {
                                            b = module[cptr] & 31;
                                            if(b == 0)
                                                b = 32;
                                        }
                                        break;
                                    case 5:
                                        if(f41)
                                        {
                                            b = (b + module[cptr]) & 31;
                                            if(b == 0)
                                                b = 32;
                                        }
                                        break;
                                    default:
                                        break;
                                }

                            }
                        }
                    }

                }
                break;
            }
            else if(val >= 0xc0)
            {
                j1 = cptr + 1;
                j11 = cptr;
                break;
            }
        }
    }
}

void SQT_GetInfo(AYSongInfo &info)
{
    memcpy(info.module, info.file_data, info.file_len);
    unsigned char *module = info.module;
    if(!SQT_PreInit(info))
    {
        info.Length = 0;
        return;
    }
    SQT_File *header = (SQT_File *)module;
    unsigned char b;
    unsigned long tm = 0;
    int i;
    char a1, a2, a3;
    unsigned short j1, j2, j3;
    unsigned short pptr, cptr;
    bool f71, f72, f73, f61, f62, f63, f41, f42, f43, flg;
    unsigned short j11, j22, j33;
    f71 = f72 = f73 = f61 = f62 = f63 = f41 = f42 = f43 = flg = false;
    j11 = j22 = j33 = 0;

    pptr = SQT_PositionsPointer;
    while(module[pptr] != 0)
    {
        if(pptr == SQT_LoopPointer)
            info.Loop = tm;
        f41 = (module[pptr] & 128) ? true : false;
        j1 = ay_sys_getword(&module[(unsigned char)(module[pptr] * 2) + SQT_PatternsPointer]);
        j1++;
        pptr += 2;
        f42 = (module[pptr] & 128) ? true : false;
        j2 = ay_sys_getword(&module[(unsigned char)(module[pptr] * 2) + SQT_PatternsPointer]);
        j2++;
        pptr += 2;
        f43 = (module[pptr] & 128) ? true : false;
        j3 = ay_sys_getword(&module[(unsigned char)(module[pptr] * 2) + SQT_PatternsPointer]);
        j3++;
        pptr += 2;
        b = module[pptr];
        pptr++;
        a1 = a2 = a3 = 0;
        unsigned char limit = module[j1 - 1];
        for(i = 0; i < limit; i++)
        {
            SQT_GetChannelInfo(info, b, tm, a1, j1, pptr, cptr, f71, f61, f41, j11, 1);
            SQT_GetChannelInfo(info, b, tm, a2, j2, pptr, cptr, f72, f62, f42, j22, 2);
            SQT_GetChannelInfo(info, b, tm, a3, j3, pptr, cptr, f73, f63, f43, j33, 3);
            tm += b;
        }
    }
    info.Length = tm;
}

void SQT_Cleanup(AYSongInfo &info)
{
    if(info.data)
    {
        delete (SQT_SongInfo *)info.data;
        info.data = 0;
    }
}

bool SQT_Detect(unsigned char *module, unsigned long length)
{
    int j, j1, j2, j3;
    SQT_File *header = (SQT_File *)module;
    unsigned short *pwrd;
    if(length < 17)
        return false;
    if(SQT_SamplesPointer < 10)
        return false;
    if(SQT_OrnamentsPointer <= SQT_SamplesPointer)
        return false;
    if(SQT_PatternsPointer < SQT_OrnamentsPointer)
        return false;
    if(SQT_PositionsPointer <= SQT_PatternsPointer)
        return false;
    if(SQT_LoopPointer < SQT_PositionsPointer)
        return false;

    j = SQT_SamplesPointer - 10;
    if(SQT_LoopPointer - j >= length)
        return false;

    j1 = SQT_PositionsPointer - j;
    if(module[j1] == 0)
        return false;
    j2 = 0;
    while(module[j1] != 0)
    {
        if(j1 + 7 >= length)
            return false;
        if(j2 < (module[j1] & 0x7f))
            j2 = module[j1] & 0x7f;
        j1 += 2;
        if(j2 < (module[j1] & 0x7f))
            j2 = module[j1] & 0x7f;
        j1 += 2;
        if(j2 < (module[j1] & 0x7f))
            j2 = module[j1] & 0x7f;
        j1 += 3;
    }

    pwrd = (unsigned short *)&module[SQT_SamplesPointer - j + 2];
    if(*pwrd - SQT_PatternsPointer - 2 != j2 * 2)
        return false;

    int F_Length = j1 + 7;
    pwrd = (unsigned short *)&module[12];
    j2 = *pwrd;
    for(j1 = 1; j1 <= (SQT_OrnamentsPointer- SQT_SamplesPointer) / 2; j1++)
    {
        pwrd++;
        j3 = *pwrd;
        if( j3 - j2 != 0x62 )return false;
        j2 = j3;
    }

    for(j1 = 1; j1 <= (SQT_PatternsPointer - SQT_OrnamentsPointer) / 2; j1++)
    {
        pwrd++;
        j3 = *pwrd;
        if( j3 - j2 != 0x22) return false;
        j2 = j3;
    }

    return true;
}
