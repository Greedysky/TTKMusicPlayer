#include "musictimesliderwidget.h"
#include "musicmovinglabelslider.h"
#include "musicgiflabelwidget.h"

#include <cmath>

MusicTimeSliderWidget::MusicTimeSliderWidget(QWidget *parent)
    : QWidget(parent)
{
    m_label = new MusicGifLabelWidget(this);
    m_label->setType(MusicGifLabelWidget::Module::BallonWhite);
    m_label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_label->move(15, 5);
    m_label->hide();

    m_slider = new MusicMovingLabelSlider(Qt::Horizontal, this);
    m_slider->setValue(0);
    m_slider->setGeometry(15, m_label->width() / 2, width() - m_label->width(), 10);
    m_slider->setStyleSheet(TTK::UI::SliderStyle07);
    m_slider->setCursor(QCursor(Qt::PointingHandCursor));
}

MusicTimeSliderWidget::~MusicTimeSliderWidget()
{
    delete m_label;
    delete m_slider;
}

void MusicTimeSliderWidget::setInputModule(QObject *object) const
{
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(sliderMovedAt(int)));
    connect(m_slider, SIGNAL(sliderReleasedAt(int)), object, SLOT(playAnyTimePosition(int)));
}

void MusicTimeSliderWidget::setCurrentPlayState(bool state)
{
    if(state)
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
        const float delta = m_slider->width() * (-0.015 / 800) + 0.0275;
        m_label->move(std::ceil(qint64(pos) * m_slider->width() * (1.0 - delta) / max) - 1, 5);
    }
}

void MusicTimeSliderWidget::setSliderStyleByType(int type)
{
    QString front = "rgb(231, 80, 229)", back = "rgb(7, 208, 255)";
    switch(type)
    {
        case 1:
        {
            front = "rgb(122, 246, 231)";
            back = "rgb(244, 247, 158)";
            break;
        }
        case 2:
        {
            front = "rgb(232, 202, 189)";
            back = "rgb(191, 252, 198)";
            break;
        }
        case 3:
        {
            front = "rgb(213, 203, 255)";
            back = "rgb(153, 236, 255)";
            break;
        }
        case 4:
        {
            front = "rgb(255, 179, 249)";
            back = "rgb(247, 246, 200)";
            break;
        }
        default: break;
    }
    const QString &prefix = "QSlider::sub-page:Horizontal{ background-color:qlineargradient("
                            "spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 " + front + ", stop:1 " + back + "); }";
    m_slider->setStyleSheet(TTK::UI::SliderStyle07 + prefix);
}

void MusicTimeSliderWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_slider->setFixedWidth(width() - m_label->width());
    sliderMovedAt(m_slider->value());
}
