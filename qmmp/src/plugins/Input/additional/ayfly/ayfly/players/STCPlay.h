const unsigned short ST_Table[96] =
{ 0xef8, 0xe10, 0xd60, 0xc80, 0xbd8, 0xb28, 0xa88, 0x9f0, 0x960, 0x8e0, 0x858, 0x7e0, 0x77c, 0x708, 0x6b0, 0x640, 0x5ec, 0x594, 0x544, 0x4f8, 0x4b0, 0x470, 0x42c, 0x3f0, 0x3be, 0x384, 0x358, 0x320, 0x2f6, 0x2ca, 0x2a2, 0x27c, 0x258, 0x238, 0x216, 0x1f8, 0x1df, 0x1c2, 0x1ac, 0x190, 0x17b, 0x165, 0x151, 0x13e, 0x12c, 0x11c, 0x10b, 0xfc, 0xef, 0xe1, 0xd6, 0xc8, 0xbd, 0xb2, 0xa8, 0x9f, 0x96, 0x8e, 0x85, 0x7e, 0x77, 0x70, 0x6b, 0x64, 0x5e, 0x59, 0x54, 0x4f, 0x4b, 0x47, 0x42, 0x3f, 0x3b, 0x38, 0x35, 0x32, 0x2f, 0x2c, 0x2a, 0x27, 0x25, 0x23, 0x21, 0x1f, 0x1d, 0x1c, 0x1a, 0x19, 0x17, 0x16, 0x15, 0x13, 0x12, 0x11, 0x10, 0xf };

struct STC_File
{
    unsigned char ST_Delay;
    unsigned char ST_PositionsPointer0, ST_PositionsPointer1;
    unsigned char ST_OrnamentsPointer0, ST_OrnamentsPointer1;
    unsigned char ST_PatternsPointer0, ST_PatternsPointer1;
    signed char ST_Name[18];
    unsigned char ST_Size0, ST_Size1;
};

struct STC_Channel_Parameters
{
    unsigned short Address_In_Pattern, SamplePointer, OrnamentPointer, Ton;
    unsigned char Amplitude, Note, Position_In_Sample, Number_Of_Notes_To_Skip;
    signed char Sample_Tik_Counter, Note_Skip_Counter;
    bool Envelope_Enabled;
};

struct STC_Parameters
{
    unsigned char DelayCounter, Transposition, CurrentPosition;
};

struct STC_SongInfo
{
    STC_Parameters STC;
    STC_Channel_Parameters STC_A, STC_B, STC_C;
};

#define STC_A ((STC_SongInfo *)info.data)->STC_A
#define STC_B ((STC_SongInfo *)info.data)->STC_B
#define STC_C ((STC_SongInfo *)info.data)->STC_C
#define STC ((STC_SongInfo *)info.data)->STC

#define ST_PositionsPointer (header->ST_PositionsPointer0 | (header->ST_PositionsPointer1 << 8))
#define ST_OrnamentsPointer (header->ST_OrnamentsPointer0 | (header->ST_OrnamentsPointer1 << 8))
#define ST_PatternsPointer (header->ST_PatternsPointer0 | (header->ST_PatternsPointer1 << 8))
#define ST_Size (header->ST_Size0 | (header->ST_Size1 << 8))
#define ST_Delay (header->ST_Delay)

void STC_Init(AYSongInfo &info)
{
    unsigned char *module = info.module;
    STC_File *header = (STC_File *)module;
    if(info.data)
    {
        delete (STC_SongInfo *)info.data;
        info.data = 0;
    }
    info.data = (void *)new STC_SongInfo;
    if(!info.data)
        return;
    memset(&STC_A, 0, sizeof(STC_Channel_Parameters));
    memset(&STC_B, 0, sizeof(STC_Channel_Parameters));
    memset(&STC_C, 0, sizeof(STC_Channel_Parameters));

    STC.CurrentPosition = 0;
    STC.Transposition = module[ST_PositionsPointer + 2];
    STC.DelayCounter = 1;

    unsigned long i = 0;
    while(module[ST_PatternsPointer + 7 * i] != module[ST_PositionsPointer + 1])
        i++;
    STC_A.Address_In_Pattern = ay_sys_getword(&module[ST_PatternsPointer + 7 * i + 1]);
    STC_B.Address_In_Pattern = ay_sys_getword(&module[ST_PatternsPointer + 7 * i + 3]);
    STC_C.Address_In_Pattern = ay_sys_getword(&module[ST_PatternsPointer + 7 * i + 5]);

    STC_A.Note_Skip_Counter = 0;
    STC_A.Envelope_Enabled = false;
    STC_A.Number_Of_Notes_To_Skip = 0;
    STC_A.Sample_Tik_Counter = -1;
    STC_A.Position_In_Sample = 0;
    STC_A.OrnamentPointer = ST_OrnamentsPointer + 1;
    STC_A.Ton = 0;

    STC_B.Note_Skip_Counter = 0;
    STC_B.Envelope_Enabled = false;
    STC_B.Number_Of_Notes_To_Skip = 0;
    STC_B.Sample_Tik_Counter = -1;
    STC_B.Position_In_Sample = 0;
    STC_B.OrnamentPointer = ST_OrnamentsPointer + 1;
    STC_B.Ton = 0;

    STC_C.Note_Skip_Counter = 0;
    STC_C.Envelope_Enabled = false;
    STC_C.Number_Of_Notes_To_Skip = 0;
    STC_C.Sample_Tik_Counter = -1;
    STC_C.Position_In_Sample = 0;
    STC_C.OrnamentPointer = ST_OrnamentsPointer + 1;
    STC_C.Ton = 0;

    ay_resetay(&info, 0);
}

void STC_GetInfo(AYSongInfo &info)
{
    unsigned char *module = info.file_data;
    STC_File *header = (STC_File *)module;
    unsigned long tm = 0;
    signed long j, j1, j2, i;
    unsigned char stDelay = module[0];
    unsigned short stPosPt = ay_sys_getword(&module[1]);
    unsigned short stPatPt = ay_sys_getword(&module[5]);
    unsigned char a;

    j = -1;
    do
    {
        j++;
        j2 = stPosPt + j * 2 + 1;
        j2 = module[j2];
        i = -1;
        do
        {
            i++;
            j1 = stPatPt + 7 * i;
        }
        while(module[j1] != j2);
        j1 = ay_sys_getword(&module[j1 + 1]);
        a = 1;
        while(module[j1] != 255)
        {
            unsigned char val = module[j1];
            if((val <= 0x5f) || (val == 0x80) || (val == 0x81))
            {
                tm += a;
            }
            else if(val >= 0xa1 && val <= 0xe0)
            {
                a = val - 0xa0;
            }
            else if(val >= 0x83 && val <= 0x8e)
            {
                j1++;
            }
            j1++;
        }
    }
    while(j != module[stPosPt]);
    tm *= stDelay;
    info.Length = tm;

    unsigned char *st_name = (unsigned char *)header->ST_Name;

    if(!memcmp(st_name, "SONG BY ST COMPILE", 18) || !memcmp(st_name, "SONG BY MB COMPILE", 18) || !memcmp(st_name, "SONG BY ST-COMPILE", 18) || !memcmp(st_name, "SOUND TRACKER v1.1", 18) || !memcmp(st_name, "S.T.FULL EDITION ", 17) || !memcmp(st_name, "SOUND TRACKER v1.3", 18))
        return;
    unsigned long len = 18;
    if(st_name[18] >= 32 && st_name[18] <= 127)
    {
        len++;
        if(st_name[19] >= 32 && st_name[19] <= 127)
            len++;
    }
    info.Name = ay_sys_getstr(st_name, len);
}

void STC_PatternInterpreter(AYSongInfo &info, STC_Channel_Parameters &chan)
{
    unsigned short k;
    unsigned char *module = info.module;
    STC_File *header = (STC_File *)module;
    while(true)
    {
        unsigned char val = module[chan.Address_In_Pattern];
        if(val <= 0x5f)
        {
            chan.Note = val;
            chan.Sample_Tik_Counter = 32;
            chan.Position_In_Sample = 0;
            chan.Address_In_Pattern++;
            break;
        }
        else if(val >= 0x60 && val <= 0x6f)
        {
            k = 0;
            while(module[0x1b + 0x63 * k] != (val - 0x60))
                k++;
            chan.SamplePointer = 0x1c + 0x63 * k;
        }
        else if(val >= 0x70 && val <= 0x7f)
        {
            k = 0;
            while(module[ST_OrnamentsPointer + 0x21 * k] != (val - 0x70))
                k++;
            chan.OrnamentPointer = ST_OrnamentsPointer + 0x21 * k + 1;
            chan.Envelope_Enabled = false;
        }
        else if(val == 0x80)
        {
            chan.Sample_Tik_Counter = -1;
            chan.Address_In_Pattern++;
            break;
        }
        else if(val == 0x81)
        {
            chan.Address_In_Pattern++;
            break;
        }
        else if(val == 0x82)
        {
            k = 0;
            while(module[ST_OrnamentsPointer + 0x21 * k] != 0)
                k++;
            chan.OrnamentPointer = ST_OrnamentsPointer + 0x21 * k + 1;
            chan.Envelope_Enabled = false;
        }
        else if(val >= 0x83 && val <= 0x8e)
        {
            ay_writeay(&info, AY_ENV_SHAPE, val - 0x80);
            chan.Address_In_Pattern++;
            ay_writeay(&info, AY_ENV_FINE, module[chan.Address_In_Pattern]);
            chan.Envelope_Enabled = true;
            k = 0;
            while(module[ST_OrnamentsPointer + 0x21 * k] != 0)
                k++;
            chan.OrnamentPointer = ST_OrnamentsPointer + 0x21 * k + 1;
        }
        else
            chan.Number_Of_Notes_To_Skip = val - 0xa1;
        chan.Address_In_Pattern++;
    }
    chan.Note_Skip_Counter = chan.Number_Of_Notes_To_Skip;
}

void STC_GetRegisters(AYSongInfo &info, STC_Channel_Parameters &chan, unsigned char &TempMixer)
{
    unsigned short i;
    unsigned char j;
    unsigned char *module = info.module;
    STC_File *header = (STC_File *)module;
    if(chan.Sample_Tik_Counter >= 0)
    {
        chan.Sample_Tik_Counter--;
        chan.Position_In_Sample = (chan.Position_In_Sample + 1) & 0x1f;
        if(chan.Sample_Tik_Counter == 0)
        {
            if(module[chan.SamplePointer + 0x60] != 0)
            {
                chan.Position_In_Sample = module[chan.SamplePointer + 0x60] & 0x1f;
                chan.Sample_Tik_Counter = module[chan.SamplePointer + 0x61] + 1;
            }
            else
                chan.Sample_Tik_Counter = -1;
        }
    }
    if(chan.Sample_Tik_Counter >= 0)
    {
        i = ((chan.Position_In_Sample - 1) & 0x1f) * 3 + chan.SamplePointer;
        if((module[i + 1] & 0x80) != 0)
            TempMixer = TempMixer | 64;
        else
            ay_writeay(&info, AY_NOISE_PERIOD, module[i + 1] & 0x1f);
        if((module[i + 1] & 0x40) != 0)
            TempMixer = TempMixer | 8;
        chan.Amplitude = module[i] & 15;
        j = chan.Note + module[chan.OrnamentPointer + ((chan.Position_In_Sample - 1) & 0x1f)] + STC.Transposition;
        if(j > 95)
            j = 95;
        if((module[i + 1] & 0x20) != 0)
            chan.Ton = (ST_Table[j] + module[i + 2] + (((unsigned short)(module[i] & 0xf0)) << 4)) & 0xFFF;
        else
            chan.Ton = (ST_Table[j] - module[i + 2] - (((unsigned short)(module[i] & 0xf0)) << 4)) & 0xFFF;
        if(chan.Envelope_Enabled)
            chan.Amplitude = chan.Amplitude | 16;
    }
    else
        chan.Amplitude = 0;

    TempMixer = TempMixer >> 1;
}

void STC_Play(AYSongInfo &info)
{
    unsigned char TempMixer;
    unsigned short i;
    unsigned char *module = info.module;
    STC_File *header = (STC_File *)module;

    STC.DelayCounter--;
    if(STC.DelayCounter == 0)
    {
        STC.DelayCounter = ST_Delay;
        STC_A.Note_Skip_Counter--;
        if(STC_A.Note_Skip_Counter < 0)
        {
            if(module[STC_A.Address_In_Pattern] == 255)
            {
                if(STC.CurrentPosition == module[ST_PositionsPointer])
                    STC.CurrentPosition = 0;
                else
                    STC.CurrentPosition++;
                STC.Transposition = module[ST_PositionsPointer + 2 + STC.CurrentPosition * 2];
                i = 0;
                while(module[ST_PatternsPointer + 7 * i] != module[ST_PositionsPointer + 1 + STC.CurrentPosition * 2])
                    i++;
                STC_A.Address_In_Pattern = ay_sys_getword(&module[ST_PatternsPointer + 7 * i + 1]);
                STC_B.Address_In_Pattern = ay_sys_getword(&module[ST_PatternsPointer + 7 * i + 3]);
                STC_C.Address_In_Pattern = ay_sys_getword(&module[ST_PatternsPointer + 7 * i + 5]);
            }
            STC_PatternInterpreter(info, STC_A);
        }

        STC_B.Note_Skip_Counter--;
        if(STC_B.Note_Skip_Counter < 0)
            STC_PatternInterpreter(info, STC_B);
        STC_C.Note_Skip_Counter--;
        if(STC_C.Note_Skip_Counter < 0)
            STC_PatternInterpreter(info, STC_C);
    }

    TempMixer = 0;
    STC_GetRegisters(info, STC_A, TempMixer);
    STC_GetRegisters(info, STC_B, TempMixer);
    STC_GetRegisters(info, STC_C, TempMixer);

    ay_writeay(&info, AY_MIXER, TempMixer);

    ay_writeay(&info, AY_CHNL_A_FINE, STC_A.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_A_COARSE, (STC_A.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_B_FINE, STC_B.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_B_COARSE, (STC_B.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_C_FINE, STC_C.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_C_COARSE, (STC_C.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_A_VOL, STC_A.Amplitude);
    ay_writeay(&info, AY_CHNL_B_VOL, STC_B.Amplitude);
    ay_writeay(&info, AY_CHNL_C_VOL, STC_C.Amplitude);
}

void STC_Cleanup(AYSongInfo &info)
{
    if(info.data)
    {
        delete (STC_SongInfo *)info.data;
        info.data = 0;
    }
}

bool STC_Detect(unsigned char *module, unsigned long length)
{
    STC_File *header = (STC_File *)module;
    int j, j1, j2;
    if(length < 6)
        return false;
    if(ST_PositionsPointer> length)
        return false;
    if((int)(ST_PatternsPointer- ST_OrnamentsPointer) <= 0)
    return false;
    if(((ST_PatternsPointer - ST_OrnamentsPointer) % 0x21) != 0)
    return false;
    if((int)(ST_PositionsPointer - ST_OrnamentsPointer) >= 0)
    return false;
    if((module[ST_PositionsPointer] * 2 + 3 + ST_PositionsPointer - ST_OrnamentsPointer) != 0)
    return false;

    j = ST_OrnamentsPointer + 0x21;
    if(j> 65535)
    return false;
    if(j> length)
    return false;
    do
    {
        j--;
        if(module[j] != 0)
        return false;;
    }
    while(j != ST_OrnamentsPointer);

    j = ST_PatternsPointer;
    if(j> length)
    return false;
    j1 = 0;
    j2 = 0;
    while((j + 6 <= length) && (j + 6 < 65536) && (module[j] != 255))
    {
        j++;
        j2 = ay_sys_getword(&module[j]);
        if(j1 < j2)
        j1 = j2;
        j += 2;
        j2 = ay_sys_getword(&module[j]);
        if(j1 < j2)
        j1 = j2;
        j += 2;
        j2 = ay_sys_getword(&module[j]);
        if(j1 < j2)
        j1 = j2;
        j += 2;
    }
    if(module[j] != 255)
    return false;
    if(j1> length)
    return false;
    if(module[j1 - 1] != 255)
    return false;
    do
    {
        if((module[j1] >= 0x83) && (module[j1] <= 0x8e))
        j1++;
        j1++;
    }
    while((j1 <= 65535) && (j1 < length) && (module[j1] != 255));
    if(j1> 65535)
    return false;
    if(j1> length)
    return false;

    return true;
}

