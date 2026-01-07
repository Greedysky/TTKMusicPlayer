#include "jakdawwidget.h"
#include "inlines.h"

#include <QMenu>
#include <QPainter>
#include <QDateTime>
#include <QSettings>
#include <QActionGroup>
#include <cmath>
#include <qmmp/qmmp.h>

static constexpr int DECAY_RATE = 1;
static constexpr double ZOOM_RIPPLESIZE= 32;
static constexpr double	ZOOM_RIPPLEFACT= 0.1;
static constexpr double	ZOOM_ZOOMFACT= 0.9;
static constexpr float PLOTTER_AMPLITUDE= 0.5f;
static constexpr int PLOTTER_SCOPECOLOR= 0xff00ff;

typedef uint32_t (*transform_function) (int w, int h, int x, int y);

static void blurThen(uint32_t *table, int &index, int w, int h, int x, int y, transform_function func)
{
    uint32_t a = 0;
    a = x + 1 < w ? x + 1 : x;
    table[index++] = func(w, h, a, y);

    a = x - 1 < 0 ? 0 : x - 1;
    table[index++] = func(w, h, a, y);

    a = y + 1 < h ? y + 1 : y;
    table[index++] = func(w, h, x, a);

    a = y - 1 < 0 ? 0 : y - 1;
    table[index++] = func(w, h, x, a);
}

// Transform functions ---------------------------------------------------------
static uint32_t zoomNothing(int w, int h, int x, int y)
{
    Q_UNUSED(h);
    return (y * w) + x;
}

static uint32_t zoomScroll(int w, int h, int x, int y)
{
    const int retval = y + 6 <= h ? (((y + 6) * w) + x) : ((h >> 1) * w) + (w >> 1);
    return qBound(0, retval, w * h);
}

static uint32_t zoomRotate(int w, int h, int x, int y)
{
    double ang = 5.0 * (3.14 / 180.0);

    x -= (w >> 1);
    y -= (h >> 1);

    int nx = x * std::cos(ang) + y * std::sin(ang);
    int ny = y * std::cos(ang) - x * std::sin(ang);

    nx /= 1.2;
    ny /= 1.2;
    nx += (w >> 1);
    ny += (h >> 1);

    if(nx >= w || nx < 0 || ny >= h || ny < 0)
    {
        nx = 0;
        ny = 0;
    }

    if(nx < 0 || nx >= w || ny < 0 || ny >= h)
    {
        nx = w >> 1;
        ny = h >> 1;
    }
    return ny * w + nx;
}

static uint32_t zoomRipple(int w, int h, int x, int y)
{
    x -= (w >> 1);
    y -= (h >> 1);

    double dist = std::sqrt(x * x + y * y);
    dist *= 3.14 * ZOOM_RIPPLESIZE / std::sqrt(w * w + h * h);

    int nx = x * (ZOOM_ZOOMFACT + ZOOM_RIPPLEFACT * std::sin(dist));
    int ny = y * (ZOOM_ZOOMFACT + ZOOM_RIPPLEFACT * std::sin(dist));

    nx += (w >> 1);
    ny += (h >> 1);

    if(nx < 0 || nx >= w || ny < 0 || ny >= h)
    {
        nx = w >> 1;
        ny = h >> 1;
    }
    return ny * w + nx;
}

static uint32_t zoomRippleNew(int w, int h, int x, int y)
{
    x -= (w >> 1);
    y -= (h >> 1);

    // FIXME - implement the rest of this!
    int nx = x;
    int ny = y;

    nx += (w >> 1);
    ny += (h >> 1);

    if(nx < 0 || nx >= w || ny < 0 || ny >= h)
    {
        nx = w >> 1;
        ny = h >> 1;
    }
    return ny * w + nx;
}

static uint32_t zoomIntoScreen(int w, int h, int x, int y)
{
    x -= (w >> 1);

    double xfact = 1.0 + 0.05 * ((h - y) / (double)h);
    double yfact = 1.05 + 0.05 * (y / (double)h);

    int nx = x * xfact;
    int ny = y * yfact;

    nx += (w >> 1);

    if(nx < 0 || nx >= w || ny < 0 || ny >= h)
    {
        nx = w >> 1;
        ny = h >> 1;
    }
    return ny * w + nx;
}


JakdawWidget::JakdawWidget(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Flow Jakdaw Widget"));
    srand(QDateTime::currentMSecsSinceEpoch());

    createMenu();
    readSettings();
}

JakdawWidget::~JakdawWidget()
{
    deinit();
}

void JakdawWidget::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Jakdaw");
    m_zoomMode = static_cast<FeedbackType>(settings.value("zoom_mode", FEEDBACK_ZOOMRIPPLE).toInt());
    m_plotterColor = static_cast<PlotterColor>(settings.value("plotter_color", PLOTTER_COLOUR_SOLID).toInt());
    m_plotterType = static_cast<PlotterOptions>(settings.value("plotter_type", PLOTTER_SCOPE_LINES).toInt());
    settings.endGroup();

    for(QAction *act : m_modeActions->actions())
    {
        if(m_zoomMode == act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }

    for(QAction *act : m_colorActions->actions())
    {
        if(m_plotterColor == act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }

    for(QAction *act : m_typeActions->actions())
    {
        if(m_plotterType == act->data().toInt())
        {
            act->setChecked(true);
            break;
        }
    }
}

void JakdawWidget::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Jakdaw");
    QAction *act = m_modeActions->checkedAction();
    settings.setValue("zoom_mode", m_zoomMode = (act ? static_cast<FeedbackType>(act->data().toInt()) : FEEDBACK_ZOOMRIPPLE));
    act = m_colorActions->checkedAction();
    settings.setValue("plotter_color", m_plotterColor = (act ? static_cast<PlotterColor>(act->data().toInt()) : PLOTTER_COLOUR_SOLID));
    act = m_typeActions->checkedAction();
    settings.setValue("plotter_type", m_plotterType = (act ? static_cast<PlotterOptions>(act->data().toInt()) : PLOTTER_SCOPE_LINES));
    settings.endGroup();
}

void JakdawWidget::resizeEvent(QResizeEvent *)
{
    m_width = width();
    m_height = height();

    feedbackReset();
}

void JakdawWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawImage(rect(), QImage((uchar*)m_texture, m_width, m_height, QImage::Format_RGB32));
}

void JakdawWidget::contextMenuEvent(QContextMenuEvent *)
{
    m_menu->exec(QCursor::pos());
}

void JakdawWidget::processData(float *left, float *)
{
    short dest[256];
    calc_freq(dest, left);

    /* PCM Buffer */
    float pcm[QMMP_VISUAL_NODE_SIZE];
    for(size_t i = 0; i < QMMP_VISUAL_NODE_SIZE; ++i)
    {
        pcm[i] = left[i] * 128.0;
    }

    feedbackRender();
    plotterDraw(pcm, (float*)dest);
    update();
}

void JakdawWidget::deinit()
{
    delete[] m_image;
    delete[] m_table;
    delete[] m_texture;
}

void JakdawWidget::createMenu()
{
    m_menu = new QMenu(this);
    connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(writeSettings()));

    m_menu->addAction(m_screenAction);
    m_menu->addSeparator();

    m_modeActions = new QActionGroup(this);
    m_modeActions->setExclusive(true);
    m_modeActions->addAction(tr("Zoom Ripple"))->setData(FEEDBACK_ZOOMRIPPLE);
    m_modeActions->addAction(tr("Blur Only"))->setData(FEEDBACK_BLURONLY);
    m_modeActions->addAction(tr("Zoom Rotate"))->setData(FEEDBACK_ZOOMROTATE);
    m_modeActions->addAction(tr("Scroll"))->setData(FEEDBACK_SCROLL);
    m_modeActions->addAction(tr("Into Screen"))->setData(FEEDBACK_INTOSCREEN);
    m_modeActions->addAction(tr("Ripple"))->setData(FEEDBACK_NEWRIPPLE);

    QMenu *modeMenu = m_menu->addMenu(tr("Zoom Mode"));
    for(QAction *act : m_modeActions->actions())
    {
        act->setCheckable(true);
        modeMenu->addAction(act);
    }

    m_colorActions = new QActionGroup(this);
    m_colorActions->setExclusive(true);
    m_colorActions->addAction(tr("Solid"))->setData(PLOTTER_COLOUR_SOLID);
    m_colorActions->addAction(tr("Random"))->setData(PLOTTER_COLOUR_RANDOM);
    m_colorActions->addAction(tr("On Music"))->setData(PLOTTER_COLOUR_MUSICTRIG);

    QMenu *colorMenu = m_menu->addMenu(tr("Plotter Color"));
    for(QAction *act : m_colorActions->actions())
    {
        act->setCheckable(true);
        colorMenu->addAction(act);
    }

    m_typeActions = new QActionGroup(this);
    m_typeActions->setExclusive(true);
    m_typeActions->addAction(tr("Lines"))->setData(PLOTTER_SCOPE_LINES);
    m_typeActions->addAction(tr("Dots"))->setData(PLOTTER_SCOPE_DOTS);
    m_typeActions->addAction(tr("Solid"))->setData(PLOTTER_SCOPE_SOLID);
    m_typeActions->addAction(tr("Nothing"))->setData(PLOTTER_SCOPE_NOTHING);

    QMenu *typeMenu = m_menu->addMenu(tr("Plotter Type"));
    for(QAction *act : m_typeActions->actions())
    {
        act->setCheckable(true);
        typeMenu->addAction(act);
    }
}

void JakdawWidget::feedbackReset()
{
    // Check whether res or feedback type have changed here
    deinit();

    m_table= new uint32_t[m_width * m_height * sizeof(uint32_t) * 4]{0}; // 4 points / pixel
    m_image= new uint32_t[m_width * m_height * 4]{0};
    m_texture = new uint32_t[m_width * m_height]{0};
    m_tableIndex=0;

    for(int b = 0; b < m_height; ++b)
    {
        for(int a = 0; a < m_width; ++a)
        {
            switch(m_zoomMode)
            {
                case FEEDBACK_ZOOMRIPPLE:
                    blurThen(m_table, m_tableIndex, m_width, m_height, a, b, zoomRipple);
                    break;
                case FEEDBACK_BLURONLY:
                    blurThen(m_table, m_tableIndex, m_width, m_height, a, b, zoomNothing);
                    break;
                case FEEDBACK_ZOOMROTATE:
                    blurThen(m_table, m_tableIndex, m_width, m_height, a, b, zoomRotate);
                    break;
                case FEEDBACK_SCROLL:
                    blurThen(m_table, m_tableIndex, m_width, m_height, a, b, zoomScroll);
                    break;
                case FEEDBACK_INTOSCREEN:
                    blurThen(m_table, m_tableIndex, m_width, m_height, a, b, zoomIntoScreen);
                    break;
                case FEEDBACK_NEWRIPPLE:
                    blurThen(m_table, m_tableIndex, m_width, m_height, a, b, zoomRippleNew);
                    break;
                default:
                    blurThen(m_table, m_tableIndex, m_width, m_height, a, b, zoomNothing);
                    break;
            }
        }
    }
}

void JakdawWidget::feedbackRender()
{
    /* Most feedback effects don't take well to the middle pixel becoming
     * a bright colour - so we just blank it here. Most effects now rely on
     * this as a black pixel to be used instead of those that fall off the
     * edges of the screen. */
    m_texture[((m_height >> 1) * m_width) + (m_width >> 1)] = 0;

    int a, r, g, b;
    int tptr = 0, niptr = 0;
    const int np = m_width * m_height;
    const int rdr = DECAY_RATE << 2;
    const int gdr = DECAY_RATE << 10;
    const int bdr = DECAY_RATE << 18;

    for(int x = 0; x < np; ++x)
    {
        /* I dunno how good a job the C compiler makes at optimizing this
         * loop. Since this is what the plugin spends pretty much all of
         * it's time doing it might be worth looking into the possiblilty
         * of optimizing it further. */
        a = m_texture[m_table[tptr++]];
        r = a & 0xff;
        g = a & 0xff00;
        b = a & 0xff0000;

        a = m_texture[m_table[tptr++]];
        r += a & 0xff;
        g += a & 0xff00;
        b += a & 0xff0000;

        a = m_texture[m_table[tptr++]];
        r += a & 0xff;
        g += a & 0xff00;
        b += a & 0xff0000;

        a = m_texture[m_table[tptr++]];
        r += a & 0xff;
        g += a & 0xff00;
        b += a & 0xff0000;

        r = r > rdr ? r - rdr : 0;
        g = g > gdr ? g - gdr : 0;
        b = b > bdr ? b - bdr : 0;

        a = (r & 0x3fc) | (g & 0x3fc00) | (b & 0x3fc0000);
        a >>= 2;

        m_image[niptr++]=a;
    }

    memcpy(m_texture, m_image, m_width * m_height * 4);
}

void JakdawWidget::plotterDraw(float *pcm, float *freq)
{
    uint32_t col = 0;
    switch(m_plotterColor)
    {
        case PLOTTER_COLOUR_SOLID:
            col = PLOTTER_SCOPECOLOR;
            break;
        case PLOTTER_COLOUR_RANDOM:
            col = rand();
            break;
        case PLOTTER_COLOUR_MUSICTRIG:
        default:
        {
            float d = 0.0f;
            for(int c = 0; c < 16; ++c)
            {
                d = d + freq[c];
            }

            col = (int)((double)d * (256.0 * 16.0));
            d = 0.0f;

            for(int c = 16; c < 108; ++c)
            {
                d = d + freq[c];
            }

            col |= ((int)((double)d * (256.0 * 64.0))) << 8;
            d = 0.0f;

            for(int c = 108; c < 255; ++c)
            {
                d = d + freq[c];
            }

            col |= ((int)((double)d * (256.0 * 128.0))) << 16;
        }
    }

    int oy = (m_height / 2) + ((pcm[0] * PLOTTER_AMPLITUDE) * (m_height / 2));
        oy = oy < 0 ? 0 : oy >= m_height ? m_height - 1 : oy;

    for(int x = 0; x < m_width; ++x)
    {
        int y = (m_height / 2) + ((pcm[x % 512] * PLOTTER_AMPLITUDE) * (m_height / 2));
        if(y < 0)
        {
            y = 0;
        }

        if(y >= m_height)
        {
            y = m_height - 1;
        }

        if(m_plotterType == PLOTTER_SCOPE_LINES)
        {
            vertLine(x, oy, y, col);
            oy = y;
        }
        else if(m_plotterType==PLOTTER_SCOPE_DOTS)
        {
            if(x > 0 && x < m_width && y > 0 && y < m_height)
            {
                m_texture[y * m_width + x] = col;
            }
        }
        else if(m_plotterType == PLOTTER_SCOPE_SOLID)
        {
            vertLine(x, m_height >> 1, y, col);
        }
    }
}

void JakdawWidget::vertLine(int x, int a, int b, uint32_t col)
{
    int y;
    if(b < a)
    {
        y = a;
        a = b;
        b = y;
    }

    if(a < 0 || a >= m_height || b < 0 || b >= m_height)
    {
        return;
    }

    int ptr = (a * m_width) + x;
    for(y = a; y <= b; ++y)
    {
        m_texture[ptr] = col;
        ptr += m_width;
    }
}
