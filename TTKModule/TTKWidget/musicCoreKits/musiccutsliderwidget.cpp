#include "musiccutsliderwidget.h"
#include "musicuiobject.h"

#include <qmath.h>
#include <QPainter>
#include <QMouseEvent>

static constexpr int PAINT_BUTTON_WIDTH = 20;
static constexpr int PAINT_BUTTON_HEIGHT = 20;
static constexpr int PAINT_SLIDER_HEIGHT = 10;
static constexpr int PAINT_HANDER = 6;

MusicMoveButton::MusicMoveButton(QWidget *parent)
    : QPushButton(parent)
{
    setIcon(QIcon(":/toolSets/btn_arrow"));
    setStyleSheet(TTK::UI::PushButtonStyle02);
    setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    setFocusPolicy(Qt::NoFocus);
#endif
}

void MusicMoveButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_mouseLeftPressed = true;
    }

    m_mousePressedAt = QtMouseGlobalPos(event);
}

void MusicMoveButton::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_mouseLeftPressed)
    {
        event->ignore();
        return;
    }

    const int xpos = QtMouseGlobalX(event) - m_mousePressedAt.x();
    m_mousePressedAt = QtMouseGlobalPos(event);

    move(x() + xpos, y());
    Q_EMIT moveChanged();
}

void MusicMoveButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_mousePressedAt = QtMouseGlobalPos(event);
    m_mouseLeftPressed = false;
    Q_EMIT buttonRelease();
}


MusicCutSliderWidget::MusicCutSliderWidget(QWidget *parent)
    : QWidget(parent),
      m_duration(0),
      m_position(0)
{
    m_leftControl = new MusicMoveButton(this);
    m_rightControl = new MusicMoveButton(this);
    m_leftControl->setGeometry(-PAINT_BUTTON_WIDTH / 2, PAINT_BUTTON_HEIGHT, PAINT_BUTTON_WIDTH, PAINT_BUTTON_HEIGHT);
    m_rightControl->setGeometry(-PAINT_BUTTON_WIDTH / 2, PAINT_BUTTON_HEIGHT, PAINT_BUTTON_WIDTH, PAINT_BUTTON_HEIGHT);

    connect(m_leftControl, SIGNAL(moveChanged()), SLOT(buttonMoveUpdate()));
    connect(m_rightControl, SIGNAL(moveChanged()), SLOT(buttonMoveUpdate()));
    connect(m_leftControl, SIGNAL(buttonRelease()), SLOT(buttonReleaseLeft()));
    connect(m_rightControl, SIGNAL(buttonRelease()), SLOT(buttonReleaseRight()));

    resizeGeometry(width(), height());
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

void MusicCutSliderWidget::resizeGeometry(int width, int height)
{
    m_width = width;
    m_height = height;

    if(height < 30)
    {
        return;
    }

    int lineStartHeight = (m_height - (PAINT_SLIDER_HEIGHT + PAINT_BUTTON_WIDTH)) / 2;
    m_leftControl->move(-PAINT_BUTTON_WIDTH / 2, lineStartHeight + PAINT_SLIDER_HEIGHT);
    m_rightControl->move(-PAINT_BUTTON_WIDTH / 2, lineStartHeight + PAINT_SLIDER_HEIGHT);
}

void MusicCutSliderWidget::buttonMoveUpdate()
{
    int leftX = m_leftControl->geometry().x() + PAINT_BUTTON_WIDTH / 2;
    int rightX = m_rightControl->geometry().x() + PAINT_BUTTON_WIDTH / 2;

    if(leftX < 0)
    {
        m_leftControl->move(-PAINT_BUTTON_WIDTH / 2, m_leftControl->geometry().y());
        return;
    }
    else if(rightX < 0)
    {
        m_rightControl->move(-PAINT_BUTTON_WIDTH / 2, m_rightControl->geometry().y());
        return;
    }
    else if(leftX > m_width)
    {
        m_leftControl->move(m_width - PAINT_BUTTON_WIDTH / 2, m_leftControl->geometry().y());
        return;
    }
    else if(rightX > m_width)
    {
        m_rightControl->move(m_width - PAINT_BUTTON_WIDTH / 2, m_leftControl->geometry().y());
        return;
    }

    leftX = leftX*m_duration/m_width;
    rightX = rightX*m_duration/m_width;

    if(leftX < rightX)
    {
        Q_EMIT posChanged(leftX, rightX);
    }
    else
    {
        Q_EMIT posChanged(rightX, leftX);
    }
    update();
}

void MusicCutSliderWidget::buttonReleaseLeft()
{
    int leftX = m_leftControl->geometry().x() + PAINT_BUTTON_WIDTH / 2;
        leftX = leftX*m_duration/m_width;
    Q_EMIT buttonReleaseChanged(leftX);
}

void MusicCutSliderWidget::buttonReleaseRight()
{
    int rightX = m_rightControl->geometry().x() + PAINT_BUTTON_WIDTH / 2;
        rightX = rightX*m_duration/m_width;
    Q_EMIT buttonReleaseChanged(rightX);
}

void MusicCutSliderWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    const int lineStartHeight = (m_height - (PAINT_SLIDER_HEIGHT + PAINT_BUTTON_WIDTH)) / 2;
    painter.fillRect(0, lineStartHeight, m_width, PAINT_SLIDER_HEIGHT, QColor(220, 220, 220));
    painter.fillRect(0, lineStartHeight, m_position, PAINT_SLIDER_HEIGHT, QColor(150, 150, 150));

    const int leftX = m_leftControl->geometry().x();
    const int rightX = m_rightControl->geometry().x();
    painter.fillRect(leftX < rightX ? leftX + PAINT_BUTTON_WIDTH / 2 : rightX + PAINT_BUTTON_WIDTH / 2, lineStartHeight, abs(leftX -rightX), PAINT_SLIDER_HEIGHT, QColor(TTK::UI::Color01));
    painter.fillRect(m_position - PAINT_HANDER / 2, lineStartHeight + (PAINT_SLIDER_HEIGHT - PAINT_HANDER) / 2, PAINT_HANDER, PAINT_HANDER, QColor(0, 0, 0));

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
