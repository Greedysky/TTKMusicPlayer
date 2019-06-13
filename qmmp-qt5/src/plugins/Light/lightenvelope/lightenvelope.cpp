#include "lightenvelope.h"
#include "inlines.h"
#include "soundcore.h"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

#define VISUAL_NODE_SIZE 512 //samples
#define VISUAL_BUFFER_SIZE (5*VISUAL_NODE_SIZE)

LightEnvelopeThead::LightEnvelopeThead(QObject *parent) :
    QThread(parent)
{
    init();
}

LightEnvelopeThead::~LightEnvelopeThead()
{
    free();
}

bool LightEnvelopeThead::init(const QString &path)
{
    free();
    init();

    m_source = InputSource::create(path, this);
    if(m_source->ioDevice() && !m_source->ioDevice()->isOpen() && !m_source->ioDevice()->open(QIODevice::ReadOnly))
    {
        m_source->deleteLater();
        m_source = nullptr;
        return false;
    }

    DecoderFactory *factory = nullptr;
    if(!m_source->path().contains("://"))
    {
        factory = Decoder::findByFilePath(m_source->path(), QmmpSettings::instance()->determineFileTypeByContent());
    }
    if(!factory)
    {
        factory = Decoder::findByMime(m_source->contentType());
    }
    if(factory && !factory->properties().noInput && m_source->ioDevice() && m_source->path().contains("://"))
    {
        factory = (factory->canDecode(m_source->ioDevice()) ? factory : 0);
    }
    if(!factory && m_source->ioDevice() && m_source->path().contains("://")) //ignore content of local files
    {
        factory = Decoder::findByContent(m_source->ioDevice());
    }
    if(!factory && m_source->path().contains("://"))
    {
        factory = Decoder::findByProtocol(m_source->path().section("://",0,0));
    }
    if(!factory)
    {
        qDebug("LightEnvelopeThead: unsupported file format");
        return false;
    }

    qDebug("LightEnvelopeThead: selected decoder: %s",qPrintable(factory->properties().shortName));
    if(factory->properties().noInput && m_source->ioDevice())
    {
        m_source->ioDevice()->close();
    }

    Decoder *decoder = factory->create(m_source->path(), m_source->ioDevice());
    if(!decoder->initialize())
    {
        qDebug("LightEnvelopeThead: invalid file format");
        delete decoder;
        return false;
    }

    m_decoder = decoder;
    AudioParameters ap = m_decoder->audioParameters();
    //output buffer for decoder
    m_bks = ap.frameSize() * QMMP_BLOCK_FRAMES; //block size
    m_output_size = m_bks * 4;
    m_sample_size = ap.sampleSize();
    m_output_buf = new unsigned char[m_output_size];
    memset(m_output_buf, 0, sizeof(unsigned char)*m_output_size);

    m_converter.configure(ap.format());
    m_recycler.configure(ap.sampleRate(), ap.channels());

    return true;
}

void LightEnvelopeThead::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void LightEnvelopeThead::start()
{
    m_run = true;
    QThread::start();
}

void LightEnvelopeThead::run()
{
    qint64 len = 0;
    while (m_run && !m_finish)
    {
        m_mutex.lock ();
        len = m_decoder->read((m_output_buf + m_output_at), m_output_size - m_output_at);
        if(len > 0)
        {
            m_bitrate = m_decoder->bitrate();
            m_output_at += len;
            flush(false);
            emit bufferChanged(m_recycler.next(), m_decoder->audioParameters().channels(), m_output_size);
        }
        else if(len == 0)
        {
            flush(true);
            m_finish = true;
            emit finished();
        }
        m_mutex.unlock();
    }
}

qint64 LightEnvelopeThead::produceSound(unsigned char *data, quint64 size, quint32 brate)
{
    Buffer *b = m_recycler.get();
    quint64 sz = size < m_bks ? size : m_bks;
    size_t samples = sz / m_sample_size;

    m_converter.toFloat(data, b->data, samples);
    b->samples = samples;
    b->rate = brate;
    memmove(data, data + sz, size - sz);
    m_recycler.add();
    return sz;
}

void LightEnvelopeThead::flush(bool final)
{
    ulong min = final ? 0 : m_bks;
    while ((!m_finish || !m_run) && m_output_at > min)
    {
        m_recycler.mutex()->lock ();
        while ((m_recycler.full() || m_recycler.blocked()) && (!m_finish))
        {
            m_mutex.unlock();
            m_recycler.done();
            m_mutex.lock ();
        }
        m_output_at -= produceSound(m_output_buf, m_output_at, m_bitrate);

        if(!m_recycler.empty())
        {
            m_recycler.cond()->wakeOne();
        }

        m_recycler.mutex()->unlock();
    }
}

void LightEnvelopeThead::free()
{
    if(m_output_buf)
    {
        delete []m_output_buf;
    }
    delete m_source;
    delete m_decoder;
}

void LightEnvelopeThead::init()
{
    m_sample_size = 0;
    m_bitrate = 0;
    m_finish = false;
    m_run = false;
    m_output_at = 0;
    m_output_size = 0;
    m_bks = 0;
    m_output_buf = nullptr;
    m_source = nullptr;
    m_decoder = nullptr;
}


LightEnvelope::LightEnvelope(QWidget *parent) :
    Light(parent)
{
    init();

    m_analyzer_falloff = 2.2;
    setWindowTitle(tr("LightEnvelope"));
    setMinimumSize(2*300-30, 105);

    m_buffer = new float[VISUAL_BUFFER_SIZE];
    m_vis_data = 0;
    m_x_scale = new int[2];

    m_lightThread = new LightEnvelopeThead(this);
    connect(m_lightThread, SIGNAL(bufferChanged(Buffer*,int,quint64)), SLOT(bufferChanged(Buffer*,int,quint64)));
    connect(m_lightThread, SIGNAL(finished()), SLOT(finished()));
    connect(SoundCore::instance(), SIGNAL(trackInfoChanged()), SLOT(mediaUrlChanged()));
}

LightEnvelope::~LightEnvelope()
{
    qDebug() << "~LightEnvelope";
    delete[] m_buffer;
    delete[] m_x_scale;

    this->stop();
    delete m_lightThread;
}

void LightEnvelope::open(const QString &path)
{
    init();
    if(m_lightThread->init(path))
    {
        start();
    }
    else
    {
        qDebug("LightEnvelopeThead: init error");
    }
}

void LightEnvelope::start()
{
    m_lightThread->start();
}

void LightEnvelope::stop()
{
    m_lightThread->stopAndQuitThread();
}

void LightEnvelope::bufferChanged(Buffer *buffer, int chans, quint64 size)
{
    if(!buffer)
    {
        return;
    }

    m_output_size = size;
    if(VISUAL_BUFFER_SIZE == m_buffer_at)
    {
        m_buffer_at -= VISUAL_NODE_SIZE;
        memmove(m_buffer, m_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
        return;
    }

    int frames = qMin(int(buffer->samples/chans), VISUAL_BUFFER_SIZE - m_buffer_at);
    mono_from_multichannel(m_buffer + m_buffer_at, buffer->data, frames, chans);

    m_buffer_at += frames;

    m_mutex.lock();
    if(m_buffer_at < VISUAL_NODE_SIZE)
    {
        m_mutex.unlock ();
        return;
    }

    process(m_buffer);

    m_buffer_at -= VISUAL_NODE_SIZE;
    memmove(m_buffer, m_buffer + VISUAL_NODE_SIZE, m_buffer_at * sizeof(float));
    m_mutex.unlock();

    update();
}

void LightEnvelope::finished()
{
    m_backgroundImage = m_backgroundImage.copy(0, 0, m_pixPos, m_rows);
    update();
}

void LightEnvelope::mediaUrlChanged()
{
    open(SoundCore::instance()->path());
}

void LightEnvelope::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::black);
    draw(&painter);
}

void LightEnvelope::init()
{
    m_pixPos = 0;
    m_buffer_at = 0;
    m_rows = 0;
    m_cols = 0;
    m_output_size = 0;
}

void LightEnvelope::process(float *buffer)
{
    if(m_pixPos >= m_output_size)
    {
        return;
    }

    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        m_backgroundImage = QImage(m_output_size, m_rows, QImage::Format_RGB32);
        m_backgroundImage.fill(Qt::black);
        for(int i=0; i<m_cols; ++i)
        {
            m_backgroundImage.setPixel(i, m_rows/2, qRgb(0xff, 0xff, 0xff));
        }

        m_vis_data = 0;
        for(int i = 0; i < 2; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y = 0;
    int k, magnitude = 0;
    calc_freq (dest, buffer);
    const double y_scale = (double) 1.25 * m_rows / log(256);

    if(m_x_scale[0] == m_x_scale[1])
    {
        y = dest[0];
    }
    for(k = m_x_scale[0]; k < m_x_scale[1]; k++)
    {
        y = qMax(dest[k], y);
    }

    y >>= 7; //256

    if(y)
    {
        magnitude = int(log (y) * y_scale);
        magnitude = qBound(0, magnitude, m_rows);
    }

    m_vis_data -= m_analyzer_falloff * m_rows / 15;
    m_vis_data = magnitude > m_vis_data ? magnitude : m_vis_data;

    for(int i=0; i<m_vis_data/2; ++i)
    {
        int g = qMin(0x5f + i*3, 0xff);
        m_backgroundImage.setPixel(m_pixPos, qMax(m_rows/2 - i, 0), qRgb(0, g, 0));
        m_backgroundImage.setPixel(m_pixPos, qMin(m_rows/2 + i, m_rows), qRgb(0, g, 0));
    }
    m_backgroundImage.setPixel(m_pixPos, m_rows/2, qRgb(0xff, 0xff, 0xff));
    m_pixPos++;
}

void LightEnvelope::draw(QPainter *p)
{
    p->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if(!m_backgroundImage.isNull())
    {
        p->drawImage(0, 0, m_backgroundImage.scaled(size()));
    }
}
