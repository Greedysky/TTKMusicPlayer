#include "musicbarragewidget.h"
#include "musictime.h"
#include "musicobject.h"

#include <QFile>
#include <QDebug>
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
    setSize(m_parentSize);
    start();
}

void MusicBarrageAnimation::setSize(const QSize &size)
{
    m_parentSize = size;
    int randHeight = qrand()%size.height();
    setStartValue(QPoint(0, randHeight));
    setEndValue(QPoint(size.width(), randHeight));
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
    m_fontSize = 10;
    m_backgroundColor = QColor(0, 0, 0);

    readBarrage();
}

MusicBarrageWidget::~MusicBarrageWidget()
{
    writeBarrage();
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
    for(int i=0; i<m_labels.count(); i++)
    {
        m_animations[i]->setSize(size);
    }
}

void MusicBarrageWidget::setLabelBackground(const QColor &color)
{
    m_backgroundColor = color;
    for(int i=0; i<m_labels.count(); i++)
    {
        QLabel *label = m_labels[i];
        QString colorString = QString("QLabel{background-color:rgb(%1,%2,%3);}")
                .arg(color.red()).arg(color.green()).arg(color.blue());
        label->setStyleSheet(label->styleSheet() + colorString);
    }
}

void MusicBarrageWidget::setLabelTextSize(int size)
{
    m_fontSize = size;
    for(int i=0; i<m_labels.count(); i++)
    {
        QLabel *label = m_labels[i];
        QFont f = label->font();
        f.setPointSize(size);
        label->setFont(f);
        label->resize(QFontMetrics(f).width(label->text()),
                      QFontMetrics(f).height());
    }
}

void MusicBarrageWidget::addBarrage(const QString &string)
{
    MusicTime::timeSRand();
    QLabel *label = new QLabel(m_parentClass);
    QString color = QString("QLabel{color:rgb(%1,%2,%3);}")
            .arg(qrand()%255).arg(qrand()%255).arg(qrand()%255);
    label->setStyleSheet(color);
    label->setText(string);
    m_barrageLists << string;

    QFont f = label->font();
    f.setPointSize(m_fontSize);
    label->setFont(f);
    label->resize(QFontMetrics(f).width(label->text()),
                  QFontMetrics(f).height());

    color = QString("QLabel{background-color:rgb(%1,%2,%3);}")
            .arg(m_backgroundColor.red()).arg(m_backgroundColor.green())
            .arg(m_backgroundColor.blue());
    label->setStyleSheet(label->styleSheet() + color);

    label->hide();
    m_labels << label;

    MusicBarrageAnimation *anim = new MusicBarrageAnimation(label, "pos");
    anim->setSize(m_parentSize);
    m_animations << anim;

    if(m_barrageState)
    {
        label->show();
        anim->start();
    }
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
    MusicTime::timeSRand();
    for(int i=0; i<m_barrageLists.count(); i++)
    {
        QLabel *label = new QLabel(m_parentClass);
        QString color = QString("QLabel{color:rgb(%1,%2,%3);}")
                .arg(qrand()%255).arg(qrand()%255).arg(qrand()%255);
        label->setStyleSheet(color);
        label->setText(m_barrageLists[qrand()%m_barrageLists.count()]);
        label->hide();
        m_labels << label;
    }
}

void MusicBarrageWidget::createAnimation()
{
    for(int i=0; i<m_labels.count(); i++)
    {
        MusicBarrageAnimation *anim = new MusicBarrageAnimation(m_labels[i], "pos");
        anim->setSize(m_parentSize);
        m_animations << anim;
    }
}

void MusicBarrageWidget::readBarrage()
{
    QFile file(BARRAGEPATH_AL);
    if(file.open(QIODevice::ReadOnly))
    {
        m_barrageLists << QString(file.readAll()).split("\r\n");
        for(int i=m_barrageLists.count() -1; i>=0; --i)
        {
            if(m_barrageLists[i].isEmpty())
            {
                m_barrageLists.removeAt(i);
            }
        }
    }
    file.close();
}

void MusicBarrageWidget::writeBarrage()
{
    QFile file(BARRAGEPATH_AL);
    if(file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QByteArray array;
        foreach(QString var, m_barrageLists)
        {
            array.append(var + '\n');
        }
        file.write(array);
    }
    file.close();
}
