#include <QSettings>
#include <QDir>

#include <libmodplug/stdafx.h>
#include <libmodplug/it_defs.h>
#include <libmodplug/sndfile.h>

#include "decoder_modplug.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

#ifndef Q_OS_LINUX
    setMinimumHeight(364);
    setMaximumHeight(364);
#endif

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ModPlug");
    //general
    m_ui.noiseCheckBox->setChecked(settings.value("NoiseReduction", false).toBool());
    m_ui.fileNameCheckBox->setChecked(settings.value("UseFileName", false).toBool());
    m_ui.amigaCheckBox->setChecked(settings.value("GrabAmigaMOD", true).toBool());
    //channels number
    if(settings.value("Channels", 2).toInt() == 2)
        m_ui.stereoRadioButton->setChecked(true);
    else
        m_ui.monoRadioButton->setChecked(true);
    //bits number
    if(settings.value("Bits", 16).toInt() == 8)
        m_ui.bit8RadioButton->setChecked(true);
    else
        m_ui.bit16RadioButton->setChecked(true);
    //resampling frequency
    int freq = settings.value("Frequency", 44100).toInt();
    if(freq == 48000)
        m_ui.khz48RadioButton->setChecked(true);
    else if(freq == 44100)
        m_ui.khz44RadioButton->setChecked(true);
    else if(freq == 22050)
        m_ui.khz22RadioButton->setChecked(true);
    else
        m_ui.khz11RadioButton->setChecked(true);
    //resampling mode
    int res = settings.value("ResamplineMode", SRCMODE_POLYPHASE).toInt();
    if(res == SRCMODE_NEAREST)
        m_ui.resampNearestRadioButton->setChecked(true);
    else if(res == SRCMODE_LINEAR)
        m_ui.resampLinearRadioButton->setChecked(true);
    else if(res == SRCMODE_SPLINE)
        m_ui.resampSplineRadioButton->setChecked(true);
    else
        m_ui.resampPolyphaseRadioButton->setChecked(true);
    //reverberation
    m_ui.reverbGroupBox->setChecked(settings.value("Reverb", false).toBool());
    m_ui.reverbDepthSlider->setValue(settings.value("ReverbDepth", 30).toInt());
    m_ui.reverbDelaySlider->setValue(settings.value("ReverbDelay", 100).toInt());
    //surround
    m_ui.surGroupBox->setChecked(settings.value("Surround", true).toBool());
    m_ui.surDepthSlider->setValue(settings.value("SurroundDepth", 20).toInt());
    m_ui.surDelaySlider->setValue(settings.value("SurroundDelay", 20).toInt());
    //bass
    m_ui.bassGroupBox->setChecked(settings.value("Megabass", false).toBool());
    m_ui.bassAmountSlider->setValue(settings.value("BassAmount", 40).toInt());
    m_ui.bassRangeSlider->setValue(settings.value("BassRange", 30).toInt());
    //preamp
    m_ui.preampGroupBox->setChecked(settings.value("PreAmp", false).toBool());
    connect(m_ui.preampSlider, SIGNAL(valueChanged(int)), SLOT(setPreamp(int)));
    m_ui.preampSlider->setValue(int(settings.value("PreAmpLevel", 0.0f).toDouble()*10));
    //looping
    int l = settings.value("LoopCount", 0).toInt();
    if(l == 0)
        m_ui.dontLoopRadioButton->setChecked(true);
    else if(l < 0)
        m_ui.loopForeverRadioButton->setChecked(true);
    else
    {
        m_ui.loopRadioButton->setChecked(true);
        m_ui.loopSpinBox->setValue(l);
    }
    settings.endGroup();
    connect(m_ui.buttonBox, SIGNAL(clicked (QAbstractButton *)), SLOT(exec(QAbstractButton *)));
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ModPlug");
    //general
    settings.setValue("NoiseReduction", m_ui.noiseCheckBox->isChecked());
    settings.setValue("UseFileName", m_ui.fileNameCheckBox->isChecked());
    settings.setValue("GrabAmigaMOD", m_ui.amigaCheckBox->isChecked());
    //channels number
    settings.setValue("Channels", m_ui.stereoRadioButton->isChecked() ? 2 : 1);
    //bits number
    settings.setValue("Bits", m_ui.bit8RadioButton->isChecked() ? 8 : 16);
    //resampling frequency
    if(m_ui.khz48RadioButton->isChecked())
        settings.setValue("Frequency", 48000);
    else if(m_ui.khz44RadioButton->isChecked())
        settings.setValue("Frequency", 44100);
    else if(m_ui.khz22RadioButton->isChecked())
        settings.setValue("Frequency", 22050);
    else
        settings.setValue("Frequency", 11025);
    //resampling mode
    if(m_ui.resampNearestRadioButton->isChecked())
        settings.setValue("ResamplineMode", SRCMODE_NEAREST);
    else if(m_ui.resampLinearRadioButton->isChecked())
        settings.setValue("ResamplineMode", SRCMODE_LINEAR);
    else if(m_ui.resampSplineRadioButton->isChecked())
        settings.setValue("ResamplineMode", SRCMODE_SPLINE);
    else
        settings.setValue("ResamplineMode", SRCMODE_POLYPHASE);
    //reverberation
    settings.setValue("Reverb", m_ui.reverbGroupBox->isChecked());
    settings.setValue("ReverbDepth", m_ui.reverbDepthSlider->value());
    settings.setValue("ReverbDelay", m_ui.reverbDelaySlider->value());
    //surround
    settings.setValue("Surround", m_ui.surGroupBox->isChecked());
    settings.setValue("SurroundDepth", m_ui.surDepthSlider->value());
    settings.setValue("SurroundDelay", m_ui.surDelaySlider->value());
    //bass
    settings.setValue("Megabass", m_ui.bassGroupBox->isChecked());
    settings.setValue("BassAmount", m_ui.bassAmountSlider->value());
    settings.setValue("BassRange", m_ui.bassRangeSlider->value());
    //preamp
    settings.setValue("PreAmp", m_ui.preampGroupBox->isChecked());
    connect(m_ui.preampSlider, SIGNAL(valueChanged(int)), SLOT(setPreamp(int)));
    settings.setValue("PreAmpLevel", (double)m_ui.preampSlider->value() / 10);
    //looping
    if(m_ui.dontLoopRadioButton->isChecked())
        settings.setValue("LoopCount", 0);
    else if(m_ui.loopForeverRadioButton->isChecked())
        settings.setValue("LoopCount", -1);
    else
        settings.setValue("LoopCount", m_ui.loopSpinBox->value());
    settings.endGroup();
    //apply settings for the created decoder
    if(DecoderModPlug::instance())
    {
        DecoderModPlug::instance()->readSettings();
    }
}

void SettingsDialog::setPreamp(int preamp)
{
    m_ui.preampLabel->setText(QString("%1").arg((double) preamp/10));
}

void SettingsDialog::exec(QAbstractButton *button)
{
    switch((int) m_ui.buttonBox->buttonRole(button))
    {
    case QDialogButtonBox::AcceptRole:
        writeSettings();
        accept();
        break;
    case QDialogButtonBox::ApplyRole:
        writeSettings();
        break;
    }
}
