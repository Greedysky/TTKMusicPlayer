#include "musicmovinglabelslider.h"
#include "musicuiobject.h"
#include "musictime.h"

MusicMovingLabelSlider::MusicMovingLabelSlider(QWidget *parent)
    : MusicMovingLabelSlider(Qt::Horizontal, parent)
{

}

MusicMovingLabelSlider::MusicMovingLabelSlider(Qt::Orientation orientation, QWidget *parent)
    : MusicClickedSlider(orientation, parent)
{
    setMouseTracking(true);

    m_isMoving = false;
    m_orientation = orientation;
    m_textLabel = new QLabel(this);
    m_textLabel->setWindowFlags( Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
    m_textLabel->setGeometry(0, 0, 40, 20);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setFocusPolicy(Qt::NoFocus);
    m_textLabel->setStyleSheet(MusicUIObject::MLabelStyle01);
}

MusicMovingLabelSlider::~MusicMovingLabelSlider()
{
    delete m_textLabel;
}

QString MusicMovingLabelSlider::getClassName()
{
    return staticMetaObject.className();
}

void MusicMovingLabelSlider::setValue(int value)
{
    if(!m_isMoving)
    {
        MusicClickedSlider::setValue(value);
    }
}

void MusicMovingLabelSlider::mousePressEvent(QMouseEvent *event)
{
    MusicClickedSlider::mousePressEvent(event);
    m_isMoving = false;
    emit sliderMoved( m_value );
#ifdef Q_OS_UNIX
    m_textLabel->show();
#endif
    m_textLabel->raise();
}

void MusicMovingLabelSlider::mouseMoveEvent(QMouseEvent *event)
{
    MusicClickedSlider::mouseMoveEvent(event);
    if(m_mousePress)
    {
        m_isMoving = true;
        emit sliderMoved( m_value );
    }

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

void MusicMovingLabelSlider::mouseReleaseEvent(QMouseEvent *event)
{
    MusicClickedSlider::mouseReleaseEvent(event);
    emit sliderReleasedAt( m_value );
    m_isMoving = false;
}

void MusicMovingLabelSlider::enterEvent(QEvent *event)
{
    MusicClickedSlider::enterEvent(event);
#ifndef Q_OS_UNIX
    m_textLabel->show();
#endif
}

void MusicMovingLabelSlider::leaveEvent(QEvent *event)
{
    MusicClickedSlider::leaveEvent(event);
    m_textLabel->hide();
}

QPoint MusicMovingLabelSlider::limitLableGeometry(int x, int y, int z)
{
    QPoint pt;
    if( 0 < x && x < z)
    {
        pt.setX(y + x);
        pt.setY(qint64(x)*maximum()/z);
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
