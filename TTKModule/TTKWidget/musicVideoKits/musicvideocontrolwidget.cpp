#include "musicvideocontrolwidget.h"
#include "musicdownloadwidget.h"
#include "musicmovinglabelslider.h"
#include "musicvideouiobject.h"
#include "musicvolumepopwidget.h"
#include "musicvideoqualitypopwidget.h"
#include "musicvideobarragestylepopwidget.h"
#include "musicsearchedit.h"

MusicVideoControlWidget::MusicVideoControlWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(TTK::UI::BackgroundStyle08);

    m_timeSlider = new MusicMovingLabelSlider(Qt::Horizontal, this);
    m_playButton = new QPushButton(this);
    m_timeLabel = new QLabel(QString("%1/%1").arg(MUSIC_TIME_INIT), this);
    m_qualityButton = new MusicVideoQualityPopWidget(this);
    m_volumeButton = new MusicVolumePopWidget(this);
#ifdef Q_OS_UNIX
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_volumeButton->setValue(100);
    m_volumeButton->setFixedSize(20, 20);
    m_playButton->setFixedSize(34, 34);

    m_timeLabel->setStyleSheet(TTK::UI::ColorStyle03);
    m_playButton->setStyleSheet(TTK::UI::VideoBtnPlay);
    m_timeSlider->setStyleSheet(TTK::UI::SliderStyle01);

    m_playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_volumeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_timeSlider->setCursor(QCursor(Qt::PointingHandCursor));
    m_qualityButton->setCursor(QCursor(Qt::PointingHandCursor));


    QVBoxLayout *controlVLayout = new QVBoxLayout(this);
    controlVLayout->setSpacing(0);
    controlVLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *controlBWidget = new QWidget(this);
    QHBoxLayout *controlBLayout = new QHBoxLayout(controlBWidget);
    controlBLayout->setContentsMargins(9, 0, 9, 0);
    controlBLayout->setSpacing(12);
    controlBLayout->addWidget(m_playButton);
    controlBLayout->addWidget(m_timeLabel);
    controlBLayout->addStretch(1);
    controlBLayout->addWidget(createVideoBarrageWidget(), 25);
    controlBLayout->addStretch(1);
    controlBLayout->addWidget(m_qualityButton);
    controlBLayout->addWidget(m_volumeButton);

    controlBWidget->setLayout(controlBLayout);

    controlVLayout->addWidget(m_timeSlider);
    controlVLayout->addWidget(controlBWidget);
    setLayout(controlVLayout);

    connect(m_timeSlider, SIGNAL(sliderReleasedAt(int)), SIGNAL(sliderValueChanged(int)));
    connect(m_volumeButton, SIGNAL(volumeChanged(int)), parent, SLOT(volumeChanged(int)));
    connect(m_playButton, SIGNAL(clicked()), parent, SLOT(play()));
    connect(m_qualityButton, SIGNAL(mediaUrlChanged(QString)), SIGNAL(mediaUrlChanged(QString)));
}

MusicVideoControlWidget::~MusicVideoControlWidget()
{
    delete m_timeLabel;
    delete m_timeSlider;
    delete m_playButton;
    delete m_volumeButton;
    delete m_qualityButton;
    delete m_pushBarrage;
    delete m_barrageSend;
    delete m_menuBarrage;
    delete m_searchEdit;
}

void MusicVideoControlWidget::setValue(qint64 position) const
{
    m_timeSlider->setValue(position * MT_S2MS);
    m_timeLabel->setText(QString("%1/%2").arg(TTKTime::formatDuration(position * MT_S2MS),
                                              TTKTime::formatDuration(m_timeSlider->maximum())));
}

void MusicVideoControlWidget::durationChanged(qint64 duration) const
{
    m_timeSlider->setRange(0, duration * MT_S2MS);
    m_timeLabel->setText(QString("%1/%2").arg(MUSIC_TIME_INIT, TTKTime::formatDuration(duration * MT_S2MS)));
}

void MusicVideoControlWidget::setButtonStyle(bool style) const
{
    m_playButton->setStyleSheet(style ? TTK::UI::VideoBtnPlay : TTK::UI::VideoBtnPause);
}

void MusicVideoControlWidget::mediaChanged(const QString &url)
{
    m_qualityButton->setQualityText(url);
}

void MusicVideoControlWidget::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
    m_timeSlider->setFixedWidth(w);
}

void MusicVideoControlWidget::setQualityActionState()
{
    m_qualityButton->setQualityActionState();
}

void MusicVideoControlWidget::pushBarrageClicked()
{
    m_pushBarrage->setStyleSheet(m_pushBarrageOn ? TTK::UI::VideoBtnBarrageOn : TTK::UI::VideoBtnBarrageOff);
    m_barrageSend->setEnabled(m_pushBarrageOn);
    m_menuBarrage->setEnabled(m_pushBarrageOn);
    m_searchEdit->setEnabled(m_pushBarrageOn);

    Q_EMIT pushBarrageChanged(m_pushBarrageOn);
    m_pushBarrageOn = !m_pushBarrageOn;
}

void MusicVideoControlWidget::sendBarrageClicked()
{
    if(m_searchEdit->text() == tr("Just one barrage"))
    {
        return;
    }

    MusicBarrageRecord record;
    record.m_value = m_searchEdit->text();
    record.m_size = m_menuBarrage->barrageSize();
    record.m_color = m_menuBarrage->barrageColor().name();
    Q_EMIT addBarrageChanged(record);
}

QWidget *MusicVideoControlWidget::createVideoBarrageWidget()
{
    m_pushBarrageOn = false;
    QWidget *barrageWidget = new QWidget(this);

    QHBoxLayout *barrageLayout = new QHBoxLayout(barrageWidget);
    barrageLayout->setContentsMargins(0, 0, 0, 0);

    m_menuBarrage = new MusicVideoBarrageStylePopWidget(barrageWidget);
    m_menuBarrage->setStyleSheet(TTK::UI::VideoBtnBarrageStyle);

    QWidget *pairWidget = new QWidget(barrageWidget);
    QHBoxLayout *pairWidgetLayout = new QHBoxLayout(pairWidget);
    pairWidgetLayout->setContentsMargins(0, 0, 0, 0);
    pairWidgetLayout->setSpacing(0);

    m_searchEdit = new MusicSearchEdit(pairWidget);
    m_searchEdit->setFixedHeight(24);
    m_searchEdit->setPlaceholderText(tr("Just one barrage"));
    m_searchEdit->setStyleSheet(TTK::UI::LineEditStyle04);
    connect(m_searchEdit, SIGNAL(enterFinished(QString)), SLOT(sendBarrageClicked()));

    m_barrageSend = new QPushButton(pairWidget);
    m_barrageSend->setStyleSheet(TTK::UI::VideoBtnBarrageSend);
    m_barrageSend->setCursor(QCursor(Qt::PointingHandCursor));
    m_barrageSend->setFixedSize(50, 24);
    connect(m_barrageSend, SIGNAL(clicked()), SLOT(sendBarrageClicked()));

    pairWidgetLayout->addWidget(m_searchEdit);
    pairWidgetLayout->addWidget(m_barrageSend);
    pairWidget->setLayout(pairWidgetLayout);

    m_pushBarrage = new QPushButton(barrageWidget);
    m_pushBarrage->setFixedSize(56, 20);
    m_pushBarrage->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_pushBarrage, SIGNAL(clicked()), SLOT(pushBarrageClicked()));
    pushBarrageClicked();

#ifdef Q_OS_UNIX
    m_barrageSend->setFocusPolicy(Qt::NoFocus);
    m_pushBarrage->setFocusPolicy(Qt::NoFocus);
#endif

    barrageLayout->addWidget(m_menuBarrage);
    barrageLayout->addWidget(pairWidget);
    barrageLayout->addWidget(m_pushBarrage);
    barrageWidget->setLayout(barrageLayout);

    return barrageWidget;
}
