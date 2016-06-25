#include "musiccutsliderwidget.h"
#include "musicuiobject.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#define PAINT_BUTTON_WIDTH  20
#define PAINT_BUTTON_HEIGHT 20
#define PAINT_SLIDER_HEIGHT 10

MusicMoveButton::MusicMoveButton(QWidget *parent)
    : QPushButton(parent)
{
    setIcon(QIcon(":/toolSets/buttonarrow"));
    setStyleSheet(MusicUIObject::MPushButtonStyle04);
    setCursor(QCursor(Qt::PointingHandCursor));
}

void MusicMoveButton::mousePressEvent(QMouseEvent *event)
{
//    QWidget::mousePressEvent(event);
    if( event->button() == Qt::LeftButton )
    {
        m_leftButtonPress = true;
    }
    m_pressAt = event->globalPos();
}

void MusicMoveButton::mouseMoveEvent(QMouseEvent *event)
{
//    QWidget::mouseMoveEvent(event);
    if( !m_leftButtonPress )
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
}


MusicCutSliderWidget::MusicCutSliderWidget(QWidget *parent)
    : QWidget(parent)
{
    m_leftControl = new MusicMoveButton(this);
    m_rightControl = new MusicMoveButton(this);
    m_leftControl->setGeometry(0, PAINT_BUTTON_HEIGHT, PAINT_BUTTON_WIDTH, PAINT_BUTTON_HEIGHT);
    m_rightControl->setGeometry(0, PAINT_BUTTON_HEIGHT, PAINT_BUTTON_WIDTH, PAINT_BUTTON_HEIGHT);
    connect(m_leftControl, SIGNAL(moveChanged()), SLOT(buttonMoveUpdate()));
    connect(m_rightControl, SIGNAL(moveChanged()), SLOT(buttonMoveUpdate()));

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
    m_leftControl->move(0, lineStartHeight + PAINT_SLIDER_HEIGHT);
    m_rightControl->move(0, lineStartHeight + PAINT_SLIDER_HEIGHT);
}

void MusicCutSliderWidget::buttonMoveUpdate()
{
    int leftX = m_leftControl->geometry().x() + 10;
    int rightX = m_rightControl->geometry().x() + 10;
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

void MusicCutSliderWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    int lineStartHeight = (m_height - (PAINT_SLIDER_HEIGHT + PAINT_BUTTON_WIDTH))/2;
    painter.setBrush(QBrush(Qt::green));
    painter.drawRect(0, lineStartHeight, m_width, PAINT_SLIDER_HEIGHT);

    painter.setBrush(QBrush(Qt::blue));
    painter.drawRect(0, lineStartHeight, m_position, PAINT_SLIDER_HEIGHT);

    painter.setBrush(QBrush(Qt::red));
    int leftX = m_leftControl->geometry().x();
    int rightX = m_rightControl->geometry().x();
    painter.drawRect(leftX < rightX ? leftX + 10 : rightX + 10,
                     lineStartHeight, abs(leftX -rightX), PAINT_SLIDER_HEIGHT);
}
