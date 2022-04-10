#include "musicsoundkmicrosettingpopwidget.h"
#include "musicaudiorecordermodule.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"

#include <QAudioDeviceInfo>

MusicSoundKMicroSettingPopWidget::MusicSoundKMicroSettingPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
}

MusicSoundKMicroSettingPopWidget::~MusicSoundKMicroSettingPopWidget()
{
    delete m_inputComboBox;
    delete m_outputComboBox;
}

void MusicSoundKMicroSettingPopWidget::initialize()
{
    setFixedSize(54, 24);
    setTranslucentBackground();

    m_recordCore = nullptr;
    m_containWidget->setFixedSize(310, 190);
    m_containWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle08 + MusicUIObject::MQSSColorStyle03);

    QCheckBox *checkBox = new QCheckBox(tr("Hear yourself singing"), m_containWidget);
    checkBox->setGeometry(10, 20, 280, 25);
    checkBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle05);

    QCheckBox *checkBox2 = new QCheckBox(tr("Noise elimination"), m_containWidget);
    checkBox2->setGeometry(10, 50, 280, 25);
    checkBox2->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle05);

#ifdef Q_OS_UNIX
    checkBox->setFocusPolicy(Qt::NoFocus);
    checkBox2->setFocusPolicy(Qt::NoFocus);
#endif

    QLabel *microIconLabel = new QLabel(m_containWidget);
    microIconLabel->setGeometry(10, 92, 26, 18);

    QSlider *slider = new QSlider(Qt::Horizontal, m_containWidget);
    slider->setGeometry(36, 90, 150, 25);
    slider->setStyleSheet(QString("QSlider{%1}").arg(MusicUIObject::MQSSBackgroundStyle01) + MusicUIObject::MQSSSliderStyle01);
    slider->setRange(0, 100);
    slider->setValue(100);
    connect(slider, SIGNAL(valueChanged(int)), SLOT(volumeChanged(int)));

    QLabel *inputLabel = new QLabel(tr("Input"), m_containWidget);
    inputLabel->setGeometry(10, 120, 50, 25);
    inputLabel->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);

    m_inputComboBox = new QComboBox(m_containWidget);
    m_inputComboBox->setGeometry(60, 120, 230, 25);
    MusicUtils::Widget::generateComboBoxFormat(m_inputComboBox, MusicUIObject::MQSSBorderStyle03);

    for(const QAudioDeviceInfo &info : QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    {
        m_inputComboBox->addItem(info.deviceName());
    }
    if(m_inputComboBox->count() == 0)
    {
        microIconLabel->setStyleSheet("background:url(':/toolSets/lb_record_off')");
    }
    else
    {
        microIconLabel->setStyleSheet("background:url(':/toolSets/lb_record_on')");
    }

    QLabel *outputLabel = new QLabel(tr("Output"), m_containWidget);
    outputLabel->setGeometry(10, 150, 50, 25);
    outputLabel->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);

    m_outputComboBox = new QComboBox(m_containWidget);
    m_outputComboBox->setGeometry(60, 150, 230, 25);
    MusicUtils::Widget::generateComboBoxFormat(m_inputComboBox, MusicUIObject::MQSSBorderStyle03);

    for(const QAudioDeviceInfo &info : QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        m_outputComboBox->addItem(info.deviceName());
    }

    m_menu->setStyleSheet(MusicUIObject::MQSSMenuStyle05);
}

void MusicSoundKMicroSettingPopWidget::setAudioCore(MusicAudioRecorderModule *core)
{
    m_recordCore = core;
    volumeChanged(100);
}

int MusicSoundKMicroSettingPopWidget::audioInputIndex() const
{
    return m_inputComboBox->currentIndex();
}

int MusicSoundKMicroSettingPopWidget::audioOutputIndex() const
{
    return m_outputComboBox->currentIndex();
}

void MusicSoundKMicroSettingPopWidget::volumeChanged(int value)
{
    if(m_recordCore)
    {
        m_recordCore->setVolume(value);
    }
}
