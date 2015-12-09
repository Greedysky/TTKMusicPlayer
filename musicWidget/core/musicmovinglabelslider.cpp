#include "musicmovinglabelslider.h"
#include "musictime.h"

MusicMovingLabelSlider::MusicMovingLabelSlider(QWidget *parent)
    : MusicMovingLabelSlider(Qt::Vertical, parent)
{

}

MusicMovingLabelSlider::MusicMovingLabelSlider(Qt::Orientation orientation,
                                               QWidget *parent)
    : QSlider(orientation, parent)
{
    setMouseTracking(true);

    m_orientation = orientation;
    m_textLabel = new QLabel;
    m_textLabel->setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
    m_textLabel->setGeometry(0, 0, m_orientation == Qt::Vertical ? 20 : 40,
                                   m_orientation == Qt::Vertical ? 40 : 20);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setStyleSheet("QLabel{color:#888888; background-color:#FFE6E6; \
                                       border:1px solid gray;}");
}

MusicMovingLabelSlider::~MusicMovingLabelSlider()
{
    delete m_textLabel;
}

void MusicMovingLabelSlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    m_textLabel->raise();
}

void MusicMovingLabelSlider::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);

    QPoint curPos = mapFromGlobal(QCursor::pos());
    QPoint glbPos = mapToGlobal(QPoint(0, 0));
    QSize sizePos = size();
    QPoint changePos;

    if(m_orientation == Qt::Vertical)
    {
        if( 0 < curPos.y() && curPos.y() < sizePos.height())
        {
            changePos.setX(glbPos.y() + curPos.y());
            changePos.setY(curPos.y());
        }
        if(curPos.y() <= 0)
        {
            changePos.setX(glbPos.y());
            changePos.setY(0);
        }
        if(curPos.y() >= sizePos.height())
        {
            changePos.setX(glbPos.y() + sizePos.height());
            changePos.setY(sizePos.height());
        }
        m_textLabel->move((glbPos + QPoint(sizePos.width(), 0)).x(), changePos.x());
    }
    else
    {
        if( 0 < curPos.x() && curPos.x() < sizePos.width())
        {
            changePos.setX(glbPos.x() + curPos.x());
            changePos.setY(curPos.x()*maximum()/sizePos.width());
        }
        if(curPos.x() <= 0)
        {
            changePos.setX(glbPos.x());
            changePos.setY(0);
        }
        if(curPos.x() >= sizePos.width())
        {
            changePos.setX(glbPos.x() + sizePos.width());
            changePos.setY(maximum());
        }
        m_textLabel->move(changePos.x(), (glbPos - QPoint(0, m_textLabel->height())).y());
    }
    m_textLabel->setText(normalizeTimeLAbel(changePos.y()));
}

void MusicMovingLabelSlider::enterEvent(QEvent *event)
{
    QSlider::enterEvent(event);
    m_textLabel->show();
}

void MusicMovingLabelSlider::leaveEvent(QEvent *event)
{
    QSlider::leaveEvent(event);
    m_textLabel->hide();
}

QString MusicMovingLabelSlider::normalizeTimeLAbel(qint64 time)
{
    MusicTime t(time, MusicTime::All_Msec);
    return t.toString("mm:ss");
}
