#include "musicabstractmovewidget.h"
#include "musicbackgroundmanager.h"
#include "musicwidgetheaders.h"

MusicAbstractMoveWidget::MusicAbstractMoveWidget(QWidget *parent)
    : MusicAbstractMoveWidget(true, parent)
{

}

MusicAbstractMoveWidget::MusicAbstractMoveWidget(bool transparent, QWidget *parent)
    : QWidget(parent),
      MusicWidgetRenderer(),
      m_moveOption(false),
      m_showShadow(true),
      m_leftButtonPress(false)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, transparent);

    G_BACKGROUND_PTR->addObserver(this);
}

MusicAbstractMoveWidget::~MusicAbstractMoveWidget()
{
    G_BACKGROUND_PTR->removeObserver(this);
}

void MusicAbstractMoveWidget::backgroundChanged()
{
    if(m_background)
    {
        setBackgroundPixmap(size());
    }
}

void MusicAbstractMoveWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if(m_showShadow)
    {
        QPainter painter(this);
        MusicUtils::Widget::setBorderShadow(this, &painter);
    }
}

void MusicAbstractMoveWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton && !m_moveOption)
    {
        m_leftButtonPress = true;
    }

    m_pressAt = QtMouseEventGlobalPos(event);
}

void MusicAbstractMoveWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(!m_leftButtonPress)
    {
        event->ignore();
        return;
    }

    const int xpos = QtMouseEventGlobalX(event) - m_pressAt.x();
    const int ypos = QtMouseEventGlobalY(event) - m_pressAt.y();
    m_pressAt = QtMouseEventGlobalPos(event);
    move(x() + xpos, y() + ypos);
}

void MusicAbstractMoveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_pressAt = QtMouseEventGlobalPos(event);
    m_leftButtonPress = false;
}



MusicAbstractMoveSingleWidget::MusicAbstractMoveSingleWidget(QWidget *parent)
    : MusicAbstractMoveSingleWidget(true, parent)
{

}

MusicAbstractMoveSingleWidget::MusicAbstractMoveSingleWidget(bool transparent, QWidget *parent)
    : MusicAbstractMoveWidget(transparent, parent)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    l->setContentsMargins(4, 4, 4, 4);
    l->setSpacing(0);

    m_container = new QWidget(this);
    l->addWidget(m_container);
    setLayout(l);
}

MusicAbstractMoveSingleWidget::~MusicAbstractMoveSingleWidget()
{
    delete m_container;
}
