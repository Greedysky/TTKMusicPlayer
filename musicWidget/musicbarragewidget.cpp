#include "musicbarragewidget.h"
#include "musictime.h"

MusicBarrageAnimation::MusicBarrageAnimation(QObject *parent)
    : QPropertyAnimation(parent)
{
    init();
}

MusicBarrageAnimation::MusicBarrageAnimation(QObject *target,
                                             const QByteArray &propertyName,
                                             QObject *parent)
    : QPropertyAnimation(target, propertyName, parent)
{
    init();
}

void MusicBarrageAnimation::animationFinished()
{
    setDuration(qrand()%10000 + 1000);
    start();
}

void MusicBarrageAnimation::init()
{
    MusicTime::timeSRand();
    setDuration(qrand()%10000 + 1000);
    setEasingCurve(QEasingCurve::Linear);
    connect(this, SIGNAL(finished()), SLOT(animationFinished()));
}



MusicBarrageWidget::MusicBarrageWidget(QObject *parent)
    : QObject(parent)
{
    m_parentClass = static_cast<QWidget*>(parent);

    createLabel();
    createAnimation();

}

MusicBarrageWidget::~MusicBarrageWidget()
{
    while(!m_labels.isEmpty())
    {
        delete m_labels.takeLast();
        delete m_animations.takeLast();
    }
}

void MusicBarrageWidget::start()
{
    for(int i=0; i<NUMBER; i++)
    {
        m_labels[i]->show();
        m_animations[i]->start();
    }
}

void MusicBarrageWidget::pause()
{
    for(int i=0; i<NUMBER; i++)
    {
        m_labels[i]->hide();
        m_animations[i]->pause();
    }
}

void MusicBarrageWidget::stop()
{
    for(int i=0; i<NUMBER; i++)
    {
        m_labels[i]->hide();
        m_animations[i]->stop();
    }
}

void MusicBarrageWidget::setSize(const QSize &size)
{
    m_parentSize = size;
    for(int i=0; i<NUMBER; i++)
    {
        m_animations[i]->setStartValue(QPoint(0, 100 + i*100));
        m_animations[i]->setEndValue(QPoint(size.width(), 100+ i*100));
    }
}

void MusicBarrageWidget::createLabel()
{
    for(int i=0; i<NUMBER; i++)
    {
        QLabel *label = new QLabel(m_parentClass);
        label->setText("test");
        label->hide();
        m_labels << label;
    }
}

void MusicBarrageWidget::createAnimation()
{
    for(int i=0; i<NUMBER; i++)
    {
        MusicBarrageAnimation *anim = new MusicBarrageAnimation(m_labels[i], "pos");
        m_animations << anim;
    }
}
