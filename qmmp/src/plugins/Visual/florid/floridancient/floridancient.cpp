#include "floridancient.h"
#include "inlines.h"

#include <QPainter>
#include <QPropertyAnimation>

#define POINT_SIZE          20
#define ANIMATION_SIZE      100
#define ANIMATION_DURATION  10000

AncientLabel::AncientLabel(QWidget *parent)
    : QWidget(parent)
{
    m_posAnimation = new QPropertyAnimation(this, QByteArray(), this);
    m_posAnimation->setDuration(ANIMATION_DURATION);
    m_posAnimation->setEasingCurve(QEasingCurve::InOutSine);

    connect(m_posAnimation, SIGNAL(finished()), SLOT(finished()));
    connect(m_posAnimation, SIGNAL(valueChanged(QVariant)), SLOT(posValueChanged(QVariant)));
}

AncientLabel::~AncientLabel()
{
    delete m_posAnimation;
}

void AncientLabel::start()
{
    QTimer::singleShot(rand() % ANIMATION_DURATION, this, SLOT(timeout()));
}

void AncientLabel::setColor(const QColor &color)
{
    m_color = color;
}

void AncientLabel::timeout()
{
    m_offset = rect().center();
    m_size = rand() % POINT_SIZE + 1;

    int x = rand() % (LABEL_RADIUS * 3), y = rand() % (LABEL_RADIUS * 3);
    if(x % 2 == 0)
    {
        x = -x;
    }

    if(y % 2 == 0)
    {
        y = -y;
    }

    m_posAnimation->setStartValue(m_offset);
    m_posAnimation->setEndValue(m_offset + QPoint(LABEL_RADIUS / 2 + x, LABEL_RADIUS / 2 + y));
    m_posAnimation->start();
}

void AncientLabel::finished()
{
    start();
}

void AncientLabel::posValueChanged(const QVariant &value)
{
    m_offset = value.toPoint();

    const QPoint &startPoint = m_posAnimation->startValue().toPoint();
    const QPoint &endPoint = m_posAnimation->endValue().toPoint();
    const int totalLength = sqrt(pow(startPoint.x() - endPoint.x(), 2) + pow(startPoint.y() - endPoint.y(), 2));
    const int currentLength = sqrt(pow(startPoint.x() - m_offset.x(), 2) + pow(startPoint.y() - m_offset.y(), 2));
    m_opacity = (totalLength - currentLength) * 1.0 / totalLength;

    update();
}

void AncientLabel::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    if(m_offset == QPoint(0, 0))
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(m_opacity);
    painter.setPen(QPen(m_color, 2));

    QVector<QLine> points;
    points << QLine(m_offset.x() - m_size, m_offset.y(), m_offset.x(), m_offset.y());
    points << QLine(m_offset.x(), m_offset.y(), m_offset.x(), m_offset.y() + m_size);
    points << QLine(m_offset.x() - m_size, m_offset.y(), m_offset.x(), m_offset.y() + m_size);
    painter.drawLines(points);
}



FloridAncient::FloridAncient(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Ancient Widget"));

    for(int i = 0; i < ANIMATION_SIZE; ++i)
    {
        AncientLabel *label = new AncientLabel(this);
        m_labels << label;
    }
}

FloridAncient::~FloridAncient()
{
    if(m_xscale)
    {
        delete[] m_xscale;
    }
}

void FloridAncient::start()
{
    Florid::start();
    for(int i = 0; i < m_labels.count(); ++i)
    {
        m_labels[i]->start();
    }
}

void FloridAncient::stop()
{
    Florid::stop();
}

void FloridAncient::paintEvent(QPaintEvent *e)
{
    Florid::paintEvent(e);

    if(m_rows == 0)
    {
        return;
    }

    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(QPen(m_averageColor, 3));
    painter.translate(rect().center());

    qreal startAngle = 0;
    for(int i = 0; i < m_cols; ++i)
    {
        const int offset = i >= m_cols / 2 ? m_cols - i : i;

        painter.save();
        painter.rotate(startAngle);
        const int value = m_intern_vis_data[int(offset * 0.6)];
        painter.drawLine(0, LABEL_RADIUS + 5 + value * 0.2, 0, LABEL_RADIUS + 5 + value * 0.3);

        painter.restore();
        startAngle += 360.0 / m_cols;
    }
    painter.restore();
}

void FloridAncient::resizeEvent(QResizeEvent *)
{
    for(AncientLabel *label : m_labels)
    {
        label->setGeometry(0, 0, width(), height());
    }
    m_roundLabel->raise();
}

void FloridAncient::processData(float *left, float *)
{
    for(int i = 0; i < m_labels.count(); ++i)
    {
        m_labels[i]->setColor(m_averageColor);
    }

    const int rows = (height() - 2) / m_cellSize.height();
    const int cols = (width() - 2) / m_cellSize.width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        if(m_intern_vis_data)
        {
            delete[] m_intern_vis_data;
        }

        if(m_xscale)
        {
            delete[] m_xscale;
        }

        m_intern_vis_data = new int[m_cols]{0};
        m_xscale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_xscale[i] = pow(pow(255.0, 1.0 / m_cols), i);
        }
    }

    short dest[256];
    short y;
    int k, magnitude;

    calc_freq(dest, left);
    const double y_scale = (double) 1.25 * m_rows / log(256);

    for(int i = 0; i < m_cols; ++i)
    {
        y = 0;
        magnitude = 0;

        if(m_xscale[i] == m_xscale[i + 1])
        {
            y = dest[i];
        }

        for(k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log(y) * y_scale);
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzerSize * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}
