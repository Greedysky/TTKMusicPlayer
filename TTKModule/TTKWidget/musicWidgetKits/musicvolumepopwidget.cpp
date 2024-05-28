#include "musicvolumepopwidget.h"
#include "musicfunctionuiobject.h"
#include "musicwidgetheaders.h"
#include "ttkclickedslider.h"

#include <QTimer>

MusicVolumePopWidget::MusicVolumePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent),
      m_menuShown(false)
{
    initialize();

    disconnect(this, SIGNAL(clicked()), this, SLOT(popupMenu()));
}

MusicVolumePopWidget::~MusicVolumePopWidget()
{
    delete m_volumeSlider;
}

void MusicVolumePopWidget::setValue(int value)
{
    m_volumeSlider->blockSignals(true);
    m_volumeSlider->setValue(value);
    m_volumeSlider->blockSignals(false);

    QString style = TTK::UI::BtnSound;
    if(66 < value && value <=100)
    {
        style += "QToolButton{ margin-left:-60px; }";
    }
    else if(33 < value && value <=66)
    {
        style += "QToolButton{ margin-left:-40px; }";
    }
    else if(0 < value && value <=33)
    {
        style += "QToolButton{ margin-left:-20px; }";
    }
    else
    {
        style += "QToolButton{ margin-left:-80px; }";
    }
    setStyleSheet(style);
    setToolTip(QString::number(value));
}

int MusicVolumePopWidget::value() const
{
    return m_volumeSlider->value();
}

void MusicVolumePopWidget::leaveEvent(QEvent *event)
{
    MusicToolMenuWidget::leaveEvent(event);
    TTK_SIGNLE_SHOT(TTK_DN_S2MS / 2, m_menu, close, TTK_SLOT);
}

void MusicVolumePopWidget::enterEvent(QtEnterEvent *event)
{
    MusicToolMenuWidget::enterEvent(event);
    if(!m_menuShown)
    {
        m_menuShown = true;
        popupMenu();
        TTK_SIGNLE_SHOT(TTK_DN_S2MS / 2, this, timeToReset, TTK_SLOT);
    }
}

void MusicVolumePopWidget::timeToReset()
{
    m_menuShown = false;
}

void MusicVolumePopWidget::initialize()
{
    setTranslucentBackground();

    m_containWidget->setFixedSize(20, 100);
    QHBoxLayout *layout = new QHBoxLayout(m_containWidget);
    layout->setContentsMargins(0, 9, 0, 9);
    layout->setSpacing(0);

    m_volumeSlider = new TTKClickedSlider(Qt::Vertical, this);
    m_volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setStyleSheet(TTK::UI::SliderStyle02);

    connect(m_volumeSlider, SIGNAL(valueChanged(int)), SIGNAL(volumeChanged(int)));

    layout->addWidget(m_volumeSlider);
    m_containWidget->setLayout(layout);
}
