#include "musictimesliderwidget.h"
#include "musicuiobject.h"
#include "musicmovinglabelslider.h"
#include "musicgiflabelwidget.h"

#include <qmath.h>

MusicTimeSliderWidget::MusicTimeSliderWidget(QWidget *parent)
    : QWidget(parent)
{
    m_label = new MusicGifLabelWidget(this);
    m_label->setType(MusicGifLabelWidget::Gif_Ballon_White);
    m_label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_label->move(15, 5);
    m_label->hide();

    m_slider = new MusicMovingLabelSlider(Qt::Horizontal, this);
    m_slider->setValue(0);
    m_slider->setGeometry(15, m_label->width()/2, width() - m_label->width(), 10);
    m_slider->setStyleSheet(MusicUIObject::MSliderStyle07);
    m_slider->setCursor(QCursor(Qt::PointingHandCursor));
}

void MusicTimeSliderWidget::setObject(QObject *object) const
{
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(sliderMovedAt(int)));
    connect(m_slider, SIGNAL(sliderReleasedAt(int)), object, SLOT(musicPlayAnyTimeAt(int)));
}

MusicTimeSliderWidget::~MusicTimeSliderWidget()
{
    delete m_label;
    delete m_slider;
}

void MusicTimeSliderWidget::setPlayState(bool state)
{
    if(!state)
    {
        m_label->show();
        m_label->raise();
        m_label->start();
    }
    else
    {
        m_label->stop();
        m_label->hide();
    }
}

void MusicTimeSliderWidget::setValue(qint64 value) const
{
    if(!m_slider->isMoving())
    {
        sliderMovedAt(value);
        m_slider->setValue(value);
    }
}

void MusicTimeSliderWidget::setRange(int min, int max)
{
    m_slider->setRange(min, max);
}

void MusicTimeSliderWidget::sliderMovedAt(int pos) const
{
    const int max = m_slider->maximum();
    if(max > 0)
    {
        const float delta = m_slider->width()*(-0.015/800) + 0.0275;
        m_label->move(ceil(qint64(pos)*m_slider->width()*(1.0 - delta)/max) - 1, 5);
    }
}

void MusicTimeSliderWidget::setSliderStyleByType(int type)
{
    QString rgba1 = "rgb(231, 80, 229)", rgba2 = "rgb(7, 208, 255)";
    switch(type)
    {
        case 1: rgba1 = "rgb(122, 246, 231)"; rgba2 = "rgb(244, 247, 158)"; break;
        case 2: rgba1 = "rgb(232, 202, 189)"; rgba2 = "rgb(191, 252, 198)"; break;
        case 3: rgba1 = "rgb(213, 203, 255)"; rgba2 = "rgb(153, 236, 255)"; break;
        case 4: rgba1 = "rgb(255, 179, 249)"; rgba2 = "rgb(247, 246, 200)"; break;
        default: break;
    }
    const QString &prefix = "QSlider::sub-page:Horizontal{background-color:qlineargradient("
                            "spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 " + rgba1 + ", stop:1 " + rgba2 + ");}";
    m_slider->setStyleSheet(MusicUIObject::MSliderStyle07 + prefix);
}

void MusicTimeSliderWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_slider->setFixedWidth(width() - m_label->width());
    sliderMovedAt(m_slider->value());
}
