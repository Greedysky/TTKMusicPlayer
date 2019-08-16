#include "musiccutsliderwidget.h"
#include "musicuiobject.h"

#include <qmath.h>
#include <QPainter>
#include <QMouseEvent>

#define PAINT_BUTTON_WIDTH  20
#define PAINT_BUTTON_HEIGHT 20
#define PAINT_SLIDER_HEIGHT 10
#define PAINT_HANDER        6

MusicMoveButton::MusicMoveButton(QWidget *parent)
    : QPushButton(parent)
{
    setIcon(QIcon(":/toolSets/btn_arrow"));
    setStyleSheet(MusicUIObject::MPushButtonStyle02);
    setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicMoveButton::mousePressEvent(QMouseEvent *event)
{
//    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_leftButtonPress = true;
    }
    m_pressAt = event->globalPos();
}

void MusicMoveButton::mouseMoveEvent(QMouseEvent *event)
{
//    QWidget::mouseMoveEvent(event);
    if(!m_leftButtonPress)
    {
        event->ignore();
        return;
    }

    int xpos = event->globalX() - m_pressAt.x();
    m_pressAt = event->globalPos();
    move( x() + xpos, y());
    emit moveChanged();
}

void MusicMoveButton::mouseReleaseEvent(QMouseEvent *event)
{
//    QWidget::mouseReleaseEvent(event);
    m_pressAt = event->globalPos();
    m_leftButtonPress = false;
    emit buttonRelease();
}


MusicCutSliderWidget::MusicCutSliderWidget(QWidget *parent)
    : QWidget(parent)
{
    m_leftControl = new MusicMoveButton(this);
    m_rightControl = new MusicMoveButton(this);
    m_leftControl->setGeometry(-PAINT_BUTTON_WIDTH/2, PAINT_BUTTON_HEIGHT, PAINT_BUTTON_WIDTH, PAINT_BUTTON_HEIGHT);
    m_rightControl->setGeometry(-PAINT_BUTTON_WIDTH/2, PAINT_BUTTON_HEIGHT, PAINT_BUTTON_WIDTH, PAINT_BUTTON_HEIGHT);

    connect(m_leftControl, SIGNAL(moveChanged()), SLOT(buttonMoveUpdate()));
    connect(m_rightControl, SIGNAL(moveChanged()), SLOT(buttonMoveUpdate()));
    connect(m_leftControl, SIGNAL(buttonRelease()), SLOT(buttonReleaseLeft()));
    connect(m_rightControl, SIGNAL(buttonRelease()), SLOT(buttonReleaseRight()));

    m_position = 0;
    m_duration = 0;
    resizeWindow(width(), height());
}

MusicCutSliderWidget::~MusicCutSliderWidget()
{
    delete m_leftControl;
    delete m_rightControl;
}

void MusicCutSliderWidget::setPosition(qint64 position)
{
    if(m_duration <= 0)
    {
        m_duration = 1;
    }
    m_position = m_width*position/m_duration;
    update();
}

void MusicCutSliderWidget::setDuration(qint64 duration)
{
    m_duration = duration;
}

void MusicCutSliderWidget::resizeWindow(int width, int height)
{
    m_width = width;
    m_height = height;

    if(height < 30)
    {
        return;
    }

    int lineStartHeight = (m_height - (PAINT_SLIDER_HEIGHT + PAINT_BUTTON_WIDTH))/2;
    m_leftControl->move(-PAINT_BUTTON_WIDTH/2, lineStartHeight + PAINT_SLIDER_HEIGHT);
    m_rightControl->move(-PAINT_BUTTON_WIDTH/2, lineStartHeight + PAINT_SLIDER_HEIGHT);
}

void MusicCutSliderWidget::buttonMoveUpdate()
{
    int leftX = m_leftControl->geometry().x() + PAINT_BUTTON_WIDTH/2;
    int rightX = m_rightControl->geometry().x() + PAINT_BUTTON_WIDTH/2;

    if(leftX < 0)
    {
        m_leftControl->move(-PAINT_BUTTON_WIDTH/2, m_leftControl->geometry().y());
        return;
    }
    else if(rightX < 0)
    {
        m_rightControl->move(-PAINT_BUTTON_WIDTH/2, m_rightControl->geometry().y());
        return;
    }
    else if(leftX > m_width)
    {
        m_leftControl->move(m_width - PAINT_BUTTON_WIDTH/2, m_leftControl->geometry().y());
        return;
    }
    else if(rightX > m_width)
    {
        m_rightControl->move(m_width - PAINT_BUTTON_WIDTH/2, m_leftControl->geometry().y());
        return;
    }

    leftX = leftX*m_duration/m_width;
    rightX = rightX*m_duration/m_width;

    if(leftX < rightX)
    {
        emit posChanged(leftX, rightX);
    }
    else
    {
        emit posChanged(rightX, leftX);
    }
    update();
}

void MusicCutSliderWidget::buttonReleaseLeft()
{
    int leftX = m_leftControl->geometry().x() + PAINT_BUTTON_WIDTH/2;
        leftX = leftX*m_duration/m_width;
    emit buttonReleaseChanged(leftX);
}

void MusicCutSliderWidget::buttonReleaseRight()
{
    int rightX = m_rightControl->geometry().x() + PAINT_BUTTON_WIDTH/2;
        rightX = rightX*m_duration/m_width;
    emit buttonReleaseChanged(rightX);
}

void MusicCutSliderWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    const int lineStartHeight = (m_height - (PAINT_SLIDER_HEIGHT + PAINT_BUTTON_WIDTH))/2;
    painter.setBrush(QBrush(QColor(220, 220, 220)));
    painter.drawRect(0, lineStartHeight, m_width, PAINT_SLIDER_HEIGHT);

    painter.setBrush(QBrush(QColor(150, 150, 150)));
    painter.drawRect(0, lineStartHeight, m_position, PAINT_SLIDER_HEIGHT);

    painter.setBrush(QBrush(QColor(MusicUIObject::MColorStyle12_S)));
    const int leftX = m_leftControl->geometry().x();
    const int rightX = m_rightControl->geometry().x();
    painter.drawRect(leftX < rightX ? leftX + PAINT_BUTTON_WIDTH/2 : rightX + PAINT_BUTTON_WIDTH/2, lineStartHeight, abs(leftX -rightX), PAINT_SLIDER_HEIGHT);

    painter.setBrush(QBrush(QColor(0, 0, 0)));
    painter.drawRect(m_position - PAINT_HANDER/2, lineStartHeight + (PAINT_SLIDER_HEIGHT - PAINT_HANDER)/2, PAINT_HANDER, PAINT_HANDER);

}

void MusicCutSliderWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicCutSliderWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MusicCutSliderWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}
