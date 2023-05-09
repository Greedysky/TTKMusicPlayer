#include "spek-audio.h"
#include "spek-fft.h"
#include "spek-ruler.h"
#include "spek-utils.h"
#include "spek-spectrogram.h"
#include <qmmp/soundcore.h>

#include <math.h>
#include <QMenu>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QDateTime>
#include <QFileInfo>
#include <QFileDialog>

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
static QString trim(QPainter *dc, const QString &s, int length, bool trim_end);
static int bits_to_bands(int bits);

LightSpectrogram::LightSpectrogram(QWidget *parent)
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

LightSpectrogram::~LightSpectrogram()
{
    stop();
}

void LightSpectrogram::open(const QString &path)
{
    m_path = path;
    m_stream = 0;
    m_channel = 0;
    start();
}

void LightSpectrogram::paintEvent(QPaintEvent *e)
{
    Light::paintEvent(e);
    QPainter p(this);
    paint(&p);
}

void LightSpectrogram::resizeEvent(QResizeEvent *e)
{
    Light::resizeEvent(e);

    if (m_widget) {
        m_widget->move(0, (height() - m_widget->height()) / 2);
    }
    start();
}

void LightSpectrogram::contextMenuEvent(QContextMenuEvent *e)
{
    Light::contextMenuEvent(e);

    QMenu menu(this);
    menu.addAction(tr("Settings"), this, SLOT(showSettings()));
    menu.addAction(tr("Screenshot"), this, SLOT(saveScreenshot()));
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

void LightSpectrogram::paint(QPainter *dc)
{
    const int w = width();
    const int h = height();

    // Initialise.
    dc->setBrush(Qt::black);
    dc->drawRect(rect());
    dc->setPen(Qt::white);

    // Border around the spectrogram.
    dc->drawRect(LPAD - 1, TPAD - 1, w - LPAD - RPAD + 2, h - TPAD - BPAD + 2);

    QFont smallFont = dc->font();
    QFont largeFont = dc->font();

    smallFont.setPointSize(8);
    largeFont.setPointSize(10);
    largeFont.setBold(true);

    const int largeHeight = QFontMetrics(largeFont).height();

    if (m_image.width() > 1 && m_image.height() > 1 &&
        w - LPAD - RPAD > 0 && h - TPAD - BPAD > 0) {
        // Draw the spectrogram.
        dc->drawImage(LPAD, TPAD, m_image.scaled(w - LPAD - RPAD, h - TPAD - BPAD));

        // File name.
        dc->setFont(largeFont);
        dc->drawText(
            LPAD,
            TPAD - 2 * GAP - largeHeight,
            trim(dc, m_path, w - LPAD - RPAD, false)
        );

        // File properties.
        dc->setFont(smallFont);
        dc->drawText(
            LPAD,
            TPAD - GAP,
            trim(dc, m_desc, w - LPAD - RPAD, true)
        );

        if (m_duration) {
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

        if (m_sampleRate) {
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
    if (h - TPAD - BPAD > 0) {
        dc->drawImage(w - RPAD + GAP, TPAD, m_paletteImage.scaled(RULER, h - TPAD - BPAD + 1));

        // Spectral density.
        int density_factors[] = {1, 2, 5, 10, 20, 50, 0};
        SpekRuler density_ruler(
            w - RPAD + GAP + RULER,
            TPAD,
            SpekRuler::RIGHT,
            // TRANSLATORS: keep "-00" unchanged, it's used to calc the text width
            "-00 dB",
            density_factors,
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
    LightSpectrogram *spek = static_cast<LightSpectrogram*>(cb_data);
    if (sample == -1) {
        return;
    }

    // TODO: check image size, quit if wrong.
    const double range = spek->getURange() - spek->getLRange();
    for (int y = 0; y < bands; ++y) {
        const double value = fmin(spek->getURange(), fmax(spek->getLRange(), values[y]));
        const double level = (value - spek->getLRange()) / range;
        const uint32_t color = VisualPalette::renderPalette(spek->getPalette(), level);
        spek->getPaintImage()->setPixel(sample, bands - y - 1, color);
    }
    spek->update();
}

void LightSpectrogram::start()
{
    if (m_path.isEmpty()) {
        return;
    }

    stop();
    // The number of samples is the number of pixels available for the image.
    // The number of bands is fixed, FFT results are very different for
    // different values but we need some consistency.
    const int samples = width() - LPAD - RPAD;
    if (samples > 0) {
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
        // TODO: extract conversion into a utility function.
        m_desc = spek_pipeline_desc(m_pipeline).c_str();
        m_streams = spek_pipeline_streams(m_pipeline);
        m_channels = spek_pipeline_channels(m_pipeline);
        m_duration = spek_pipeline_duration(m_pipeline);
        m_sampleRate = spek_pipeline_sample_rate(m_pipeline);
    }
}

void LightSpectrogram::stop()
{
    if (m_pipeline) {
        spek_pipeline_close(m_pipeline);
        m_pipeline = nullptr;
    }
}

void LightSpectrogram::mediaUrlChanged()
{
    open(SoundCore::instance()->path());
}

void LightSpectrogram::saveScreenshot()
{
    if (m_path.isEmpty()) {
        return;
    }

    QString name = ".";
    const QFileInfo file(m_path);
    name = file.exists() ? file.baseName() : tr("Untitled");
    name += ".png";

    const QString &path = QFileDialog::getSaveFileName(nullptr, tr("Screenshot"), name, "PNG images (*.png)");
    if (!path.isEmpty()) {
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        grab(rect()).save(path);
#else
        QPixmap::grabWidget(this, rect()).save(path);
#endif
    }
}

static void makeSettingButtons(QButtonGroup *group, QGridLayout *layout, int index, const char *text, const QString &tips)
{
    const QString &title = QString(text);
    const QString &style = "\
        QPushButton{ color:#666666; border:1px solid #666666; border-radius:2px; } \
        QPushButton:hover{ color:#000000; border:1px solid #000000; border-radius:2px; }";

    QPushButton *button1 = new QPushButton(title.toLower(), layout->parentWidget());
    QPushButton *button2 = new QPushButton(title.toUpper(), layout->parentWidget());
#ifdef Q_OS_UNIX
    button1->setFocusPolicy(Qt::NoFocus);
    button2->setFocusPolicy(Qt::NoFocus);
#endif
    button1->setFixedHeight(25);
    button2->setFixedHeight(25);
    button1->setStyleSheet(style);
    button2->setStyleSheet(style);
    button1->setCursor(Qt::PointingHandCursor);
    button2->setCursor(Qt::PointingHandCursor);
    button1->setToolTip(tips);
    button2->setToolTip(tips);

    group->addButton(button1, 2 * index);
    group->addButton(button2, 2 * index + 1);
    layout->addWidget(button1, index, 0);
    layout->addWidget(button2, index, 1);
}

void LightSpectrogram::showSettings()
{
    if (m_widget) {
        m_widget->setHidden(m_widget->isVisible());
    } else {
        QWidget *widget = new QWidget(this);
        widget->setFixedSize(140, 280);
        widget->move(0, (height() - widget->height()) / 2);

        QButtonGroup *group = new QButtonGroup(widget);
        connect(group, SIGNAL(buttonClicked(int)), SLOT(settingButtonClicked(int)));

        QGridLayout *layout = new QGridLayout(widget);
        widget->setLayout(layout);

        makeSettingButtons(group, layout, 0, "c", tr("Change the audio channel"));
        makeSettingButtons(group, layout, 1, "f", tr("Change the DFT window function"));
        makeSettingButtons(group, layout, 2, "l", tr("Change the lower limit of the dynamic range in dBFS"));
        makeSettingButtons(group, layout, 3, "p", tr("Change the palette"));
        makeSettingButtons(group, layout, 4, "s", tr("Change the audio stream"));
        makeSettingButtons(group, layout, 5, "u", tr("Change the upper limit of the dynamic range in dBFS"));
        makeSettingButtons(group, layout, 6, "w", tr("Change the DFT window size"));

        widget->show();
        m_widget.reset(widget);
    }
}

void LightSpectrogram::settingButtonClicked(int index)
{
    if (index == 0) { // 'c'
        if (m_channels) {
            m_channel = (m_channel + 1) % m_channels;
        }
    } else if (index == 1) { // 'C'
        if (m_channels) {
            m_channel = (m_channel - 1 + m_channels) % m_channels;
        }
    } else if (index == 2) { // 'f'
        m_window = (WindowFunction) ((m_window + 1) % WINDOW_COUNT);
    } else if (index == 3) { // 'F'
        m_window = (WindowFunction) ((m_window - 1 + WINDOW_COUNT) % WINDOW_COUNT);
    } else if (index == 4) { // 'l'
        m_lrange = spek_min(m_lrange + 1, m_urange - 1);
    } else if (index == 5) { // 'L'
        m_lrange = spek_max(m_lrange - 1, MIN_RANGE);
    } else if (index == 6) { // 'p'
        m_palette = (VisualPalette::Palette) ((m_palette + 1) % VisualPalette::PALETTE_COUNT);
        create_palette();
    } else if (index == 7) { // 'P'
        m_palette = (VisualPalette::Palette) ((m_palette - 1 + VisualPalette::PALETTE_COUNT) % VisualPalette::PALETTE_COUNT);
        create_palette();
    } else if (index == 8) { // 's'
        if (m_streams) {
            m_stream = (m_stream + 1) % m_streams;
        }
    } else if (index == 9) { // 'S'
        if (m_streams) {
            m_stream = (m_stream - 1 + m_streams) % m_streams;
        }
    } else if (index == 10) { // 'u'
        m_urange = spek_min(m_urange + 1, MAX_RANGE);
    } else if (index == 11) { // 'U'
        m_urange = spek_max(m_urange - 1, m_lrange + 1);
    } else if (index == 12) { // 'w'
        m_bits = spek_min(m_bits + 1, MAX_FFT_BITS);
        create_palette();
    } else if (index == 13) { // 'W'
        m_bits = spek_max(m_bits - 1, MIN_FFT_BITS);
        create_palette();
    }

    start();
}

void LightSpectrogram::create_palette()
{
    m_paletteImage = QImage(RULER, bits_to_bands(m_bits), QImage::Format_RGB32);
    for (int y = 0; y < bits_to_bands(m_bits); ++y) {
        const uint32_t color = VisualPalette::renderPalette(m_palette, y / (double)bits_to_bands(m_bits));
        for (int j = 0; j < RULER; ++j) {
            m_paletteImage.setPixel(
                j,
                bits_to_bands(m_bits) - y - 1,
                color
            );
        }
    }
}

static QString trim(QPainter *dc, const QString &s, int length, bool trim_end)
{
    if (length <= 0) {
        return QString();
    }

    // Check if the entire string fits.
    QFontMetrics ftm(dc->font());
    int w = QtFontWidth(ftm, s);
    if (w <= length) {
        return s;
    }

    // Binary search FTW!
    QString fix("...");
    int i = 0;
    int k = s.length();
    while (k - i > 1) {
        int j = (i + k) / 2;
        w = QtFontWidth(ftm, trim_end ? s.mid(0, j) + fix : fix + s.mid(j));
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
