#include "musicbarragewidget.h"
#include "musictime.h"
#include "musicobject.h"

#include <QFile>

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
    m_parentClass = MStatic_cast(QWidget*, parent);
    m_barrageState = false;
    m_fontSize = 15;
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
    if(m_barrageState && !m_barrageLists.isEmpty())
    {
        deleteItems();
        createLabel();
        createAnimation();
        setLabelTextSize(m_fontSize);
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
    foreach(MusicBarrageAnimation *anima, m_animations)
    {
        anima->setSize(size);
    }
}

void MusicBarrageWidget::setLabelBackground(const QColor &color)
{
    m_backgroundColor = color;
    foreach(QLabel *label, m_labels)
    {
        setLabelBackground(label);
    }
}

void MusicBarrageWidget::setLabelTextSize(int size)
{
    m_fontSize = size;
    foreach(QLabel *label, m_labels)
    {
        setLabelTextSize(label);
    }
}

void MusicBarrageWidget::addBarrage(const QString &string)
{
    MusicTime::timeSRand();
    QLabel *label = new QLabel(m_parentClass);
    createLabel(label);
    createAnimation(label);
    setLabelBackground(label);
    setLabelTextSize(label);

    m_barrageLists << string;
    label->setText(string);

    if(m_barrageState)
    {
        if(m_labels.count() == 1)
        {
            deleteItems();
            createLabel();
            createAnimation();
        }
        start();
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
    foreach(QString str, m_barrageLists)
    {
        Q_UNUSED(str);
        QLabel *label = new QLabel(m_parentClass);
        createLabel(label);
    }
}

void MusicBarrageWidget::createLabel(QLabel *label)
{
    QString color = QString("QLabel{color:rgb(%1,%2,%3);}")
            .arg(qrand()%255).arg(qrand()%255).arg(qrand()%255);
    label->setStyleSheet(color);
    if(!m_barrageLists.isEmpty())
    {
        label->setText(m_barrageLists[qrand()%m_barrageLists.count()]);
    }
    label->hide();
    m_labels << label;
}

void MusicBarrageWidget::createAnimation()
{
    foreach(QLabel *label, m_labels)
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

void MusicBarrageWidget::setLabelBackground(QLabel *label)
{
    QString colorString = QString("QLabel{background-color:rgb(%1,%2,%3);}")
            .arg(m_backgroundColor.red()).arg(m_backgroundColor.green())
            .arg(m_backgroundColor.blue());
    label->setStyleSheet(label->styleSheet() + colorString);
}

void MusicBarrageWidget::setLabelTextSize(QLabel *label)
{
    QFont ft = label->font();
    ft.setPointSize(m_fontSize);
    label->setFont(ft);
    QFontMetrics ftMcs(ft);
    label->resize(ftMcs.width(label->text()), ftMcs.height());
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
