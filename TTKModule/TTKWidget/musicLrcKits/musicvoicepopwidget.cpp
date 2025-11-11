#include "musicvoicepopwidget.h"
#include "musicfunctionuiobject.h"
#include "musicwidgetheaders.h"
#include "musicinteriorlrcuiobject.h"
#include "musicqmmputils.h"
#include "ttkclickedslider.h"

#include <QTimer>

MusicVoicePopWidget::MusicVoicePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent),
      m_menuShown(false)
{
    initialize();

    disconnect(this, SIGNAL(clicked()), this, SLOT(popupMenu()));
    connect(this, SIGNAL(clicked()), SLOT(updateVoiceRemove()));
}

MusicVoicePopWidget::~MusicVoicePopWidget()
{
    delete m_slider;
}

void MusicVoicePopWidget::leaveEvent(QEvent *event)
{
    MusicToolMenuWidget::leaveEvent(event);
    TTK_SIGNLE_SHOT(TTK_DN_S2MS / 2, m_menu, close, TTK_SLOT);
}

void MusicVoicePopWidget::enterEvent(QtEnterEvent *event)
{
    MusicToolMenuWidget::enterEvent(event);
    if(!m_menuShown && styleSheet().contains(TTK::UI::InteriorMicrophoneOn))
    {
        int value = 100;
        TTK::TTKQmmp::updateVoiceConfig(false, value);
        m_slider->setValue(value);

        m_menuShown = true;
        popupMenu();
        TTK_SIGNLE_SHOT(TTK_DN_S2MS / 2, this, timeToReset, TTK_SLOT);
    }
}

void MusicVoicePopWidget::timeToReset()
{
    m_menuShown = false;
}

void MusicVoicePopWidget::valueChanged(int value)
{
    TTK::TTKQmmp::updateVoiceConfig(true, value);
}

void MusicVoicePopWidget::updateVoiceRemove(bool v)
{
    const bool state = TTK::TTKQmmp::isEffectEnabled("muffler");
    setStyleSheet((v ? state : !state) ? TTK::UI::InteriorMicrophone : TTK::UI::InteriorMicrophoneOn);

    if(v)
    {
        TTK::TTKQmmp::setEffectEnabled("muffler", !state);
    }
}

void MusicVoicePopWidget::initialize()
{
    setTranslucentBackground();

    setFixedSize(30, 30);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(TTK::UI::InteriorMicrophone);
    setToolTip(tr("VoiceRemove"));

    m_containWidget->setFixedSize(20, 100);
    QHBoxLayout *layout = new QHBoxLayout(m_containWidget);
    layout->setContentsMargins(0, 9, 0, 9);
    layout->setSpacing(0);

    m_slider = new TTKClickedSlider(Qt::Vertical, this);
    m_slider->setCursor(QCursor(Qt::PointingHandCursor));
    m_slider->setRange(0, 100);
    m_slider->setStyleSheet(TTK::UI::SliderStyle02);
    connect(m_slider, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));

    layout->addWidget(m_slider);
    m_containWidget->setLayout(layout);
}
