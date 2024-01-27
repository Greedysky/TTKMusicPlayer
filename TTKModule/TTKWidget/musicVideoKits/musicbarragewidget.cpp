#include "musicbarragewidget.h"
#include "musicwidgetutils.h"
#include "musicbarragerequest.h"

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
    : QObject(parent),
      m_state(false),
      m_parent(TTKObjectCast(QWidget*, parent))
{
    m_networkRequest = new MusicBarrageRequest(this);
    connect(m_networkRequest, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
}

MusicBarrageWidget::~MusicBarrageWidget()
{
    clearItems();
    delete m_networkRequest;
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
    for(MusicBarrageAnimation *animation : qAsConst(m_animations))
    {
        animation->setSize(size);
    }
}

void MusicBarrageWidget::setBarrage(const QString &name, const QString &id)
{
    if(m_lastQueryID == id)
    {
        return;
    }

    clearBarrages();
    m_lastQueryID = id;
    m_networkRequest->startToRequest(name);
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

void MusicBarrageWidget::downLoadFinished(const QByteArray &bytes)
{
    TTKAbstractXml xml;
    if(xml.fromByteArray(bytes))
    {
        for(const TTKXmlNode &node : xml.readMultiNodeByTagName("d"))
        {
            if(m_barrageRecords.count() > 20) // limit size
            {
                break;
            }

            QString attrValue;
            for(const TTKXmlAttr &attr : qAsConst(node.m_attrs))
            {
                if(attr.m_key == "p")
                {
                    attrValue = attr.m_value.toString();
                    break;
                }
            }

            if(!attrValue.isEmpty())
            {
                const QStringList &keys = attrValue.split(",");
                if(keys.count() < 8)
                {
                    continue;
                }

                const int size = keys[2].toInt();

                MusicBarrageRecord record;
                if(size < 25)
                {
                    record.m_size = 15;
                }
                else if(size > 25)
                {
                    record.m_size = 30;
                }
                else
                {
                    record.m_size = 20;
                }

                record.m_color = QColor(keys[3].toInt()).name();
                record.m_value = node.m_text;
                addBarrage(record);
            }
        }
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

void MusicBarrageWidget::clearBarrages()
{
    clearItems();
    m_barrageRecords.clear();
}

QLabel *MusicBarrageWidget::createLabel(const MusicBarrageRecord &record)
{
    QLabel *label = new QLabel(m_parent);
    label->setStyleSheet(QString("QLabel{ color:%1; }").arg(record.m_color));
    label->setText(record.m_value);

    TTK::Widget::setFontSize(label, record.m_size);
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
