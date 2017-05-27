#include "musicsoundkmicrosettingpopwidget.h"
#include "musicuiobject.h"

#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QAudioDeviceInfo>
#include <QAbstractItemView>
#include <QStyledItemDelegate>

MusicSoundKMicroSettingPopWidget::MusicSoundKMicroSettingPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

MusicSoundKMicroSettingPopWidget::~MusicSoundKMicroSettingPopWidget()
{

}

QString MusicSoundKMicroSettingPopWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSoundKMicroSettingPopWidget::initWidget()
{
    setFixedSize(54, 24);
    setTranslucentBackground();

    m_containWidget->setFixedSize(310, 190);
    m_containWidget->setStyleSheet(MusicUIObject::MBackgroundStyle08 + MusicUIObject::MColorStyle03);

    QCheckBox *checkBox = new QCheckBox(tr("Hear Yourself Singing"), m_containWidget);
    checkBox->setGeometry(10, 20, 280, 25);
    checkBox->setStyleSheet(MusicUIObject::MCheckBoxStyle04);

    QCheckBox *checkBox2 = new QCheckBox(tr("Noise Elimination"), m_containWidget);
    checkBox2->setGeometry(10, 50, 280, 25);
    checkBox2->setStyleSheet(MusicUIObject::MCheckBoxStyle04);

    QLabel *microIconLabel = new QLabel(m_containWidget);
    microIconLabel->setGeometry(10, 92, 26, 18);
    microIconLabel->setStyleSheet("background:url(':/toolSets/lb_record_off')");

    QSlider *slider = new QSlider(Qt::Horizontal, m_containWidget);
    slider->setGeometry(36, 90, 150, 25);
    slider->setStyleSheet(MusicUIObject::MSliderStyle01);

    QLabel *microStateLabel = new QLabel(tr("Input Error"), m_containWidget);
    microStateLabel->setGeometry(192, 90, 97, 25);

    QLabel *inputLabel = new QLabel(tr("Input"), m_containWidget);
    inputLabel->setGeometry(10, 120, 50, 25);
    QComboBox *inputComboBox = new QComboBox(m_containWidget);
    inputComboBox->setGeometry(60, 120, 230, 25);
    inputComboBox->setStyleSheet(MusicUIObject::MBorderStyle04);
    inputComboBox->setItemDelegate(new QStyledItemDelegate(inputComboBox));
    inputComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    foreach(const QAudioDeviceInfo &info, QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    {
        inputComboBox->addItem(info.deviceName());
    }

    QLabel *outputLabel = new QLabel(tr("Output"), m_containWidget);
    outputLabel->setGeometry(10, 150, 50, 25);
    QComboBox *outputComboBox = new QComboBox(m_containWidget);
    outputComboBox->setGeometry(60, 150, 230, 25);
    outputComboBox->setStyleSheet(MusicUIObject::MBorderStyle04);
    outputComboBox->setItemDelegate(new QStyledItemDelegate(outputComboBox));
    outputComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    foreach(const QAudioDeviceInfo &info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    {
        outputComboBox->addItem(info.deviceName());
    }

    m_menu->setStyleSheet(MusicUIObject::MMenuStyle05);
}
