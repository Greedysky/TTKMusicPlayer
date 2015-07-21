#include "musiclrcfloatwidget.h"

#include <QPropertyAnimation>
#include <QPushButton>

MusicLrcFloatWidget::MusicLrcFloatWidget(QWidget *parent)
    : QLabel(parent)
{
    m_posX = parent->width() - 235;
    m_posY = parent->height()/4;
    setStyleSheet("background:rgba(255, 255, 255, 30)");
    setGeometry( m_posX + 110, m_posY, 115, 210);

    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDuration(500);

    QPushButton *p1 = new QPushButton(this);
    p1->setGeometry(10, 10, 80, 30);

    QPushButton *p2 = new QPushButton(this);
    p2->setGeometry(10, 50, 80, 30);

    QPushButton *p3 = new QPushButton(this);
    p3->setGeometry(10, 90, 80, 30);

    QPushButton *p4 = new QPushButton(this);
    p4->setGeometry(10, 130, 80, 30);

    QPushButton *p5 = new QPushButton(this);
    p5->setGeometry(10, 170, 80, 30);
}

MusicLrcFloatWidget::~MusicLrcFloatWidget()
{
    delete m_animation;
}

void MusicLrcFloatWidget::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);
    m_animation->setStartValue(QRect(m_posX + 110, m_posY, 115, 210));
    m_animation->setEndValue(QRect(m_posX, m_posY, 115, 210));
    m_animation->start();
}

void MusicLrcFloatWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_animation->setStartValue(QRect(m_posX, m_posY, 115, 210));
    m_animation->setEndValue(QRect(m_posX + 110, m_posY, 115, 210));
    m_animation->start();
}
