#include "musicgrabwidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>

MusicGrabWidget::MusicGrabWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setWindowOpacity(0.1);
    resize(QApplication::desktop()->width(), QApplication::desktop()->height());
    setCursor(Qt::CrossCursor);
    m_isDrawing = false;
}

MusicGrabWidget::~MusicGrabWidget()
{

}

QString MusicGrabWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicGrabWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    m_ptCursor.setX(event->x());
    m_ptCursor.setY(event->y());
    update();
}

void MusicGrabWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::black);

    int width, height;
    if(m_isDrawing)
    {
        width = m_ptCursor.x() - m_ptStart.x();
        height = m_ptCursor.y() - m_ptStart.y();
        painter.drawRect(m_ptStart.x(), m_ptStart.y(), width, height);
    }
    else
    {
        if(m_ptEnd != m_ptStart)
        {
            width = m_ptEnd.x() - m_ptStart.x();
            height = m_ptEnd.y() - m_ptStart.y();
            painter.drawRect(m_ptStart.x(), m_ptStart.y(), width, height);
        }
    }
}

void MusicGrabWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_ptStart = event->pos();
        m_isDrawing = true;
    }
    update();
}

void MusicGrabWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        m_ptEnd = event->pos();
        m_isDrawing = false;
    }
}

void MusicGrabWidget::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if(event->key() == Qt::Key_Enter)
    {
        int width = m_ptEnd.x() - m_ptStart.x();
        int height = m_ptEnd.y() - m_ptStart.y();
        QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId(),
                                             m_ptStart.x(), m_ptStart.y(), width, height);
        QDateTime dt = QDateTime::currentDateTime();
        QString filename = dt.toString("yyyyMMddhhmmss") + ".jpg";
        pixmap.save(filename, 0, 100);
        deleteLater();
    }
}
