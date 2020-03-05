#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <stdlib.h>

#include <qmmp/qmmp.h>
#include "fft.h"
#include "inlines.h"
#include "outerewave.h"
#include "colorwidget.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPolygonItem>

#define HEIGHT_OFFSET  2
#define HEIGHT_LIMIT   0.7

OuterEWave::OuterEWave (QWidget *parent)
    : Visual(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    m_color = QColor(0x0, 0xff, 0xff);
    m_opacity = 1.0;
    m_intern_vis_data = nullptr;
    m_x_scale = nullptr;
    m_running = false;
    m_rows = 0;
    m_cols = 0;

    setWindowTitle(tr("Outer EWave Widget"));
    setMinimumWidth(2*300-30);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    m_analyzer_falloff = 1.2;
    m_timer->setInterval(QMMP_VISUAL_INTERVAL);
    m_cell_size = QSize(6, 2);

    clear();

    m_graphics_view = new QGraphicsView(this);
    m_graphics_view->setStyleSheet("background: transparent; border:0px");
    m_graphics_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_graphics_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphics_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGraphicsScene *scene = new QGraphicsScene(m_graphics_view);
    m_graphics_view->setScene(scene);

    m_graphics_item = new QGraphicsPolygonItem;
    m_graphics_item->setFlag(QGraphicsPolygonItem::ItemIsMovable, false);
    m_graphics_item->setFlag(QGraphicsPolygonItem::ItemIsSelectable, false);
    m_graphics_item->setFlag(QGraphicsPolygonItem::ItemIsFocusable, false);
    m_graphics_item->setFlag(QGraphicsPolygonItem::ItemIgnoresTransformations, true);

    scene->addItem(m_graphics_item);

    QGraphicsBlurEffect *blur_effect = new QGraphicsBlurEffect(this);
    blur_effect->setBlurRadius(20);
    m_graphics_item->setGraphicsEffect(blur_effect);
}

OuterEWave::~OuterEWave()
{
    if(m_intern_vis_data)
    {
        delete[] m_intern_vis_data;
    }
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void OuterEWave::start()
{
    m_running = true;
    if(isVisible())
    {
        m_timer->start();
    }
    //load last color settings
    readSettings();
}

void OuterEWave::stop()
{
    m_running = false;
    m_timer->stop();
    clear();
}

void OuterEWave::clear()
{
    m_rows = 0;
    m_cols = 0;
    update();
}

void OuterEWave::timeout()
{
    if(takeData(m_left_buffer, m_right_buffer))
    {
        process();
        update();
    }
}

void OuterEWave::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OuterEWave");
    m_color = ColorWidget::readSingleColorConfig(settings.value("colors").toString());
    m_opacity = settings.value("opacity").toDouble();
    settings.endGroup();
}

void OuterEWave::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void OuterEWave::showEvent(QShowEvent *)
{
    if(m_running)
    {
        m_timer->start();
    }
}

void OuterEWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    draw(&painter);
}

void OuterEWave::resizeEvent(QResizeEvent *e)
{
    double offset = 6 * e->size().width() * 0.6 / minimumWidth();
    m_cell_size.setWidth(offset < 6 ? 6 : offset);
}

void OuterEWave::process()
{
    static fft_state *state = nullptr;
    if(!state)
    {
        state = fft_init();
    }

    const int rows = (height() - 2) / m_cell_size.height();
    const int cols = (width() - 2) / m_cell_size.width() / 2;

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;
        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }
        if(m_x_scale)
        {
            delete[] m_x_scale;
        }
        m_intern_vis_data = new double[m_cols * 2];
        m_x_scale = new int[m_cols + 1];

        for(int i = 0; i < m_cols * 2; ++i)
        {
            m_intern_vis_data[i] = 0;
        }
        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest_l[256];
    short dest_r[256];
    short yl, yr;
    int j, k, magnitude_l, magnitude_r;

    calc_freq (dest_l, m_left_buffer);
    calc_freq (dest_r, m_right_buffer);

    const double y_scale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; i++)
    {
        j = m_cols + i; //mirror index
        yl = yr = 0;
        magnitude_l = magnitude_r = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            yl = dest_l[i];
            yr = dest_r[i];
        }
        for(k = m_x_scale[i]; k < m_x_scale[i + 1]; k++)
        {
            yl = qMax(dest_l[k], yl);
            yr = qMax(dest_r[k], yr);
        }

        yl >>= 7; //256
        yr >>= 7;

        if(yl)
        {
            magnitude_l = int(log (yl) * y_scale);
            magnitude_l = qBound(0, magnitude_l, m_rows);
        }
        if(yr)
        {
            magnitude_r = int(log (yr) * y_scale);
            magnitude_r = qBound(0, magnitude_r, m_rows);
        }

        const int mirror_index = m_cols - i - 1;
        m_intern_vis_data[mirror_index] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[mirror_index] = magnitude_l > m_intern_vis_data[mirror_index] ? magnitude_l : m_intern_vis_data[mirror_index];

        m_intern_vis_data[j] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[j] = magnitude_r > m_intern_vis_data[j] ? magnitude_r : m_intern_vis_data[j];
    }
}

void OuterEWave::draw(QPainter *p)
{
    Q_UNUSED(p);
    if(m_cols == 0)
    {
        return;
    }

    int x = 0;
    const int rdx = qMax(0, width() - 2 * m_cell_size.width() * m_cols);
    const float maxed = takeMaxRange();

    QPolygonF points;
    points << viewToItemPoint(QPoint(0, height() + HEIGHT_OFFSET));
    for(int i = 0; i < m_cols * 2; ++i)
    {
        x = i * m_cell_size.width() + 1;
        if(i == m_cols)
        {
            continue;
        }

        if(i > m_cols)
        {
            x += rdx; //correct right part position
        }

        int offset = height() - m_intern_vis_data[i] * maxed * m_cell_size.height() * HEIGHT_LIMIT;
        if(offset == height())
        {
            offset = height() + HEIGHT_OFFSET;
        }
        points << viewToItemPoint(QPoint(x, offset));
    }
    points << viewToItemPoint(QPoint(width(), height() + HEIGHT_OFFSET));

    m_graphics_view->setGeometry(0, 0, width(), height() + HEIGHT_OFFSET);

    m_graphics_item->setBrush(m_color);
    m_graphics_item->setPen(m_color);
    m_graphics_item->setOpacity(m_opacity);
    m_graphics_item->setPolygon(points);
}

QPointF OuterEWave::viewToItemPoint(const QPoint &pt)
{
    return QPointF(m_graphics_item->mapFromScene(m_graphics_view->mapToScene(pt)));
}
