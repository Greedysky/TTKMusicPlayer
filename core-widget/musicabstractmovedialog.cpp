#include "musicabstractmovedialog.h"

#include <QMouseEvent>
#include <QPainter>

MusicAbstractMoveDialog::MusicAbstractMoveDialog(QWidget *parent)
    : QDialog(parent)
{
    ///Remove the title bar
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint );
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_leftButtonPress = false;
}

MusicAbstractMoveDialog::~MusicAbstractMoveDialog()
{

}

void MusicAbstractMoveDialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
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

void MusicAbstractMoveDialog::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if( event->button() == Qt::LeftButton )///Press the left key
    {
        m_leftButtonPress = true;
    }
    m_pressAt = event->globalPos();
}

void MusicAbstractMoveDialog::mouseMoveEvent(QMouseEvent *event)
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
    move( this->x()+xpos, this->y()+ypos);
}

void MusicAbstractMoveDialog::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_pressAt = event->globalPos();
    m_leftButtonPress = false;
}
