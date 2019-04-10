#include "optimfrogwrap.h"

OptimFROGWrap::OptimFROGWrap(QIODevice *device)
   : decoder(OptimFROG_createInstance())
{
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

    if(decoder == nullptr)
        throw InvalidFile();

    if(!OptimFROG_openExt(decoder, &rint, device, C_TRUE))
    {
        OptimFROG_destroyInstance(decoder);
        throw InvalidFile();
    }

    OptimFROG_getInfo(decoder, &m_info);
    /* 24- and 32-bit audio is converted to 16-bit. */
    if(m_info.bitspersample > 16)
        m_info.bitspersample = 16;
    if(strncmp(m_info.sampleType, "SINT", 4) != 0 && strncmp(m_info.sampleType, "UINT", 4) != 0)
    {
        OptimFROG_destroyInstance(decoder);
        throw InvalidFile();
    }

    m_signed = m_info.sampleType[0] == 'S';

    OptimFROG_getTags(decoder, &ofr_tags);
    for(uInt32_t i = 0; i < ofr_tags.keyCount; i++)
    {
        m_tags.insert(std::pair<std::string, std::string>(ofr_tags.keys[i], ofr_tags.values[i]));
    }
    OptimFROG_freeTags(&ofr_tags);
}

OptimFROGWrap::~OptimFROGWrap()
{
    OptimFROG_close(decoder);
    OptimFROG_destroyInstance(decoder);
}

bool OptimFROGWrap::canPlay(const std::string &name)
{
    OptimFROG_Info junk_info;
    return OptimFROG_infoFile(const_cast<char *>(name.c_str()), &junk_info, nullptr) == OptimFROG_NoError;
}

int OptimFROGWrap::read(void *buf, long size)
{
    sInt32_t n;
    int bytes = depth() / 8;
    sInt32_t point_conversion = bytes * m_info.channels;

    n = OptimFROG_read(decoder, buf, size / point_conversion, C_TRUE);
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

void OptimFROGWrap::seek(int pos)
{
    if(OptimFROG_seekable(decoder))
        OptimFROG_seekTime(decoder, pos);
}
