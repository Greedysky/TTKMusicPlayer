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
    m_slider->setGeometry(17,17,325,10);
    m_slider->setStyleSheet("QSlider::add-page:Horizontal{background-color: rgb(87, 97, 106);height:4px;}\
              QSlider::sub-page:Horizontal{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1,\
              y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));height:4px;}\
              QSlider::groove:Horizontal{background:transparent;height:4px;}QSlider::handle:Horizontal{\
              border-image:url(':/image/musicslider');width:8px;height: 6px;margin: -2px -3px -2px 0px;}");
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
    m_label->setGeometry(5 + ceil(pos * 318 / m_duration), 5,35,35);
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
