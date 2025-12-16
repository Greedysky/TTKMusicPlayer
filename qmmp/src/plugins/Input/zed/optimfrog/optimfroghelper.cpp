#include "optimfroghelper.h"

#if defined Q_OS_WIN && defined __GNUC__
typedef void* (*_OptimFROG_createInstance)(void);
typedef condition_t (*_OptimFROG_openExt)(void*, ReadInterface*, void*, condition_t);
typedef void (*_OptimFROG_destroyInstance)(void*);
typedef condition_t (*_OptimFROG_getInfo)(void*, OptimFROG_Info*);
typedef condition_t (*_OptimFROG_getTags)(void*, OptimFROG_Tags*);
typedef void (*_OptimFROG_freeTags)(OptimFROG_Tags*);
typedef condition_t (*_OptimFROG_close)(void*);
typedef sInt32_t (*_OptimFROG_read)(void*, void*, uInt32_t, condition_t);
typedef condition_t (*_OptimFROG_seekable)(void*);
typedef condition_t (*_OptimFROG_seekTime)(void*, sInt64_t);
//
#define CREATE_OFR_MODULE(Module) ((_##Module)m_instance->resolve(#Module))
//
#define OptimFROG_createInstance  CREATE_OFR_MODULE(OptimFROG_createInstance)
#define OptimFROG_openExt         CREATE_OFR_MODULE(OptimFROG_openExt)
#define OptimFROG_destroyInstance CREATE_OFR_MODULE(OptimFROG_destroyInstance)
#define OptimFROG_getInfo         CREATE_OFR_MODULE(OptimFROG_getInfo)
#define OptimFROG_getTags         CREATE_OFR_MODULE(OptimFROG_getTags)
#define OptimFROG_freeTags        CREATE_OFR_MODULE(OptimFROG_freeTags)
#define OptimFROG_close           CREATE_OFR_MODULE(OptimFROG_close)
#define OptimFROG_read            CREATE_OFR_MODULE(OptimFROG_read)
#define OptimFROG_seekable        CREATE_OFR_MODULE(OptimFROG_seekable)
#define OptimFROG_seekTime        CREATE_OFR_MODULE(OptimFROG_seekTime)
#endif

OptimFROGHelper::OptimFROGHelper(QIODevice *input)
   : m_input(input)
{
#if defined Q_OS_WIN && defined __GNUC__
    m_instance = new QLibrary;
#endif
}

OptimFROGHelper::~OptimFROGHelper()
{
    deinit();
    delete m_instance;
}

void OptimFROGHelper::deinit()
{
    if(!m_decoder)
    {
        return;
    }

    OptimFROG_close(m_decoder);
    OptimFROG_destroyInstance(m_decoder);
}

bool OptimFROGHelper::initialize()
{
#if defined Q_OS_WIN && defined __GNUC__
    m_instance->setFileName("libOptimFROG.dll");
    if(!m_instance->load())
    {
        qWarning("OptimFROGHelper: load plugin failed");
        return false;
    }
#endif
    m_decoder = OptimFROG_createInstance();

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

    if(!m_decoder)
    {
        qWarning("OptimFROGHelper: OptimFROG_createInstance failed");
        return false;
    }

    if(!OptimFROG_openExt(m_decoder, &rint, m_input, C_TRUE))
    {
        qWarning("OptimFROGHelper: OptimFROG_openExt failed");
        return false;
    }
    OptimFROG_getInfo(m_decoder, &m_info);

    /* 24- and 32-bit audio is converted to 16-bit. */
    if(m_info.bitspersample > 16)
    {
        m_info.bitspersample = 16;
    }

#if defined Q_OS_UNIX
    if(strncmp(m_info.sampleType, "SINT", 4) != 0 && strncmp(m_info.sampleType, "UINT", 4) != 0)
    {
        qWarning("OptimFROGHelper: sampleType failed");
        return false;
    }
#endif

    OptimFROG_Tags ofr_tags;
    m_signed = m_info.sampleType[0] == 'S';
    OptimFROG_getTags(m_decoder, &ofr_tags);

    for(uInt32_t i = 0; i < ofr_tags.keyCount; ++i)
    {
        const QString key(ofr_tags.keys[i]);
        m_metaData.insert(key.toLower(), QString(ofr_tags.values[i]));
    }

    OptimFROG_freeTags(&ofr_tags);
    return true;
}

void OptimFROGHelper::seek(qint64 time)
{
    if(OptimFROG_seekable(m_decoder))
    {
        OptimFROG_seekTime(m_decoder, time);
    }
}

qint64 OptimFROGHelper::totalTime() const
{
#if defined Q_OS_WIN && defined __GNUC__
    return (static_cast<QIODevice*>(m_input)->size() * 8.0) / m_info.bitrate;
#else
    return m_info.length_ms;
#endif
}

qint64 OptimFROGHelper::read(unsigned char *data, qint64 maxSize)
{
    const int bytes = depth() / 8;
    const sInt32_t point_conversion = bytes * m_info.channels;

    sInt32_t n = OptimFROG_read(m_decoder, data, maxSize / point_conversion, C_TRUE);
    n = n > 0 ? n * point_conversion : 0;

    /* Qmmp doesn't support unsigned samples, so convert here. */
    if(!m_signed)
    {
        unsigned char *overlay = data;
        for(sInt32_t i = bytes - 1; i < n; i += bytes)
        {
            overlay[i] ^= 0x80;
        }
    }
    return n;
}
