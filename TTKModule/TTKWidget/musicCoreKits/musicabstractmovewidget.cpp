#include "musicabstractmovewidget.h"
#include "musicbackgroundmanager.h"
#include "musicbackgroundconfigmanager.h"
#include "musicextractwrap.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"

#include <QPainter>

MusicAbstractMoveWidget::MusicAbstractMoveWidget(QWidget *parent)
    : MusicAbstractMoveWidget(true, parent)
{

}

MusicAbstractMoveWidget::MusicAbstractMoveWidget(bool transparent, QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, transparent);

    m_moveOption = false;
    m_leftButtonPress = false;
    m_showShadow = true;
    m_background = nullptr;

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

    if(event->button() == Qt::LeftButton && !m_moveOption)///Press the left key
    {
        m_leftButtonPress = true;
    }
    m_pressAt = event->globalPos();
}

void MusicAbstractMoveWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    if(!m_leftButtonPress)///Not press the left key
    {
        event->ignore();
        return;
    }

    const int xpos = event->globalX() - m_pressAt.x();
    const int ypos = event->globalY() - m_pressAt.y();
    m_pressAt = event->globalPos();
    move(x() + xpos, y() + ypos);
}

void MusicAbstractMoveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    m_pressAt = event->globalPos();
    m_leftButtonPress = false;
}

void MusicAbstractMoveWidget::setBackgroundPixmap(QLabel *label, const QSize &size)
{
    m_background = label;
    setBackgroundPixmap(size);
}

void MusicAbstractMoveWidget::setBackgroundPixmap(const QSize &size)
{
    QLabel *label = TTKStatic_cast(QLabel*, m_background);
    MusicBackgroundImage image;
    if(MusicExtractWrap::outputSkin(&image, G_BACKGROUND_PTR->getBackgroundUrl()))
    {
        label->setPixmap(image.m_pix.scaled(size));
    }
    else
    {
        label->setPixmap(QPixmap(G_BACKGROUND_PTR->getBackgroundUrl()).scaled(size));
    }
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
