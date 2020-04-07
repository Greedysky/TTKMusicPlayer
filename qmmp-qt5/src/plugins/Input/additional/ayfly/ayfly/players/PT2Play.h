struct PT2_File
{
    unsigned char PT2_Delay;
    unsigned char PT2_NumberOfPositions;
    unsigned char PT2_LoopPosition;
    unsigned char PT2_SamplesPointers0[64];
    unsigned char PT2_OrnamentsPointers0[32];
    unsigned char PT2_PatternsPointer0, PT2_PatternsPointer1;
    char PT2_MusicName[30];
    unsigned char PT2_PositionList[65535 - 131];
};

struct PT2_Channel_Parameters
{
    unsigned short Address_In_Pattern, OrnamentPointer, SamplePointer, Ton;
    unsigned char Loop_Ornament_Position, Ornament_Length, Position_In_Ornament, Loop_Sample_Position, Sample_Length, Position_In_Sample, Volume, Number_Of_Notes_To_Skip, Note, Slide_To_Note, Amplitude;
    signed char Current_Ton_Sliding, Ton_Delta;
    int GlissType;
    bool Envelope_Enabled, Enabled;
    signed char Glissade, Addition_To_Noise, Note_Skip_Counter;
};

struct PT2_Parameters
{
    unsigned char DelayCounter, Delay, CurrentPosition;
};

struct PT2_SongInfo
{
    PT2_Parameters PT2;
    PT2_Channel_Parameters PT2_A, PT2_B, PT2_C;
};

#define PT2_A ((PT2_SongInfo *)info.data)->PT2_A
#define PT2_B ((PT2_SongInfo *)info.data)->PT2_B
#define PT2_C ((PT2_SongInfo *)info.data)->PT2_C
#define PT2 ((PT2_SongInfo *)info.data)->PT2

#define PT2_SamplesPointers(x) (header->PT2_SamplesPointers0 [(x) * 2] | (header->PT2_SamplesPointers0 [(x) * 2 + 1] << 8))
#define PT2_OrnamentsPointers(x) (header->PT2_OrnamentsPointers0 [(x) * 2] | (header->PT2_OrnamentsPointers0 [(x) * 2 + 1] << 8))
#define PT2_PositionList(x) (header->PT2_PositionList [(x)])
#define PT2_PatternsPointer (header->PT2_PatternsPointer0 | (header->PT2_PatternsPointer1 << 8))
#define PT2_Delay (header->PT2_Delay)
#define PT2_NumberOfPositions (header->PT2_NumberOfPositions)
#define PT2_LoopPosition (header->PT2_LoopPosition)

void PT2_Init(AYSongInfo &info)
{
    unsigned char *module = info.module;
    PT2_File *header = (PT2_File *)module;
    if(info.data)
    {
        delete (PT2_SongInfo *)info.data;
        info.data = 0;
    }
    info.data = (void *)new PT2_SongInfo;
    if(!info.data)
        return;
    memset(&PT2_A, 0, sizeof(PT2_Channel_Parameters));
    memset(&PT2_B, 0, sizeof(PT2_Channel_Parameters));
    memset(&PT2_C, 0, sizeof(PT2_Channel_Parameters));
    PT2.DelayCounter = 1;
    PT2.Delay = PT2_Delay;
    PT2.CurrentPosition = 0;

    PT2_A.Address_In_Pattern = ay_sys_getword(&module[PT2_PatternsPointer+ PT2_PositionList(0) * 6]);
    PT2_B.Address_In_Pattern = ay_sys_getword(&module [PT2_PatternsPointer + PT2_PositionList(0) * 6 + 2]);
    PT2_C.Address_In_Pattern = ay_sys_getword(&module [PT2_PatternsPointer + PT2_PositionList(0) * 6 + 4]);

    PT2_A.OrnamentPointer = PT2_OrnamentsPointers(0);
    PT2_A.Ornament_Length = module[PT2_A.OrnamentPointer];
    PT2_A.OrnamentPointer++;
    PT2_A.Loop_Ornament_Position = module[PT2_A.OrnamentPointer];
    PT2_A.OrnamentPointer++;
    PT2_A.Envelope_Enabled = false;
    PT2_A.Position_In_Sample = 0;
    PT2_A.Position_In_Ornament = 0;
    PT2_A.Addition_To_Noise = 0;
    PT2_A.Glissade = 0;
    PT2_A.Current_Ton_Sliding=0;
    PT2_A.GlissType = 0;
    PT2_A.Enabled = false;
    PT2_A.Number_Of_Notes_To_Skip = 0;
    PT2_A.Note_Skip_Counter = 0;
    PT2_A.Volume = 15;
    PT2_A.Ton = 0;

    PT2_B.OrnamentPointer = PT2_A.OrnamentPointer;
    PT2_B.Loop_Ornament_Position = PT2_A.Loop_Ornament_Position;
    PT2_B.Ornament_Length = PT2_A.Ornament_Length;
    PT2_B.Envelope_Enabled = false;
    PT2_B.Position_In_Sample = 0;
    PT2_B.Position_In_Ornament = 0;
    PT2_B.Addition_To_Noise = 0;
    PT2_B.Glissade = 0;
    PT2_B.Current_Ton_Sliding = 0;
    PT2_B.GlissType = 0;
    PT2_B.Enabled = false;
    PT2_B.Number_Of_Notes_To_Skip = 0;
    PT2_B.Note_Skip_Counter = 0;
    PT2_B.Volume = 15;
    PT2_B.Ton = 0;

    PT2_C.OrnamentPointer = PT2_A.OrnamentPointer;
    PT2_C.Loop_Ornament_Position = PT2_A.Loop_Ornament_Position;
    PT2_C.Ornament_Length = PT2_A.Ornament_Length;
    PT2_C.Envelope_Enabled = false;
    PT2_C.Position_In_Sample = 0;
    PT2_C.Position_In_Ornament = 0;
    PT2_C.Addition_To_Noise = 0;
    PT2_C.Glissade = 0;
    PT2_C.Current_Ton_Sliding = 0;
    PT2_C.GlissType = 0;
    PT2_C.Enabled = false;
    PT2_C.Number_Of_Notes_To_Skip = 0;
    PT2_C.Note_Skip_Counter = 0;
    PT2_C.Volume = 15;
    PT2_C.Ton = 0;

    ay_resetay(&info, 0);

}

void PT2_GetInfo(AYSongInfo &info)
{
    unsigned char *module = info.file_data;
    short a1, a2, a3, a11, a22, a33;
    unsigned long j1, j2, j3;
    long i, tm = 0;
    unsigned char b;
    unsigned char ptDelay = module[0];
    unsigned char ptNumPos = module[1];
    unsigned short ptLoopPos = module[2];
    unsigned short ptPatPt = ay_sys_getword(&module[99]);
    const unsigned char *ptPosList = (unsigned char *)&module[131];

    b = ptDelay;
    a1 = a2 = a3 = a11 = a22 = a33 = 0;
    for(i = 0; i < ptNumPos; i++)
    {
        if(i == ptLoopPos)
        {
            info.Loop = tm;
        }
        j1 = ay_sys_getword(&module[ptPatPt + ptPosList[i] * 6]);
        j2 = ay_sys_getword(&module[ptPatPt + ptPosList[i] * 6 + 2]);
        j3 = ay_sys_getword(&module[ptPatPt + ptPosList[i] * 6 + 4]);
        do
        {
            a1--;
            if(a1 < 0)
            {
                if(module[j1] == 0)
                    break;
                do
                {
                    unsigned char val = (unsigned char)module[j1];
                    if(val == 0x70 || (val >= 0x80 && val <= 0xe0))
                    {
                        a1 = a11;
                        j1++;
                        break;
                    }
                    else if(val >= 0x71 && val <= 0x7e)
                    {
                        j1 += 2;
                    }
                    else if(val >= 0x20 && val <= 0x5f)
                    {
                        a11 = module[j1] - 0x20;
                    }
                    else if(val == 0xf)
                    {
                        j1++;
                        b = module[j1];
                    }
                    else if((val >= 1 && val <= 0xb) || val == 0xe)
                    {
                        j1++;
                    }
                    else if(val == 0xd)
                    {
                        j1 += 3;
                    }
                    j1++;
                }
                while(true);
            }

            a2--;
            if(a2 < 0)
            {
                do
                {
                    unsigned char val = (unsigned char)module[j2];
                    if(val == 0x70 || (val >= 0x80 && val <= 0xe0))
                    {
                        a2 = a22;
                        j2++;
                        break;
                    }
                    else if(val >= 0x71 && val <= 0x7e)
                    {
                        j2 += 2;
                    }
                    else if(val >= 0x20 && val <= 0x5f)
                    {
                        a22 = module[j2] - 0x20;
                    }
                    else if(val == 0xf)
                    {
                        j2++;
                        b = module[j2];
                    }
                    else if((val >= 1 && val <= 0xb) || val == 0xe)
                    {
                        j2++;
                    }
                    else if(val == 0xd)
                    {
                        j2 += 3;
                    }
                    j2++;
                }
                while(true);
            }
            a3--;
            if(a3 < 0)
            {
                do
                {
                    unsigned char val = (unsigned char)module[j3];
                    if(val == 0x70 || (val >= 0x80 && val <= 0xe0))
                    {
                        a3 = a33;
                        j3++;
                        break;
                    }
                    else if(val >= 0x71 && val <= 0x7e)
                    {
                        j3 += 2;
                    }
                    else if(val >= 0x20 && val <= 0x5f)
                    {
                        a33 = module[j3] - 0x20;
                    }
                    else if(val == 0xf)
                    {
                        j3++;
                        b = module[j3];
                    }
                    else if((val >= 1 && val <= 0xb) || val == 0xe)
                    {
                        j3++;
                    }
                    else if(val == 0xd)
                    {
                        j3 += 3;
                    }
                    j3++;
                }
                while(true);
            }
            tm += b;
        }
        while(true);
    }
    info.Length = tm;
    unsigned char *ptr = module + 101;
    info.Name = ay_sys_getstr(ptr, 30);
}

void PT2_PatternInterpreter(AYSongInfo &info, PT2_Channel_Parameters &chan)
{
    unsigned char *module = info.module;
    PT2_File *header = (PT2_File *)module;
    bool quit, gliss;
    quit = false;
    gliss = false;
    do
    {
        unsigned char val = module[chan.Address_In_Pattern];
        if(val >= 0xe1)
        {
            chan.SamplePointer = PT2_SamplesPointers(val - 0xe0);
            chan.Sample_Length = module[chan.SamplePointer];
            chan.SamplePointer++;
            chan.Loop_Sample_Position = module[chan.SamplePointer];
            chan.SamplePointer++;
        }
        else if(val == 0xe0)
        {
            chan.Position_In_Sample = 0;
            chan.Position_In_Ornament = 0;
            chan.Current_Ton_Sliding = 0;
            chan.GlissType = 0;
            chan.Enabled = false;
            quit = true;
        }
        else if(val >= 0x80 && val <= 0xdf)
        {
            chan.Position_In_Sample = 0;
            chan.Position_In_Ornament = 0;
            chan.Current_Ton_Sliding = 0;
            if(gliss)
            {
                chan.Slide_To_Note = val - 0x80;
                if(chan.GlissType == 1)
                    chan.Note = chan.Slide_To_Note;
            }
            else
            {
                chan.Note = val - 0x80;
                chan.GlissType = 0;
            }
            chan.Enabled = true;
            quit = true;
        }
        else if(val == 0x7f)
            chan.Envelope_Enabled = false;
        else if(val >= 0x71 && val <= 0x7e)
        {
            chan.Envelope_Enabled = true;
            ay_writeay(&info, AY_ENV_SHAPE, val - 0x70);
            chan.Address_In_Pattern++;
            ay_writeay(&info, AY_ENV_FINE, module[chan.Address_In_Pattern]);
            chan.Address_In_Pattern++;
            ay_writeay(&info, AY_ENV_COARSE, module[chan.Address_In_Pattern]);
        }
        else if(val == 0x70)
            quit = true;
        else if(val >= 0x60 && val <= 0x6f)
        {
            chan.OrnamentPointer = PT2_OrnamentsPointers(val - 0x60);
            chan.Ornament_Length = module[chan.OrnamentPointer];
            chan.OrnamentPointer++;
            chan.Loop_Ornament_Position = module[chan.OrnamentPointer];
            chan.OrnamentPointer++;
            chan.Position_In_Ornament = 0;
        }
        else if(val >= 0x20 && val <= 0x5f)
            chan.Number_Of_Notes_To_Skip = val - 0x20;
        else if(val >= 0x10 && val <= 0x1f)
            chan.Volume = val - 0x10;
        else if(val == 0xf)
        {
            chan.Address_In_Pattern++;
            PT2.Delay = module[chan.Address_In_Pattern];
        }
        else if(val == 0xe)
        {
            chan.Address_In_Pattern++;
            chan.Glissade = module[chan.Address_In_Pattern];
            chan.GlissType = 1;
            gliss = true;
        }
        else if(val == 0xd)
        {
            chan.Address_In_Pattern++;
            chan.Glissade = abs((signed char)(module[chan.Address_In_Pattern]));
            chan.Address_In_Pattern += 2; //Not use precalculated Ton_Delta
            //to avoide error with first note of pattern
            chan.GlissType = 2;
            gliss = true;
        }
        else if(val == 0xc)
            chan.GlissType = 0;
        else
        {
            chan.Address_In_Pattern++;
            chan.Addition_To_Noise = module[chan.Address_In_Pattern];
        }
        chan.Address_In_Pattern++;
    }
    while(!quit);
    if(gliss && (chan.GlissType == 2))
    {
        chan.Ton_Delta = abs(PT2_Table[chan.Slide_To_Note] - PT2_Table[chan.Note]);
        if(chan.Slide_To_Note > chan.Note)
            chan.Glissade = -chan.Glissade;
    }
    chan.Note_Skip_Counter = chan.Number_Of_Notes_To_Skip;
}

void PT2_GetRegisters(AYSongInfo &info, PT2_Channel_Parameters &chan, unsigned char &TempMixer)
{
    unsigned char j, b0, b1;
    unsigned char *module = info.module;
    PT2_File *header = (PT2_File *)module;
    if(chan.Enabled)
    {
        b0 = module[chan.SamplePointer + chan.Position_In_Sample * 3];
        b1 = module[chan.SamplePointer + chan.Position_In_Sample * 3 + 1];
        chan.Ton = module[chan.SamplePointer + chan.Position_In_Sample * 3 + 2] + (unsigned short)((b1 & 15) << 8);
        if((b0 & 4) == 0)
            chan.Ton = -chan.Ton;
        j = chan.Note + module[chan.OrnamentPointer + chan.Position_In_Ornament];
        if(j > 95)
            j = 95;
        chan.Ton = (chan.Ton + chan.Current_Ton_Sliding + PT2_Table[j]) & 0xfff;
        if(chan.GlissType == 2)
        {
            chan.Ton_Delta = chan.Ton_Delta - abs(chan.Glissade);
            if(chan.Ton_Delta < 0)
            {
                chan.Note = chan.Slide_To_Note;
                chan.GlissType = 0;
                chan.Current_Ton_Sliding = 0;
            }
        }
        if(chan.GlissType != 0)
            chan.Current_Ton_Sliding += chan.Glissade;
        chan.Amplitude = (chan.Volume * 17 + (unsigned char)(chan.Volume > 7)) * (b1 >> 4) / 256;
        if(chan.Envelope_Enabled)
            chan.Amplitude = chan.Amplitude | 16;
        if((module[chan.SamplePointer + chan.Position_In_Sample * 3] & 1) != 0)
            TempMixer = TempMixer | 64;
        else
            ay_writeay(&info, AY_NOISE_PERIOD, ((b0 >> 3) + chan.Addition_To_Noise) & 31);
        if((b0 & 2) != 0)
            TempMixer = TempMixer | 8;
        chan.Position_In_Sample++;
        if(chan.Position_In_Sample == chan.Sample_Length)
            chan.Position_In_Sample = chan.Loop_Sample_Position;
        chan.Position_In_Ornament++;
        if(chan.Position_In_Ornament == chan.Ornament_Length)
            chan.Position_In_Ornament = chan.Loop_Ornament_Position;
    }
    else
        chan.Amplitude = 0;
    TempMixer = TempMixer >> 1;
}

void PT2_Play(AYSongInfo &info)
{
    unsigned char TempMixer;
    unsigned char *module = info.module;
    PT2_File *header = (PT2_File *)module;
    PT2.DelayCounter--;
    if(PT2.DelayCounter == 0)
    {
        PT2_A.Note_Skip_Counter--;
        if(PT2_A.Note_Skip_Counter < 0)
        {
            if(module[PT2_A.Address_In_Pattern] == 0)
            {
                PT2.CurrentPosition++;
                if(PT2.CurrentPosition == PT2_NumberOfPositions)
                    PT2.CurrentPosition = PT2_LoopPosition;
                PT2_A.Address_In_Pattern = ay_sys_getword(&module[PT2_PatternsPointer+ PT2_PositionList(PT2.CurrentPosition) * 6]);
                PT2_B.Address_In_Pattern = ay_sys_getword(&module[PT2_PatternsPointer + PT2_PositionList(PT2.CurrentPosition) * 6 + 2]);
                PT2_C.Address_In_Pattern = ay_sys_getword(&module[PT2_PatternsPointer + PT2_PositionList(PT2.CurrentPosition) * 6 + 4]);
            }
            PT2_PatternInterpreter(info, PT2_A);
        }
        PT2_B.Note_Skip_Counter--;
        if(PT2_B.Note_Skip_Counter < 0)
        PT2_PatternInterpreter(info, PT2_B);
        PT2_C.Note_Skip_Counter--;
        if(PT2_C.Note_Skip_Counter < 0)
        PT2_PatternInterpreter(info, PT2_C);
        PT2.DelayCounter = PT2.Delay;
    }

    TempMixer = 0;
    PT2_GetRegisters(info, PT2_A, TempMixer);
    PT2_GetRegisters(info, PT2_B, TempMixer);
    PT2_GetRegisters(info, PT2_C, TempMixer);

    ay_writeay(&info, AY_MIXER, TempMixer);

    ay_writeay(&info, AY_CHNL_A_FINE, PT2_A.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_A_COARSE, (PT2_A.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_B_FINE, PT2_B.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_B_COARSE, (PT2_B.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_C_FINE, PT2_C.Ton & 0xff);
    ay_writeay(&info, AY_CHNL_C_COARSE, (PT2_C.Ton >> 8) & 0xf);
    ay_writeay(&info, AY_CHNL_A_VOL, PT2_A.Amplitude);
    ay_writeay(&info, AY_CHNL_B_VOL, PT2_B.Amplitude);
    ay_writeay(&info, AY_CHNL_C_VOL, PT2_C.Amplitude);

}

void PT2_Cleanup(AYSongInfo &info)
{
    if(info.data)
    {
        delete (PT2_SongInfo *)info.data;
        info.data = 0;
    }
}

bool PT2_Detect0(unsigned char *module, unsigned long length)
{
    int j, j1, j2;
    PT2_File *header = (PT2_File *)module;
    if(length < 132)
        return false;
    if(PT2_PatternsPointer> length)
        return false;
    if(module[PT2_PatternsPointer-1] != 255)
        return false;
    if(PT2_SamplesPointers(0) != 0)
        return false;
    j = 0;
    memcpy(&j, &module[PT2_OrnamentsPointers(0)], 3);
    if(j != 1)
        return false;

    j = ay_sys_getword(&module[PT2_PatternsPointer]);
    if(j > length)
        return false;
    if(j - (int)(PT2_PatternsPointer) <= 0)
        return false;
    if(((j - int(PT2_PatternsPointer)) % 6) != 2)
        return false;

    j1 = 0;
    j2 = 0;
    while(((j2 < 256) && (j2 <= length - 131)) && (header->PT2_PositionList[j2] < 128))
    {
        if((unsigned long)(j1) < header->PT2_PositionList[j2])
            j1 = header->PT2_PositionList[j2];
        j2++;
    }
    if(((j - (int)(PT2_PatternsPointer)) / 6) != j1 + 1)
        return false;

    j = 15;
    while((j > 0) && (PT2_OrnamentsPointers(j) == 0))
        j--;
    int F_Length = PT2_OrnamentsPointers(j) + module[PT2_OrnamentsPointers(j)] + 2;
    if(F_Length > length + 1)
        return false;

    PT2_NumberOfPositions = j2;
    return true;
}

bool PT2_Detect1(unsigned char *module, unsigned long length)
{
    int j, j1, j2, j3;
    PT2_File *header = (PT2_File *)module;
    if(length < 132)
        return false;
    if(PT2_PatternsPointer> length)
        return false;
    if(module[PT2_PatternsPointer-1] != 255)
        return false;
    if(PT2_SamplesPointers(0) != 0)
        return false;

    j3 = PT2_SamplesPointers(0);
    if(PT2_OrnamentsPointers(0) - j3 - 2 > length)
        return false;
    if(PT2_OrnamentsPointers(0)- j3 < 0)
        return false;

    j = 0;
    memcpy(&j, &module[PT2_OrnamentsPointers(0)], 3);
    if(j != 1)
        return false;

    j = ay_sys_getword(&module[PT2_PatternsPointer]);
    j -= j3;
    if(j > length)
        return false;
    if(j - (int)(PT2_PatternsPointer) <= 0)
        return false;
    if(((j - int(PT2_PatternsPointer)) % 6) != 2)
        return false;

    j1 = 0;
    j2 = 0;
    while(((j2 < 256) && (j2 <= length - 131)) && (header->PT2_PositionList[j2] < 128))
    {
        if((unsigned long)(j1) < header->PT2_PositionList[j2])
            j1 = header->PT2_PositionList[j2];
        j2++;
    }
    if(((j - (int)(PT2_PatternsPointer)) / 6) != j1 + 1)
        return false;

    j = 15;
    while((j > 0) && (PT2_OrnamentsPointers(j) == j3))
        j--;
    int F_Length = PT2_OrnamentsPointers(j) - j3 + module[PT2_OrnamentsPointers(j) - j3] + 2;
    if(F_Length > length + 1)
        return false;

    PT2_NumberOfPositions = j2;
    for(j = 0; j < 32; j++)
    {
        ay_sys_writeword(&header->PT2_SamplesPointers0[j * 2], PT2_SamplesPointers(j) - j3);
    }
    for(j = 0; j < 16; j++)
    {
        ay_sys_writeword(&header->PT2_OrnamentsPointers0[j * 2], PT2_OrnamentsPointers(j) - j3);
    }
    for(j2 = 0; j2 < j1 * 3 + 1; j2++)
    {
        j = ay_sys_getword(&module[PT2_PatternsPointer+j2 * 2]);
        j -= j3;
        ay_sys_writeword(&module[PT2_PatternsPointer+j2 * 2], j);
    }
    return true;
}

bool PT2_Detect(unsigned char *module, unsigned long length)
{
    if(!PT2_Detect0(module, length))
        if(!PT2_Detect1(module, length))
            return false;
    return true;

}
