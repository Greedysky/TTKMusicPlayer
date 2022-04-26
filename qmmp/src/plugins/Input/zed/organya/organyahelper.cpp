#include "organyahelper.h"

#define INPUT_BUFFER_SIZE   1024

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
    if(m_org)
    {
        org_decoder_destroy(m_org);
    }

    if(m_pxs)
    {
        m_pxs->clear();
        delete m_pxs;
        delete m_pxd;
    }
}

bool OrganyaHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("OrganyaHelper: open file failed");
        file.close();
        return false;
    }

    const qint64 size = file.size();
    m_type = m_path.toLower().endsWith(".org") ? Organya : PxTone;

    switch(m_type)
    {
    case Organya:
    {
        m_org = org_decoder_create(QmmpPrintable(m_path), 1);
        if(!m_org)
        {
            qWarning("OrganyaHelper: org_decoder_create error");
            return false;
        }

        break;
    }
    case PxTone:
    {
        m_pxs = new pxtnService;
        m_pxd = new pxtnDescriptor;

        if(m_pxs->init() != pxtnOK)
        {
            qWarning("OrganyaHelper: pxtnService init error");
            return false;
        }

        if(!m_pxs->set_destination_quality(2, sampleRate()))
        {
            qWarning("OrganyaHelper: set_destination_quality error");
            return false;
        }

        const QByteArray &module = file.readAll();
        if(!m_pxd->set_memory_r((void*)module.constData(), size))
        {
            qWarning("OrganyaHelper: set_memory_r error");
            return false;
        }

        if(m_pxs->read(m_pxd) != pxtnOK)
        {
            qWarning("OrganyaHelper: pxtnService read error");
            return false;
        }

        if(m_pxs->tones_ready() != pxtnOK)
        {
            qWarning("OrganyaHelper: pxtnService tones_ready error");
            m_pxs->evels->Release();
            return false;
        }

        pxtnVOMITPREPARATION prep = {0};
//        prep.flags |= pxtnVOMITPREPFLAG_loop; // don't loop
        prep.start_pos_float = 0;
        prep.master_volume = 1.0f; //(volume / 100.0f);

        if(!m_pxs->moo_preparation(&prep))
        {
            qWarning("OrganyaHelper: moo_preparation error");
            return false;
        }
        break;
    }
    default: break;
    }

    file.close();
    m_bitrate = size * 8.0 / totalTime() + 1.0f;

    seek(0);
    return true;
}

void OrganyaHelper::seek(qint64 time)
{
    switch(m_type)
    {
    case Organya:
    {
        org_decoder_seek_sample(m_org, time * sampleRate() / 1000);
        break;
    }
    case PxTone:
    {
        pxtnVOMITPREPARATION prep = {0};
        prep.start_pos_sample = sampleRate() * time / 1000;
        prep.master_volume = 1.0f; //(volume / 100.0f);
        m_pxs->moo_preparation(&prep);
        break;
    }
    default: break;
    }
}

qint64 OrganyaHelper::totalTime() const
{
    switch(m_type)
    {
    case Organya: return org_decoder_get_total_samples(m_org) / sampleRate() * 1000;
    case PxTone:  return m_pxs->moo_get_total_sample() / sampleRate() * 1000;
    default: return 0;
    }
}

qint64 OrganyaHelper::read(unsigned char *data, qint64 maxSize)
{
    switch(m_type)
    {
    case Organya:
    {
        const unsigned int sample = maxSize / channels() / sizeof(int16_t);
        return org_decode_samples(m_org, (int16_t*)data, sample) * channels() * sizeof(int16_t);
    }
    case PxTone:
    {
        if(m_pxs->moo_is_end_vomit() || !m_pxs->moo_is_valid_data())
        {
            return 0;
        }

        const unsigned int sample = pxtnBITPERSAMPLE / 8 * channels() * INPUT_BUFFER_SIZE;
        return m_pxs->Moo((void*)data, sample) ? sample : 0;
    }
    default: return 0;
    }
}
