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

const unsigned long BitBufSiz = 16;
const unsigned long UCharMax = 255;
const unsigned long DicBit = 13;
const unsigned long DicSiz = 1 << DicBit;
const unsigned long MatchBit = 8;
const unsigned long MaxMatch = 1 << MatchBit;
const unsigned long ThResHold = 3;
const unsigned long PercFlag = 0x8000;
const unsigned long Nc = (UCharMax + MaxMatch + 2 - ThResHold);
const unsigned long CBit = 9;
const unsigned long CodeBit = 16;
const unsigned long Np = DicBit + 1;
const unsigned long Nt = CodeBit + 3;
const unsigned long PBit = 4;
const unsigned long TBit = 5;
const unsigned long Npt = Nt;
const unsigned long Nul = 0;
const unsigned long MaxHashVal = (3 * DicSiz + ((DicSiz >> 9) + 1) * UCharMax);
const unsigned long WinBit = 14;
const unsigned long WindowSize = 1 << WinBit;
const unsigned long BufBit = 13;
const unsigned long BufSize = 1 << BufBit;

struct lha_params
{
    unsigned char *file_data;
    unsigned long file_len;
    unsigned long file_offset;
    unsigned long OrigSize, CompSize;
    unsigned char *OutPtr;
    unsigned short BitBuf;
    unsigned short SubBitBuf, BitCount;
    unsigned char Buffer[BufSize];
    unsigned long BufPtr;
    unsigned short BlockSize;
    unsigned short Left[2 * Nc], Right[2 * Nc];
    unsigned short PtTable[256];
    unsigned char PtLen[Npt];
    unsigned short CTable[4096];
    unsigned char CLen[Nc];
    unsigned short Decode_I;
    short Decode_J;
};

unsigned char GetC(lha_params &params)
{
    unsigned char res = params.file_data [params.BufPtr];
    params.BufPtr++;
    return res;
}

void BWrite(lha_params &params, unsigned char *P, int N)
{
    int T;
    unsigned char *Scan;
    Scan = P;
    for(T = 1; T <= N; T++)
    {
        *params.OutPtr = *Scan;
        Scan++;
        params.OutPtr++;
    }
}

void FillBuf(lha_params &params, int n)
{
    params.BitBuf = (params.BitBuf << n);
    while(n > params.BitCount)
    {
        n -= params.BitCount;
        params.BitBuf = params.BitBuf | (params.SubBitBuf << n);
        if(params.CompSize != 0)
        {
            params.CompSize--;
            params.SubBitBuf = GetC(params);
        }
        else
            params.SubBitBuf = 0;
        params.BitCount = 8;
    }
    params.BitCount -= n;
    params.BitBuf = params.BitBuf | (params.SubBitBuf >> params.BitCount);
}

unsigned short GetBits(lha_params &params, int n)
{
    unsigned short res = params.BitBuf >> (BitBufSiz - n);
    FillBuf(params, n);
    return res;
}

void InitGetBits(lha_params &params)
{
    params.BitBuf = 0;
    params.SubBitBuf = 0;
    params.BitCount = 0;
    FillBuf(params, BitBufSiz);
}

bool MakeTable(lha_params &params, int nChar, unsigned char *BitLen, int TableBits, unsigned short *Table)
{
    unsigned short Count[17], Weight[17];
    unsigned short Start[18];
    unsigned short *p;
    int i, k, Len, Ch, JutBits, Avail, NextCode, Mask;
    for(i = 1; i <= 16; i++)
        Count[i] = 0;
    for(i = 0; i < nChar; i++)
        Count[BitLen[i]]++;
    Start[1] = 0;
    for(i = 1; i <= 16; i++)
        Start[i + 1] = Start[i] + (Count[i] << (16 - i));
    if(Start[17] != 0)
        return false;
    JutBits = 16 - TableBits;
    for(i = 1; i <= TableBits; i++)
    {
        Start[i] = Start[i] >> JutBits;
        Weight[i] = 1 << (TableBits - i);
    }
    i = TableBits + 1;
    while(i <= 16)
    {
        Weight[i] = 1 << (16 - i);
        i++;
    }
    i = Start[TableBits + 1] >> JutBits;
    if(i != 0)
    {
        k = 1 << TableBits;
        while(i != k)
        {
            Table[i] = 0;
            i++;
        }
    }
    Avail = nChar;
    Mask = 1 << (15 - TableBits);
    for(Ch = 0; Ch < nChar; Ch++)
    {
        Len = BitLen[Ch];
        if(Len == 0)
            continue;
        k = Start[Len];
        NextCode = k + Weight[Len];
        if(Len <= TableBits)
        {
            for(i = k; i < NextCode; i++)
                Table[i] = Ch;
        }
        else
        {
            p = &(Table[k >> JutBits]);
            i = Len - TableBits;
            while(i != 0)
            {
                if(p[0] == 0)
                {
                    params.Right[Avail] = 0;
                    params.Left[Avail] = 0;
                    p[0] = Avail;
                    Avail++;
                }
                if((k & Mask) != 0)
                    p = &(params.Right[p[0]]);
                else
                    p = &(params.Left[p[0]]);
                k = k << 1;
                i--;
            }
            p[0] = Ch;
        }
        Start[Len] = NextCode;
    }
    return true;
}

void ReadPtLen(lha_params &params, int Nn, int nBit, int Ispecial)
{
    int i, c, n;
    unsigned short Mask;
    n = GetBits(params, nBit);
    if(n == 0)
    {
        c = GetBits(params, nBit);
        for(i = 0; i < Nn; i++)
            params.PtLen[i] = 0;
        for(i = 0; i <= 255; i++)
            params.PtTable[i] = c;
    }
    else
    {
        i = 0;
        while(i < n)
        {
            c = params.BitBuf >> (BitBufSiz - 3);
            if(c == 7)
            {
                Mask = 1 << (BitBufSiz - 4);
                while((Mask & params.BitBuf) != 0)
                {
                    Mask = Mask >> 1;
                    c++;
                }
            }
            if(c < 7)
                FillBuf(params, 3);
            else
                FillBuf(params, (unsigned char)(c - 3));                
                
            params.PtLen[i] = c;
            i++;
            if(i == Ispecial)
            {
                c = GetBits(params, 2) - 1;
                while(c >= 0)
                {
                    params.PtLen[i] = 0;
                    i++;
                    c--;
                }
            }
        }
        while(i < Nn)
        {
            params.PtLen[i] = 0;
            i++;
        }
        MakeTable(params, Nn, params.PtLen, 8, params.PtTable);
    }
}

void ReadCLen(lha_params &params)
{
    int i, c, n;
    unsigned short Mask;
    n = GetBits(params, CBit);
    if(n == 0)
    {
        c = GetBits(params, CBit);
        for(i = 0; i < Nc; i++)
            params.CLen[i] = 0;
        for(i = 0; i <= 4095; i++)
            params.CTable[i] = c;
    }
    else
    {
        i = 0;
        while(i < n)
        {
            c = params.PtTable[params.BitBuf >> (BitBufSiz - 8)];
            if(c >= Nt)
            {
                Mask = 1 << (BitBufSiz - 9);
                do
                {
                    if((params.BitBuf & Mask) != 0)
                        c = params.Right[c];
                    else
                        c = params.Left[c];
                    Mask = Mask >> 1;
                }
                while(c >= Nt);
            }
            FillBuf(params, params.PtLen[c]);
            if(c <= 2)
            {
                if(c == 1)
                    c = 2 + GetBits(params, 4);
                else if(c == 2)
                    c = 19 + GetBits(params, CBit);
                while(c >= 0)
                {
                    params.CLen[i] = 0;
                    i++;
                    c--;
                }
            }
            else
            {
                params.CLen[i] = c - 2;
                i++;
            }
        }
        while(i < Nc)
        {
            params.CLen[i] = 0;
            i++;
        }
        MakeTable(params, Nc, params.CLen, 12, params.CTable);
    }
}

unsigned short DecodeC(lha_params &params)
{
    unsigned short j, Mask, DecodeC;
    if(params.BlockSize == 0)
    {
        params.BlockSize = GetBits(params, 16);
        ReadPtLen(params, Nt, TBit, 3);
        ReadCLen(params);
        ReadPtLen(params, Np, PBit, -1);
    }
    params.BlockSize--;
    j = params.CTable[params.BitBuf >> (BitBufSiz - 12)];
    if(j >= Nc)
    {
        Mask = 1 << (BitBufSiz - 13);
        do
        {
            if((params.BitBuf & Mask) != 0)
                j = params.Right[j];
            else
                j = params.Left[j];
            Mask = Mask >> 1;
        }
        while(j >= Nc);
    }
    FillBuf(params, params.CLen[j]);
    return j;
}

unsigned short DecodeP(lha_params &params)
{
    unsigned short j, Mask, DecodeP;
    j = params.PtTable[params.BitBuf >> (BitBufSiz - 8)];
    if(j >= Np)
    {
        Mask = 1 << (BitBufSiz - 9);
        do
        {
            if((params.BitBuf & Mask) != 0)
                j = params.Right[j];
            else
                j = params.Left[j];
            Mask = Mask >> 1;
        }
        while(j >= Np);
    }
    FillBuf(params, params.PtLen[j]);
    if(j != 0)
    {
        j--;
        j = (1 << j) + GetBits(params, j);
    }
    return j;
}

void DecodeBuffer(lha_params &params, unsigned short Count, unsigned char *Buffer)
{
    unsigned short c, r;
    r = 0;
    params.Decode_J--;
    while(params.Decode_J >= 0)
    {
        Buffer[r] = Buffer[params.Decode_I];
        params.Decode_I = (params.Decode_I + 1) & (DicSiz - 1);
        r++;
        if(r == Count)
            return;
        params.Decode_J--;
    }
    while(true)
    {
        c = DecodeC(params);
        if(c <= UCharMax)
        {
            Buffer[r] = c;
            r++;
            if(r == Count)
                return;
        }
        else
        {
            params.Decode_J = c - (UCharMax + 1 - ThResHold);
            params.Decode_I = (r - DecodeP(params) - 1) & (DicSiz - 1);
            params.Decode_J--;
            while(params.Decode_J >= 0)
            {
                Buffer[r] = Buffer[params.Decode_I];
                params.Decode_I = (params.Decode_I + 1) & (DicSiz - 1);
                r++;
                if(r == Count)
                    return;
                params.Decode_J--;
            }
        }
    }
}

void ay_sys_decodelha(AYSongInfo &info, unsigned long offset)
{
  unsigned char p [DicSiz];
  int l;
  unsigned short a;
  lha_params params;
  memset(&params, 0, sizeof(lha_params));
  params.OutPtr = info.module;
  params.file_data = info.file_data + offset;
  params.file_len = info.file_len;
  params.CompSize = info.file_len - offset;
  params.OrigSize = info.module_len;
  params.BufPtr = 0;
  InitGetBits(params);
  params.BlockSize = 0;
  params.Decode_J = 0;
  l = params.OrigSize;
  while(l > 0)
  {
    if(l > DicSiz)
        a = DicSiz;
    else 
        a = l;
    DecodeBuffer (params, a, p);
    BWrite (params, p, a);
    l -= a;
  }
}
