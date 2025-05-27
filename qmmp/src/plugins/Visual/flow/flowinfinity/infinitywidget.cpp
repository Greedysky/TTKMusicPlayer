#include "infinitywidget.h"

#include <QMenu>
#include <QPainter>
#include <QDateTime>
#include <QSettings>
#include <qmath.h>
#include <qmmp/qmmp.h>

#define ASSIGN_MAX(p, a) (*p = ( *p > a ? *p : a ))
#define SWAP(x, y) \
    x = x + y;	   \
    y = x - y;	   \
    x = x - y;

struct sincos
{
    int i;
    float *f;
};

/* Little optimization for cos/sin functions */
static struct sincos cosw = {0, nullptr};
static struct sincos sinw = {0, nullptr};
static VEffect etables[] = {
    {6, 1842944, 0, 59, 14, 255, 4, 82},
    {6, 1824128, 0, 59, 11, 255, 1, 40},
    {6, 1676608, 0, 59, 29, 255, 4, 0},
    {6, 1635200, 0, 59, 4, 255, 3, 73},
    {6, 1598592, 255, 75, 4, 0, 3, 113},
    {5, 1545728, 255, 59, 4, 255, 1, 98},
    {3, 1471744, 255, 82, 41, 0, 4, 397},
    {4, 1466816, 255, 82, 41, 0, 4, 397},
    {3, 1405824, 0, 51, 36, 255, 4, 0},
    {4, 1360256, 0, 51, 15, 255, 4, 82},
    {4, 1345600, 0, 51, 9, 255, 3, 61},
    {3, 1265344, 0, 51, 9, 255, 1, 73},
    {3, 1239040, 0, 51, 9, 255, 4, 76},
    {3, 1108480, 0, 51, 9, 255, 4, 77},
    {3, 1084736, 0, 51, 6, 255, 3, 63},
    {3, 1000000, 0, 51, 6, 255, 2, 56},
    {2, 814848, 0, 51, 6, 255, 4, 82},
    {0, 715328, 255, 51, 6, 255, 4, 102},
    {2, 706112, 255, 51, 6, 255, 4, 102},
    {2, 657408, 255, 58, 29, 0, 4, 397},
    {2, 655808, 255, 58, 29, 0, 4, 397},
    {2, 612416, 0, 84, 29, 255, 4, 0},
    {1, 566272, 0, 84, 23, 255, 4, 0},
    {1, 516736, 255, 84, 10, 255, 4, 0},
    {1, 495296, 255, 84, 10, 255, 4, 0},
    {0, 375616, 255, 84, 4, 0, 3, 45},
    {0, 297280, 0, 0, 11, 255, 3, 62},
    {0, 253888, 0, 0, 11, 255, 4, 76},
    {0, 151488, 0, 0, 43, 255, 0, 0}
};


InfinityWidget::InfinityWidget(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow Infinity Widget"));
    srand(QDateTime::currentMSecsSinceEpoch());

    initialize();

    createMenu();
    readSettings();
}

InfinityWidget::~InfinityWidget()
{
    deinit();
}

void InfinityWidget::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Infinity");
    m_effectTime = settings.value("effect_time", 200).toInt();
    m_colorTime = settings.value("palette_time", 200).toInt();
    settings.endGroup();

    for(QAction *act : m_effectActions->actions())
    {
        if(m_effectTime == act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }

    for(QAction *act : m_colorActions->actions())
    {
        if(m_colorTime == act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }
}

void InfinityWidget::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Infinity");
    QAction *act = m_effectActions->checkedAction();
    settings.setValue("effect_time", m_effectTime = (act ? act->data().toInt() : 200));
    act = m_colorActions->checkedAction();
    settings.setValue("palette_time", m_colorTime = (act ? act->data().toInt() : 200));
    settings.endGroup();
}

void InfinityWidget::resizeEvent(QResizeEvent *)
{
    deinit();
    createModule();
}

void InfinityWidget::paintEvent(QPaintEvent *)
{
    unsigned char *data = m_renders[0];
    if(!data)
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    const int w = width();
    const int h = height();
    QImage image(w, h, QImage::Format_RGB32);

    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            image.setPixel(j, i, m_currentColors[*data++]);
        }
    }

    painter.drawImage(QRect(0, 0, w, h), image);
}

void InfinityWidget::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
}

void InfinityWidget::processData(float *left, float *right)
{
    short buf[2][QMMP_VISUAL_NODE_SIZE];
    for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
    {
        buf[0][i] = left[i] * 32767.0;
        buf[1][i] = right[i] * 32767.0;
    }

    displayBlur();
    displaySpectral(buf[0], buf[1]);
    displayCurve();

    if(m_colorCounter <= 32)
    {
        const int w = m_colorCounter * 8;
        for(int i = 0; i < 255; ++i)
        {
            const int r = (m_colorTable[m_newColor][i].r * w + m_colorTable[m_oldColor][i].r * (256 - w)) >> 11;
            const int g = (m_colorTable[m_newColor][i].g * w + m_colorTable[m_oldColor][i].g * (256 - w)) >> 10;
            const int b = (m_colorTable[m_newColor][i].b * w + m_colorTable[m_oldColor][i].b * (256 - w)) >> 11;
            m_currentColors[i] = (r << 11) + (g << 5) + b;
        }
    }

    ++m_colorCounter;
    ++m_effectCounter;

    if(m_effectCounter % m_effectTime == 0)
    {
        loadRandomEffect();
        m_effectCounter = 0;
    }

    if(m_colorCounter % m_colorTime == 0)
    {
        m_oldColor = m_newColor;
        m_newColor = rand() % NB_PALETTES;
        m_colorCounter = 0;
    }

    update();
}

void InfinityWidget::deinit()
{
    delete[] m_renders[0];
    delete[] m_renders[1];
    delete[] m_field;
}

void InfinityWidget::initialize()
{
    const float colors[NB_PALETTES][2][3] = {{{1, 1, 1}, {1, 1, 1}},
                                             {{2, 1.5, 0}, {0, 0.5, 2}},
                                             {{0, 1, 2}, {0, 1, 0}},
                                             {{0, 2, 1}, {0, 0, 1}},
                                             {{2, 0, 0}, {0, 1, 1}}};
    for(int k = 0; k < NB_PALETTES; ++k)
    {
        for(int i = 0; i < 128; ++i)
        {
            m_colorTable[k][i].r = colors[k][0][0] * i;
            m_colorTable[k][i].g = colors[k][0][1] * i;
            m_colorTable[k][i].b = colors[k][0][2] * i;
        }

        for(int i = 0; i < 128; ++i)
        {
            m_colorTable[k][i + 128].r = colors[k][0][0] * 127 + colors[k][1][0] * i;
            m_colorTable[k][i + 128].g = colors[k][0][1] * 127 + colors[k][1][1] * i;
            m_colorTable[k][i + 128].b = colors[k][0][2] * 127 + colors[k][1][2] * i;
        }
    }

    createModule();
    loadRandomEffect();
}

void InfinityWidget::createMenu()
{
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    m_menu->addAction(m_screenAction);
    m_menu->addSeparator();

    m_effectActions = new QActionGroup(this);
    m_effectActions->setExclusive(true);
    m_effectActions->addAction(tr("Slowest"))->setData(500);
    m_effectActions->addAction(tr("Slow"))->setData(400);
    m_effectActions->addAction(tr("Medium"))->setData(300);
    m_effectActions->addAction(tr("Fast"))->setData(200);
    m_effectActions->addAction(tr("Fastest"))->setData(100);

    QMenu *effectMenu = m_menu->addMenu(tr("Effect Time"));
    for(QAction *act : m_effectActions->actions())
    {
        act->setCheckable(true);
        effectMenu->addAction(act);
    }

    m_colorActions = new QActionGroup(this);
    m_colorActions->setExclusive(true);
    m_colorActions->addAction(tr("Slowest"))->setData(500);
    m_colorActions->addAction(tr("Slow"))->setData(400);
    m_colorActions->addAction(tr("Medium"))->setData(300);
    m_colorActions->addAction(tr("Fast"))->setData(200);
    m_colorActions->addAction(tr("Fastest"))->setData(100);

    QMenu *colorMenu = m_menu->addMenu(tr("Palette Time"));
    for(QAction *act : m_colorActions->actions())
    {
        act->setCheckable(true);
        colorMenu->addAction(act);
    }
}

void InfinityWidget::createModule()
{
    const int w = width();
    const int h = height();
    m_renders[0] = new unsigned char[w * h]{0};
    m_renders[1] = new unsigned char[w * h]{0};
    m_field = new VInterpol[w * h * NB_FCT];

    for(int i = 0; i < NB_FCT; ++i)
    {
        for(int j = 0; j < h; j += 10)
        {
            int fin = j + 10;
            if(fin > h)
            {
                fin = h;
            }

            constexpr int prop = 249;
            const int add1 = i * w * h;
            VCoord c;

            for(c.y = j; c.y < fin; ++c.y)
            {
                for(c.x = 0; c.x < w; ++c.x)
                {
                    VComplex a;
                    a.x = (float)c.x;
                    a.y = (float)c.y;
                    a = fct(a, i, 2, 2);
                    const int add2 = c.x + c.y * w;
                    m_field[add1 + add2].coord = (int(a.x) << 16) | int(a.y);

                    const float fpy = a.y - std::floor(a.y);
                    const int rw = (int)((a.x - std::floor(a.x)) * prop);
                    const int lw = prop - rw;
                    unsigned int w4 = (int)(fpy * rw);
                    unsigned int w2 = rw - w4;
                    unsigned int w3 = (int)(fpy * lw);
                    unsigned int w1 = lw - w3;
                    m_field[add1 + add2].weight = (w1 << 24) | ( w2 << 16) | (w3 << 8) | w4;
                }
            }
        }
    }
}

void InfinityWidget::loadRandomEffect()
{
    m_currentEffect = etables[rand() % NB_EFFECT_COUNT];
}

void InfinityWidget::displayBlur()
{
    const int w = width();
    const int h = height();
    VInterpol *field = &m_field[w * h * m_currentEffect.num_effect];

    int index = 0;
    register unsigned char *pix = nullptr;

    for(int i = 0; i < h; ++i)
    {
        for(int j = 0; j < w; ++j)
        {
            VInterpol *interpol = &field[index];
            pix = &(m_renders[0])[(interpol->coord & 0xFFFF) * w + (interpol->coord >> 16)];
            const int color = (*(pix) * (interpol->weight >> 24) +
                               *(pix + 1) * ((interpol->weight & 0xFFFFFF) >> 16) +
                               *(pix + w) * ((interpol->weight & 0xFFFF) >> 8) +
                               *(pix + w + 1) * (interpol->weight & 0xFF)) >> 8;
            m_renders[1][index] = color > 255 ? 255 : color;
            ++index;
        }
    }

    unsigned char *v = m_renders[0];
    m_renders[0] = m_renders[1];
    m_renders[1] = v;
}

void InfinityWidget::displaySpectral(short *left, short *right)
{
    const int w = width();
    const int h = height();
    VEffect *effect = &m_currentEffect;

    float y1 = (((left[0] + right[0]) >> 9) * effect->spectral_amplitude * h) >> 12;
    float y2 = (((left[0] + right[0]) >> 9) * effect->spectral_amplitude * h) >> 12;
    constexpr int step = 4;

    if(cosw.i != w || sinw.i != w)
    {
        free(cosw.f);
        free(sinw.f);
        sinw.f = cosw.f = nullptr;
        sinw.i = cosw.i = 0;
    }

    if(cosw.i == 0 || cosw.f == nullptr)
    {
        cosw.i = w;
        cosw.f = (float*)malloc(sizeof(float) * w);

        for(int i = 0; i < w; i += step)
        {
            cosw.f[i] = std::cos((float)i / w * M_PI + M_PI / 2);
        }
    }

    if(sinw.i == 0 || sinw.f == nullptr)
    {
        sinw.i = w;
        sinw.f = (float*)malloc(sizeof(float) * w);

        for(int i = 0; i < w; i += step)
        {
            sinw.f[i] = std::sin((float)i / w * M_PI + M_PI / 2);
        }
    }

    if(effect->mode_spectre == 3)
    {
        if(y1 < 0)
        {
            y1 = 0;
        }

        if(y2 < 0)
        {
            y2 = 0;
        }
    }

    const int h2 = h >> 1;
    const int w2 = w >> 1;
    const int density = 5;
    const int shift = (effect->spectral_shift * h) >> 8;

    for(int i = step; i <  w; i += step)
    {
        const float y11 = y1;
        const float y22 = y2;
        y1 = ((right[(i << 9) / w / density] >> 8) * effect->spectral_amplitude * h) >> 12;
        y2 = ((left[(i << 9) / w / density] >> 8) * effect->spectral_amplitude * h) >> 12;

        switch(effect->mode_spectre)
        {
        case 0:
            displayLine(i - step, h2 + shift + y22, i, h2 + shift + y2, effect->spectral_color);
            break;
        case 1:
            displayLine(i - step, h2 + shift + y11, i, h2 + shift + y1, effect->spectral_color);
            displayLine(i - step, h2 - shift + y22, i, h2 - shift + y2, effect->spectral_color);
            break;
        case 2:
            displayLine(i - step, h2 + shift + y11, i, h2 + shift + y1, effect->spectral_color);
            displayLine(i - step, h2 - shift + y11, i, h2 - shift + y1, effect->spectral_color);
            displayLine(w2 + shift + y22, i - step, w2 + shift + y2, i, effect->spectral_color);
            displayLine(w2 - shift + y22, i - step, w2 - shift + y2, i, effect->spectral_color);
            break;
        case 3:
            if(y1 < 0)
            {
                y1 = 0;
            }

            if(y2 < 0)
            {
                y2 = 0;
            }

            displayLine(w2 + cosw.f[i - step] * (shift + y11), h2 + sinw.f[i - step] * (shift + y11),
                        w2 + cosw.f[i] * (shift + y1), h2 + sinw.f[i] * (shift + y1),
                        effect->spectral_color);
            displayLine(w2 - cosw.f[i - step] * (shift + y22), h2 + sinw.f[i - step] * (shift + y22),
                        w2 - cosw.f[i] * (shift + y2), h2 + sinw.f[i] * (shift + y2),
                        effect->spectral_color);
            break;
        case 4:
            displayLine(w2 + cosw.f[i - step] * (shift + y11), h2 + sinw.f[i - step] * (shift + y11),
                        w2 + cosw.f[i] * (shift + y1), h2 + sinw.f[i] * (shift + y1), effect->spectral_color);
            displayLine(w2 - cosw.f[i - step] * (shift + y22), h2 + sinw.f[i - step] * (shift + y22),
                        w2 - cosw.f[i] * (shift + y2), h2 + sinw.f[i] * (shift + y2),
                        effect->spectral_color);
            break;
        }
    }

    if(effect->mode_spectre == 3 || effect->mode_spectre == 4)
    {
        displayLine(w2 + cosw.f[w - step] * (shift + y1), h2 + sinw.f[w - step] * (shift + y1),
                    w2 - cosw.f[w - step] * (shift + y2), h2 + sinw.f[w - step] * (shift + y2),
                    effect->spectral_color);
    }
}

void InfinityWidget::displayCurve()
{
    const int w = width();
    const int h = height();
    VEffect *effect = &m_currentEffect;

    int k = 0;
    constexpr float v = 80;
    constexpr float vr = 0.001;
    const float amplitude = (float)effect->curve_amplitude / 256;

    for(int i = 0; i < 2; ++i)
    {
        k = effect->curve;
        for(int j = 0; j < 64; ++j)
        {
            const float x = std::cos((float)(k) / (v + v * i * 1.34)) * h * amplitude;
            const float y = std::sin((float)(k) / (1.756 * (v + v * i * 0.93))) * h * amplitude;

            const int x1 = x * std::cos((float)k * vr) + y * std::sin((float)k * vr) + w / 2;
            const int y1 = x * std::sin((float)k * vr) - y * std::cos((float)k * vr) + h / 2;
            const int c = effect->curve_color;

            if(x1 > 0 && x1 < w - 3 && y1 > 0 && y1 < h - 3)
            {
                const int ty = y1 * w;
                ASSIGN_MAX(&(m_renders[0])[x1 + ty], c);
                ASSIGN_MAX(&(m_renders[0])[x1 + 1 + ty], c);
                ASSIGN_MAX(&(m_renders[0])[x1 + ty + w], c);
                ASSIGN_MAX(&(m_renders[0])[x1 + 1 + ty + w], c);
            }
            ++k;
        }
    }
    effect->curve = k;
}

void InfinityWidget::displayLine(int x1, int y1, int x2, int y2, int c)
{
    /* calculate the distances */
    const int dx = std::abs(x1 - x2);
    const int dy = std::abs(y1 - y2);
    const int w = width();
    const int h = height();

    int cxy = 0, dxy;
    if(dy > dx)
    {
        /* Follow Y axis */
        if(y1 > y2)
        {
            SWAP(y1, y2);
            SWAP(x1, x2);
        }

        dxy = x1 > x2 ? -1 : 1;
        for(; y1 < y2; ++y1)
        {
            cxy += dx;
            if(cxy >= dy)
            {
                x1 += dxy;
                cxy -= dy;
            }

            if(x1 > 0 && x1 < w - 3 && y1 > 0 && y1 < h - 3)
            {
                ASSIGN_MAX(&(m_renders[0])[x1 + y1 * w], c);
            }
        }
    }
    else
    {
        /* Follow X axis */
        if(x1 > x2)
        {
            SWAP(x1, x2);
            SWAP(y1, y2);
        }

        dxy = y1 > y2 ? -1 : 1;
        for(; x1 < x2; ++x1)
        {
            cxy += dy;
            if(cxy >= dx)
            {
                y1 += dxy;
                cxy -= dx;
            }

            if(x1 > 0 && x1 < w - 3 && y1 > 0 && y1 < h - 3)
            {
                ASSIGN_MAX(&(m_renders[0])[x1 + y1 * w], c);
            }
        }
    }
}

VComplex InfinityWidget::fct(VComplex c, int n, int p1, int p2)   /*p1 et p2:0-4 */
{
    const int w = width();
    const int h = height();

    VComplex b;
    float an, co, si, size, speed, fact;

    c.x -= w / 2;
    c.y -= h / 2;

    switch(n)
    {
    case 0:
        an = 0.025 * (p1 - 2) + 0.002;
        co = std::cos(an);
        si = std::sin(an);
        size = h * 0.25;
        speed = 2000 + p2 * 500;
        b.x = co * c.x - si * c.y;
        b.y = si * c.x + co * c.y;
        fact = -(std::sqrt(b.x * b.x + b.y * b.y) - size) / speed + 1;
        b.x = b.x * fact;
        b.y = b.y * fact;
        break;
    case 1:
        an = 0.015 * (p1 - 2) + 0.002;
        co = std::cos(an);
        si = std::sin(an);
        size = h * 0.45;
        speed = 4000 + p2 * 1000;
        b.x = co * c.x - si * c.y;
        b.y = si * c.x + co * c.y;
        fact = (std::sqrt(b.x * b.x + b.y * b.y) - size) / speed + 1;
        b.x = b.x * fact;
        b.y = b.y * fact;
        break;
    case 2:
        an = 0.002;
        co = std::cos(an);
        si = std::sin(an);
        size = h * 0.25;
        speed = 400 + p2 * 100;
        b.x = co * c.x - si * c.y;
        b.y = si * c.x + co * c.y;
        fact = -(std::sqrt(b.x * b.x + b.y * b.y) - size) / speed + 1;
        b.x = b.x * fact;
        b.y = b.y * fact;
        break;
    case 3:
        an = std::sin(std::sqrt(c.x * c.x + c.y * c.y) / 20) / 20 + 0.002;
        co = std::cos(an);
        si = std::sin(an);
        size = h * 0.25;
        speed = 4000;
        b.x = co * c.x - si * c.y;
        b.y = si * c.x + co * c.y;
        fact = -(std::sqrt(b.x * b.x + b.y * b.y) - size) / speed + 1;
        b.x = b.x * fact;
        b.y = b.y * fact;
        break;
    case 4:
        an = 0.002;
        co = std::cos(an);
        si = std::sin(an);
        size = h * 0.25;
        speed = std::sin(std::sqrt(c.x * c.x + c.y * c.y) / 5) * 3000 + 4000;
        b.x = co * c.x - si * c.y;
        b.y = si * c.x + co * c.y;
        fact = -(std::sqrt(b.x * b.x + b.y * b.y) - size) / speed + 1;
        b.x = b.x * fact;
        b.y = b.y * fact;
        break;
    case 5:
        b.x = c.x * 1.02;
        b.y = c.y * 1.02;
        break;
    case 6:
        an = 0.002;
        co = std::cos(an);
        si = std::sin(an);
        fact = std::cos(std::atan(c.x / (c.y + 0.00001)) * 6) * 0.02 + 1;
        b.x = co * c.x - si * c.y;
        b.y = si * c.x + co * c.y;
        b.x = b.x * fact;
        b.y = b.y * fact;
        break;
    }

    b.x += w / 2;
    b.y += h / 2;

    if(b.x < 0)
    {
        b.x = 0;
    }

    if(b.y < 0)
    {
        b.y = 0;
    }

    if(b.x > w - 1)
    {
        b.x = w - 1;
    }

    if(b.y > h - 1)
    {
        b.y = h - 1;
    }
    return b;
}
