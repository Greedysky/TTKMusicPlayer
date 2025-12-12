#include "settingsdialog.h"
#include "decoder_adlmidi.h"

#include <QSettings>
#include <adlmidi.h>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
#ifdef Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    const char *const *banks = adl_getBankNames();
    if(!banks)
    {
        return;
    }

    for(int i = 0; i < adl_getBanksCount(); ++i)
    {
        m_ui.banksComboBox->addItem(banks[i]);
    }

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    m_ui.banksComboBox->setCurrentIndex(settings.value("index", -1).toInt());
    m_ui.checkBox->setChecked(settings.value("use_wopl", false).toBool());
    m_ui.configPathEdit->setText(settings.value("conf_path").toString());
    m_ui.configPathEdit->setEnabled(m_ui.checkBox->isChecked());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    settings.setValue("index", m_ui.banksComboBox->currentIndex());
    settings.setValue("use_wopl", m_ui.checkBox->isChecked());
    settings.setValue("conf_path", m_ui.configPathEdit->text());
    settings.endGroup();

    if(DecoderAdlMidi::instance())
    {
        DecoderAdlMidi::instance()->readSettings();
    }
    QDialog::accept();
}
