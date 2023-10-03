#include "musictransitionanimationlabel.h"
#include "ttktime.h"

#include "qalgorithm/imagewrapper.h"

#include <QPainter>
#include <QPropertyAnimation>

MusicTransitionAnimationLabel::MusicTransitionAnimationLabel(QWidget *parent)
    : QLabel(parent),
      m_type(Module::FadeEffect),
      m_isAnimating(false),
      m_currentValue(0),
      m_noAnimationSet(false),
      m_cubeWave(nullptr),
      m_waterWave(nullptr)
{
    TTK::initRandom();

    m_animation = new QPropertyAnimation(this, {});
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

QPixmap MusicTransitionAnimationLabel::rendererPixmap() const
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
    if(pix.isNull())
    {
        return;
    }

    const QPixmap &pixmap = QtLablePixmap(this);
    if(m_noAnimationSet || pixmap.isNull())
    {
        m_rendererPixmap = pix;
        QLabel::setPixmap(pix);
        return;
    }

    stop();

    m_previousPixmap = pixmap;
    m_currentPixmap = pix;
    m_isAnimating = true;

    m_type = TTKStaticCast(Module, TTK::random(5));
    switch(m_type)
    {
        case Module::FadeEffect:
        {
            m_animation->setDuration(200);
            break;
        }
        case Module::BlindsEffect:
        {
            m_animation->setDuration(500);
            break;
        }
        case Module::CubeEffect:
        {
            delete m_cubeWave;
            m_cubeWave = new QAlgorithm::CubeWave;
            m_cubeWave->input(rect());
            m_animation->setDuration(500);
            break;
        }
        case Module::WaterEffect:
        {
            delete m_waterWave;
            m_waterWave = new QAlgorithm::WaterWave(m_currentPixmap.toImage(), height() / 6);
            m_waterWave->input(rect());
            m_animation->setDuration(1000);
            break;
        }
        case Module::LeftToRightEffect:
        {
            m_animation->setDuration(150);
            break;
        }
        case Module::TopToBottomEffect:
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
            case Module::FadeEffect:
            {
                painter.drawPixmap(rect(), m_previousPixmap);
                QPixmap pix(size());
                pix.fill(Qt::transparent);
                QPainter painter(&pix);
                painter.fillRect(rect(), QColor(0xFF, 0xFF, 0xFF, 2.55 * m_currentValue));
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                painter.drawPixmap(rect(), m_currentPixmap);
                m_rendererPixmap = pix;
                break;
            }
            case Module::BlindsEffect:
            {
                QPixmap pix(m_previousPixmap);
                QPainter painter(&pix);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                constexpr int number = 10;
                const int perHeight = height() / number;
                for(int i = 0; i < number; ++i)
                {
                    const QRect rect(0, perHeight * i, width(), perHeight * m_currentValue / 100.0);
                    painter.drawPixmap(rect, m_currentPixmap.copy(rect));
                }
                m_rendererPixmap = pix;
                break;
            }
            case Module::CubeEffect:
            {
                painter.drawPixmap(rect(), m_previousPixmap);
                m_rendererPixmap = m_cubeWave->render(m_currentPixmap, m_currentValue);
                break;
            }
            case Module::WaterEffect:
            {
                painter.drawPixmap(rect(), m_previousPixmap);
                m_rendererPixmap = m_waterWave->render(m_currentPixmap, m_currentValue);
                break;
            }
            case Module::LeftToRightEffect:
            {
                QPixmap pix(m_previousPixmap);
                QPainter painter(&pix);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                const QRect rect(0, 0, width() * m_currentValue / 100.0, height());
                painter.drawPixmap(rect, m_currentPixmap.copy(rect));
                m_rendererPixmap = pix;
                break;
            }
            case Module::TopToBottomEffect:
            {
                QPixmap pix(m_previousPixmap);
                QPainter painter(&pix);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                const QRect rect(0, 0, width(), height() * m_currentValue / 100.0);
                painter.drawPixmap(rect, m_currentPixmap.copy(rect));
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
