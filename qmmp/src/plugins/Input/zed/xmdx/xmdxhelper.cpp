#include "xmdxhelper.h"

extern "C" {
#include <libxmdx/mdxmini/mdxmini.h>
#include <libxmdx/pmdmini/pmdmini.h>
#include <libxmdx/mucom88/mucomtag.h>
#include <libxmdx/mucom88/mucom_module.h>
#include <libxmdx/vgs/vgsdec.h>
#include <libxmdx/vgs/vgsmml.h>
}

#define INPUT_BUFFER_SIZE   1024

class MDXFileReader : public AbstractReader
{
public:
    MDXFileReader() = default;
    virtual ~MDXFileReader();

    virtual bool load(const QString &path) override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

private:
    MDXMini *m_input = nullptr;

};

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


class PMDFileReader : public AbstractReader
{
public:
    PMDFileReader() = default;
    virtual ~PMDFileReader();

    virtual bool load(const QString &path) override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

private:
    PMDMini *m_input = nullptr;

};

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
    MUCFileReader() = default;
    virtual ~MUCFileReader();

    virtual bool load(const QString &path) override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

private:
    MucomModule *m_input = nullptr;

};

MUCFileReader::~MUCFileReader()
{
    delete m_input;
}

bool MUCFileReader::load(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("MUCFileReader: open file failed");
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

qint64 MUCFileReader::read(unsigned char *data, qint64)
{
    if(m_length > 0 && m_input->IsEnd())
    {
        return 0;	// stop song
    }

    m_input->Mix((short*)data, INPUT_BUFFER_SIZE);
    return INPUT_BUFFER_SIZE * 4;
}


class VGSFileReader : public AbstractReader
{
public:
    VGSFileReader() = default;
    virtual ~VGSFileReader();

    virtual bool load(const QString &path) override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;

    virtual int sampleRate() const override final { return 22050 / 2; }

private:
    void *m_input = nullptr;

};

VGSFileReader::~VGSFileReader()
{
    if(m_input)
    {
        vgsdec_release_context(m_input);
    }
}

bool VGSFileReader::load(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("VGSFileReader: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = vgsdec_create_context();
    if(!m_input)
    {
        qWarning("VGSFileReader: vgsdec create context failed");
        return false;
    }

    if(path.toLower().endsWith(".mml"))
    {
        struct VgsMmlErrorInfo error;
        struct VgsBgmData *data = vgsmml_compile_from_memory((void*)buffer.constData(), buffer.length(), &error);
        if(!data)
        {
            qWarning("VGSFileReader: vgsmml compile data failed");
            return false;
        }

        if(vgsdec_load_bgm_from_memory(m_input, data->data, data->size))
        {
            qWarning("VGSFileReader: vgsdec load bgm from mml data failed");
            return false;
        }
        vgsmml_free_bgm_data(data);
    }
    else
    {
        if(vgsdec_load_bgm_from_memory(m_input, (void*)buffer.constData(), buffer.length()))
        {
            qWarning("VGSFileReader: vgsdec load bgm data failed");
            return false;
        }
    }

    struct VgsMetaData* meta = vgsdec_get_meta_data(m_input, 0);
    if(meta)
    {
        m_title = QString::fromStdString(meta->song);
        m_author = QString::fromStdString(meta->team);
    }

    m_length = vgsdec_get_value(m_input, VGSDEC_REG_TIME_LENGTH) * 1.0 / 22050 * 1000 + 5000;
    return true;
}

qint64 VGSFileReader::read(unsigned char *data, qint64)
{
    if(!vgsdec_get_value(m_input, VGSDEC_REG_PLAYING))
    {
        return 0;
    }

    if(vgsdec_get_value(m_input, VGSDEC_REG_LOOP_COUNT))
    {
        /* waiting for the end of fadeout if looped */
        vgsdec_set_value(m_input, VGSDEC_REG_FADEOUT, 1);
    }

    /* decoding loop */
    vgsdec_execute(m_input, data, INPUT_BUFFER_SIZE);
    return INPUT_BUFFER_SIZE;
}


XMDXHelper::XMDXHelper(const QString &path)
    : m_path(path)
{

}

XMDXHelper::~XMDXHelper()
{
    deinit();
}

void XMDXHelper::deinit()
{
    delete m_input;
}

bool XMDXHelper::initialize()
{
    const QString &suffix = m_path.toLower();
    if(suffix.endsWith(".mdx") || suffix.endsWith(".pdx")) m_input = new MDXFileReader;
    else if(suffix.endsWith(".m")) m_input = new PMDFileReader;
    else if(suffix.endsWith(".mub") || suffix.endsWith(".muc")) m_input = new MUCFileReader;
    else if(suffix.endsWith(".vgs") || suffix.endsWith(".bgm") || suffix.endsWith(".mml")) m_input = new VGSFileReader;

    if(!m_input)
    {
        qWarning("XMDXHelper: load file suffix error");
        return false;
    }

    if(!m_input->load(m_path))
    {
       qWarning("XMDXHelper: unable to open file");
       return false;
    }
    return true;
}
