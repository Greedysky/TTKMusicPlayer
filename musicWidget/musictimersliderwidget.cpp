#include "musictimersliderwidget.h"
#include "musicuiobject.h"
#include "musicmovinglabelslider.h"

#include <qmath.h>
#include <QLabel>
#include <QSlider>

MusicTimerSliderWidget::MusicTimerSliderWidget(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, 376, 40);

    m_label = new QLabel(this);
    m_label->move(5, 5);
    m_label->setFixedSize(35, 35);
    m_label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_label->hide();

    m_slider = new MusicMovingLabelSlider(Qt::Horizontal, this);
    m_slider->setGeometry(17, 17, 345, 10);
    m_slider->setStyleSheet(MusicUIObject::MSliderStyle05);
    m_slider->setCursor(QCursor(Qt::PointingHandCursor));

    m_picIndex = -1;
    m_reverse = false;

}

void MusicTimerSliderWidget::setObject(QObject *object) const
{
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(sliderMovedAt(int)));
    connect(m_slider, SIGNAL(sliderMoved(int)), object, SLOT(musicPlayAnyTimeAt(int)));
}

MusicTimerSliderWidget::~MusicTimerSliderWidget()
{
    delete m_label;
    delete m_slider;
}

void MusicTimerSliderWidget::setPlayState(bool state)
{
    if(!state)
    {
        m_timer.start(200);
        m_label->show();
        m_label->raise();
    }
    else
    {
        m_timer.stop();
        m_label->hide();
    }
}

void MusicTimerSliderWidget::setValue(qint64 value) const
{
    sliderMovedAt(value);
    m_slider->setValue(value);
}

void MusicTimerSliderWidget::setRange(int min, int max)
{
    m_duration = max;
    m_slider->setRange(min, max);
}

void MusicTimerSliderWidget::sliderMovedAt(int pos) const
{
    m_label->move(5 + ceil(pos * 338 / m_duration), 5);
}

void MusicTimerSliderWidget::timeout()
{
    if(!m_reverse)
    {
        if(++m_picIndex == 6)
        {
            m_reverse = true;
        }
    }
    else
    {
        if(--m_picIndex == 0)
        {
            m_reverse = false;
        }
    }
    m_label->setPixmap(QPixmap("://slidergif/" + QString::number(m_picIndex)));
}

void MusicTimerSliderWidget::setSliderStyleByType(int type)
{
    QString rgba1 = "rgb(231, 80, 229)", rgba2 = "rgb(7, 208, 255)";
    switch(type)
    {
        case 1: rgba1 = "rgb(232, 202, 189)"; rgba2 = "rgb(191, 252, 198)"; break;
        case 2: rgba1 = "rgb(255, 179, 249)"; rgba2 = "rgb(247, 246, 200)"; break;
        case 3: rgba1 = "rgb(122, 246, 231)"; rgba2 = "rgb(244, 247, 191)"; break;
        case 4: rgba1 = "rgb(213, 203, 255)"; rgba2 = "rgb(153, 236, 255)"; break;
    }
    QString prefix = "QSlider::sub-page:Horizontal{background-color:qlineargradient("
                     "spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 " + rgba1 + ", stop:1 " + rgba2 + ");}";
    m_slider->setStyleSheet(MusicUIObject::MSliderStyle05 + prefix);
}
