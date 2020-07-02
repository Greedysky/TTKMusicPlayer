/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "optimfroghelper.h"

#if defined Q_OS_WIN && defined __GNUC__
typedef void* (*OFROG_createInstance)(void);
typedef condition_t (*OFROG_openExt)(void*, ReadInterface*, void*, condition_t);
typedef void (*OFROG_destroyInstance)(void*);
typedef condition_t (*OFROG_getInfo)(void*, OptimFROG_Info*);
typedef condition_t (*OFROG_getTags)(void*, OptimFROG_Tags*);
typedef void (*OFROG_freeTags)(OptimFROG_Tags*);
typedef condition_t (*OFROG_close)(void*);
typedef sInt32_t (*OFROG_read)(void*, void*, uInt32_t, condition_t);
typedef condition_t (*OFROG_seekable)(void*);
typedef condition_t (*OFROG_seekTime)(void*, sInt64_t);
#endif

OptimFROGHelper::OptimFROGHelper(QIODevice *i)
   : m_decoder(nullptr),
     m_reader(i)
{
#if defined Q_OS_WIN && defined __GNUC__
    m_instance = nullptr;
#endif
}

OptimFROGHelper::~OptimFROGHelper()
{
    if(!m_decoder)
    {
        return;
    }
#if defined Q_OS_WIN && defined __GNUC__
    ((OFROG_close)GetSymbolAddress("OptimFROG_close"))(m_decoder);
    ((OFROG_destroyInstance)GetSymbolAddress("OptimFROG_destroyInstance"))(m_decoder);
#else
    OptimFROG_close(m_decoder);
    OptimFROG_destroyInstance(m_decoder);
#endif
}

bool OptimFROGHelper::initialize()
{
#if defined Q_OS_WIN && defined __GNUC__
    m_instance = LoadLibraryA("libOptimFROG.dll");
    if(m_instance == nullptr)
    {
        return false;
    }
    m_decoder = ((OFROG_createInstance)GetSymbolAddress("OptimFROG_createInstance"))();
#else
    m_decoder = OptimFROG_createInstance();
#endif
    static ReadInterface rint =
    {
        ofr_close,
        ofr_read,
        ofr_eof,
        ofr_seekable,
        ofr_length,
        ofr_get_pos,
        ofr_seek,
    };
    OptimFROG_Tags ofr_tags;

    if(m_decoder == nullptr)
    {
        return false;
    }

#if defined Q_OS_WIN && defined __GNUC__
    if(!((OFROG_openExt)GetSymbolAddress("OptimFROG_openExt"))(m_decoder, &rint, m_reader, C_TRUE))
    {
        ((OFROG_destroyInstance)GetSymbolAddress("OptimFROG_destroyInstance"))(m_decoder);
        return false;
    }
    ((OFROG_getInfo)GetSymbolAddress("OptimFROG_getInfo"))(m_decoder, &m_info);
#else
    if(!OptimFROG_openExt(m_decoder, &rint, m_reader, C_TRUE))
    {
        OptimFROG_destroyInstance(m_decoder);
        return false;
    }
    OptimFROG_getInfo(m_decoder, &m_info);
#endif

    /* 24- and 32-bit audio is converted to 16-bit. */
    if(m_info.bitspersample > 16)
    {
        m_info.bitspersample = 16;
    }
    if(strncmp(m_info.sampleType, "SINT", 4) != 0 && strncmp(m_info.sampleType, "UINT", 4) != 0)
    {
#if defined Q_OS_WIN && defined __GNUC__
        ((OFROG_destroyInstance)GetSymbolAddress("OptimFROG_destroyInstance"))(m_decoder);
#else
        OptimFROG_destroyInstance(m_decoder);
#endif
        return false;
    }

    m_signed = m_info.sampleType[0] == 'S';
#if defined Q_OS_WIN && defined __GNUC__
    ((OFROG_getTags)GetSymbolAddress("OptimFROG_getTags"))(m_decoder, &ofr_tags);
#else
    OptimFROG_getTags(m_decoder, &ofr_tags);
#endif
    for(uInt32_t i = 0; i < ofr_tags.keyCount; i++)
    {
        m_tags.insert(std::pair<std::string, std::string>(ofr_tags.keys[i], ofr_tags.values[i]));
    }
#if defined Q_OS_WIN && defined __GNUC__
    ((OFROG_freeTags)GetSymbolAddress("OptimFROG_freeTags"))(&ofr_tags);
#else
    OptimFROG_freeTags(&ofr_tags);
#endif
    return true;
}

int OptimFROGHelper::read(void *buf, long size)
{
    sInt32_t n;
    int bytes = depth() / 8;
    sInt32_t point_conversion = bytes * m_info.channels;
#if defined Q_OS_WIN && defined __GNUC__
    n = ((OFROG_read)GetSymbolAddress("OptimFROG_read"))(m_decoder, buf, size / point_conversion, C_TRUE);
#else
    n = OptimFROG_read(m_decoder, buf, size / point_conversion, C_TRUE);
#endif
    n = n > 0 ? n * point_conversion : 0;

    /* Qmmp doesn't support unsigned samples, so convert here. */
    if(!m_signed)
    {
        unsigned char *overlay = reinterpret_cast<unsigned char *>(buf);

        for(sInt32_t i = bytes - 1; i < n; i += bytes)
        {
            overlay[i] ^= 0x80;
        }
    }
    return n;
}

void OptimFROGHelper::seek(int pos)
{
#if defined Q_OS_WIN && defined __GNUC__
    if(((OFROG_seekable)GetSymbolAddress("OptimFROG_seekable"))(m_decoder))
    {
        ((OFROG_seekTime)GetSymbolAddress("OptimFROG_seekTime"))(m_decoder, pos);
    }
#else
    if(OptimFROG_seekable(m_decoder))
    {
        OptimFROG_seekTime(m_decoder, pos);
    }
#endif
}

#if defined Q_OS_WIN && defined __GNUC__
FARPROC OptimFROGHelper::GetSymbolAddress(const char* name) const
{
    FARPROC func = nullptr;
    if(m_instance)
    {
        func = GetProcAddress(m_instance, name);
    }
    return func;
}
#endif
