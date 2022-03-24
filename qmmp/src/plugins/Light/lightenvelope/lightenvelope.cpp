#include "lightenvelope.h"

#include <QMenu>
#include <QPainter>
#include <QSettings>

#include <math.h>
#include <qmmp/buffer.h>
#include <qmmp/decoder.h>
#include <qmmp/soundcore.h>
#include <qmmp/inputsource.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/audioconverter.h>

#define NUMBER_OF_VALUES 4096

LightEnvelopeScanner::LightEnvelopeScanner(QObject *parent)
    : QThread(parent)
{

}

LightEnvelopeScanner::~LightEnvelopeScanner()
{
    stop();
}

bool LightEnvelopeScanner::scan(const QString &path)
{
    InputSource *source = InputSource::create(path, this);
    if(!source->initialize())
    {
        delete source;
        qWarning("LightEnvelopeScanner: invalid path");
        return false;
    }

    if(source->ioDevice() && !source->ioDevice()->open(QIODevice::ReadOnly))
    {
        source->deleteLater();
        qWarning("LightEnvelopeScanner: cannot open input stream, error: %s",
                 qPrintable(source->ioDevice()->errorString()));
        return false;

    }

    DecoderFactory *factory = nullptr;

    if(!source->path().contains("://"))
        factory = Decoder::findByFilePath(source->path());
    if(!factory)
        factory = Decoder::findByMime(source->contentType());
    if(!factory && source->ioDevice() && source->path().contains("://")) //ignore content of local files
        factory = Decoder::findByContent(source->ioDevice());
    if(!factory && source->path().contains("://"))
        factory = Decoder::findByProtocol(source->path().section("://",0,0));
    if(!factory)
    {
        qWarning("LightEnvelopeScanner: unsupported file format");
        source->deleteLater();
        return false;
    }

    qDebug("LightEnvelopeScanner: selected decoder: %s", qPrintable(factory->properties().shortName));
    if(factory->properties().noInput && source->ioDevice())
    {
        source->ioDevice()->close();
    }

    Decoder *decoder = factory->create(source->path(), source->ioDevice());
    if(!decoder->initialize())
    {
        qWarning("LightEnvelopeScanner: invalid file format");
        source->deleteLater();
        delete decoder;
        return false;
    }

    m_decoder = decoder;
    m_input = source;
    if(!decoder->totalTime())
    {
        source->setOffset(-1);
    }
    m_user_stop = false;

    return true;
}

void LightEnvelopeScanner::stop()
{
    if(isRunning())
    {
        m_mutex.lock();
        m_user_stop = true;
        m_mutex.unlock();
        wait();
    }

    if(m_decoder)
    {
        delete m_decoder;
        m_decoder = nullptr;
    }

    if(m_input)
    {
        delete m_input;
        m_input = nullptr;
    }
}

const QList<int> &LightEnvelopeScanner::data() const
{
    QMutexLocker locker(&m_mutex);
    return m_data;
}

const AudioParameters &LightEnvelopeScanner::audioParameters() const
{
    return m_ap;
}

void LightEnvelopeScanner::run()
{
    m_ap = m_decoder->audioParameters();
    unsigned char in[QMMP_BLOCK_FRAMES * m_ap.frameSize() * 4];
    float out[QMMP_BLOCK_FRAMES * m_ap.channels() * sizeof(float)];
    AudioConverter converter;
    converter.configure(m_ap.format());
    m_data.clear();

    const qint64 frames = m_decoder->totalTime() * m_ap.sampleRate() / 1000;
    int samplesPerValue = frames / NUMBER_OF_VALUES * m_ap.channels();

    m_mutex.lock();
    float *max = new float[m_ap.channels()]{ -1.0 };
    float *min = new float[m_ap.channels()]{ 1.0 };
    float *rms = new float[m_ap.channels()]{ 0 };

    int counter = 0;
    int channels = m_ap.channels();
    float value = 0.f;

    while(!m_user_stop)
    {
        m_mutex.unlock();
        const qint64 len = m_decoder->read(in, sizeof(in));
        if(len > 0)
        {
            const qint64 samples = len / m_ap.sampleSize();
            converter.toFloat(in, out, samples);

            for(uint sample = 0; sample < samples - channels; sample += channels)
            {
                for(int ch = 0; ch < channels; ++ch)
                {
                    value = out[sample + ch];
                    min[ch] = qMin(min[ch], value);
                    max[ch] = qMax(max[ch], value);
                    rms[ch] += (value * value);
                }
                counter += channels;

                if(counter >= samplesPerValue)
                {
                    m_mutex.lock();
                    for(int ch = 0; ch < channels; ++ch)
                    {
                        m_data << max[ch] * 1000;
                        m_data << min[ch] * 1000;
                        m_data << std::sqrt(rms[ch] / (counter / channels)) * 1000;
                        max[ch] = -1.0;
                        min[ch] = 1.0;
                        rms[ch] = 0;
                    }

                    if(m_data.count() / 3 / channels % (NUMBER_OF_VALUES / 64) == 0)
                        emit dataChanged();
                    m_mutex.unlock();
                    counter = 0;
                }
            }
        }
        else
        {
            m_mutex.lock();
            break;
        }

        m_mutex.lock();
    }
    delete [] min;
    delete [] max;
    delete [] rms;
    m_mutex.unlock();
}



LightEnvelope::LightEnvelope(QWidget *parent) :
    Light(parent)
{
    m_scanner = new LightEnvelopeScanner(this);
    connect(m_scanner, SIGNAL(finished()), SLOT(scanFinished()));
    connect(m_scanner, SIGNAL(dataChanged()), SLOT(dataChanged()));

    m_showTwoChannelsAction = new QAction(tr("Double Channels"), this);
    m_showTwoChannelsAction->setCheckable(true);
    //: Root mean square
    m_showRmsAction = new QAction(tr("Root Mean Square"), this);
    m_showRmsAction->setCheckable(true);

    connect(SoundCore::instance(), SIGNAL(trackInfoChanged()), SLOT(mediaUrlChanged()));
    connect(SoundCore::instance(), SIGNAL(elapsedChanged(qint64)), SLOT(positionChanged(qint64)));

    readSettings();
}

LightEnvelope::~LightEnvelope()
{
    stop();
    delete m_scanner;
}

void LightEnvelope::open(const QString &path)
{
    if(m_scanner->scan(path))
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
    m_scanner->start();
}

void LightEnvelope::stop()
{
    m_scanner->stop();
}

void LightEnvelope::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("LightEnvelope");
    m_showTwoChannelsAction->setChecked(settings.value("show_two_channels", true).toBool());
    m_showRmsAction->setChecked(settings.value("show_rms", true).toBool());
    settings.endGroup();
    drawWaveform();
}

void LightEnvelope::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("LightEnvelope");
    settings.setValue("show_two_channels", m_showTwoChannelsAction->isChecked());
    settings.setValue("show_rms", m_showRmsAction->isChecked());
    settings.endGroup();
    drawWaveform();
}

void LightEnvelope::scanFinished()
{
    m_data = m_scanner->data();
    m_channels = m_scanner->audioParameters().channels();
    drawWaveform();
}

void LightEnvelope::dataChanged()
{
    if(!m_scanner->isRunning())
    {
        return;
    }

    m_data = m_scanner->data();
    m_channels = m_scanner->audioParameters().channels();
    drawWaveform();
}

void LightEnvelope::mediaUrlChanged()
{
    stop();
    open(SoundCore::instance()->path());
}

void LightEnvelope::positionChanged(qint64 elapsed)
{
    m_elapsed = elapsed;
    m_duration = SoundCore::instance()->duration();
    if(isVisible())
    {
        update();
    }
}

void LightEnvelope::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if(!m_pixmap.isNull())
    {
        painter.drawPixmap(0, 0, width(), height(), m_pixmap);
    }

    if(m_duration > 0)
    {
        const int x = width() * m_elapsed / m_duration;
        QColor color = "#33CA10";
        color.setAlpha(0x96);
        QBrush brush(color);
        painter.fillRect(0, 0, x, height(), brush);
        color.setAlpha(0xff);
        painter.setPen(color);
        painter.drawLine(x, 0, x, height());
    }
}

void LightEnvelope::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(m_showTwoChannelsAction);
    menu.addAction(m_showRmsAction);
    menu.exec(QCursor::pos());
}

void LightEnvelope::drawWaveform()
{
    if(m_data.isEmpty())
    {
        m_pixmap = QPixmap();
        update();
        return;
    }

    const bool showTwoChannels = m_showTwoChannelsAction->isChecked();
    const bool showRms = m_showRmsAction->isChecked();

    m_pixmap = QPixmap(width(), height());
    m_pixmap.fill(Qt::black);

    float step = float(width()) / NUMBER_OF_VALUES;

    QPainter painter(&m_pixmap);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);

    for(int i = 0; i < m_data.count() - m_channels * 3; i += 3)
    {
        const int ch = (i / 3) % m_channels;
        const float x1 = step * (i / m_channels / 3);
        const float x2 = step * (i / m_channels / 3 + 1);
        bool draw = false;
        float zeroPos = 0, ratio = 0;
        if(ch == 0 && (m_channels == 1 || !showTwoChannels))
        {
            zeroPos = height() / 2;
            ratio = float(height() / 4) / 1000;
            draw = true;
        }
        else if(ch == 0 || (ch == 1 && showTwoChannels))
        {
            zeroPos = ((ch == 0) ? 1 : 3) * height() / 4;
            ratio = float(height() / 8) / 1000;
            draw = true;
        }

        if(draw)
        {
            float y1 = zeroPos - m_data[i] * ratio;
            float y2 = zeroPos - m_data[i + 1] * ratio;
            float y3 = zeroPos - m_data[i + m_channels * 3] * ratio;
            float y4 = zeroPos - m_data[i + m_channels * 3 + 1] * ratio;

            QPointF points[4] = {
                { x1, y1 },
                { x1, y2 },
                { x2, y4 },
                { x2, y3 }
            };

            painter.drawPolygon(points, 4);
        }
    }

    if(!showRms)
    {
        update();
        return;
    }

    painter.setPen(QColor("#DDDDDD"));
    painter.setBrush(QColor("#DDDDDD"));

    for(int i = 0; i < m_data.count() - m_channels * 3; i += 3)
    {
        const int ch = (i / 3) % m_channels;
        const float x1 = step * (i / m_channels / 3);
        const float x2 = step * (i / m_channels / 3 + 1);
        bool draw = false;
        float zeroPos = 0, ratio = 0;

        if(ch == 0 && (m_channels == 1 || !showTwoChannels))
        {
            zeroPos = height() / 2;
            ratio = float(height() / 4) / 1000;
            draw = true;
        }
        else if(ch == 0 || (ch == 1 && showTwoChannels))
        {
            zeroPos = ((ch == 0) ? 1 : 3) * height() / 4;
            ratio = float(height() / 8) / 1000;
            draw = true;
        }

        if(draw)
        {
            float y1 = zeroPos + m_data[i + 2] * ratio;
            float y2 = zeroPos - m_data[i + 2] * ratio;
            float y3 = zeroPos + m_data[i + m_channels * 3 + 2] * ratio;
            float y4 = zeroPos - m_data[i + m_channels * 3 + 2] * ratio;

            QPointF points[4] = {
                { x1, y1 },
                { x1, y2 },
                { x2, y4 },
                { x2, y3 }
            };
            painter.drawPolygon(points, 4);
        }
    }
    update();
}
