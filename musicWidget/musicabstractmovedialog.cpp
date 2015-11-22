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

    painter.drawPixmap(0, 0, WIDTH, HEIGHT, QPixmap(":/shadow/corner1"));
    painter.drawPixmap(width() - WIDTH, 0, WIDTH, HEIGHT, QPixmap(":/shadow/corner2"));
    painter.drawPixmap(0, height() - HEIGHT, WIDTH, HEIGHT, QPixmap(":/shadow/corner3"));
    painter.drawPixmap(width() - WIDTH, height() - HEIGHT, WIDTH, HEIGHT, QPixmap(":/shadow/corner4"));

    painter.drawPixmap(0, WIDTH, HEIGHT, height() - 2*WIDTH,
                       QPixmap(":/shadow/left").scaled(WIDTH, height() - 2*HEIGHT));
    painter.drawPixmap(width() - WIDTH, WIDTH, HEIGHT, height() - 2*HEIGHT,
                       QPixmap(":/shadow/right").scaled(WIDTH, height() - 2*HEIGHT));
    painter.drawPixmap(HEIGHT, 0, width() - 2*WIDTH, HEIGHT,
                       QPixmap(":/shadow/top").scaled(width() - 2*WIDTH, HEIGHT));
    painter.drawPixmap(WIDTH, height() - HEIGHT, width() - 2*WIDTH, HEIGHT,
                       QPixmap(":/shadow/bottom").scaled(width() - 2*WIDTH, HEIGHT));
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
    move( x() + xpos, y() + ypos);
}

void MusicAbstractMoveDialog::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_pressAt = event->globalPos();
    m_leftButtonPress = false;
}
