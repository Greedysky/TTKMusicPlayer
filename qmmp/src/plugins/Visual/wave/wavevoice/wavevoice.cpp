#include "wavevoice.h"
#include "inlines.h"

#include <QMenu>
#include <QSettings>
#include <QPainter>
#include <math.h>
#include <qmmp/qmmp.h>

#define MIN_ROW     270
#define MIN_COLUMN  300

static void actionChecked(QAction *action, int value, int data)
{
    action->setData(value);
    action->setCheckable(true);
    action->setChecked(data == value);
}


WaveVoice::WaveVoice(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Wave Voice Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    m_channelsAction = new QAction(tr("Double Channels"), this);
    m_channelsAction->setCheckable(true);
    connect(m_channelsAction, SIGNAL(triggered(bool)), this, SLOT(writeSettings()));

    createPalette(MIN_ROW);
    readSettings();
}

WaveVoice::~WaveVoice()
{
    if(m_xscale)
    {
        delete[] m_xscale;
    }
}

void WaveVoice::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("WaveVoice");
    m_channelsAction->setChecked(settings.value("show_two_channels", true).toBool());
    settings.endGroup();
}

void WaveVoice::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("WaveVoice");
    settings.setValue("show_two_channels", m_channelsAction->isChecked());
    settings.endGroup();
    initialize();
}

void WaveVoice::typeChanged(QAction *action)
{
    switch(action->data().toInt())
    {
        case 0: m_palette = VisualPalette::PALETTE_SPECTRUM; break;
        case 1: m_palette = VisualPalette::PALETTE_RAINBOW; break;
        case 2: m_palette = VisualPalette::PALETTE_PERCEPTUAL; break;
        case 3: m_palette = VisualPalette::PALETTE_SOX; break;
        case 4: m_palette = VisualPalette::PALETTE_MONO; break;
        default: break;
    }
}

void WaveVoice::rangeChanged(QAction *action)
{
    m_rangeValue = action->data().toInt();
}

void WaveVoice::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if(m_backgroundImage.isNull())
    {
        return;
    }

    const bool showTwoChannels = m_channelsAction->isChecked();
    const int level = 255 - m_rangeValue;

    for(int i = 1; i < m_rows; ++i)
    {
        const int w = m_backgroundImage.width();
        if(m_offset >= w)
        {
            m_offset = w - 1;
            m_backgroundImage = m_backgroundImage.copy(1, 0, w, m_backgroundImage.height());
        }

        if(!showTwoChannels)
        {
            const double left = qBound(0, m_intern_vis_data[i - 1] / 2, level) * 1.0 / level;
            m_backgroundImage.setPixel(m_offset, m_rows - i, VisualPalette::renderPalette(m_palette, left));
        }
        else
        {
            const double left = qBound(0, m_intern_vis_data[i - 1] / 2, level) * 1.0 / level;
            m_backgroundImage.setPixel(m_offset, m_rows - i, VisualPalette::renderPalette(m_palette, left));

            const double right = qBound(0, m_intern_vis_data[m_rows + i - 1] / 2, level) * 1.0 / level;
            m_backgroundImage.setPixel(m_offset, 2 * m_rows - i, VisualPalette::renderPalette(m_palette, right));
        }
    }

    ++m_offset;
    painter.drawImage(0, (height() - (showTwoChannels ? 2 : 1) * m_rows) / 2, m_backgroundImage);
}

void WaveVoice::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_channelsAction);

    QMenu typeMenu(tr("Type"), &menu);
    actionChecked(typeMenu.addAction(tr("Spectrum")), 0, m_palette);
    actionChecked(typeMenu.addAction(tr("Rainbow")), 1, m_palette);
    actionChecked(typeMenu.addAction(tr("Perceptual")), 2, m_palette);
    actionChecked(typeMenu.addAction(tr("Sox")), 3, m_palette);
    actionChecked(typeMenu.addAction(tr("Mono")), 4, m_palette);
    connect(&typeMenu, SIGNAL(triggered(QAction*)), this, SLOT(typeChanged(QAction*)));
    menu.addMenu(&typeMenu);

    QMenu rangeMenu(tr("Range"), &menu);
    actionChecked(rangeMenu.addAction(tr("0 DB")), 0, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("10 DB")), 10, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("20 DB")), 20, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("30 DB")), 30, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("40 DB")), 40, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("50 DB")), 50, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("60 DB")), 60, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("70 DB")), 70, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("80 DB")), 80, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("90 DB")), 90, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("100 DB")), 100, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("110 DB")), 110, m_rangeValue);
    actionChecked(rangeMenu.addAction(tr("120 DB")), 120, m_rangeValue);
    connect(&rangeMenu, SIGNAL(triggered(QAction*)), this, SLOT(rangeChanged(QAction*)));
    menu.addMenu(&rangeMenu);
    menu.exec(QCursor::pos());
}

void WaveVoice::process(float *left, float *right)
{
    const int rows = height();
    const int cols = width();

    if(rows < 2 * MIN_ROW && m_rows != rows / 2)
    {
        createPalette(rows / 2);
        initialize();
    }
    else if(rows >= 2 * MIN_ROW && m_rows != MIN_ROW)
    {
        createPalette(MIN_ROW);
        initialize();
    }
    else if(m_backgroundImage.width() != cols)
    {
        initialize();
    }

    short dest_l[256];
    short dest_r[256];

    calc_freq(dest_l, left);
    calc_freq(dest_r, right);

    double y_scale = (double) 1.25 * m_cols / log(256);

    for(int i = 0; i < m_rows; ++i)
    {
        short yl = 0;
        short yr = 0;
        int magnitude_l = 0;
        int magnitude_r = 0;

        if(m_xscale[i] == m_xscale[i + 1])
        {
            yl = dest_l[i];
            yr = dest_r[i];
        }

        for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            yl = qMax(dest_l[k], yl);
            yr = qMax(dest_r[k], yr);
        }

        yl >>= 7; //256
        yr >>= 7;

        if(yl)
        {
            magnitude_l = int(log(yl) * y_scale);
            magnitude_l = qBound(0, magnitude_l, m_cols);
        }

        if(yr)
        {
            magnitude_r = int(log(yr) * y_scale);
            magnitude_r = qBound(0, magnitude_r, m_cols);
        }

        m_intern_vis_data[i] -= m_analyzerSize * m_cols / 15;
        m_intern_vis_data[i] = magnitude_l > m_intern_vis_data[i] ? magnitude_l : m_intern_vis_data[i];

        const int j = m_rows + i;
        m_intern_vis_data[j] -= m_analyzerSize * m_cols / 15;
        m_intern_vis_data[j] = magnitude_r > m_intern_vis_data[j] ? magnitude_r : m_intern_vis_data[j];
    }
}

void WaveVoice::createPalette(int row)
{
    m_rows = row;
    m_cols = MIN_COLUMN;

    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }

    if(m_xscale)
    {
        delete[] m_xscale;
    }

    m_intern_vis_data = new int[m_rows * 2]{0};
    m_xscale = new int[m_rows + 1]{0};

    for(int i = 0; i < m_rows + 1; ++i)
    {
        m_xscale[i] = pow(pow(255.0, 1.0 / m_rows), i);
    }
}

void WaveVoice::initialize()
{
    m_offset = 0;
    m_backgroundImage = QImage(width(), (m_channelsAction->isChecked() ? 2 : 1) * m_rows, QImage::Format_RGB32);
    m_backgroundImage.fill(Qt::black);
}
