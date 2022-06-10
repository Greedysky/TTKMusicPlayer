#include "mdxhelper.h"

extern "C" {
#include <libmdx/mdxmini/mdxmini.h>
#include <libmdx/pdxmini/pdxmini.h>
#include <libmdx/pmdmini/pmdmini.h>
#include <libmdx/mucom88/mucomtag.h>
#include <libmdx/mucom88/mucom_module.h>
}

#define INPUT_BUFFER_SIZE   1024

class MDXFileReader : public AbstractReader
{
public:
    MDXFileReader();
    virtual ~MDXFileReader();

    virtual bool load(const QString &path) override final;
    virtual qint64 totalTime() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

private:
    MDXMini *m_input = nullptr;

};

MDXFileReader::MDXFileReader()
{

}

MDXFileReader::~MDXFileReader()
{
    if(m_input)
    {
        mdx_close(m_input);
        delete m_input;
    }
}

bool MDXFileReader::load(const QString &path)
{
    m_input = new MDXMini;
    mdx_set_rate(sampleRate());

    if(mdx_open(m_input, QmmpPrintable(path), nullptr) != 0)
    {
        qWarning("MDXFileReader: mdx_open error");
        delete m_input;
        m_input = nullptr;
        return false;
    }

    char buffer[INPUT_BUFFER_SIZE] = {0};
    mdx_set_max_loop(m_input, 0);
    mdx_get_title(m_input, buffer);
    m_title = buffer;
    m_length = mdx_get_length(m_input) * 1000;
    return true;
}

qint64 MDXFileReader::totalTime() const
{
    return m_length;
}

qint64 MDXFileReader::read(unsigned char *data, qint64)
{
    if(m_length > 0 && m_offset >= m_length)
    {
        return 0;	// stop song
    }

    mdx_calc_sample(m_input, (short*)data, INPUT_BUFFER_SIZE);
    m_offset += INPUT_BUFFER_SIZE * 1000.0 / sampleRate();
    return INPUT_BUFFER_SIZE * 4;
}


class PDXFileReader : public AbstractReader
{
public:
    PDXFileReader();
    virtual ~PDXFileReader();

    virtual bool load(const QString &path) override final;
    virtual qint64 totalTime() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

    virtual int sampleRate() const override final { return m_input->sampleRate(); }

private:
    PDXMini *m_input = nullptr;

};

PDXFileReader::PDXFileReader()
{

}

PDXFileReader::~PDXFileReader()
{
    delete m_input;
}

bool PDXFileReader::load(const QString &path)
{
    m_input = new PDXMini;

    if(!m_input->open(QmmpPrintable(path)))
    {
        qWarning("MDXFileReader: mdx_open error");
        delete m_input;
        m_input = nullptr;
        return false;
    }
    return true;
}

qint64 PDXFileReader::totalTime() const
{
    return m_input->length();
}

qint64 PDXFileReader::read(unsigned char *data, qint64)
{
    return m_input->render((char*)data);
}


class PMDFileReader : public AbstractReader
{
public:
    PMDFileReader();
    virtual ~PMDFileReader();

    virtual bool load(const QString &path) override final;
    virtual qint64 totalTime() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

private:
    PMDMini *m_input = nullptr;

};

PMDFileReader::PMDFileReader()
{

}

PMDFileReader::~PMDFileReader()
{
    if(m_input)
    {
        m_input->pmd_stop();
        delete m_input;
    }
}

bool PMDFileReader::load(const QString &path)
{
    m_input = new PMDMini;
    m_input->pmd_init();
    m_input->pmd_setrate(sampleRate());

    if(m_input->pmd_play(QmmpPrintable(path), nullptr) != 0)
    {
        qWarning("PMDFileReader: pmd_play error");
        return false;
    }

    char buffer[INPUT_BUFFER_SIZE] = {0};
    m_input->pmd_get_title(buffer);
    m_title = buffer;
    m_input->pmd_get_compo(buffer);
    m_author = buffer;
    m_length = m_input->pmd_length_sec() * 1000;
    return true;
}

qint64 PMDFileReader::totalTime() const
{
    return m_length;
}

qint64 PMDFileReader::read(unsigned char *data, qint64)
{
    if(m_length > 0 && m_offset >= m_length)
    {
        return 0;	// stop song
    }

    m_input->pmd_renderer((short*)data, INPUT_BUFFER_SIZE);
    m_offset += INPUT_BUFFER_SIZE * 1000.0 / sampleRate();
    return INPUT_BUFFER_SIZE * 4;
}


class MUCFileReader : public AbstractReader
{
public:
    MUCFileReader();
    virtual ~MUCFileReader();

    virtual bool load(const QString &path) override final;
    virtual qint64 totalTime() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

private:
    MucomModule *m_input = nullptr;

};

MUCFileReader::MUCFileReader()
{

}

MUCFileReader::~MUCFileReader()
{
    delete m_input;
}

bool MUCFileReader::load(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("PMDFileReader: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = new MucomModule;
    m_input->SetRate(sampleRate());
    m_input->OpenMemory((unsigned char *)buffer.constData(), buffer.length(), path.toLower().endsWith(".mub"));
    m_input->UseFader(true);
    m_input->Play();

    m_title = QString::fromStdString(m_input->tag->title);
    m_author = QString::fromStdString(m_input->tag->author);
    m_length = m_input->GetLength() * 1000;
    return true;
}

qint64 MUCFileReader::totalTime() const
{
    return m_length;
}

qint64 MUCFileReader::read(unsigned char *data, qint64)
{
    if(m_length > 0 && m_input->IsEnd())
    {
        return 0;	// stop song
    }

    m_input->Mix((short*)data, INPUT_BUFFER_SIZE);
    return INPUT_BUFFER_SIZE * 4;
}


MDXHelper::MDXHelper(const QString &path)
    : m_path(path)
{

}

MDXHelper::~MDXHelper()
{
    deinit();
}

void MDXHelper::deinit()
{
    delete m_input;
}

bool MDXHelper::initialize()
{
    const QString &suffix = m_path.toLower();
    if(suffix.endsWith(".mdx")) m_input = new MDXFileReader;
    else if(suffix.endsWith(".pdx")) m_input = new PDXFileReader;
    else if(suffix.endsWith(".m")) m_input = new PMDFileReader;
    else if(suffix.endsWith(".mub") || suffix.endsWith(".muc")) m_input = new MUCFileReader;

    if(!m_input)
    {
        qWarning("MDXHelper: load file suffix error");
        return false;
    }

    if(!m_input->load(m_path))
    {
       qWarning("MDXHelper: unable to open file");
       return false;
    }
    return true;
}
