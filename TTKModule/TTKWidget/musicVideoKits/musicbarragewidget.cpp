#include "musicbarragewidget.h"
#include "musicwidgetutils.h"

MusicBarrageAnimation::MusicBarrageAnimation(QObject *parent)
    : QPropertyAnimation(parent)
{
    initialize();
}

MusicBarrageAnimation::MusicBarrageAnimation(QObject *target, const QByteArray &name, QObject *parent)
    : QPropertyAnimation(target, name, parent)
{
    initialize();
}

void MusicBarrageAnimation::animationFinished()
{
    setDuration(TTK::random(10 * TTK_DN_S2MS) + TTK_DN_S2MS);
    setSize(m_parentSize);
    start();
}

void MusicBarrageAnimation::setSize(const QSize &size)
{
    m_parentSize = size;
    const int height = TTK::random(size.height());
    setStartValue(QPoint(0, height));
    setEndValue(QPoint(size.width(), height));
}

void MusicBarrageAnimation::initialize()
{
    setDuration(TTK::random(10000) + TTK_DN_S2MS);
    setEasingCurve(QEasingCurve::Linear);

    connect(this, SIGNAL(finished()), SLOT(animationFinished()));
}


MusicBarrageWidget::MusicBarrageWidget(QObject *parent)
    : QObject(parent)
{
    m_parent = TTKObjectCast(QWidget*, parent);
    m_state = false;
}

MusicBarrageWidget::~MusicBarrageWidget()
{
    clearItems();
}

void MusicBarrageWidget::start()
{
    if(!m_state)
    {
        return;
    }

    for(int i = 0; i < m_labels.count(); ++i)
    {
        m_labels[i]->show();
        m_animations[i]->start();
    }
}

void MusicBarrageWidget::pause()
{
    if(!m_state)
    {
        return;
    }

    for(int i = 0; i < m_labels.count(); ++i)
    {
        m_labels[i]->hide();
        m_animations[i]->pause();
    }
}

void MusicBarrageWidget::stop()
{
    for(int i = 0; i < m_labels.count(); ++i)
    {
        m_labels[i]->hide();
        m_animations[i]->stop();
    }
}

void MusicBarrageWidget::setSize(const QSize &size)
{
    m_parentSize = size;
    for(MusicBarrageAnimation *anima : qAsConst(m_animations))
    {
        anima->setSize(size);
    }
}

void MusicBarrageWidget::barrageStateChanged(bool on)
{
    m_state = on;
    if(m_state && !m_barrageRecords.isEmpty())
    {
        clearItems();
        createLabel();
        createAnimation();
        start();
    }
    else
    {
        stop();
    }
}

void MusicBarrageWidget::addBarrage(const MusicBarrageRecord &record)
{
    TTK::initRandom();

    QLabel *label = createLabel(record);
    createAnimation(label);
    m_barrageRecords << record;

    if(m_state)
    {
        start();
    }
}

void MusicBarrageWidget::clearItems()
{
    qDeleteAll(m_labels);
    qDeleteAll(m_animations);
    m_labels.clear();
    m_animations.clear();
}

void MusicBarrageWidget::createLabel()
{
    TTK::initRandom();
    for(const MusicBarrageRecord &record : qAsConst(m_barrageRecords))
    {
        createLabel(record);
    }
}

QLabel *MusicBarrageWidget::createLabel(const MusicBarrageRecord &record)
{
    QLabel *label = new QLabel(m_parent);
    label->setStyleSheet(QString("QLabel{ color:%1 }").arg(record.m_color));
    label->setText(record.m_value);

    TTK::Widget::setLabelFontSize(label, record.m_size);
    label->resize(TTK::Widget::fontTextWidth(label->font(), label->text()), TTK::Widget::fontTextHeight(label->font()));
    label->hide();
    m_labels << label;
    return label;
}

void MusicBarrageWidget::createAnimation()
{
    for(QLabel *label : qAsConst(m_labels))
    {
        createAnimation(label);
    }
}

void MusicBarrageWidget::createAnimation(QLabel *label)
{
    MusicBarrageAnimation *anim = new MusicBarrageAnimation(label, "pos");
    anim->setSize(m_parentSize);
    m_animations << anim;
}
