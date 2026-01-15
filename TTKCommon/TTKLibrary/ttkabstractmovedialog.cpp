#include "ttkabstractmovedialog.h"
#include "ttkglobalinterface.h"

TTKAbstractMoveDialog::TTKAbstractMoveDialog(QWidget *parent)
    : TTKAbstractMoveDialog(true, parent)
{

}

TTKAbstractMoveDialog::TTKAbstractMoveDialog(bool transparent, QWidget *parent)
    : QDialog(parent),
      m_mouseLeftPressed(false),
      m_blockOption(false),
      m_shadowOption(true)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, transparent);
}

void TTKAbstractMoveDialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);

    if(m_shadowOption)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        TTK::setBorderShadow(this, &painter);
    }
}

void TTKAbstractMoveDialog::mousePressEvent(QMouseEvent *event)
{
    QDialog::mousePressEvent(event);
    if(event->button() == Qt::LeftButton && !m_blockOption)
    {
        m_mouseLeftPressed = true;
        m_windowPos = pos();
        m_pressedPos = QtGlobalPosition(event);
    }
}

void TTKAbstractMoveDialog::mouseMoveEvent(QMouseEvent *event)
{
    QDialog::mouseMoveEvent(event);
    if(!m_mouseLeftPressed)
    {
        event->ignore();
        return;
    }

    move(m_windowPos + QtGlobalPosition(event) - m_pressedPos);
}

void TTKAbstractMoveDialog::mouseReleaseEvent(QMouseEvent *event)
{
    QDialog::mouseReleaseEvent(event);
    m_mouseLeftPressed = false;
}
