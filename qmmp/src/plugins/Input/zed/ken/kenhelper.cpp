#include "kenhelper.h"

#include <libken/kdmdecoder.h>
#include <libken/ksmdecoder.h>
#include <libken/smdecoder.h>

class KDMFileReader : public AbstractReader
{
public:
    KDMFileReader() = default;
    virtual ~KDMFileReader();

    virtual bool load(const QString &path) override final;
    virtual void seek(qint64 time) override final;
    virtual qint64 read(unsigned char *data) override final;

private:
    KDMDecoder *m_input = nullptr;

};

KDMFileReader::~KDMFileReader()
{
    delete m_input;
}

bool KDMFileReader::load(const QString &path)
{
    m_input = new KDMDecoder(sampleRate(), channels());
    if(!m_input)
    {
        qWarning("KDMFileReader: failed to create KDMDecoder");
        return false;
    }

    m_length = m_input->load(QmmpPrintable(path));
    if(m_length <= 0)
    {
        qWarning("KenHelper: failed load ken file");
        return false;
    }

    m_input->start();
    return true;
}

void KDMFileReader::seek(qint64 time)
{
    m_input->seek(time);
}

qint64 KDMFileReader::read(unsigned char *data)
{
    unsigned int samples = 576;
    samples -= (samples % (sampleRate() / 120)); //In (44100,2,2): 2304->2202
    samples <<= 2;
    return m_input->rendersound(data, samples) < 1 ? samples: 0;
}


class KSMFileReader : public AbstractReader
{
public:
    KSMFileReader() = default;
    virtual ~KSMFileReader();

    virtual bool load(const QString &path) override final;
    virtual void seek(qint64 time) override final;
    virtual qint64 read(unsigned char *data) override final;

private:
    KSMDecoder *m_input = nullptr;

};

KSMFileReader::~KSMFileReader()
{
    delete m_input;
}

bool KSMFileReader::load(const QString &path)
{
    m_input = new KSMDecoder(sampleRate(), channels());
    if(!m_input)
    {
        qWarning("KSMFileReader: failed to create KSMDecoder");
        return false;
    }

    m_length = m_input->load(QmmpPrintable(path));
    if(m_length <= 0)
    {
        qWarning("KSMFileReader: failed load ken file");
        return false;
    }

    m_input->start();
    return true;
}

void KSMFileReader::seek(qint64 time)
{
    m_input->seek(time);
}

qint64 KSMFileReader::read(unsigned char *data)
{
    const unsigned int samples = 576 << 2;
    return m_input->rendersound(data, samples) < 1 ? samples: 0;
}


class SMFileReader : public AbstractReader
{
public:
    SMFileReader() = default;
    virtual ~SMFileReader();

    virtual bool load(const QString &path) override final;
    virtual void seek(qint64 time) override final;
    virtual qint64 read(unsigned char *data) override final;

private:
    SMDecoder *m_input = nullptr;

};

SMFileReader::~SMFileReader()
{
    delete m_input;
}

bool SMFileReader::load(const QString &path)
{
    m_input = new SMDecoder(sampleRate(), channels());
    if(!m_input)
    {
        qWarning("SMFileReader: failed to create SMDecoder");
        return false;
    }

    m_length = m_input->load(QmmpPrintable(path));
    if(m_length <= 0)
    {
        qWarning("SMFileReader: failed load ken file");
        return false;
    }

    m_input->start();
    return true;
}

void SMFileReader::seek(qint64 time)
{
    m_input->seek(time);
}

qint64 SMFileReader::read(unsigned char *data)
{
    const unsigned int samples = 576 << 2;
    return m_input->rendersound(data, samples) < 1 ? samples: 0;
}


KenHelper::KenHelper(const QString &path)
    : m_path(path)
{

}

KenHelper::~KenHelper()
{
    deinit();
}

void KenHelper::deinit()
{
    delete m_input;
}

bool KenHelper::initialize()
{
    const QString &suffix = m_path.toLower();
    if(suffix.endsWith(".kdm")) m_input = new KDMFileReader;
    else if(suffix.endsWith(".ksm")) m_input = new KSMFileReader;
    else if(suffix.endsWith(".sm") || suffix.endsWith(".snd")) m_input = new SMFileReader;

    if(!m_input)
    {
        qWarning("KenHelper: load file suffix error");
        return false;
    }

    if(!m_input->load(m_path))
    {
       qWarning("KenHelper: unable to open file");
       return false;
    }
    return true;
}
