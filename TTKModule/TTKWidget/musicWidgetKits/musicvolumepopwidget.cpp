#include "musicvolumepopwidget.h"
#include "musicfunctionuiobject.h"
#include "musicclickedslider.h"
#include "musicwidgetheaders.h"

#include <QTimer>

MusicVolumePopWidget::MusicVolumePopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
    m_menuShown = false;

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

    QString style = MusicUIObject::MQSSBtnSound;
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
    QTimer::singleShot(500 * MT_MS, m_menu, SLOT(close()));
}

#if TTK_QT_VERSION_CHECK(6,0,0)
void MusicVolumePopWidget::enterEvent(QEnterEvent *event)
#else
void MusicVolumePopWidget::enterEvent(QEvent *event)
#endif
{
    MusicToolMenuWidget::enterEvent(event);
    if(!m_menuShown)
    {
        m_menuShown = true;
        popupMenu();
        QTimer::singleShot(500 * MT_MS, this, SLOT(timeToReset()));
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

    m_volumeSlider = new MusicClickedSlider(Qt::Vertical, this);
    m_volumeSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle02);

    connect(m_volumeSlider, SIGNAL(valueChanged(int)), SIGNAL(musicVolumeChanged(int)));

    layout->addWidget(m_volumeSlider);
    m_containWidget->setLayout(layout);
}
