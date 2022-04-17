#include "settingsdialog.h"
#include "wildmidihelper.h"

#include <QSettings>
#include <qmmp/qmmp.h>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_OS_WIN
    setFixedHeight(110);
#elif defined Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    m_ui.configPathEdit->setText(settings.value("conf_path").toString());
    m_ui.sampleRateComboBox->addItem(tr("44100 Hz"), 44100);
    m_ui.sampleRateComboBox->addItem(tr("48000 Hz"), 48000);
    m_ui.sampleRateComboBox->setCurrentIndex(m_ui.sampleRateComboBox->findData(settings.value("sample_rate", 44100).toInt()));
    m_ui.enhancedResamplingCheckBox->setChecked(settings.value("enhanced_resampling", false).toBool());
    m_ui.reverbCheckBox->setChecked(settings.value("reverberation", false).toBool());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    settings.setValue("conf_path", m_ui.configPathEdit->text());
    settings.setValue("sample_rate", m_ui.sampleRateComboBox->itemData(m_ui.sampleRateComboBox->currentIndex()));
    settings.setValue("enhanced_resampling", m_ui.enhancedResamplingCheckBox->isChecked());
    settings.setValue("reverberation", m_ui.reverbCheckBox->isChecked());
    settings.endGroup();

    WildMidiHelper::instance()->readSettings();
    QDialog::accept();
}
