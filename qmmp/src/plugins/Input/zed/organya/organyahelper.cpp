#include "organyahelper.h"

#include <liborganya/organya/organya.h>
#include <liborganya/pxtone/pxtnService.h>

#define INPUT_BUFFER_SIZE   1024

class OrgFileReader : public AbstractReader
{
public:
    OrgFileReader() = default;
    virtual ~OrgFileReader();

    virtual bool load(const QString &path) override final;
    virtual void seek(qint64 time) override final;
    virtual qint64 totalTime() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

private:
    org_decoder_t *m_input = nullptr;

};

OrgFileReader::~OrgFileReader()
{
    if(m_input)
    {
        org_decoder_destroy(m_input);
    }
}

bool OrgFileReader::load(const QString &path)
{
    m_input = org_decoder_create(QmmpPrintable(path), 1);
    if(!m_input)
    {
        qWarning("OrgFileReader: org_decoder_create error");
        return false;
    }
    return true;
}

void OrgFileReader::seek(qint64 time)
{
    org_decoder_seek_sample(m_input, time * sampleRate() / 1000);
}

qint64 OrgFileReader::totalTime() const
{
    return org_decoder_get_total_samples(m_input) / sampleRate() * 1000;
}

qint64 OrgFileReader::read(unsigned char *data, qint64 maxSize)
{
    const int size = sizeof(int16_t) * channels();
    return org_decode_samples(m_input, (int16_t*)data, maxSize / size) * size;
}


class PxFileReader : public AbstractReader
{
public:
    PxFileReader() = default;
    virtual ~PxFileReader();

    virtual bool load(const QString &path) override final;
    virtual void seek(qint64 time) override final;
    virtual qint64 totalTime() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

private:
    pxtnService *m_pxs = nullptr;
    pxtnDescriptor *m_pxd = nullptr;

};

PxFileReader::~PxFileReader()
{
    if(m_pxs)
    {
        m_pxs->clear();
        delete m_pxs;
        delete m_pxd;
    }
}

bool PxFileReader::load(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("PxFileReader: open file failed");
        return false;
    }

    m_pxs = new pxtnService;
    m_pxd = new pxtnDescriptor;

    if(m_pxs->init() != pxtnOK)
    {
        qWarning("PxFileReader: pxtnService init error");
        return false;
    }

    if(!m_pxs->set_destination_quality(channels(), sampleRate()))
    {
        qWarning("PxFileReader: set_destination_quality error");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    if(!m_pxd->set_memory_r((void*)buffer.constData(), buffer.length()))
    {
        qWarning("PxFileReader: set_memory_r error");
        return false;
    }

    if(m_pxs->read(m_pxd) != pxtnOK)
    {
        qWarning("PxFileReader: pxtnService read error");
        return false;
    }

    if(m_pxs->tones_ready() != pxtnOK)
    {
        qWarning("PxFileReader: pxtnService tones_ready error");
        m_pxs->evels->Release();
        return false;
    }

    pxtnVOMITPREPARATION prep;
    memset(&prep, 0, sizeof(prep));
//        prep.flags |= pxtnVOMITPREPFLAG_loop; // don't loop
    prep.start_pos_float = 0;
    prep.master_volume = 1.0f; //(volume / 100.0f);

    if(!m_pxs->moo_preparation(&prep))
    {
        qWarning("PxFileReader: moo_preparation error");
        return false;
    }
    return true;
}

void PxFileReader::seek(qint64 time)
{
    pxtnVOMITPREPARATION prep;
    memset(&prep, 0, sizeof(prep));
    prep.start_pos_sample = sampleRate() * time / 1000;
    prep.master_volume = 1.0f; //(volume / 100.0f);
    m_pxs->moo_preparation(&prep);
}

qint64 PxFileReader::totalTime() const
{
    return m_pxs->moo_get_total_sample() / sampleRate() * 1000;
}

qint64 PxFileReader::read(unsigned char *data, qint64)
{
    if(m_pxs->moo_is_end_vomit() || !m_pxs->moo_is_valid_data())
    {
        return 0;
    }

    const unsigned int sample = pxtnBITPERSAMPLE / 8 * channels() * INPUT_BUFFER_SIZE;
    return m_pxs->Moo((void*)data, sample) ? sample : 0;
}


OrganyaHelper::OrganyaHelper(const QString &path)
    : m_path(path)
{

}

OrganyaHelper::~OrganyaHelper()
{
    deinit();
}

void OrganyaHelper::deinit()
{
    delete m_input;
}

bool OrganyaHelper::initialize()
{
    const QString &suffix = m_path.toLower();
    if(suffix.endsWith(".org")) m_input = new OrgFileReader;
    else if(suffix.endsWith(".pttune") || suffix.endsWith(".ptcop")) m_input = new PxFileReader;

    if(!m_input)
    {
        qWarning("OrganyaHelper: load file suffix error");
        return false;
    }

    if(!m_input->load(m_path))
    {
       qWarning("OrganyaHelper: unable to open file");
       return false;
    }

    seek(0);
    return true;
}
