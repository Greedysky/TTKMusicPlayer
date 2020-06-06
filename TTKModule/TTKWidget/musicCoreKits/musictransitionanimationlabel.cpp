#include "musictransitionanimationlabel.h"
#include "musictime.h"

#include <QPainter>
#include <QPropertyAnimation>

MusicTransitionAnimationLabel::MusicTransitionAnimationLabel(QWidget *parent)
    : QLabel(parent)
{
    MusicTime::initRandom();
    m_type = FadeEffect;
    m_isAnimating = false;
    m_currentValue = 0;
    m_noAnimationSet = false;

    m_animation = new QPropertyAnimation(this, QByteArray());
    m_animation->setDuration(200);
    m_animation->setEasingCurve(QEasingCurve::Linear);
    m_animation->setStartValue(0);
    m_animation->setEndValue(101);
    connect(m_animation, SIGNAL(valueChanged(QVariant)), SLOT(valueChanged(QVariant)));
    connect(m_animation, SIGNAL(finished()), SLOT(animationFinished()));
}

MusicTransitionAnimationLabel::~MusicTransitionAnimationLabel()
{
    delete m_animation;
}

QPixmap MusicTransitionAnimationLabel::getRendererPixmap() const
{
    return m_rendererPixmap;
}

void MusicTransitionAnimationLabel::stop()
{
    if(m_animation->state() == QPropertyAnimation::Running)
    {
        m_animation->stop();
    }
}

void MusicTransitionAnimationLabel::setPixmap(const QPixmap &pix)
{
//    if(m_isAnimating)
//    {
//        return;
//    }

    if(m_noAnimationSet || !pixmap())
    {
        m_rendererPixmap = pix;
        QLabel::setPixmap(pix);
        return;
    }

    m_type = TTKStatic_cast(AnimationType, MusicTime::random(5));
    switch(m_type)
    {
        case FadeEffect: m_animation->setDuration(200); break;
        case BlindsEffect: m_animation->setDuration(500); break;
        case CubeEffect: m_animation->setDuration(200); break;
        case LeftToRightEffect: m_animation->setDuration(150); break;
        case TopToBottomEffect: m_animation->setDuration(150); break;
        default: break;
    }

    m_previousPixmap = *pixmap();
    m_currentPixmap = pix;
    m_isAnimating = true;
    m_animation->start();
}

void MusicTransitionAnimationLabel::valueChanged(const QVariant &value)
{
    m_currentValue = value.toFloat();
    update();
}

void MusicTransitionAnimationLabel::animationFinished()
{
    m_currentValue = 0;
    m_isAnimating = false;
    m_rendererPixmap = m_currentPixmap;

    QLabel::setPixmap(m_rendererPixmap);
}

void MusicTransitionAnimationLabel::paintEvent(QPaintEvent *event)
{
    if(m_isAnimating)
    {
        QPainter painter(this);
        switch(m_type)
        {
            case FadeEffect:
            {
                painter.drawPixmap(rect(), m_previousPixmap);

                QPixmap pix(size());
                pix.fill(Qt::transparent);
                QPainter paint(&pix);
                paint.fillRect(rect(), QColor(0xFF, 0xFF, 0xFF, 2.55 * m_currentValue));
                paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
                paint.drawPixmap(rect(), m_currentPixmap);
                paint.end();
                m_rendererPixmap = pix;
                break;
            }
            case BlindsEffect:
            {
                QPixmap pix(m_previousPixmap);
                QPainter paint(&pix);
                paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
                const int number = 10;
                const int perHeight = height() / number;
                for(int i=0; i<number; ++i)
                {
                    const QRect rect(0, perHeight * i, width(), perHeight * m_currentValue / 100.0);
                    paint.drawPixmap(rect, m_currentPixmap.copy(rect));
                }
                m_rendererPixmap = pix;
                break;
            }
            case CubeEffect:
            {
                painter.drawPixmap(rect(), m_previousPixmap);

                QPixmap pix(size());
                pix.fill(Qt::transparent);
                const int s = 100;
                for(int i=0; i<=width()/s; i+=2)
                {
                    for(int j=0; j<=height()/s; ++j)
                    {
                        const int index = (j % 2 == 0) ? i : (i + 1);
                        QPainter paint(&pix);
                        const QRect rect(index * s, j * s, s, s);
                        paint.fillRect(rect, QColor(0xFF, 0xFF, 0xFF, 2.55 * m_currentValue));
                        paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
                        paint.drawPixmap(rect, m_currentPixmap.copy(rect));
                        paint.end();
                    }
                }
                m_rendererPixmap = pix;
                break;
            }
            case LeftToRightEffect:
            {
                QPixmap pix(m_previousPixmap);
                QPainter paint(&pix);
                paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
                const QRect rect(0, 0, width() * m_currentValue / 100.0, height());
                paint.drawPixmap(rect, m_currentPixmap.copy(rect));
                m_rendererPixmap = pix;
                break;
            }
            case TopToBottomEffect:
            {
                QPixmap pix(m_previousPixmap);
                QPainter paint(&pix);
                paint.setCompositionMode(QPainter::CompositionMode_SourceIn);
                const QRect rect(0, 0, width(), height() * m_currentValue / 100.0);
                paint.drawPixmap(rect, m_currentPixmap.copy(rect));
                m_rendererPixmap = pix;
                break;
            }
            default: break;
        }
        painter.drawPixmap(rect(), m_rendererPixmap);
    }
    else
    {
        QLabel::paintEvent(event);
    }
}
