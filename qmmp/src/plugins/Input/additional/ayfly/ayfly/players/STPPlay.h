//Sound tracker PRO player. KSA.

struct STP_File
{
    unsigned char STP_Delay;
    unsigned char STP_PositionsPointer0, STP_PositionsPointer1;
    unsigned char STP_PatternsPointer0, STP_PatternsPointer1;
    unsigned char STP_OrnamentsPointer0, STP_OrnamentsPointer1;
    unsigned char STP_SamplesPointer0, STP_SamplesPointer1;
    unsigned char STP_Init_Id;
};

struct STP_Channel_Parameters
{
    unsigned short OrnamentPointer, SamplePointer, Address_In_Pattern, Ton;
    unsigned char Position_In_Ornament, Loop_Ornament_Position, Ornament_Length, Position_In_Sample, Loop_Sample_Position, Sample_Length, Volume, Number_Of_Notes_To_Skip, Note, Amplitude;
    signed short Current_Ton_Sliding;
    bool Envelope_Enabled, Enabled;
    signed char Glissade, Note_Skip_Counter;
};

struct STP_Parameters
{
    unsigned char DelayCounter, CurrentPosition, Transposition;
};

struct STP_SongInfo
{
    STP_Parameters STP;
    STP_Channel_Parameters STP_A, STP_B, STP_C;
};

#define STP_A (((STP_SongInfo *)info.data)->STP_A)
#define STP_B (((STP_SongInfo *)info.data)->STP_B)
#define STP_C (((STP_SongInfo *)info.data)->STP_C)
#define STP (((STP_SongInfo *)info.data)->STP)

static const char KsaId[] = "KSA SOFTWARE COMPILATION OF ";

#define STP_PositionsPointer (header->STP_PositionsPointer0 | (header->STP_PositionsPointer1 << 8))
#define STP_OrnamentsPointer (header->STP_OrnamentsPointer0 | (header->STP_OrnamentsPointer1 << 8))
#define STP_PatternsPointer (header->STP_PatternsPointer0 | (header->STP_PatternsPointer1 << 8))
#define STP_SamplesPointer (header->STP_SamplesPointer0 | (header->STP_SamplesPointer1 << 8))
#define STP_Delay (header->STP_Delay)

void STP_Init(AYSongInfo &info)
{
    unsigned char *module = info.module;
    STP_File *header = (STP_File *)module;
    if(info.data)
    {
        delete (STP_SongInfo *)info.data;
        info.data = 0;
    }
    info.data = (void *)new STP_SongInfo;
    if(!info.data)
        return;
    memset(&STP_A, 0, sizeof(STP_Channel_Parameters));
    memset(&STP_B, 0, sizeof(STP_Channel_Parameters));
    memset(&STP_C, 0, sizeof(STP_Channel_Parameters));

    STP.DelayCounter = 1;
    STP.Transposition = module[STP_PositionsPointer + 3];
    STP.CurrentPosition = 0;
    STP_A.Address_In_Pattern = ay_sys_getword(&module[STP_PatternsPointer + module[STP_PositionsPointer + 2]]);
    STP_B.Address_In_Pattern = ay_sys_getword(&module[STP_PatternsPointer + module[STP_PositionsPointer + 2] + 2]);
    STP_C.Address_In_Pattern = ay_sys_getword(&module[STP_PatternsPointer + module[STP_PositionsPointer + 2] + 4]);

    STP_A.SamplePointer = ay_sys_getword(&module[STP_SamplesPointer]);
    STP_A.Loop_Sample_Position = module[STP_A.SamplePointer];
    STP_A.SamplePointer++;
    STP_A.Sample_Length = module[STP_A.SamplePointer];
    STP_A.SamplePointer++;
    STP_B.SamplePointer = STP_A.SamplePointer;
    STP_B.Loop_Sample_Position = STP_A.Loop_Sample_Position;
    STP_B.Sample_Length = STP_A.Sample_Length;
    STP_C.SamplePointer = STP_A.SamplePointer;
    STP_C.Loop_Sample_Position = STP_A.Loop_Sample_Position;
    STP_C.Sample_Length = STP_A.Sample_Length;

    STP_A.OrnamentPointer = ay_sys_getword(&module[STP_OrnamentsPointer]);
    STP_A.Loop_Ornament_Position = module[STP_A.OrnamentPointer];
    STP_A.OrnamentPointer++;
    STP_A.Ornament_Length = module[STP_A.OrnamentPointer];
    STP_A.OrnamentPointer++;
    STP_B.OrnamentPointer = STP_A.OrnamentPointer;
    STP_B.Loop_Ornament_Position = STP_A.Loop_Ornament_Position;
    STP_B.Ornament_Length = STP_A.Ornament_Length;
    STP_C.OrnamentPointer = STP_A.OrnamentPointer;
    STP_C.Loop_Ornament_Position = STP_A.Loop_Ornament_Position;
    STP_C.Ornament_Length = STP_A.Ornament_Length;

    STP_A.Envelope_Enabled = false;
    STP_A.Glissade = 0;
    STP_A.Current_Ton_Sliding = 0;
    STP_A.Enabled = false;
    STP_A.Number_Of_Notes_To_Skip = 0;
    STP_A.Note_Skip_Counter = 0;
    STP_A.Volume = 0;
    STP_A.Ton = 0;

    STP_B.Envelope_Enabled = false;
    STP_B.Glissade = 0;
    STP_B.Current_Ton_Sliding = 0;
    STP_B.Enabled = false;
    STP_B.Number_Of_Notes_To_Skip = 0;
    STP_B.Note_Skip_Counter = 0;
    STP_B.Volume = 0;
    STP_B.Ton = 0;

    STP_C.Envelope_Enabled = false;
    STP_C.Glissade = 0;
    STP_C.Current_Ton_Sliding = 0;
    STP_C.Enabled = false;
    STP_C.Number_Of_Notes_To_Skip = 0;
    STP_C.Note_Skip_Counter = 0;
    STP_C.Volume = 0;
    STP_C.Ton = 0;

    ay_resetay(&info, 0);
}

void STP_GetInfo(AYSongInfo &info)
{
    unsigned char *module = info.file_data;
    unsigned long tm = 0;
    unsigned char a = 1;
    unsigned long i, j1;
    unsigned char stDelay = module[0];
    unsigned short stPosPt = ay_sys_getword(&module[1]);
    unsigned long stPatPt = ay_sys_getword(&module[3]);

    for(i = 0; i < module[stPosPt]; i++)
    {
        if(i == module[stPosPt + 1])
            info.Loop = tm * stDelay;
        j1 = ay_sys_getword(&module[stPatPt + module[stPosPt + 2 + i * 2]]);
        while(module[j1] != 0)
        {
            unsigned char val = module[j1];
            if((val >= 1 && val <= 0x60) || (val >= 0xd0 && val <= 0xef))
            {
                tm += a;
            }
            else if(val >= 0x80 && val <= 0xbf)
            {
                a = val - 0x7f;
            }
            else if((val >= 0xc0 && val <= 0xcf) || val == 0xf0)
            {
                j1++;
            }
            j1++;
        }
    }
    tm *= stDelay;
    info.Length = tm;
    if(!memcmp(&module[10], KsaId, 28))
        info.Name = ay_sys_getstr(&module[38], 25);
}

void STP_PatternInterpreter(AYSongInfo &info, STP_Channel_Parameters &chan)
{
    unsigned char *module = info.module;
    STP_File *header = (STP_File *)module;
    bool quit = false;

    do
    {
        unsigned char val = module[chan.Address_In_Pattern];
        if(val >= 1 && val <= 0x60)
        {
            chan.Note = val - 1;
            chan.Position_In_Sample = 0;
            chan.Position_In_Ornament = 0;
            chan.Current_Ton_Sliding = 0;
            chan.Enabled = true;
            quit = true;
        }
        else if(val >= 0x61 && val <= 0x6f)
        {
            chan.SamplePointer = ay_sys_getword(&module[STP_SamplesPointer + (val - 0x61) * 2]);
            chan.Loop_Sample_Position = module[chan.SamplePointer];
            chan.SamplePointer++;
            chan.Sample_Length = module[chan.SamplePointer];
            chan.SamplePointer++;
        }
        else if(val >= 0x70 && val <= 0x7f)
        {
            chan.OrnamentPointer = ay_sys_getword(&module[STP_OrnamentsPointer + (val - 0x70) * 2]);
            chan.Loop_Ornament_Position = module[chan.OrnamentPointer];
            chan.OrnamentPointer++;
            chan.Ornament_Length = module[chan.OrnamentPointer];
            chan.OrnamentPointer++;
            chan.Envelope_Enabled = false;
            chan.Glissade = 0;
        }
        else if(val >= 0x80 && val <= 0xbf)
        {
            chan.Number_Of_Notes_To_Skip = val - 0x80;
        }
        else if(val >= 0xc0 && val <= 0xcf)
        {
            if(val != 0xc0)
            {
                ay_writeay(&info, AY_ENV_SHAPE, val - 0xc0);
                chan.Address_In_Pattern++;
                ay_writeay(&info, AY_ENV_FINE, module[chan.Address_In_Pattern]);
            }
            chan.Envelope_Enabled = true;
            chan.Loop_Ornament_Position = 0;
            chan.Glissade = 0;
            chan.Ornament_Length = 1;
        }
        else if(val >= 0xd0 && val <= 0xdf)
        {
            chan.Enabled = false;
            quit = true;
        }
        else if(val >= 0xe0 && val <= 0xef)
        {
            quit = true;
        }
        else if(val == 0xf0)
        {
            chan.Address_In_Pattern++;
            chan.Glissade = module[chan.Address_In_Pattern];
        }
        else if(val >= 0xf1 && val <= 0xff)
        {
            chan.Volume = val - 0xf1;
        }
        chan.Address_In_Pattern++;
    }
    while(!quit);
    chan.Note_Skip_Counter = chan.Number_Of_Notes_To_Skip;
}

void STP_GetRegisters(AYSongInfo &info, STP_Channel_Parameters &chan, unsigned char &TempMixer)
{
    unsigned char *module = info.module;
    STP_File *header = (STP_File *)module;
    unsigned char j, b0, b1;
    if(chan.Enabled)
    {
        chan.Current_Ton_Sliding += chan.Glissade;
        if(chan.Envelope_Enabled)
            j = chan.Note + STP.Transposition;
        else
            j = chan.Note + STP.Transposition + module[chan.OrnamentPointer + chan.Position_In_Ornament];
        if(j > 95)
            j = 95;
        b0 = module[chan.SamplePointer + chan.Position_In_Sample * 4];
        b1 = module[chan.SamplePointer + chan.Position_In_Sample * 4 + 1];
        chan.Ton = (ST_Table[j] + chan.Current_Ton_Sliding + ay_sys_getword(&module[chan.SamplePointer + chan.Position_In_Sample * 4 + 2])) & 0xfff;
        chan.Amplitude = (b0 & 15) - chan.Volume;
        if((char)(chan.Amplitude) < 0)
            chan.Amplitude = 0;
        if(((b1 & 1) != 0) && chan.Envelope_Enabled)
            chan.Amplitude = chan.Amplitude | 16;
        TempMixer = ((b0 >> 1) & 0x48) | TempMixer;
        if((char)(b0) >= 0)
            ay_writeay(&info, AY_NOISE_PERIOD, (b1 >> 1) & 31);
        chan.Position_In_Ornament++;
        if(chan.Position_In_Ornament >= chan.Ornament_Length)
            chan.Position_In_Ornament = chan.Loop_Ornament_Position;
        chan.Position_In_Sample++;
        if(chan.Position_In_Sample >= chan.Sample_Length)
        {
            chan.Position_In_Sample = chan.Loop_Sample_Position;
            if((char)(chan.Loop_Sample_Position) < 0)
                chan.Enabled = false;
        }
    }
    else
    {
        TempMixer = TempMixer | 0x48;
        chan.Amplitude = 0;
    }
    TempMixer = TempMixer >> 1;
}

void STP_Play(AYSongInfo &info)
{
    unsigned char *module = info.module;
    STP_File *header = (STP_File *)module;
    unsigned char TempMixer;
    STP.DelayCounter--;
    if(STP.DelayCounter == 0)
    {
        STP.DelayCounter = STP_Delay;
        STP_A.Note_Skip_Counter--;
        if(STP_A.Note_Skip_Counter < 0)
        {
            if(module[STP_A.Address_In_Pattern] == 0)
            {
                STP.CurrentPosition++;
                if(STP.CurrentPosition == module[STP_PositionsPointer])
                    STP.CurrentPosition = module[STP_PositionsPointer + 1];
                STP_A.Address_In_Pattern = ay_sys_getword(&module[STP_PatternsPointer + module[STP_PositionsPointer + 2 + STP.CurrentPosition * 2]]);
                STP_B.Address_In_Pattern = ay_sys_getword(&module[STP_PatternsPointer + module[STP_PositionsPointer + 2 + STP.CurrentPosition * 2] + 2]);
                STP_C.Address_In_Pattern = ay_sys_getword(&module[STP_PatternsPointer + module[STP_PositionsPointer + 2 + STP.CurrentPosition * 2] + 4]);
                STP.Transposition = module[STP_PositionsPointer + 3 + STP.CurrentPosition * 2];
            }
            STP_PatternInterpreter(info, STP_A);
        }
        STP_B.Note_Skip_Counter--;
        if(STP_B.Note_Skip_Counter < 0)
            STP_PatternInterpreter(info, STP_B);
        STP_C.Note_Skip_Counter--;
        if(STP_C.Note_Skip_Counter < 0)
            STP_PatternInterpreter(info, STP_C);
    }

    TempMixer = 0;
    STP_GetRegisters(info, STP_A, TempMixer);
    STP_GetRegisters(info, STP_B, TempMixer);
    STP_GetRegisters(info, STP_C, TempMixer);

    ay_writeay(&info, AY_MIXER, TempMixer);

    ay_writeay(&info, AY_CHNL_A_FINE, STP_A.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_A_COARSE, (STP_A.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_B_FINE, STP_B.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_B_COARSE, (STP_B.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_C_FINE, STP_C.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_C_COARSE, (STP_C.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_A_VOL, STP_A.Amplitude);
    ay_writeay(&info, AY_CHNL_B_VOL, STP_B.Amplitude);
    ay_writeay(&info, AY_CHNL_C_VOL, STP_C.Amplitude);
}

void STP_Cleanup(AYSongInfo &info)
{
    if(info.data)
    {
        delete (STP_SongInfo *)info.data;
        info.data = 0;
    }
}

bool STP_Detect(unsigned char *module, unsigned long length)
{
    STP_File *header = (STP_File *)module;
    int j, j1, j2, j3;
    if(length < 10)
        return false;
    if(STP_PositionsPointer> length)
        return false;
    if(STP_PatternsPointer> length)
        return false;
    if(STP_OrnamentsPointer> length)
        return false;
    if(STP_SamplesPointer> length)
        return false;
    if((STP_SamplesPointer- STP_OrnamentsPointer) != 0x20)
    return false;
    if((int)(STP_OrnamentsPointer - STP_PatternsPointer) <= 0)
    return false;
    if(((STP_OrnamentsPointer - STP_PatternsPointer) % 6) != 0)
    return false;
    if((module[STP_PositionsPointer] * 2 + 2 + STP_PositionsPointer - STP_PatternsPointer) != 0)
    return false;
    int F_Length = STP_SamplesPointer + 30;
    if(F_Length> 65535)
    return false;
    if(F_Length> length + 1)
    return false;

    j2 = 0;
    j3 = header->STP_Init_Id;
    if(j3 == 0)
    {
        j2 = ay_sys_getword(&module[STP_PatternsPointer]);
        if(!strncmp((char *)&module [10], KsaId, strlen(KsaId)))
        j2 -= 0xa + 53;
        else
        j2 -= 0xa;
        if(j2 < 0)
        return false;
        int F_Address = j2;
        j3 = (F_Length - STP_PatternsPointer) / 2;
        for(j1 = 0; j1 < j3; j1++)
        {
            j = ay_sys_getword(&module[STP_PatternsPointer + j1 * 2]);
            j -= j2;
            ay_sys_writeword(&module[STP_PatternsPointer + j1 * 2], j);
        }
    }

    j = ay_sys_getword(&module[STP_OrnamentsPointer]);
    j--;
    if((unsigned long)(j) <= (unsigned long)(length - 1))
    {
        j = ay_sys_getword(&module [j]);
        if(j == 0)
        {
            header->STP_Init_Id = j3;
            return true;
        }
    }

    /*for(j1 = 0; j1 < j3; j1++)
    {
        j = ay_sys_getword(&module[STP_PatternsPointer + j1 * 2]);
        j += j2;
        ay_sys_writeword(&module[STP_PatternsPointer + j1 * 2], j);
    }*/
        return false;
}
