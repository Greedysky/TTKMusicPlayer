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
      m_animationEnabled(true),
      m_imageRender(nullptr)
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
    delete m_imageRender;
}

const QPixmap& MusicTransitionAnimationLabel::renderPixmap() const noexcept
{
    return m_renderPixmap;
}

void MusicTransitionAnimationLabel::start()
{
    stop();
    m_isAnimating = true;
    m_animation->start();
}

void MusicTransitionAnimationLabel::stop()
{
    if(isRunning())
    {
        m_animation->stop();
        animationFinished();
    }
}

bool MusicTransitionAnimationLabel::isRunning() const
{
    return m_animation->state() == QPropertyAnimation::Running;
}

void MusicTransitionAnimationLabel::setPixmap(const QPixmap &pix)
{
    if(pix.isNull())
    {
        return;
    }

    const QPixmap &pixmap = QtLablePixmap(this);
    if(!m_animationEnabled || pixmap.isNull())
    {
        m_renderPixmap = pix;
        QLabel::setPixmap(pix);
        return;
    }

    stop();

    m_previousPixmap = pixmap;
    m_currentPixmap = pix;
    m_isAnimating = true;

    m_type = TTKStaticCast(Module, TTK::random(TTKStaticCast(int, Module::MAX)));
    switch(m_type)
    {
        case Module::FadeEffect:
        {
            m_animation->setDuration(500);
            break;
        }
        case Module::BlindsEffect:
        {
            m_animation->setDuration(500);
            break;
        }
        case Module::CubeEffect:
        {
            delete m_imageRender;
            m_imageRender = new QAlgorithm::CubeWave;
            m_imageRender->input(rect());
            m_animation->setDuration(1000);
            break;
        }
        case Module::WaterEffect:
        {
            delete m_imageRender;
            m_imageRender = new QAlgorithm::WaterWave(m_currentPixmap.toImage(), height() / 6);
            m_imageRender->input(rect());
            m_animation->setDuration(1000);
            break;
        }
        case Module::LeftToRightEffect:
        {
            m_animation->setDuration(250);
            break;
        }
        case Module::TopToBottomEffect:
        {
            m_animation->setDuration(250);
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
    m_renderPixmap = m_currentPixmap;

    QLabel::setPixmap(m_renderPixmap);
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
                m_renderPixmap = pix;
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
                m_renderPixmap = pix;
                break;
            }
            case Module::CubeEffect:
            {
                painter.drawPixmap(rect(), m_previousPixmap);
                m_renderPixmap = m_imageRender->render(m_currentPixmap, m_currentValue);
                break;
            }
            case Module::WaterEffect:
            {
                painter.drawPixmap(rect(), m_previousPixmap);
                m_renderPixmap = m_imageRender->render(m_currentPixmap, m_currentValue);
                break;
            }
            case Module::LeftToRightEffect:
            {
                QPixmap pix(m_previousPixmap);
                QPainter painter(&pix);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                const QRect rect(0, 0, width() * m_currentValue / 100.0, height());
                painter.drawPixmap(rect, m_currentPixmap.copy(rect));
                m_renderPixmap = pix;
                break;
            }
            case Module::TopToBottomEffect:
            {
                QPixmap pix(m_previousPixmap);
                QPainter painter(&pix);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                const QRect rect(0, 0, width(), height() * m_currentValue / 100.0);
                painter.drawPixmap(rect, m_currentPixmap.copy(rect));
                m_renderPixmap = pix;
                break;
            }
            default: break;
        }
        painter.drawPixmap(rect(), m_renderPixmap);
    }
    else
    {
        QLabel::paintEvent(event);
    }
}
