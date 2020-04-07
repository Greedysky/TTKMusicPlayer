//LZH-file header
struct LZHFileHeader
{
    unsigned char HSize;
    unsigned char ChkSum;
    char Method[5];
    unsigned char CompSize0, CompSize1, CompSize2, CompSize3; //int
    unsigned char UCompSize0, UCompSize1, UCompSize2, UCompSize3; //int
    unsigned char Dos_DT0, Dos_DT1, Dos_DT2, Dos_DT3; //int
    unsigned char Attr0, Attr1; //unsigned short
    unsigned char FileNameLen;
};

struct YM5FileHeader
{
    unsigned char Id0, Id1, Id2, Id3; //dword
    char Leo[8];
    unsigned char Num_of_tiks0, Num_of_tiks1, Num_of_tiks2, Num_of_tiks3; //dword
    unsigned char Song_Attr0, Song_Attr1, Song_Attr2, Song_Attr3; //dword
    unsigned char Num_of_Dig0, Num_of_Dig1; //word
    unsigned char ChipFrq0, ChipFrq1, ChipFrq2, ChipFrq3; //dword
    unsigned char InterFrq0, InterFrq1; //word
    unsigned char Loop0, Loop1, Loop2, Loop3; //dword
    unsigned char Add_Size0, Add_Size1; //word
};

#define LZH_CompSize (lzh_header->CompSize0 | (lzh_header->CompSize1 << 8) | (lzh_header->CompSize2 << 16) | (lzh_header->CompSize3 << 24))
#define LZH_UCompSize (lzh_header->UCompSize0 | (lzh_header->UCompSize1 << 8) | (lzh_header->UCompSize2 << 16) | (lzh_header->UCompSize3 << 24))
#define LZH_DosDT (lzh_header->Dos_DT0 | (lzh_header->Dos_DT1 << 8) | (lzh_header->Dos_DT2 << 16) | (lzh_header->Dos_DT3 << 24))
#define LZH_Attr (lzh_header->Attr0 | (lzh_header->Attr1 << 8))

#define YM5_Num_of_tiks ((ym5_header->Num_of_tiks0 << 24) | (ym5_header->Num_of_tiks1 << 16) | (ym5_header->Num_of_tiks2 << 8) | (ym5_header->Num_of_tiks3))
#define YM5_Song_Attr ((ym5_header->Song_Attr0 << 24) | (ym5_header->Song_Attr1 << 16) | (ym5_header->Song_Attr2 << 8) | (ym5_header->Song_Attr3))
#define YM5_Num_of_Dig ((ym5_header->Num_of_Dig0 << 8) | (ym5_header->Num_of_Dig1))
#define YM5_ChipFrq ((ym5_header->ChipFrq0 << 24) | (ym5_header->ChipFrq1 << 16) | (ym5_header->ChipFrq2 << 8) | (ym5_header->ChipFrq3))
#define YM5_InterFrq ((ym5_header->InterFrq0 << 24) | (ym5_header->InterFrq1 << 16) | (ym5_header->InterFrq2 << 8) | (ym5_header->InterFrq3))
#define YM5_Loop ((ym5_header->Loop0 << 24) | (ym5_header->Loop1 << 16) | (ym5_header->Loop2 << 8) | (ym5_header->Loop3))
#define YM5_Add_Size ((ym5_header->Add_Size0 << 8) | (ym5_header->Add_Size1))

struct YM_SongInfo
{
    int i;
    unsigned long YM_Offset;
    unsigned long Position_In_YM;

};

#define YM ((YM_SongInfo *)info.data)

void YM6i_Play(AYSongInfo &info);

void YM_Init(AYSongInfo &info)
{
    unsigned char *module = info.file_data;
    LZHFileHeader *lzh_header = (LZHFileHeader *)module;

    if(info.data)
    {
        delete (YM_SongInfo *)info.data;
        info.data = 0;
    }
    info.data = (void *)new YM_SongInfo;
    if(!info.data)
        return;
    YM->Position_In_YM = 0;
    YM->YM_Offset = 0;

    if(!memcmp(lzh_header->Method, "-lh5-", 5))
    {
        if(info.module != 0)
        {
            delete[] info.module;
            info.module = 0;

        }
        info.module_len = LZH_UCompSize * 2;
        info.module = new unsigned char[info.module_len];
        memset(info.module, 0, info.module_len);
        ay_sys_decodelha(info, lzh_header->HSize + 2);
    }
    module = info.module;
    YM5FileHeader *ym5_header = (YM5FileHeader *)module;
    if(!memcmp((char *)ym5_header, "YM6!", 4))
    {
        info.Length = YM5_Num_of_tiks;
        ay_setayfreq(&info, YM5_ChipFrq);
        info.Loop = YM5_Loop;
        unsigned short k = YM5_Add_Size;
        unsigned long ym5_offset = sizeof(YM5FileHeader);
        for(unsigned short i = 0; i < k; i++)
        {

            unsigned long dwrd = module[ym5_offset];
            ym5_offset++;
        }
        for(unsigned short i = 0; i < YM5_Num_of_Dig; i++)
        {
            unsigned long dwrd = ay_sys_getdword(&module[ym5_offset]);
            ym5_offset += 4;
            k += 4 + dwrd;
            for(unsigned long j = 0; j < dwrd; j++)
                ym5_offset++;
        }
        unsigned char *p = module + ym5_offset;
        int len = strlen((char *)p);
        p += len + 1;
        k += len + 1;
        len = strlen((char *)p);
        p += len + 1;
        k += len + 1;
        len = strlen((char *)p);
        p += len + 1;
        k += len + 1;
        YM->YM_Offset = k + 34;
        
        if((module [19] & 1) != 0)
            info.play_proc = YM6i_Play;
    }

}

void YM6_Play(AYSongInfo &info)
{
    
}

void YM6i_Play(AYSongInfo &info)
{
    unsigned char *module = info.module;    
    int k = YM->Position_In_YM + YM->YM_Offset;
    ay_writeay(&info, AY_CHNL_A_FINE, module [k]);
    k += info.Length;
    ay_writeay(&info, AY_CHNL_A_COARSE, module [k] & 15);
    k += info.Length;
    
    ay_writeay(&info, AY_CHNL_B_FINE, module [k]);
    k += info.Length;
    ay_writeay(&info, AY_CHNL_B_COARSE, module [k] & 15);
    k += info.Length;
    
    ay_writeay(&info, AY_CHNL_C_FINE, module [k]);
    k += info.Length;
    ay_writeay(&info, AY_CHNL_C_COARSE, module [k] & 15);
    k += info.Length;
    
    ay_writeay(&info, AY_NOISE_PERIOD, module [k] & 31);
    k += info.Length;
    
    unsigned char mx = module [k] & 63;
    ay_writeay(&info, AY_MIXER, mx);
    k += info.Length;
    
    ay_writeay(&info, AY_CHNL_A_VOL, module [k] & 31);
    k += info.Length;
    ay_writeay(&info, AY_CHNL_B_VOL, module [k] & 31);
    k += info.Length;
    ay_writeay(&info, AY_CHNL_C_VOL, module [k] & 31);
    k += info.Length;
    
    ay_writeay(&info, AY_ENV_FINE, module [k]);
    k += info.Length;
    ay_writeay(&info, AY_ENV_COARSE, module [k]);
    k += info.Length;
    
    if(module [k] != 255)
        ay_writeay(&info, AY_ENV_SHAPE, module [k] & 15);
    YM->Position_In_YM++;
    if(YM->Position_In_YM > info.Length)
        YM->Position_In_YM = info.Loop;
    
}

bool YM_Detect(unsigned char *module, unsigned long length)
{
    LZHFileHeader *header = (LZHFileHeader *)module;
    if(!memcmp(header->Method, "-lh5-", 5) || !memcmp(header->Method, "YM", 2))
        return true;
    return false;

}

void YM_GetInfo(AYSongInfo &info)
{    
    unsigned char *module = info.file_data;
    LZHFileHeader *header = (LZHFileHeader *)module;
    if(!memcmp(header->Method, "-lh5-", 5))
    {
        if(info.module != 0)
        {
            delete[] info.module;
            info.module = 0;

        }
        LZHFileHeader *lzh_header = (LZHFileHeader *)module;
        info.module = new unsigned char[LZH_UCompSize * 2];
        memset(info.module, 0, LZH_UCompSize * 2);
        ay_sys_decodelha(info, lzh_header->HSize + 2);
        module = info.module;
    }
    YM5FileHeader *ym5_header = (YM5FileHeader *)module;
    info.Length = YM5_Num_of_tiks;
    info.Loop = YM5_Loop;
    unsigned short k = YM5_Add_Size;
    unsigned long ym5_offset = sizeof(YM5FileHeader);
    for(unsigned short i = 0; i < k; i++)
    {
        unsigned long dwrd = module[ym5_offset];
        ym5_offset++;
    }
    for(unsigned short i = 0; i < YM5_Num_of_Dig; i++)
    {
        unsigned long dwrd = ay_sys_getdword(&module[ym5_offset]);
        ym5_offset += 4;
        k += 4 + dwrd;
        for(unsigned long j = 0; j < dwrd; j++)
            ym5_offset++;
    }
    unsigned char *p = module + ym5_offset;
    int len = strlen((char *)p);
    info.Name = ay_sys_getstr(p, len);    
    p += len + 1;
    k += len + 1;
    len = strlen((char *)p);
    info.Author = ay_sys_getstr(p, len);
    p += len + 1;
    k += len + 1;
    len = strlen((char *)p);
    info.CompName = ay_sys_getstr(p, len);
    p += len + 1;
    k += len + 1;
}

void YM_Cleanup(AYSongInfo &info)
{
    if(info.data)
    {
        delete (YM_SongInfo *)info.data;
        info.data = 0;
    }
}
