#include "spek-audio.h"
#include "spek-fft.h"
#include "spek-ruler.h"
#include "spek-utils.h"
#include "spek-spectrogram.h"
#include <qmmp/soundcore.h>

#include <math.h>
#include <QMenu>
#include <QPainter>
#include <QDateTime>
#include <QKeyEvent>
#include <QApplication>

enum
{
    MIN_RANGE = -140,
    MAX_RANGE = 0,
    URANGE = 0,
    LRANGE = -120,
    FFT_BITS = 11,
    MIN_FFT_BITS = 8,
    MAX_FFT_BITS = 14,
    LPAD = 60,
    TPAD = 60,
    RPAD = 90,
    BPAD = 40,
    GAP = 10,
    RULER = 10,
};

// Forward declarations.
static QString trim(const QString& s, int length, bool trim_end);
static int bits_to_bands(int bits);

LightSpectrum::LightSpectrum(QWidget *parent)
    : Light(parent),
      m_audio(new Audio()), // TODO: refactor
      m_fft(new FFT()),
      m_bits(FFT_BITS),
      m_urange(URANGE),
      m_lrange(LRANGE)
{
    create_palette();
    connect(SoundCore::instance(), SIGNAL(trackInfoChanged()), SLOT(mediaUrlChanged()));
}

LightSpectrum::~LightSpectrum()
{
    stop();
}

void LightSpectrum::open(const QString &path)
{
    m_path = path;
    m_stream = 0;
    m_channel = 0;
    start();
}

void LightSpectrum::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_C:
        if(m_channels) {
            if(event->modifiers() == Qt::NoModifier) {   // 'c'
                m_channel = (m_channel + 1) % m_channels;
            } else if(event->modifiers() == Qt::ShiftModifier) {   // 'C'
                m_channel = (m_channel - 1 + m_channels) % m_channels;
            }
        }
        break;
    case Qt::Key_F:
        if(event->modifiers() == Qt::NoModifier) {   // 'f'
            m_window = (WindowFunction) ((m_window + 1) % WINDOW_COUNT);
        } else if(event->modifiers() == Qt::ShiftModifier) {   // 'F'
            m_window = (WindowFunction) ((m_window - 1 + WINDOW_COUNT) % WINDOW_COUNT);
        }
        break;
    case Qt::Key_L:
        if(event->modifiers() == Qt::NoModifier) {   // 'l'
            m_lrange = spek_min(m_lrange + 1, m_urange - 1);
        } else if(event->modifiers() == Qt::ShiftModifier) {   // 'L'
            m_lrange = spek_max(m_lrange - 1, MIN_RANGE);
        }
        break;
    case Qt::Key_P:
        if(event->modifiers() == Qt::NoModifier) {   // 'p'
            m_palette = (VisualPalette::Palette) ((m_palette + 1) % VisualPalette::PALETTE_COUNT);
            create_palette();
        } else if(event->modifiers() == Qt::ShiftModifier) {   // 'P'
            m_palette = (VisualPalette::Palette) ((m_palette - 1 + VisualPalette::PALETTE_COUNT) % VisualPalette::PALETTE_COUNT);
            create_palette();
        }
        break;
    case Qt::Key_S:
        if(m_streams) {
            if(event->modifiers() == Qt::NoModifier) {   // 's'
                m_stream = (m_stream + 1) % m_streams;
            } else if(event->modifiers() == Qt::ShiftModifier) {   // 'S'
                m_stream = (m_stream - 1 + m_streams) % m_streams;
            }
        }
        break;
    case Qt::Key_U:
        if(event->modifiers() == Qt::NoModifier) {   // 'u'
            m_urange = spek_min(m_urange + 1, MAX_RANGE);
        } else if(event->modifiers() == Qt::ShiftModifier) {   // 'U'
            m_urange = spek_max(m_urange - 1, m_lrange + 1);
        }
        break;
    case Qt::Key_W:
        if(event->modifiers() == Qt::NoModifier) {   // 'w'
            m_bits = spek_min(m_bits + 1, MAX_FFT_BITS);
            this->create_palette();
        } else if(event->modifiers() == Qt::ShiftModifier) {   // 'W'
            m_bits = spek_max(m_bits - 1, MIN_FFT_BITS);
            this->create_palette();
        }
        break;
    default:
        event->ignore();
        return;
    }

    start();
}

void LightSpectrum::paintEvent(QPaintEvent *e)
{
    Light::paintEvent(e);
    QPainter p(this);
    paint(&p);
}

void LightSpectrum::resizeEvent(QResizeEvent *e)
{
    Light::resizeEvent(e);
    start();
}

void LightSpectrum::contextMenuEvent(QContextMenuEvent *e)
{
    Light::contextMenuEvent(e);

    QMenu menu(this);
    QMenu typeMenu(tr("Type"), &menu);
    typeMenu.addAction(tr("Spectrum"))->setData(10);
    typeMenu.addAction(tr("Spectrogram"))->setData(20);
    typeMenu.addAction(tr("Sox"))->setData(30);
    typeMenu.addAction(tr("Mono"))->setData(40);
    connect(&typeMenu, SIGNAL(triggered(QAction*)), this, SLOT(typeChanged(QAction*)));
    menu.addMenu(&typeMenu);
    menu.exec(QCursor::pos());
}

static QString time_formatter(int unit)
{
    // TODO: i18n
    return QTime(0, 0).addSecs(unit).toString("mm:ss");
}

static QString freq_formatter(int unit)
{
    return QString("%1 kHz").arg(unit / 1000);
}

static QString density_formatter(int unit)
{
    return QString("%1 dB").arg(-unit);
}

void LightSpectrum::paint(QPainter *dc)
{
    int w = width();
    int h = height();

    // Initialise.
    dc->setBrush(Qt::black);
    dc->drawRect(rect());
    dc->setPen(Qt::white);

    // Border around the spectrogram.
    dc->drawRect(LPAD - 1, TPAD - 1, w - LPAD - RPAD + 2, h - TPAD - BPAD + 2);

    if(m_image.width() > 1 && m_image.height() > 1 &&
        w - LPAD - RPAD > 0 && h - TPAD - BPAD > 0) {
        // Draw the spectrogram.
        dc->drawImage(LPAD, TPAD, m_image.scaled(w - LPAD - RPAD, h - TPAD - BPAD));
        // File name.
        dc->drawText(
            LPAD,
            TPAD - 2 * GAP,
            trim(m_path, w - LPAD - RPAD, false)
        );

        if(m_duration) {
            // Time ruler.
            int time_factors[] = {1, 2, 5, 10, 20, 30, 1 * 60, 2 * 60, 5 * 60, 10 * 60, 20 * 60, 30 * 60, 0};
            SpekRuler time_ruler(
                LPAD,
                h - BPAD,
                SpekRuler::BOTTOM,
                // TODO: i18n
                "00:00",
                time_factors,
                0,
                (int)m_duration,
                1.5,
                (w - LPAD - RPAD) / m_duration,
                0.0,
                time_formatter
                );
            time_ruler.draw(*dc);
        }

        if(m_sampleRate) {
            // Frequency ruler.
            int freq = m_sampleRate / 2;
            int freq_factors[] = {1000, 2000, 5000, 10000, 20000, 0};
            SpekRuler freq_ruler(
                LPAD,
                TPAD,
                SpekRuler::LEFT,
                // TRANSLATORS: keep "00" unchanged, it's used to calc the text width
                "00 kHz",
                freq_factors,
                0,
                freq,
                3.0,
                (h - TPAD - BPAD) / (double)freq,
                0.0,
                freq_formatter
                );
            freq_ruler.draw(*dc);
        }
    }

    // The palette.
    if(h - TPAD - BPAD > 0) {
        dc->drawImage(w - RPAD + GAP, TPAD, m_paletteImage.scaled(RULER, h - TPAD - BPAD + 1));

        int factors[] = {1, 2, 5, 10, 20, 50, 0};
        SpekRuler density_ruler(
            w - RPAD + GAP + RULER,
            TPAD,
            SpekRuler::RIGHT,
            // TRANSLATORS: keep "-00" unchanged, it's used to calc the text width
            "-00 dB",
            factors,
            -m_urange,
            -m_lrange,
            3.0,
            (h - TPAD - BPAD) / (double)(m_lrange - m_urange),
            h - TPAD - BPAD,
            density_formatter
        );
        density_ruler.draw(*dc);
    }
}

static void pipeline(int bands, int sample, float *values, void *cb_data)
{
    LightSpectrum *spek = static_cast<LightSpectrum*>(cb_data);
    if(sample == -1) {
        return;
    }

    // TODO: check image size, quit if wrong.
    const double range = spek->getURange() - spek->getLRange();
    for(int y = 0; y < bands; ++y) {
        const double value = fmin(spek->getURange(), fmax(spek->getLRange(), values[y]));
        const double level = (value - spek->getLRange()) / range;
        const uint32_t color = VisualPalette::renderPalette(spek->getPalette(), level);
        spek->getPaintImage()->setPixel(sample, bands - y - 1, color);
    }
    spek->update();
}

void LightSpectrum::start()
{
    if(m_path.isEmpty()) {
        return;
    }

    stop();
    // The number of samples is the number of pixels available for the image.
    // The number of bands is fixed, FFT results are very different for
    // different values but we need some consistency.
    int samples = width() - LPAD - RPAD;
    if(samples > 0) {
        m_image = QImage(samples, bits_to_bands(m_bits), QImage::Format_RGB32);
        m_pipeline = spek_pipeline_open(
            m_audio->open(std::string(m_path.toUtf8().data()), m_stream),
            m_fft->create(m_bits),
            m_stream,
            m_channel,
            m_window,
            samples,
            pipeline,
            this
        );
        spek_pipeline_start(m_pipeline);
//        // TODO: extract conversion into a utility function.
        m_streams = spek_pipeline_streams(m_pipeline);
        m_channels = spek_pipeline_channels(m_pipeline);
        m_duration = spek_pipeline_duration(m_pipeline);
        m_sampleRate = spek_pipeline_sample_rate(m_pipeline);
    }
}

void LightSpectrum::stop()
{
    if(m_pipeline) {
        spek_pipeline_close(m_pipeline);
        m_pipeline = nullptr;
    }
}

void LightSpectrum::mediaUrlChanged()
{
    open(SoundCore::instance()->path());
}

void LightSpectrum::typeChanged(QAction *action)
{
    switch(action->data().toInt())
    {
        case 10: m_palette = VisualPalette::PALETTE_SPECTRUM; break;
        case 20: m_palette = VisualPalette::PALETTE_SPECTROGRAM; break;
        case 30: m_palette = VisualPalette::PALETTE_SOX; break;
        case 40: m_palette = VisualPalette::PALETTE_MONO; break;
        default: break;
    }

    create_palette();
    start();
}

void LightSpectrum::create_palette()
{
    m_paletteImage = QImage(RULER, bits_to_bands(m_bits), QImage::Format_RGB32);
    for(int y = 0; y < bits_to_bands(m_bits); ++y) {
        const uint32_t color = VisualPalette::renderPalette(m_palette, y / (double)bits_to_bands(m_bits));
        for(int j = 0; j < RULER; ++j) {
            m_paletteImage.setPixel(
                j,
                bits_to_bands(m_bits) - y - 1,
                color
            );
        }
    }
}

static QString trim(const QString& s, int length, bool trim_end)
{
    if(length <= 0) {
        return QString();
    }

    // Check if the entire string fits.
    QFontMetrics f(QApplication::font());
    int w = f.width(s);
    if(w <= length) {
        return s;
    }

    // Binary search FTW!
    QString fix("...");
    int i = 0;
    int k = s.length();
    while(k - i > 1) {
        int j = (i + k) / 2;
        w = f.width(trim_end ? s.mid(0, j) + fix : fix + s.mid(j));
        if(trim_end != (w > length)) {
            i = j;
        } else {
            k = j;
        }
    }

    return trim_end ? s.mid(0, i) + fix : fix + s.mid(k);
}

static int bits_to_bands(int bits) {
    return (1 << (bits - 1)) + 1;
}
