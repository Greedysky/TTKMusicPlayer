#include "musictimersliderwidget.h"
#include <QLabel>
#include <QSlider>

MusicTimerSliderWidget::MusicTimerSliderWidget(QWidget *parent) : QWidget(parent)
{
    setGeometry(0,0,356,40);

    m_label = new QLabel(this);
    m_label->setFixedSize(35,35);
    m_label->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    m_label->hide();

    m_slider = new QSlider(Qt::Horizontal,this);
    m_slider->setGeometry(17,17,321,5);
    m_slider->setStyleSheet(MusicObject::MusicVolumeStyleHorizontal);
    m_slider->setCursor(QCursor(Qt::PointingHandCursor));

    m_picIndex = -1;
    m_reverse = false;

}

void MusicTimerSliderWidget::setObject(QObject* object)
{
    connect(&m_timer,SIGNAL(timeout()),SLOT(timeout()));
    connect(m_slider,SIGNAL(sliderMoved(int)),SLOT(sliderMovedAt(int)));
    connect(m_slider,SIGNAL(sliderMoved(int)),object,SLOT(musicPlayAnyTimeAt(int)));
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

void MusicTimerSliderWidget::setValue(qint64 value)
{
    sliderMovedAt(value);
    m_slider->setValue(value);
}

void MusicTimerSliderWidget::setRange(int min, int max)
{
    m_duration = max;
    m_slider->setRange(min, max);
}

void MusicTimerSliderWidget::sliderMovedAt(int pos)
{
    m_label->setGeometry(pos * 321.0 / m_duration, 2,35,35);
}

void MusicTimerSliderWidget::timeout()
{
    if(!m_reverse)
    {
        if(++m_picIndex == 6)
            m_reverse = true;
    }
    else
    {
        if(--m_picIndex == 0)
            m_reverse = false;
    }
    m_label->setPixmap(QPixmap("://slidergif/" + QString::number(m_picIndex)));
}
