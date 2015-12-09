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
    m_textLabel->setGeometry(0, 0, 40, 20);
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
        changePos = limitLableGeometry(curPos.y(), glbPos.y(), sizePos.height());
        m_textLabel->move((glbPos + QPoint(sizePos.width(), 0)).x(), changePos.x());
    }
    else
    {
        changePos = limitLableGeometry(curPos.x(), glbPos.x(), sizePos.width());
        m_textLabel->move(changePos.x(), (glbPos - QPoint(0, m_textLabel->height())).y());
    }
    m_textLabel->setText(MusicTime::msecTime2LabelJustified(changePos.y()));
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

QPoint MusicMovingLabelSlider::limitLableGeometry(int x, int y, int z)
{
    QPoint pt;
    if( 0 < x && x < z)
    {
        pt.setX(y + x);
        pt.setY(x*maximum()/z);
    }
    if(x <= 0)
    {
        pt.setX(y);
        pt.setY(0);
    }
    if(x >= z)
    {
        pt.setX(y + z);
        pt.setY(maximum());
    }
    return pt;
}
