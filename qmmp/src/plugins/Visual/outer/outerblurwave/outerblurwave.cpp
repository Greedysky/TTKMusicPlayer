#include "outerblurwave.h"
#include "colorwidget.h"
#include "inlines.h"

#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <qmmp/qmmp.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPolygonItem>

#define HEIGHT_OFFSET  2
#define HEIGHT_LIMIT   0.7

OuterBlurWave::OuterBlurWave(QWidget *parent)
    : Visual(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setWindowTitle(tr("Outer BlurWave Widget"));
    setMinimumWidth(2 * 300 - 30);

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

OuterBlurWave::~OuterBlurWave()
{
    if(m_x_scale)
    {
        delete[] m_x_scale;
    }
}

void OuterBlurWave::start()
{
    Visual::start();
    //load last color settings
    readSettings();
}

void OuterBlurWave::stop()
{
    Visual::stop();
}

void OuterBlurWave::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OuterBlurWave");
    m_color = ColorWidget::readSingleColorConfig(settings.value("colors").toString());
    m_opacity = settings.value("opacity").toDouble();
    settings.endGroup();
}

void OuterBlurWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    draw(&painter);
}

void OuterBlurWave::resizeEvent(QResizeEvent *e)
{
    double offset = 6 * e->size().width() * 0.6 / minimumWidth();
    m_cell_size.setWidth(offset < 6 ? 6 : offset);
}

void OuterBlurWave::process(float *left, float *right)
{
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

        m_intern_vis_data = new int[m_cols * 2]{0};
        m_x_scale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest_l[256];
    short dest_r[256];

    calc_freq(dest_l, left);
    calc_freq(dest_r, right);

    const double y_scale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        int j = m_cols + i; //mirror index
        short yl = 0;
        short yr = 0;
        int magnitude_l = 0;
        int magnitude_r = 0;

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            yl = dest_l[i];
            yr = dest_r[i];
        }

        for(int k = m_x_scale[i]; k < m_x_scale[i + 1]; ++k)
        {
            yl = qMax(dest_l[k], yl);
            yr = qMax(dest_r[k], yr);
        }

        yl >>= 7; //256
        yr >>= 7;

        if(yl)
        {
            magnitude_l = int(log(yl) * y_scale);
            magnitude_l = qBound(0, magnitude_l, m_rows);
        }

        if(yr)
        {
            magnitude_r = int(log(yr) * y_scale);
            magnitude_r = qBound(0, magnitude_r, m_rows);
        }

        const int mirror_index = m_cols - i - 1;
        m_intern_vis_data[mirror_index] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[mirror_index] = magnitude_l > m_intern_vis_data[mirror_index] ? magnitude_l : m_intern_vis_data[mirror_index];

        m_intern_vis_data[j] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[j] = magnitude_r > m_intern_vis_data[j] ? magnitude_r : m_intern_vis_data[j];
    }
}

void OuterBlurWave::draw(QPainter *p)
{
    Q_UNUSED(p);
    if(m_rows == 0)
    {
        return;
    }

    const int rdx = qMax(0, width() - 2 * m_cell_size.width() * m_cols);

    QPolygonF points;
    points << viewToItemPoint(QPoint(0, height() + HEIGHT_OFFSET));
    for(int i = 0; i < m_cols * 2; ++i)
    {
        int x = i * m_cell_size.width() + 1;
        if(i == m_cols)
        {
            continue;
        }

        if(i > m_cols)
        {
            x += rdx; //correct right part position
        }

        int offset = height() - m_intern_vis_data[i] * m_cell_size.height() * HEIGHT_LIMIT;
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

QPointF OuterBlurWave::viewToItemPoint(const QPoint &pt)
{
    return QPointF(m_graphics_item->mapFromScene(m_graphics_view->mapToScene(pt)));
}
