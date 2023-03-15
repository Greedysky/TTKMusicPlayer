#include "ttkabstractmovedialog.h"
#include "ttkglobalhelper.h"

TTKAbstractMoveDialog::TTKAbstractMoveDialog(QWidget *parent)
    : TTKAbstractMoveDialog(true, parent)
{

}

TTKAbstractMoveDialog::TTKAbstractMoveDialog(bool transparent, QWidget *parent)
    : QDialog(parent),
      m_moveOption(false),
      m_showShadow(true),
      m_leftButtonPress(false)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, transparent);
}

void TTKAbstractMoveDialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    if(m_showShadow)
    {
        QPainter painter(this);
        TTK::setBorderShadow(this, &painter);
    }
}

void TTKAbstractMoveDialog::mousePressEvent(QMouseEvent *event)
{
    QDialog::mousePressEvent(event);
    if(event->button() == Qt::LeftButton && !m_moveOption)
    {
        m_leftButtonPress = true;
    }

    m_pressAt = QtMouseGlobalPos(event);
}

void TTKAbstractMoveDialog::mouseMoveEvent(QMouseEvent *event)
{
    QDialog::mouseMoveEvent(event);
    if(!m_leftButtonPress )
    {
        event->ignore();
        return;
    }

    const int xpos = QtMouseGlobalX(event) - m_pressAt.x();
    const int ypos = QtMouseGlobalY(event) - m_pressAt.y();
    m_pressAt = QtMouseGlobalPos(event);
    move(x() + xpos, y() + ypos);
}

void TTKAbstractMoveDialog::mouseReleaseEvent(QMouseEvent *event)
{
    QDialog::mouseReleaseEvent(event);
    m_pressAt = QtMouseGlobalPos(event);
    m_leftButtonPress = false;
}
