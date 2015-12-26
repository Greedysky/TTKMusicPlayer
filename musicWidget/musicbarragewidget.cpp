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
    m_barrageState = false;
}

MusicBarrageWidget::~MusicBarrageWidget()
{
    deleteItems();
}

void MusicBarrageWidget::start()
{
    if(m_barrageState)
    {
        for(int i=0; i<m_labels.count(); i++)
        {
            m_labels[i]->show();
            m_animations[i]->start();
        }
    }
}

void MusicBarrageWidget::pause()
{
    if(m_barrageState)
    {
        for(int i=0; i<m_labels.count(); i++)
        {
            m_labels[i]->hide();
            m_animations[i]->pause();
        }
    }
}

void MusicBarrageWidget::stop()
{
    for(int i=0; i<m_labels.count(); i++)
    {
        m_labels[i]->hide();
        m_animations[i]->stop();
    }
}

void MusicBarrageWidget::barrageStateChanged(bool on)
{
    m_barrageState = on;
    if(m_barrageState)
    {
        deleteItems();
        createLabel();
        createAnimation();
        start();
    }
    else
    {
        stop();
    }
}

void MusicBarrageWidget::setSize(const QSize &size)
{
    m_parentSize = size;
}

void MusicBarrageWidget::deleteItems()
{
    while(!m_labels.isEmpty())
    {
        delete m_labels.takeLast();
        delete m_animations.takeLast();
    }
}

void MusicBarrageWidget::createLabel()
{
    for(int i=0; i<NUMBER; i++)
    {
        QLabel *label = new QLabel(m_parentClass);
        label->setStyleSheet("color:white;");
        label->setText("test");
        label->hide();
        m_labels << label;
    }
}

void MusicBarrageWidget::createAnimation()
{
    for(int i=0; i<m_labels.count(); i++)
    {
        MusicBarrageAnimation *anim = new MusicBarrageAnimation(m_labels[i], "pos");
        anim->setStartValue(QPoint(0, 100 + i*100));
        anim->setEndValue(QPoint(m_parentSize.width(), 100+ i*100));
        m_animations << anim;
    }
}
