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
    QTimer::singleShot(qrand() % ANIMATION_DURATION, this, SLOT(timeout()));
}

void AncientLabel::setColor(const QColor &color)
{
    m_color = color;
}

void AncientLabel::timeout()
{
    m_pos = rect().center();
    m_size = qrand() % POINT_SIZE + 1;
    int pos_x = qrand() % (DISTANCE * 3), pos_y = qrand() % (DISTANCE * 3);
    if(pos_x % 2 == 0)
    {
        pos_x = -pos_x;
    }
    if(pos_y % 2 == 0)
    {
        pos_y = -pos_y;
    }

    m_posAnimation->setStartValue(m_pos);
    m_posAnimation->setEndValue(m_pos + QPoint(DISTANCE / 2 + pos_x, DISTANCE / 2 + pos_y));
    m_posAnimation->start();
}

void AncientLabel::finished()
{
    start();
}

void AncientLabel::posValueChanged(const QVariant &value)
{
    m_pos = value.toPoint();

    const QPoint &startPoint = m_posAnimation->startValue().toPoint();
    const QPoint &endPoint = m_posAnimation->endValue().toPoint();
    const int totalLength = sqrt(pow(startPoint.x() - endPoint.x(), 2) + pow(startPoint.y() - endPoint.y(), 2));
    const int currentLength = sqrt(pow(startPoint.x() - m_pos.x(), 2) + pow(startPoint.y() - m_pos.y(), 2));
    const float delta = (totalLength - currentLength) * 1.0 / totalLength;
    m_opacity = delta;

    update();
}

void AncientLabel::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    if(m_pos == QPoint(0, 0))
    {
        return;
    }

    QPainter painter(this);
    painter.setOpacity(m_opacity);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(m_color, 2));

    QVector<QLine> points;
    points << QLine(m_pos.x() - m_size, m_pos.y(), m_pos.x(), m_pos.y());
    points << QLine(m_pos.x(), m_pos.y(), m_pos.x(), m_pos.y() + m_size);
    points << QLine(m_pos.x() - m_size, m_pos.y(), m_pos.x(), m_pos.y() + m_size);
    painter.drawLines(points);
}



FloridAncient::FloridAncient(QWidget *parent)
    : Florid(parent)
{
    setWindowTitle(tr("Florid Ancient Widget"));

    m_gradientOn = true;
    for(int i = 0; i < ANIMATION_SIZE; ++i)
    {
        AncientLabel *label = new AncientLabel(this);
        m_labels << label;
    }
}

FloridAncient::~FloridAncient()
{
    if(m_x_scale)
    {
        delete[] m_x_scale;
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
    QPainter painter(this);
    draw(&painter);
}

void FloridAncient::resizeEvent(QResizeEvent *e)
{
    Florid::resizeEvent(e);
    for(AncientLabel *label : m_labels)
    {
        label->setGeometry(0, 0, width(), height());
    }
    m_roundLabel->raise();
}

void FloridAncient::process(float *left, float *)
{
    for(int i = 0; i < m_labels.count(); ++i)
    {
        m_labels[i]->setColor(m_averageColor);
    }
    //
    const int rows = (height() - 2) / m_cell_size.height();
    const int cols = (width() - 2) / m_cell_size.width();

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

        m_intern_vis_data = new int[m_cols]{0};
        m_x_scale = new int[m_cols + 1]{0};

        for(int i = 0; i < m_cols + 1; ++i)
        {
            m_x_scale[i] = pow(pow(255.0, 1.0 / m_cols), i);
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

        if(m_x_scale[i] == m_x_scale[i + 1])
        {
            y = dest[i];
        }

        for(k = m_x_scale[i]; k < m_x_scale[i + 1]; ++k)
        {
            y = qMax(dest[k], y);
        }

        y >>= 7; //256

        if(y)
        {
            magnitude = int(log(y) * y_scale);
            magnitude = qBound(0, magnitude, m_rows);
        }

        m_intern_vis_data[i] -= m_analyzer_falloff * m_rows / 15;
        m_intern_vis_data[i] = magnitude > m_intern_vis_data[i] ? magnitude : m_intern_vis_data[i];
    }
}

void FloridAncient::draw(QPainter *p)
{
    if(m_rows == 0)
    {
        return;
    }

    p->save();
    p->setRenderHints(QPainter::Antialiasing);
    p->setPen(QPen(m_averageColor, 3));
    p->translate(rect().center());

    qreal startAngle = 0;
    for(int i = 0; i < m_cols; ++i)
    {
        int offset = i;
        if(i >= m_cols / 2)
        {
            offset = m_cols - i;
        }//mirror

        p->save();
        p->rotate(startAngle);
        const int value = m_intern_vis_data[int(offset * 0.6)];
        p->drawLine(0, DISTANCE + 5 + value * 0.2, 0, DISTANCE + 5 + value * 0.3);

        p->restore();
        startAngle += 360.0 / m_cols;
    }
    p->restore();
}
