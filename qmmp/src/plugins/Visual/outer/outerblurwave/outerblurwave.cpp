#include "outerblurwave.h"
#include "colorwidget.h"
#include "inlines.h"

#include <QTimer>
#include <QSettings>
#include <QPainter>
#include <cmath>
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

    m_view = new QGraphicsView(this);
    m_view->setStyleSheet("background: transparent; border:0px");
    m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGraphicsScene *scene = new QGraphicsScene(m_view);
    m_view->setScene(scene);

    m_item = new QGraphicsPolygonItem;
    m_item->setFlag(QGraphicsPolygonItem::ItemIsMovable, false);
    m_item->setFlag(QGraphicsPolygonItem::ItemIsSelectable, false);
    m_item->setFlag(QGraphicsPolygonItem::ItemIsFocusable, false);
    m_item->setFlag(QGraphicsPolygonItem::ItemIgnoresTransformations, true);
    scene->addItem(m_item);

    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
    effect->setBlurRadius(20);
    m_item->setGraphicsEffect(effect);
}

OuterBlurWave::~OuterBlurWave()
{
    delete[] m_xscale;
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
    m_colors = ColorWidget::readColorConfig(settings.value("colors").toString());
    m_opacity = settings.value("opacity").toDouble();
    settings.endGroup();
}

void OuterBlurWave::paintEvent(QPaintEvent *)
{
    if(m_rows == 0)
    {
        return;
    }

    const int rdx = qMax(0, width() - 2 * m_cellSize.width() * m_cols);

    QPolygonF points;
    points << viewToItemPoint(QPoint(0, height() + HEIGHT_OFFSET));
    for(int i = 0; i < m_cols * 2; ++i)
    {
        if(i == m_cols)
        {
            continue;
        }

        int x = i * m_cellSize.width() + 1;
        if(i > m_cols)
        {
            x += rdx; //correct right part position
        }

        int offset = height() - m_visualData[i] * m_cellSize.height() * HEIGHT_LIMIT;
        if(offset == height())
        {
            offset = height() + HEIGHT_OFFSET;
        }

        points << viewToItemPoint(QPoint(x, offset));
    }
    points << viewToItemPoint(QPoint(width(), height() + HEIGHT_OFFSET));

    const QColor &color = m_colors.first();
    m_view->setGeometry(0, 0, width(), height() + HEIGHT_OFFSET);
    m_item->setBrush(color);
    m_item->setPen(color);
    m_item->setOpacity(m_opacity);
    m_item->setPolygon(points);
}

void OuterBlurWave::resizeEvent(QResizeEvent *)
{
    const double offset = 6 * width() * 0.6 / minimumWidth();
    m_cellSize.setWidth(offset < 6 ? 6 : offset);
}

void OuterBlurWave::processData(float *left, float *right)
{
    const int rows = (height() - 2) / m_cellSize.height();
    const int cols = (width() - 2) / m_cellSize.width() / 2;

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_visualData;
        delete[] m_xscale;

        m_visualData = new int[m_cols * 2]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = std::pow(255.0, float(i) / m_cols);
        }
    }

    short destl[256], destr[256];
    calc_freq(destl, left);
    calc_freq(destr, right);

    const double yscale = (double)1.25 * m_rows / std::log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        short yl = 0, yr = 0;
        int magnitudel = 0, magnituder = 0;

        if(m_xscale[i] == m_xscale[i + 1])
        {
            yl = (i >= 256 ? 0 : (destl[i] >> 7)); //128
            yr = (i >= 256 ? 0 : (destr[i] >> 7)); //128
        }

        for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            yl = (k >= 256 ? 0 : qMax(short(destl[k] >> 7), yl));
            yr = (k >= 256 ? 0 : qMax(short(destr[k] >> 7), yr));
        }

        if(yl > 0)
        {
            magnitudel = qBound(0, int(std::log(yl) * yscale), m_rows);
        }

        if(yr > 0)
        {
            magnituder = qBound(0, int(std::log(yr) * yscale), m_rows);
        }

        const int mirror_index = m_cols - i - 1;
        m_visualData[mirror_index] -= m_analyzerSize * m_rows / 15;
        m_visualData[mirror_index] = magnitudel > m_visualData[mirror_index] ? magnitudel : m_visualData[mirror_index];

        const int j = m_cols + i;
        m_visualData[j] -= m_analyzerSize * m_rows / 15;
        m_visualData[j] = magnituder > m_visualData[j] ? magnituder : m_visualData[j];
    }
}

QPointF OuterBlurWave::viewToItemPoint(const QPoint &pt)
{
    return QPointF(m_item->mapFromScene(m_view->mapToScene(pt)));
}
