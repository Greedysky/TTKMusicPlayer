#include "musicanimationstackedwidget.h"
#include "musicnumberdefine.h"

#include <QPixmap>

MusicAnimationStackedWidget::MusicAnimationStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    m_type = LeftToRight;
    m_isAnimating = false;
    m_currentValue = 0;
    m_currentIndex = 0;
    m_previousIndex = 0;

    m_animation = new QPropertyAnimation(this, QByteArray(), this);
    m_animation->setDuration(120*MT_MS);
    m_animation->setEasingCurve(QEasingCurve::Linear);
    m_animation->setStartValue(0);
    m_animation->setEndValue(0);

    connect(m_animation, SIGNAL(valueChanged(QVariant)), SLOT(valueChanged(QVariant)));
    connect(m_animation, SIGNAL(finished()), SLOT(animationFinished()));
}

MusicAnimationStackedWidget::~MusicAnimationStackedWidget()
{
    delete m_animation;
}

void MusicAnimationStackedWidget::paintEvent(QPaintEvent * event)
{
    if(m_isAnimating)
    {
        QPainter painter(this);
        renderCurrentWidget(painter);
        renderPreviousWidget(painter);
    }
    else
    {
        QWidget::paintEvent(event);
    }
}

void MusicAnimationStackedWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicAnimationStackedWidget::renderPreviousWidget(QPainter &painter)
{
    QWidget *w = widget(m_previousIndex);
    QPixmap pixmap( w->size() );
    pixmap.fill(Qt::transparent);
    w->setAttribute(Qt::WA_TranslucentBackground, true);
    w->render(&pixmap);
    w->setAttribute(Qt::WA_TranslucentBackground, false);

    switch(m_type)
    {
        case BottomToTop :
                {
                    painter.drawPixmap(0, height()/2, pixmap);
                    break;
                }
        case TopToBottom :
                {
                    painter.drawPixmap(0, -height()/2, pixmap);
                    break;
                }
        case LeftToRight :
                {
                    painter.drawPixmap(width()/2, 0, pixmap);
                    break;
                }
        case RightToLeft :
                {
                    painter.drawPixmap(-width()/2, 0, pixmap);
                    break;
                }
        default: break;
    }
}

void MusicAnimationStackedWidget::renderCurrentWidget(QPainter &painter)
{
    QWidget *w = widget(m_currentIndex);
    QPixmap pixmap( w->size() );
    pixmap.fill(Qt::transparent);
    w->setAttribute(Qt::WA_TranslucentBackground, true);
    w->render(&pixmap);
    w->setAttribute(Qt::WA_TranslucentBackground, false);

    switch(m_type)
    {
        case BottomToTop :
                {
                    painter.translate(0, m_currentValue);
                    painter.drawPixmap(0, -height()/2, pixmap);
                    break;
                }
        case TopToBottom :
                {
                    painter.translate(0, m_currentValue);
                    painter.drawPixmap(0, height()/2, pixmap);
                    break;
                }
        case LeftToRight :
                {
                    painter.translate(m_currentValue, 0);
                    painter.drawPixmap(-width()/2, 0, pixmap);
                    break;
                }
        case RightToLeft :
                {
                    painter.translate(m_currentValue, 0);
                    painter.drawPixmap(width()/2, 0, pixmap);
                    break;
                }
        default: break;
    }
}

void MusicAnimationStackedWidget::start(int index)
{
    if(m_isAnimating)
    {
        return;
    }
    m_previousIndex = m_currentIndex;
    m_currentIndex = index;

    QWidget *w = widget(m_currentIndex);
    if(!w)
    {
        return;
    }

    const int offsetx = frameRect().width();
    const int offsety = frameRect().height();
    w->setGeometry(0, 0, offsetx, offsety);

    currentWidget()->hide();
    m_isAnimating = true;
    m_animation->start();
}

void MusicAnimationStackedWidget::setIndex(int previous, int current)
{
    m_currentIndex = current;
    m_previousIndex = previous;
}

void MusicAnimationStackedWidget::setLength(int length, AnimationType type)
{
    switch(m_type = type)
    {
        case BottomToTop :
        case LeftToRight :
                {
                    m_animation->setStartValue(-length/2);
                    m_animation->setEndValue(length/2);
                    break;
                }
        case TopToBottom :
        case RightToLeft :
                {
                    m_animation->setStartValue(length/2);
                    m_animation->setEndValue(-length/2);
                    break;
                }
        default: break;
    }
}

int MusicAnimationStackedWidget::getPreviousIndex() const
{
    return m_previousIndex;
}

int MusicAnimationStackedWidget::getCurrentIndex() const
{
    return m_currentIndex;
}

void MusicAnimationStackedWidget::setDuration(int duration)
{
    m_animation->setDuration(duration);
}

int MusicAnimationStackedWidget::getDuration() const
{
    return m_animation->duration();
}

void MusicAnimationStackedWidget::valueChanged(const QVariant &value)
{
    m_currentValue = value.toFloat();
    update();
}

void MusicAnimationStackedWidget::animationFinished()
{
    m_currentValue = 0;
    m_isAnimating = false;

    QWidget *w = widget(m_currentIndex);
    w->show();
    w->raise();

    setCurrentWidget( w );
    update();
}
