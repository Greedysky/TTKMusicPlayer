#include "soniquewidget.h"
#include <qmmp/qmmp.h>

#include <QDir>
#include <QDateTime>

#ifdef Q_OS_UNIX
#include <dlfcn.h>
#endif

typedef VisInfo* (*SoniqueModule)();

#define SPECTRUM_SIZE   256
#define FFT_SIZE        (SPECTRUM_SIZE * 2)

void customZoomandBlur(unsigned long *v, unsigned long *vt, int xs, int ys)
{
    const float zoom = 0.8;
    unsigned long *vtp = vt;
    //zoom
    const float centerX = xs / 2;
    const float centerY = ys / 2;
    for(int y=0; y<ys; y++)
    {
        for(int x=0; x<xs; x++)
        {
            const int origX = (int)((x - centerX) * zoom + centerX);
            const int origY = (int)((y - centerY) * zoom + centerY);
            *(vtp++) = v[origX + origY * xs];
        }
    }
    //blur and darkness
    const int s = 3;
    const int darkness = 24;
    for(int y=0; y<ys; y++)
    {
        for(int x=0; x<xs; x++)
        {
            if(y == 0 || x == 0 || x == (xs - 1) || y == (ys - 1))
            {
                v[x + xs * y] = vt[x + xs * y];
                continue;
            }

            int t[4] = {0};
            vtp = vt + xs * (y - s / 2) + (x - s / 2);
            for(int i=0; i<s; i++)
            {
                for(int j=0; j<s; j++)
                {
                    for(int k=0; k<4; k++)
                    {
                        t[k] += ((unsigned char*)vtp)[k];
                    }
                    vtp++;
                }
                vtp += xs - s;
            }

            for(int k=0; k<4; k++)
            {
                ((unsigned char*)(&v[x + xs * y]))[k] = std::max(t[k] / (s * s) - darkness, 0);
            }
        }
    }
}

QFileInfoList getFileListByDir(const QString &dpath, const QStringList &filter, bool recursively)
{
    QDir dir(dpath);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    if(recursively)
    {
        const QFileInfoList& folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach(const QFileInfo &file, folderList)
        {
            fileList.append(getFileListByDir(file.absoluteFilePath(), filter, recursively));
        }
    }

    return fileList;
}


SoniqueWidget::SoniqueWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setMinimumSize(580, 320);
    qsrand(QDateTime::currentMSecsSinceEpoch());

    m_sonique = nullptr;
    m_visData= new VisData;
    m_texture = nullptr;
    m_visproc  = nullptr;
    m_instance = nullptr;
    m_currentIndex = -1;

    m_kiss_cfg = kiss_fft_alloc(FFT_SIZE, 0, nullptr, nullptr);
    m_in_freq_data = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * FFT_SIZE);
    m_out_freq_data = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * FFT_SIZE);
}

SoniqueWidget::~SoniqueWidget()
{
    delete m_visData;
    delete m_visproc;

    closePreset();

    kiss_fft_free(m_kiss_cfg);
    free(m_in_freq_data);
    free(m_out_freq_data);
}

void SoniqueWidget::addBuffer(float *left, float *right)
{
    if(!m_sonique)
    {
       return;
    }

    if(m_sonique->lRequired & VI_WAVEFORM)
    {
        for(int i=0; i<FFT_SIZE; i++)
        {
            m_visData->Waveform[0][i] = left[i] * 64.0;
            m_visData->Waveform[1][i] = right[i] * 64.0;
        }
    }

    if(m_sonique->lRequired & VI_SPECTRUM)
    {
        for(int i=0; i<FFT_SIZE; i++)
        {
            m_in_freq_data[i].r = left[i];
            m_in_freq_data[i].i = 0;
        }
        kiss_fft(m_kiss_cfg, m_in_freq_data, m_out_freq_data);
        m_visData->Spectrum[0][0] = std::min(255, int(m_out_freq_data[0].r * 512));
        for(int j=0; j<SPECTRUM_SIZE; j++)
        {
            const int value = sqrt(pow((((m_out_freq_data[j].r) / 512) * 2), 2) + pow((((m_out_freq_data[j].i) / 512) * 2), 2)) * 512;
            m_visData->Spectrum[0][j] = std::min(255, value);
        }

        for(int i=0; i<FFT_SIZE; i++)
        {
            m_in_freq_data[i].r = right[i];
            m_in_freq_data[i].i = 0;
        }
        kiss_fft(m_kiss_cfg, m_in_freq_data, m_out_freq_data);
        m_visData->Spectrum[1][0] = std::min(255, int(m_out_freq_data[0].r * 512));
        for(int j=0; j<SPECTRUM_SIZE; j++)
        {
            const int value = sqrt(pow((((m_out_freq_data[j].r) / 512) * 2), 2) + pow((((m_out_freq_data[j].i) / 512) * 2), 2)) * 512;
            m_visData->Spectrum[1][j] = std::min(255, value);
        }
    }

    const int w = width();
    const int h = height();

    if(m_sonique->lRequired & SONIQUEVISPROC)
    {
        if(!m_visproc)
        {
            m_visproc = new unsigned long[w * h];
        }
        customZoomandBlur(m_texture, m_visproc, w, h);
    }

    m_visData->MillSec = QDateTime::currentMSecsSinceEpoch();
    m_sonique->Render(m_texture, w, h, w, m_visData);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture);
    update();
}

void SoniqueWidget::initializeGL()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    //visual texture
    glBindTexture(GL_TEXTURE_2D, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,	GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,	GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,		GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,		GL_CLAMP);

    if(m_presetList.isEmpty())
    {
        const QString &dir = Qmmp::pluginPath() + "/../MPlugins/config/sonique";
        const QFileInfoList folderList(getFileListByDir(dir, QStringList() << "*.svp", true));
        foreach(const QFileInfo &info, folderList)
        {
            m_presetList << info.absoluteFilePath();
        }

        randomPreset();
    }
    else
    {
        QImage image(width(), height(), QImage::Format_RGB32);
        image.fill(Qt::black);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }
}

void SoniqueWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(!m_sonique)
    {
        return;
    }

    delete m_visproc;
    m_visproc = nullptr;

    delete m_texture;
    m_texture = new unsigned long[w * h]{0};
}

void SoniqueWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f( 0.0, 0.0 ); glVertex2f( -1.0, 1.0);
        glTexCoord2f( 1.0, 0.0 ); glVertex2f( 1.0, 1.0);
        glTexCoord2f( 1.0, 1.0 ); glVertex2f( 1.0, -1.0);
        glTexCoord2f( 0.0, 1.0 ); glVertex2f( -1.0, -1.0);
    glEnd();
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
        m_currentIndex = m_presetList.count();
    }
    generatePreset();
}

void SoniqueWidget::randomPreset()
{
    if(m_presetList.isEmpty())
    {
        return;
    }

    m_currentIndex = qrand() % m_presetList.count();
    generatePreset();
}

void SoniqueWidget::closePreset()
{
    if(m_instance)
    {
        if(m_sonique && m_sonique->Version > 0)
        {
            m_sonique->Deinit();
        }
#ifdef Q_OS_UNIX
        dlclose(m_instance);
#else
        FreeLibrary(m_instance);
#endif
        m_sonique = nullptr;
    }
}

void SoniqueWidget::generatePreset()
{
    closePreset();

    const char *module_path = m_presetList[m_currentIndex].toLocal8Bit().constData();
#ifdef Q_OS_UNIX
    m_instance = dlopen(module_path, RTLD_LAZY);
#else
    m_instance = LoadLibraryA(module_path);
#endif
    qDebug("[SoniqueWidget] url is %s", module_path);

    if(!m_instance)
    {
        qDebug("Could not load the svp file");
        return;
    }

#ifdef Q_OS_UNIX
    SoniqueModule module = (SoniqueModule)dlsym(m_instance, "QueryModule");
#else
    SoniqueModule module = (SoniqueModule)GetProcAddress(m_instance, "QueryModule");
#endif
    if(!module)
    {
        qDebug("Wrong svp file loaded");
        return;
    }

    m_sonique = module();

    const QString &dir = QFileInfo(m_presetList[m_currentIndex]).absolutePath();
    const QFileInfoList iniList(getFileListByDir(dir, QStringList() << "*.ini", true));
    if(!iniList.isEmpty())
    {
        char *init_path = iniList.first().absoluteFilePath().toLocal8Bit().data();
        m_sonique->OpenSettings(init_path);
        qDebug("[SoniqueWidget] ini url is %s", init_path);
    }
    else
    {
        m_sonique->OpenSettings(nullptr);
    }

    m_sonique->Initialize();
}
