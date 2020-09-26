#include "musictransitionanimationlabel.h"
#include "musictime.h"
#include "qalg/qimagewrap.h"

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

    m_cubeWave = nullptr;
    m_waterWave = nullptr;

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
        animationFinished();
    }
}

void MusicTransitionAnimationLabel::setPixmap(const QPixmap &pix)
{
#if TTK_QT_VERSION_CHECK(5,15,0)
    if(m_noAnimationSet || pixmap(Qt::ReturnByValue).isNull())
#else
    if(m_noAnimationSet || !pixmap())
#endif
    {
        m_rendererPixmap = pix;
        QLabel::setPixmap(pix);
        return;
    }

    stop();
#if TTK_QT_VERSION_CHECK(5,15,0)
    m_previousPixmap = pixmap(Qt::ReturnByValue);
#else
    m_previousPixmap = *pixmap();
#endif
    m_currentPixmap = pix;
    m_isAnimating = true;

    m_type = TTKStatic_cast(AnimationType, MusicTime::random(5));
    switch(m_type)
    {
        case FadeEffect:
        {
            m_animation->setDuration(200);
            break;
        }
        case BlindsEffect:
        {
            m_animation->setDuration(500);
            break;
        }
        case CubeEffect:
        {
            delete m_cubeWave;
            m_cubeWave = new QImageWrap::QCubeWave;
            m_cubeWave->input(rect());
            m_animation->setDuration(500);
            break;
        }
        case WaterEffect:
        {
            delete m_waterWave;
            m_waterWave = new QImageWrap::QWaterWave(m_currentPixmap.toImage(), height() / 6);
            m_waterWave->input(rect());
            m_animation->setDuration(1000);
            break;
        }
        case LeftToRightEffect:
        {
            m_animation->setDuration(150);
            break;
        }
        case TopToBottomEffect:
        {
            m_animation->setDuration(150);
            break;
        }
        default: break;
    }

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
                m_rendererPixmap = m_cubeWave->render(m_currentPixmap, m_currentValue);
                break;
            }
            case WaterEffect:
            {
                painter.drawPixmap(rect(), m_previousPixmap);
                m_rendererPixmap = m_waterWave->render(m_currentPixmap, m_currentValue);
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
