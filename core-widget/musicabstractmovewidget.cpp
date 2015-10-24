#include "musicabstractmovewidget.h"

#include <QPainter>

MusicAbstractMoveWidget::MusicAbstractMoveWidget(QWidget *parent)
    : QWidget(parent)
{
    ///Remove the title bar
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint );

    m_moveOption = false;
    m_leftButtonPress = false;
    m_showShadow = true;
}

MusicAbstractMoveWidget::~MusicAbstractMoveWidget()
{

}

void MusicAbstractMoveWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if(m_showShadow)
    {
        QPainter painter(this);

        painter.drawPixmap(0, 0, WIDTH_S, HEIGHT_S, QPixmap(":/shadow/corner1"));
        painter.drawPixmap(width() - WIDTH_S, 0, WIDTH_S, HEIGHT_S, QPixmap(":/shadow/corner2"));
        painter.drawPixmap(0, height() - HEIGHT_S, WIDTH_S, HEIGHT_S, QPixmap(":/shadow/corner3"));
        painter.drawPixmap(width() - WIDTH_S, height() - HEIGHT_S, WIDTH_S, HEIGHT_S, QPixmap(":/shadow/corner4"));

        painter.drawPixmap(0, WIDTH_S, HEIGHT_S, height() - 2*WIDTH_S,
                           QPixmap(":/shadow/left").scaled(WIDTH_S, height() - 2*HEIGHT_S));
        painter.drawPixmap(width() - WIDTH_S, WIDTH_S, HEIGHT_S, height() - 2*HEIGHT_S,
                           QPixmap(":/shadow/right").scaled(WIDTH_S, height() - 2*HEIGHT_S));
        painter.drawPixmap(HEIGHT_S, 0, width() - 2*WIDTH_S, HEIGHT_S,
                           QPixmap(":/shadow/top").scaled(width() - 2*WIDTH_S, HEIGHT_S));
        painter.drawPixmap(WIDTH_S, height() - HEIGHT_S, width() - 2*WIDTH_S, HEIGHT_S,
                           QPixmap(":/shadow/bottom").scaled(width() - 2*WIDTH_S, HEIGHT_S));
    }
}

void MusicAbstractMoveWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if( event->button() == Qt::LeftButton && !m_moveOption)///Press the left key
    {
        m_leftButtonPress = true;
    }
    m_pressAt = event->globalPos();
}

void MusicAbstractMoveWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if( !m_leftButtonPress )///Not press the left key
    {
        event->ignore();
        return;
    }
    int xpos = event->globalX() - m_pressAt.x();
    int ypos = event->globalY() - m_pressAt.y();
    m_pressAt = event->globalPos();
    move( x() + xpos, y() + ypos);
}

void MusicAbstractMoveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_pressAt = event->globalPos();
    m_leftButtonPress = false;
}
