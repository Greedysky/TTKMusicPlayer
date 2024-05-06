#include "bs2bplugin.h"
#include "settingsdialog.h"

#include <QSettings>
#include <bs2b/bs2b.h>

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
    m_ui.feedSlider->setRange(BS2B_MINFEED, BS2B_MAXFEED);
    m_ui.freqSlider->setRange(BS2B_MINFCUT, BS2B_MAXFCUT);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_level = settings.value("Bs2b/level", BS2B_DEFAULT_CLEVEL).toUInt();
    m_ui.feedSlider->setValue(m_level >> 16);
    m_ui.freqSlider->setValue(m_level & 0xffff);
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Bs2b/level", m_ui.feedSlider->value() << 16 | m_ui.freqSlider->value());
    QDialog::accept();
}

void SettingsDialog::SettingsDialog::reject()
{
    if(Bs2bPlugin::instance())
    {
        Bs2bPlugin::instance()->setCrossfeedLevel(m_level); // restore settings
    }
    QDialog::reject();
}

void SettingsDialog::on_freqSlider_valueChanged(int value)
{
    m_ui.freqLabel->setText(tr("%1 Hz, %2 us").arg(value).arg(bs2b_level_delay(value)));
    if(Bs2bPlugin::instance())
    {
        Bs2bPlugin::instance()->setCrossfeedLevel(m_ui.feedSlider->value() << 16 | m_ui.freqSlider->value());
    }
}

void SettingsDialog::on_feedSlider_valueChanged(int value)
{
    m_ui.feedLabel->setText(tr("%1 dB").arg((double)value / 10));
    if(Bs2bPlugin::instance())
    {
        Bs2bPlugin::instance()->setCrossfeedLevel(m_ui.feedSlider->value() << 16 | m_ui.freqSlider->value());
    }
}

void SettingsDialog::on_defaultButton_pressed()
{
    m_ui.feedSlider->setValue(BS2B_DEFAULT_CLEVEL >> 16);
    m_ui.freqSlider->setValue(BS2B_DEFAULT_CLEVEL & 0xffff);
}

void SettingsDialog::on_cmButton_pressed()
{
    m_ui.feedSlider->setValue(BS2B_CMOY_CLEVEL >> 16);
    m_ui.freqSlider->setValue(BS2B_CMOY_CLEVEL & 0xffff);
}

void SettingsDialog::on_jmButton_pressed()
{
    m_ui.feedSlider->setValue(BS2B_JMEIER_CLEVEL >> 16);
    m_ui.freqSlider->setValue(BS2B_JMEIER_CLEVEL & 0xffff);
}
