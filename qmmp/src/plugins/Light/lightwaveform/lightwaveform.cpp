#include "lightwaveform.h"
#include "colorwidget.h"

#include <QMenu>
#include <QPainter>
#include <QSettings>
#include <QMouseEvent>

#include <math.h>
#include <qmmp/buffer.h>
#include <qmmp/decoder.h>
#include <qmmp/soundcore.h>
#include <qmmp/inputsource.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/audioconverter.h>

#define COLOR_RMS 0
#define COLOR_WAVE 1
#define COLOR_BACKGROUND 2
#define COLOR_PROGRESS 3
#define COLOR_RULER 4

#define RULER_COUNT 5
#define RULER_HEIGHT 20
#define NUMBER_OF_VALUES 4096
// Number of floats fitting into a 128-bit XMM register
#define FLOATS_PER_LANE 4
#define VECTORIZE_HINT 8

/* copied from ardour3 */
static inline float logMeter(float power, double lower_db, double upper_db, double non_linearity)
{
    return (power < lower_db ? 0.0 : pow ((power - lower_db) / (upper_db - lower_db), non_linearity));
}

static inline float logScale(float sample)
{
    const int v = sample > 0.0 ? 1 : -1;
    return v * logMeter(20.0f * log10(v * sample), -192.0, 0.0, 8.0);
}

static inline QRgb colorContrast(const QRgb color)
{
    // Counting the perceptive luminance - human eye favors green color...
    int v = 255 - (2.0 * qRed(color) + 3.0 * qGreen(color) + qBlue(color)) / 6.0;
        v = v < 128 ? 0 : 255;
    // 0, bright colors; 255, dark colors
    return qRgb(v, v, v);
}

static inline QString formatDuration(qint64 ms)
{
    if(ms <= 0)
    {
        return "00:00";
    }

    QString v;
    const qint64 seconds = ms / 1000;
    if(seconds >= 3600)
    {
        v = QString("%1:%2").arg(seconds / 3600).arg(seconds % 3600 / 60, 2, 10, QChar('0'));
    }
    else
    {
        v = QString("%1").arg(seconds % 3600 / 60, 2, 10, QChar('0'));
    }
    v += QString(":%1").arg(seconds % 60, 2, 10, QChar('0'));
    return v;
}

namespace std
{
//    static int gcd(int m, int n)
//    {
//        return n == 0 ? m : gcd(n, m % n);
//    }

//    static int lcm(int m, int n)
//    {
//        return abs(m) / gcd(m, n) * abs(n);
//    }

    static int lcm(int a, int b)
    {
        return b ? lcm(b, a % b) : a;
    }
}



LightWaveFormScanner::LightWaveFormScanner(QObject *parent)
    : QThread(parent)
{

}

LightWaveFormScanner::~LightWaveFormScanner()
{
    stop();
}

bool LightWaveFormScanner::scan(const QString &path)
{
    InputSource *source = InputSource::create(path, this);
    if(!source->initialize())
    {
        delete source;
        qWarning("LightWaveFormScanner: invalid path");
        return false;
    }

    if(source->ioDevice() && !source->ioDevice()->open(QIODevice::ReadOnly))
    {
        source->deleteLater();
        qWarning("LightWaveFormScanner: cannot open input stream, error: %s", qPrintable(source->ioDevice()->errorString()));
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
        factory = Decoder::findByProtocol(source->path().section("://", 0, 0));
    if(!factory)
    {
        qWarning("LightWaveFormScanner: unsupported file format");
        source->deleteLater();
        return false;
    }

    qDebug("LightWaveFormScanner: selected decoder: %s", qPrintable(factory->properties().shortName));
    if(factory->properties().noInput && source->ioDevice())
    {
        source->ioDevice()->close();
    }

    Decoder *decoder = factory->create(source->path(), source->ioDevice());
    if(!decoder->initialize())
    {
        qWarning("LightWaveFormScanner: invalid file format");
        source->deleteLater();
        delete decoder;
        return false;
    }

    if(decoder->totalTime() <= 0)
    {
        source->deleteLater();
        delete decoder;
        return false;
    }

    m_decoder = decoder;
    m_input = source;
    m_stop = false;
    return true;
}

void LightWaveFormScanner::stop()
{
    if(isRunning())
    {
        m_mutex.lock();
        m_stop = true;
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

const QList<int> &LightWaveFormScanner::data() const
{
    QMutexLocker locker(&m_mutex);
    return m_data;
}

const AudioParameters &LightWaveFormScanner::audioParameters() const
{
    return m_ap;
}

void LightWaveFormScanner::run()
{
    m_ap = m_decoder->audioParameters();

    const int channels = m_ap.channels();
    const int frame_size = m_ap.frameSize();
    const int sample_size = m_ap.sampleSize();
    const qint64 frames = m_decoder->totalTime() * m_ap.sampleRate() / 1000;
    const qint64 frames_per_value = frames / NUMBER_OF_VALUES;

    const int in_size = QMMP_BLOCK_FRAMES * frame_size * 4;
    unsigned char* in = new unsigned char[in_size];
    float* out = new float[QMMP_BLOCK_FRAMES * channels * 4];

    AudioConverter converter;
    converter.configure(m_ap.format());

    // Number of elements in each stat buffer
    //  channels = 1, items = 4,  layout = |1111|
    //  channels = 2, items = 4,  layout = |1212|
    //  channels = 3, items = 12, layout = |1231|2312|3123|
    //  channels = 4, items = 4,  layout = |1234|
    //  channels = 5, items = 20, layout = |1234|5123|4512|3451|2345|
    //  channels = 6, items = 12, layout = |1234|5612|3456|
    //  channels = 7, items = 28, layout = |1234|5671|2345|6712|3456|7123|4567|
    //  channels = 8, items = 8,  layout = |1234|5678|
    //
    // For layouts containing less than 16 items, GCC or Clang do not produce
    // vectorized code at -O2. However, multiplying 'samples_per_stat' by a constant
    // results in the desired output.
    const int samples_per_stat = std::lcm(FLOATS_PER_LANE, channels) * VECTORIZE_HINT;
    const int frames_per_stat = samples_per_stat / channels;

    float *vmax = new float[samples_per_stat]{ -1.f };
    float *vmin = new float[samples_per_stat]{  1.f };
    float *vrms = new float[samples_per_stat]{  0.f };

    qint64 frame_counter = 0;
    qint64 frames_left = frames_per_value;

    m_data.clear();
    m_mutex.lock();

    while (!m_stop)
    {
        m_mutex.unlock();
        qint64 len = m_decoder->read(in, in_size);
        if(len > 0)
        {
            qint64 samples = len / sample_size;
            converter.toFloat(in, out, samples);

            for(qint64 sample = 0, samples_left = samples; sample < samples;)
            {
                // Vectorized path which should be taken most of the time
                if(frames_left >= frames_per_stat && samples_left >= samples_per_stat)
                {
                    for(int i = 0; i < samples_per_stat; ++i)
                    {
                        const float value = out[sample + i];
                        vmin[i] = qMin(vmin[i], value);
                        vmax[i] = qMax(vmax[i], value);
                        vrms[i] += (value * value);
                    }

                    sample += samples_per_stat;
                    samples_left -= samples_per_stat;
                    frame_counter += frames_per_stat;
                    frames_left -= frames_per_stat;

                    // Collect stats into the first frame when transitioning to the scalar path
                    if(frames_left < frames_per_stat || samples_left < samples_per_stat)
                    {
                        for(int i = 1; i < frames_per_stat; ++i)
                        {
                            for(int ch = 0; ch < channels; ++ch)
                            {
                                const int offset = channels * i + ch;
                                vmin[ch] = qMin(vmin[ch], vmin[offset]);
                                vmax[ch] = qMax(vmax[ch], vmax[offset]);
                                vrms[ch] += vrms[offset];
                                vmin[offset] =  1.f;
                                vmax[offset] = -1.f;
                                vrms[offset] =  0.f;
                            }
                        }
                    }
                }
                // Scalar path which should be taken when either
                // - the sample buffer is almost exhasted
                // - or, there are almost enough frames to emit a value
                else
                {
                    for(int ch = 0; ch < channels; ++ch)
                    {
                        const float value = out[sample + ch];
                        vmin[ch] = qMin(vmin[ch], value);
                        vmax[ch] = qMax(vmax[ch], value);
                        vrms[ch] += (value * value);
                    }

                    sample += channels;
                    samples_left -= channels;
                    ++frame_counter;
                    --frames_left;
                }

                if(frame_counter >= frames_per_value)
                {
                    m_mutex.lock();
                    for(int ch = 0; ch < channels; ++ch)
                    {
                        m_data << vmax[ch] * 1000;
                        m_data << vmin[ch] * 1000;
                        m_data << std::sqrt(vrms[ch] / frame_counter) * 1000;
                        vmax[ch] = -1.f;
                        vmin[ch] =  1.f;
                        vrms[ch] =  0.f;
                    }

                    if(m_data.size() / 3 / channels % (NUMBER_OF_VALUES / 64) == 0)
                    {
                        emit dataChanged();
                    }

                    m_mutex.unlock();
                    frame_counter = 0;
                    frames_left = frames_per_value;
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

    delete[] in;
    delete[] out;
    delete[] vmin;
    delete[] vmax;
    delete[] vrms;
    m_mutex.unlock();
}



LightWaveForm::LightWaveForm(QWidget *parent)
    : Light(parent)
{
    m_scanner = new LightWaveFormScanner(this);
    connect(m_scanner, SIGNAL(finished()), SLOT(scanFinished()));
    connect(m_scanner, SIGNAL(dataChanged()), SLOT(dataChanged()));

    m_channelsAction = new QAction(tr("Double Channels"), this);
    m_channelsAction->setCheckable(true);
    //: Root mean square
    m_rmsAction = new QAction(tr("Root Mean Square"), this);
    m_rmsAction->setCheckable(true);

    m_rulerAction = new QAction(tr("Display Ruler"), this);
    m_rulerAction->setCheckable(true);

    m_logScaleAction = new QAction(tr("Logarithmic Scale"), this);
    m_logScaleAction->setCheckable(true);

    m_shadeAction = new QAction(tr("Shade Mode"), this);
    m_shadeAction->setCheckable(true);

    m_cloudAction = new QAction(tr("Cloud Mode"), this);
    m_cloudAction->setCheckable(true);

    m_pointsAction = new QAction(tr("Points Mode"), this);
    m_pointsAction->setCheckable(true);

    m_fatAction = new QAction(tr("Fat Mode"), this);
    m_fatAction->setCheckable(true);

    connect(SoundCore::instance(), SIGNAL(trackInfoChanged()), SLOT(mediaUrlChanged()));
    connect(SoundCore::instance(), SIGNAL(elapsedChanged(qint64)), SLOT(positionChanged(qint64)));

    readSettings();
}

LightWaveForm::~LightWaveForm()
{
    stop();
    delete m_scanner;
}

void LightWaveForm::open(const QString &path)
{
    if(m_scanner->scan(path))
    {
        start();
    }
    else
    {
        qDebug("LightWaveFormThread: init error");
    }
}

void LightWaveForm::start()
{
    m_scanner->start();
}

void LightWaveForm::stop()
{
    m_scanner->stop();
}

void LightWaveForm::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("LightWaveForm");
    m_channelsAction->setChecked(settings.value("show_two_channels", true).toBool());
    m_rmsAction->setChecked(settings.value("show_rms", true).toBool());
    m_rulerAction->setChecked(settings.value("show_ruler", false).toBool());
    m_logScaleAction->setChecked(settings.value("logarithmic_scale", false).toBool());
    m_shadeAction->setChecked(settings.value("shade_mode", false).toBool());
    m_cloudAction->setChecked(settings.value("cloud_mode", false).toBool());
    m_pointsAction->setChecked(settings.value("points_mode", false).toBool());
    m_fatAction->setChecked(settings.value("fat_mode", false).toBool());
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    settings.endGroup();

    if(m_colors.count() < 5)
    {
        m_colors.clear();
        //: RMS\Wave\Background\Progress\Ruler
        m_colors << QColor(0xDD, 0xDD, 0xDD) << QColor(0xFF, 0xFF, 0xFF) << QColor(0x00, 0x00, 0x00) << QColor(0x33, 0xCA, 0x10) << QColor(0xBB, 0xBB, 0xBB);
    }

    drawWaveform();
}

void LightWaveForm::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("LightWaveForm");
    settings.setValue("show_two_channels", m_channelsAction->isChecked());
    settings.setValue("show_rms", m_rmsAction->isChecked());
    settings.setValue("show_ruler", m_rulerAction->isChecked());
    settings.setValue("logarithmic_scale", m_logScaleAction->isChecked());
    settings.setValue("shade_mode", m_shadeAction->isChecked());
    settings.setValue("cloud_mode", m_cloudAction->isChecked());
    settings.setValue("points_mode", m_pointsAction->isChecked());
    settings.setValue("fat_mode", m_fatAction->isChecked());
    settings.setValue("colors", ColorWidget::writeColorConfig(m_colors));
    settings.endGroup();

    drawWaveform();
}

void LightWaveForm::scanFinished()
{
    m_data = m_scanner->data();
    m_channels = m_scanner->audioParameters().channels();
    drawWaveform();
}

void LightWaveForm::dataChanged()
{
    if(!m_scanner->isRunning())
    {
        return;
    }

    m_data = m_scanner->data();
    m_channels = m_scanner->audioParameters().channels();
    drawWaveform();
}

void LightWaveForm::mediaUrlChanged()
{
    stop();
    open(SoundCore::instance()->path());
}

void LightWaveForm::positionChanged(qint64 elapsed)
{
    m_elapsed = elapsed;
    m_duration = SoundCore::instance()->duration();

    if(isVisible())
    {
        update();
    }
}

void LightWaveForm::typeChanged(QAction *action)
{
    int type = -1;
    switch(action->data().toInt())
    {
        case 10: type = COLOR_RMS; break;
        case 20: type = COLOR_WAVE; break;
        case 30: type = COLOR_BACKGROUND; break;
        case 40: type = COLOR_PROGRESS; break;
        case 50: type = COLOR_RULER; break;
        default: break;
    }

    if(type == -1)
    {
        return;
    }

    ColorWidget dialog;
    dialog.setSingleMode(true);
    dialog.setColors(QList<QColor>() << m_colors[type]);
    if(dialog.exec())
    {
        const QList<QColor> &colors = dialog.colors();
        if(!colors.isEmpty())
        {
            m_colors[type] = colors.front();
        }
    }
}

void LightWaveForm::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_colors[COLOR_BACKGROUND]);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    const bool shadeMode = m_shadeAction->isChecked();
    if(!m_pixmap.isNull() && !shadeMode)
    {
        painter.drawPixmap(0, 0, width(), height(), m_pixmap);
    }

    if(m_duration > 0)
    {
        const int x = width() * m_elapsed / m_duration;
        QColor color = m_colors[COLOR_PROGRESS];

        if(!shadeMode)
        {
            color.setAlpha(0x96);
            painter.fillRect(0, 0, x, height(), color);
        }
        else
        {
            QImage shadeImage = m_pixmap.toImage();
            for(int h = 0; h < shadeImage.height(); ++h)
            {
                for(int w = 0; w < x; ++w)
                {
                    if(shadeImage.pixel(w, h) == m_colors[COLOR_WAVE].rgb())
                    {
                        shadeImage.setPixel(w, h, m_colors[COLOR_PROGRESS].rgb());
                    }
                    else if(shadeImage.pixel(w, h) == m_colors[COLOR_RMS].rgb())
                    {
                        const QRgb rms = m_colors[COLOR_PROGRESS].rgb();
                        shadeImage.setPixel(w, h, qRgb(qRed(rms) * 0.8, qGreen(rms) * 0.8, qBlue(rms) * 0.8));
                    }
                }
            }
            painter.drawImage(0, 0, shadeImage);
        }

        color.setAlpha(0xFF);
        painter.fillRect(x, 0, 3, height(), color);

        if(m_seekPos >= 0)
        {
            painter.fillRect(m_seekPos, 0, 3, height(), color);

            QFont ft = painter.font();
            ft.setPixelSize(18);
            painter.setFont(ft);

            const QString &text = formatDuration(m_seekPos * m_duration / width());
            const QFontMetrics ftm(ft);
#if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
            const QSize bound(ftm.horizontalAdvance(text) + 20, ftm.height() + 4);
#else
            const QSize bound(ftm.width(text) + 20, ftm.height() + 4);
#endif
            const int left = m_seekPos - bound.width();
            const QRect rect(QPoint(left < 0 ? 0 : left, (height() - bound.height()) / 2), bound);
            painter.fillRect(rect, color);
            painter.setPen(QColor(colorContrast(m_colors[COLOR_PROGRESS].rgb())));
            painter.drawText(rect, Qt::AlignCenter, text);
        }

        if(m_rulerAction->isChecked())
        {
            const int height = this->height() - RULER_HEIGHT;
            const int padding = this->height() - 2;
            const int outWidth = width() / RULER_COUNT;
            const int inWidth = outWidth / RULER_COUNT;
            const int perTime = m_duration / RULER_COUNT;

            painter.setPen(m_colors[COLOR_RULER]);
            // outside line
            for(int i = 0; i <= RULER_COUNT; ++i)
            {
                for(int j = 0; j < RULER_COUNT; ++j)
                {
                    const int lineHeight = height + (j == 0 ? 0 : RULER_HEIGHT * 2 / 3);
                    painter.drawLine(outWidth * i + inWidth * j + 1, lineHeight, outWidth * i + inWidth * j + 1, padding);
                }

                QFont ft = painter.font();
                ft.setPixelSize(8);
                painter.setFont(ft);
                painter.drawText(outWidth * i + 1 + 3, height, outWidth, RULER_HEIGHT * 2 / 3, Qt::AlignLeft, formatDuration(perTime * i));
            }
            // right line
            painter.drawLine(width() - 1, height, width() - 1, padding);
            // bottom line
            painter.drawLine(0, padding + 1, width(), padding + 1);
        }
    }
}

void LightWaveForm::resizeEvent(QResizeEvent *)
{
    drawWaveform();
}

void LightWaveForm::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_seekPos = e->pos().x();
        update();
    }
}

void LightWaveForm::mouseMoveEvent(QMouseEvent *e)
{
    if(m_seekPos >= 0)
    {
        m_seekPos = qBound(0, e->pos().x(), width());
        update();
    }
}

void LightWaveForm::mouseReleaseEvent(QMouseEvent *)
{
    if(m_seekPos >= 0 && m_duration > 0)
    {
        m_elapsed = m_seekPos * m_duration / width();
        SoundCore::instance()->seek(m_elapsed);
        m_seekPos = -1;
        update();
    }
}

void LightWaveForm::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    menu.addAction(m_logScaleAction);

    QMenu displayMenu(tr("Display"), &menu);
    displayMenu.addAction(m_channelsAction);
    displayMenu.addAction(m_rmsAction);
    displayMenu.addAction(m_rulerAction);
    menu.addMenu(&displayMenu);

    QMenu modeMenu(tr("Mode"), &menu);
    modeMenu.addAction(m_shadeAction);
    modeMenu.addAction(m_cloudAction);
    modeMenu.addAction(m_pointsAction);
    modeMenu.addAction(m_fatAction);
    menu.addMenu(&modeMenu);

    QMenu colorMenu(tr("Color"), &menu);
    colorMenu.addAction(tr("RMS"))->setData(10);
    colorMenu.addAction(tr("Wave"))->setData(20);
    colorMenu.addAction(tr("Background"))->setData(30);
    colorMenu.addAction(tr("Progress"))->setData(40);
    colorMenu.addAction(tr("Ruler"))->setData(50);
    connect(&colorMenu, SIGNAL(triggered(QAction*)), this, SLOT(typeChanged(QAction*)));
    menu.addMenu(&colorMenu);
    menu.exec(QCursor::pos());
}

void LightWaveForm::drawWaveform()
{
    if(m_data.isEmpty())
    {
        m_pixmap = QPixmap();
        update();
        return;
    }

    m_pixmap = QPixmap(width(), height());
    m_pixmap.fill(Qt::transparent);

    const bool showTwoChannels = m_channelsAction->isChecked();
    const bool showRms = m_rmsAction->isChecked();
    const bool logMode = m_logScaleAction->isChecked();
    const bool cloudMode = m_cloudAction->isChecked();
    const bool pointsMode = m_pointsAction->isChecked();

    const float step = float(width()) / NUMBER_OF_VALUES;

    QBrush brush = m_colors[COLOR_WAVE];
    if(cloudMode)
    {
        QColor color = brush.color();
        QLinearGradient gradient(0, 0, 0, showTwoChannels ? height() / 2 : height());
        color.setAlpha(0);
        gradient.setColorAt(0.00, color);
        color.setAlpha(40);
        gradient.setColorAt(0.49, color);
        color.setAlpha(10);
        gradient.setColorAt(0.50, color);
        gradient.setColorAt(1.00, color);
        gradient.setSpread(QGradient::RepeatSpread);
        brush = gradient;
    }

    QPainter painter(&m_pixmap);
    painter.setPen(QPen(brush, m_fatAction->isChecked() ? 2 : 1));
    painter.setBrush(brush);

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
            ratio = float(height() / 4) / (logMode ? (4.0 / height() * 10) : 1000);
            draw = true;
        }
        else if(ch == 0 || (ch == 1 && showTwoChannels))
        {
            zeroPos = ((ch == 0) ? 1 : 3) * height() / 4;
            ratio = float(height() / 8) / (logMode ? (8.0 / height() * 10) : 1000);
            draw = true;
        }

        if(draw)
        {
            const float y1 = zeroPos - (logMode ? logScale(m_data[i] * ratio) : m_data[i] * ratio);
            const float y2 = zeroPos - (logMode ? logScale(m_data[i + 1] * ratio) : m_data[i + 1] * ratio);
            const float y3 = zeroPos - (logMode ? logScale(m_data[i + m_channels * 3] * ratio) : m_data[i + m_channels * 3] * ratio);
            const float y4 = zeroPos - (logMode ? logScale(m_data[i + m_channels * 3 + 1] * ratio) : m_data[i + m_channels * 3 + 1] * ratio);

            const QPointF points[4] = {
                { x1, y1 },
                { x1, y2 },
                { x2, y4 },
                { x2, y3 }
            };

            pointsMode ? painter.drawPoints(points, 4) : painter.drawPolygon(points, 4);
        }
    }

    if(!showRms)
    {
        update();
        return;
    }

    if(!cloudMode)
    {
        brush = m_colors[COLOR_RMS];
    }

    painter.setPen(QPen(brush, 1));
    painter.setBrush(brush);

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
            ratio = float(height() / 4) / (logMode ? (4.0 / height() * 10) : 1000);
            draw = true;
        }
        else if(ch == 0 || (ch == 1 && showTwoChannels))
        {
            zeroPos = ((ch == 0) ? 1 : 3) * height() / 4;
            ratio = float(height() / 8) / (logMode ? (8.0 / height() * 10) : 1000);
            draw = true;
        }

        if(draw)
        {
            const float y1 = zeroPos + (logMode ? logScale(m_data[i + 2] * ratio) : m_data[i + 2] * ratio);
            const float y2 = zeroPos - (logMode ? logScale(m_data[i + 2] * ratio) : m_data[i + 2] * ratio);
            const float y3 = zeroPos + (logMode ? logScale(m_data[i + m_channels * 3 + 2] * ratio) : m_data[i + m_channels * 3 + 2] * ratio);
            const float y4 = zeroPos - (logMode ? logScale(m_data[i + m_channels * 3 + 2] * ratio) : m_data[i + m_channels * 3 + 2] * ratio);

            const QPointF points[4] = {
                { x1, y1 },
                { x1, y2 },
                { x2, y4 },
                { x2, y3 }
            };

            pointsMode ? painter.drawPoints(points, 4) : painter.drawPolygon(points, 4);
        }
    }

    update();
}
