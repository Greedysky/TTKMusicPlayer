#include "soniquewidget.h"
#include <qmmp/qmmp.h>

#include <QDir>
#include <QMenu>
#include <QPainter>
#include <QDateTime>
#include <QLibrary>

typedef VisInfo* (*SoniqueModule)();

#define SPECTRUM_SIZE   256
#define FFT_SIZE        (SPECTRUM_SIZE * 2)

static void customZoomAndBlur(unsigned int *v, unsigned int *vt, int xs, int ys)
{
    constexpr float zoom = 0.8;
    unsigned int *vtp = vt;
    //zoom
    const float centerX = xs / 2;
    const float centerY = ys / 2;
    for(int y = 0; y < ys; ++y)
    {
        for(int x = 0; x < xs; ++x)
        {
            const int origX = (int)((x - centerX) * zoom + centerX);
            const int origY = (int)((y - centerY) * zoom + centerY);
            *(vtp++) = v[origX + origY * xs];
        }
    }
    //blur and darkness
    constexpr int s = 3;
    constexpr int darkness = 24;
    for(int y = 0; y < ys; ++y)
    {
        for(int x = 0; x < xs; ++x)
        {
            if(y == 0 || x == 0 || x == (xs - 1) || y == (ys - 1))
            {
                v[x + xs * y] = vt[x + xs * y];
                continue;
            }

            int t[4] = {0};
            vtp = vt + xs * (y - s / 2) + (x - s / 2);
            for(int i = 0; i < s; ++i)
            {
                for(int j = 0; j < s; ++j)
                {
                    for(int k = 0; k < 4; ++k)
                    {
                        t[k] += ((unsigned char*)vtp)[k];
                    }
                    vtp++;
                }
                vtp += xs - s;
            }

            for(int k = 0; k < 4; ++k)
            {
                ((unsigned char*)(&v[x + xs * y]))[k] = std::max(t[k] / (s * s) - darkness, 0);
            }
        }
    }
}

static QFileInfoList fileListByPath(const QString &dpath, const QStringList &filter)
{
    QDir dir(dpath);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden);
    const QFileInfoList& folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(const QFileInfo &fin : folderList)
    {
        fileList.append(fileListByPath(fin.absoluteFilePath(), filter));
    }

    return fileList;
}


SoniqueWidget::SoniqueWidget(QWidget *parent)
    : Visual(parent)
{
    setlocale(LC_NUMERIC, "C"); //fixes problem with non-english locales
    setWindowTitle(tr("Flow Sonique Widget"));

    setMinimumSize(580, 320);
    srand(QDateTime::currentMSecsSinceEpoch());

    m_visData = new VisData;
    m_instance = new QLibrary;
    m_kissCfg = kiss_fft_alloc(FFT_SIZE, 0, nullptr, nullptr);
    m_inputFreqData = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * FFT_SIZE);
    m_outFreqData = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * FFT_SIZE);

    initialize();
}

SoniqueWidget::~SoniqueWidget()
{
    closePreset();

    delete m_visData;
    delete[] m_texture;
    delete[] m_visProc;
    delete m_instance;

    kiss_fft_free(m_kissCfg);
    free(m_inputFreqData);
    free(m_outFreqData);
}

void SoniqueWidget::nextPreset()
{
    if(m_presetList.isEmpty())
    {
        return;
    }

    m_currentIndex++;
    if(m_currentIndex >= m_presetList.count())
    {
        m_currentIndex = 0;
    }

    generatePreset();
}

void SoniqueWidget::previousPreset()
{
    if(m_presetList.isEmpty())
    {
        return;
    }

    m_currentIndex--;
    if(m_currentIndex < 0)
    {
        m_currentIndex = m_presetList.count() - 1;
    }

    generatePreset();
}

void SoniqueWidget::resizeEvent(QResizeEvent *)
{
    if(!m_sonique)
    {
        return;
    }

    delete[] m_visProc;
    m_visProc = nullptr;

    delete[] m_texture;
    m_texture = new unsigned int[width() * height()]{0};
}

void SoniqueWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawImage(rect(), QImage((uchar*)m_texture, width(), height(), QImage::Format_RGB32));
}

void SoniqueWidget::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.addSeparator();

    menu.addAction(tr("&Next Preset"), this, SLOT(nextPreset()), tr("N"));
    menu.addAction(tr("&Previous Preset"), this, SLOT(previousPreset()), tr("P"));
    menu.addAction(tr("&Random Preset"), this, SLOT(randomPreset()), tr("R"));
    menu.exec(QCursor::pos());
}

void SoniqueWidget::processData(float *left, float *right)
{
    if(!m_sonique)
    {
       return;
    }

    if(m_sonique->lRequired & VI_WAVEFORM)
    {
        for(int i = 0; i < FFT_SIZE; ++i)
        {
            m_visData->Waveform[0][i] = left[i] * 64.0;
            m_visData->Waveform[1][i] = right[i] * 64.0;
        }
    }

    if(m_sonique->lRequired & VI_SPECTRUM)
    {
        for(int i = 0; i < FFT_SIZE; ++i)
        {
            m_inputFreqData[i].r = left[i];
            m_inputFreqData[i].i = 0;
        }

        kiss_fft(m_kissCfg, m_inputFreqData, m_outFreqData);
        m_visData->Spectrum[0][0] = std::min(255, int(m_outFreqData[0].r * 512));

        for(int i = 0; i < SPECTRUM_SIZE; ++i)
        {
            const int value = sqrt(pow((((m_outFreqData[i].r) / 512) * 2), 2) + pow((((m_outFreqData[i].i) / 512) * 2), 2)) * 512;
            m_visData->Spectrum[0][i] = std::min(255, value);
        }

        for(int i = 0; i < FFT_SIZE; ++i)
        {
            m_inputFreqData[i].r = right[i];
            m_inputFreqData[i].i = 0;
        }

        kiss_fft(m_kissCfg, m_inputFreqData, m_outFreqData);
        m_visData->Spectrum[1][0] = std::min(255, int(m_outFreqData[0].r * 512));

        for(int i = 0; i < SPECTRUM_SIZE; ++i)
        {
            const int value = sqrt(pow((((m_outFreqData[i].r) / 512) * 2), 2) + pow((((m_outFreqData[i].i) / 512) * 2), 2)) * 512;
            m_visData->Spectrum[1][i] = std::min(255, value);
        }
    }

    const int w = width();
    const int h = height();

    if(m_sonique->lRequired & SONIQUEVISPROC)
    {
        if(!m_visProc)
        {
            m_visProc = new unsigned int[w * h];
        }
        customZoomAndBlur(m_texture, m_visProc, w, h);
    }

    m_visData->MillSec = QDateTime::currentMSecsSinceEpoch();
    m_sonique->Render(m_texture, w, h, w, m_visData);

    update();
}

void SoniqueWidget::randomPreset()
{
    if(m_presetList.isEmpty())
    {
        return;
    }

    m_currentIndex = rand() % m_presetList.count();
    generatePreset();
}

void SoniqueWidget::initialize()
{
    const QString &dir = Qmmp::ttkPluginPath() + "/config/sonique";
    const QFileInfoList folderList(fileListByPath(dir, QStringList() << "*.svp"));
    for(const QFileInfo &fin : folderList)
    {
        m_presetList << fin.absoluteFilePath();
    }

    randomPreset();
}

void SoniqueWidget::closePreset()
{
    if(m_instance->isLoaded())
    {
        if(m_sonique && m_sonique->Version > 0)
        {
            m_sonique->Deinit();
        }

        m_instance->unload();
        m_sonique = nullptr;
    }
}

void SoniqueWidget::generatePreset()
{
    closePreset();

    const QString &module_path = m_presetList[m_currentIndex];
    qDebug("[SoniqueWidget] url is %s", qPrintable(module_path));

    m_instance->setFileName(module_path);
    if(!m_instance->load())
    {
        qDebug("Could not load the svp file");
        return;
    }

    SoniqueModule module = (SoniqueModule)m_instance->resolve("QueryModule");
    if(!module)
    {
        qDebug("Wrong svp file loaded");
        return;
    }

    m_sonique = module();

    const QString &dir = QFileInfo(m_presetList[m_currentIndex]).absolutePath();
    const QFileInfoList iniList(fileListByPath(dir, QStringList() << "*.ini"));
    if(!iniList.isEmpty())
    {
        char *init_path = iniList.front().absoluteFilePath().toLocal8Bit().data();
        m_sonique->OpenSettings(init_path);
        qDebug("[SoniqueWidget] ini url is %s", init_path);
    }
    else
    {
        m_sonique->OpenSettings(nullptr);
    }

    m_sonique->Initialize();
}
