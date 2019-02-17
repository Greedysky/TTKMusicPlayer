#include "spek-audio.h"
#include "spek-fft.h"
#include "spek-ruler.h"
#include "spek-spectrogram.h"
#include "soundcore.h"

#include <cmath>
#include <QMenu>
#include <QPainter>
#include <QDateTime>
#include <QApplication>
#include <QDebug>

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

SpectrumSpek::SpectrumSpek(QWidget *parent) :
    Spek(parent),
    audio(new Audio()), // TODO: refactor
    fft(new FFT()),
    pipeline(NULL),
    streams(0),
    stream(0),
    channels(0),
    channel(0),
    window_function(WINDOW_DEFAULT),
    duration(0.0),
    sample_rate(0),
    palette(PALETTE_SPECTRUM),
    prev_width(-1),
    fft_bits(FFT_BITS),
    urange(URANGE),
    lrange(LRANGE)
{
    this->create_palette();
    connect(SoundCore::instance(), SIGNAL(metaDataChanged()), SLOT(urlChanged()));
}

SpectrumSpek::~SpectrumSpek()
{
    qDebug() << "~SpectrumSpek";
    this->stop();
}

void SpectrumSpek::open(const QString &path)
{
    this->path = path;
    this->stream = 0;
    this->channel = 0;
    start();
}

void SpectrumSpek::paintEvent(QPaintEvent *event)
{
    Spek::paintEvent(event);
    QPainter p(this);
    paint(&p);
}

void SpectrumSpek::resizeEvent(QResizeEvent *event)
{
    Spek::resizeEvent(event);
    start();
}

void SpectrumSpek::contextMenuEvent(QContextMenuEvent *event)
{
    Spek::contextMenuEvent(event);
    QMenu menu(this);
    QMenu typeMenu("Type", &menu);
    typeMenu.addAction("Spectrum")->setData(10);
    typeMenu.addAction("Sox")->setData(20);
    typeMenu.addAction("Mono")->setData(30);
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

void SpectrumSpek::paint(QPainter *dc)
{
    int w = width();
    int h = height();

    // Initialise.
    dc->setBrush(Qt::black);
    dc->drawRect(this->rect());
    dc->setPen(Qt::white);

    // Border around the spectrogram.
    dc->drawRect(LPAD - 1, TPAD - 1, w - LPAD - RPAD + 2, h - TPAD - BPAD + 2);

    if (this->image.width() > 1 && this->image.height() > 1 &&
        w - LPAD - RPAD > 0 && h - TPAD - BPAD > 0) {
        // Draw the spectrogram.
        dc->drawImage(LPAD, TPAD, this->image.scaled(w - LPAD - RPAD, h - TPAD - BPAD));
        // File name.
        dc->drawText(
            LPAD,
            TPAD - 2 * GAP,
            trim(this->path, w - LPAD - RPAD, false)
        );

        if (this->duration) {
            // Time ruler.
            int time_factors[] = {1, 2, 5, 10, 20, 30, 1*60, 2*60, 5*60, 10*60, 20*60, 30*60, 0};
            SpekRuler time_ruler(
                LPAD,
                h - BPAD,
                SpekRuler::BOTTOM,
                // TODO: i18n
                "00:00",
                time_factors,
                0,
                (int)this->duration,
                1.5,
                (w - LPAD - RPAD) / this->duration,
                0.0,
                time_formatter
                );
            time_ruler.draw(*dc);
        }

        if (this->sample_rate) {
            // Frequency ruler.
            int freq = this->sample_rate / 2;
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
    if (h - TPAD - BPAD > 0) {
        dc->drawImage(w - RPAD + GAP, TPAD, this->palette_image.scaled(RULER, h - TPAD - BPAD + 1));

        int density_factors[] = {1, 2, 5, 10, 20, 50, 0};
        SpekRuler density_ruler(
            w - RPAD + GAP + RULER,
            TPAD,
            SpekRuler::RIGHT,
            // TRANSLATORS: keep "-00" unchanged, it's used to calc the text width
            "-00 dB",
            density_factors,
            -this->urange,
            -this->lrange,
            3.0,
            (h - TPAD - BPAD) / (double)(this->lrange - this->urange),
            h - TPAD - BPAD,
            density_formatter
        );
        density_ruler.draw(*dc);
    }
}

static void pipeline_cb(int bands, int sample, float *values, void *cb_data)
{
    SpectrumSpek *spek = static_cast<SpectrumSpek*>(cb_data);
    if (sample == -1) {
//        spek->stop();
        return;
    }

    // TODO: check image size, quit if wrong.
    double range = spek->getURange() - spek->getLRange();
    for (int y = 0; y < bands; y++) {
        double value = fmin(spek->getURange(), fmax(spek->getLRange(), values[y]));
        double level = (value - spek->getLRange()) / range;
        uint32_t color = spek_palette(spek->getPalette(), level);
        spek->getPaintImage()->setPixel(sample, bands - y - 1, color);
    }
    spek->update();
}

void SpectrumSpek::start()
{
    if (this->path.isEmpty()) {
        return;
    }

    this->stop();
    // The number of samples is the number of pixels available for the image.
    // The number of bands is fixed, FFT results are very different for
    // different values but we need some consistency.
    int samples = width() - LPAD - RPAD;
    if (samples > 0) {
        this->image = QImage(samples, bits_to_bands(this->fft_bits), QImage::Format_RGB32);
        this->pipeline = spek_pipeline_open(
            this->audio->open(std::string(this->path.toUtf8().data()), this->stream),
            this->fft->create(this->fft_bits),
            this->stream,
            this->channel,
            this->window_function,
            samples,
            pipeline_cb,
            this
        );
        spek_pipeline_start(this->pipeline);
//        // TODO: extract conversion into a utility function.
//        this->desc = QString(spek_pipeline_desc(this->pipeline).c_str());
        this->streams = spek_pipeline_streams(this->pipeline);
        this->channels = spek_pipeline_channels(this->pipeline);
        this->duration = spek_pipeline_duration(this->pipeline);
        this->sample_rate = spek_pipeline_sample_rate(this->pipeline);
    }
}

void SpectrumSpek::stop()
{
    if (this->pipeline) {
        spek_pipeline_close(this->pipeline);
        this->pipeline = NULL;
    }
}

void SpectrumSpek::typeChanged(QAction *action)
{
    switch(action->data().toInt())
    {
        case 10: palette = PALETTE_SPECTRUM; break;
        case 20: palette = PALETTE_SOX; break;
        case 30: palette = PALETTE_MONO; break;
        default: break;
    }
    create_palette();
    start();
}

void SpectrumSpek::urlChanged()
{
    open(SoundCore::instance()->path());
}

void SpectrumSpek::create_palette()
{
    this->palette_image = QImage(RULER, bits_to_bands(this->fft_bits), QImage::Format_RGB32);
    for (int y = 0; y < bits_to_bands(this->fft_bits); y++) {
        uint32_t color = spek_palette(this->palette, y / (double)bits_to_bands(this->fft_bits));
        for(int j =0; j < RULER; ++j) {
            this->palette_image.setPixel(
                j,
                bits_to_bands(this->fft_bits) - y - 1,
                color
            );
        }
    }
}

static QString trim(const QString& s, int length, bool trim_end)
{
    if (length <= 0) {
        return QString();
    }

    // Check if the entire string fits.
    QFontMetrics f(QApplication::font());
    int w = f.width(s);
    if (w <= length) {
        return s;
    }

    // Binary search FTW!
    QString fix("...");
    int i = 0;
    int k = s.length();
    while (k - i > 1) {
        int j = (i + k) / 2;
        w = f.width(trim_end ? s.mid(0, j) + fix : fix + s.mid(j));
        if (trim_end != (w > length)) {
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
