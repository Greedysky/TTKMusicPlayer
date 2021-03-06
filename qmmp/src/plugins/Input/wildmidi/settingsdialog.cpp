#include <QSettings>
#include <QStringList>
#include <qmmp/qmmp.h>
#include "wildmidihelper.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    m_ui.sampleRateComboBox->addItem(tr("44100 Hz"), 44100);
    m_ui.sampleRateComboBox->addItem(tr("48000 Hz"), 48000);

    int i = m_ui.sampleRateComboBox->findData(settings.value("sample_rate", 44100).toInt());
    m_ui.sampleRateComboBox->setCurrentIndex(i);
    m_ui.enhancedResamplingCheckBox->setChecked(settings.value("enhanced_resampling", false).toBool());
    m_ui.reverbCheckBox->setChecked(settings.value("reverberation", false).toBool());
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    settings.setValue("sample_rate", m_ui.sampleRateComboBox->itemData(m_ui.sampleRateComboBox->currentIndex()));
    settings.setValue("enhanced_resampling", m_ui.enhancedResamplingCheckBox->isChecked());
    settings.setValue("reverberation", m_ui.reverbCheckBox->isChecked());
    settings.endGroup();

    WildMidiHelper::instance()->readSettings();
    QDialog::accept();
}
