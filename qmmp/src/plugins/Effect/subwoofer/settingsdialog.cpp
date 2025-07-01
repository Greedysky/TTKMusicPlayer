#include "settingsdialog.h"
#include "subwooferplugin.h"

#include <QSettings>
#include <QAbstractButton>

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
    m_ui.levelSlider->setRange(3, 30);
    m_ui.cutOffSlider->setRange(50, 500);

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_level = settings.value("Subwoofer/level", 10).toUInt();
    m_cutoff = settings.value("Subwoofer/cutoff", 250).toUInt();

    m_ui.levelSlider->setValue(m_level);
    m_ui.cutOffSlider->setValue(m_cutoff);
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Subwoofer/level", m_ui.levelSlider->value());
    settings.setValue("Subwoofer/cutoff", m_ui.cutOffSlider->value());
    QDialog::accept();
}

void SettingsDialog::SettingsDialog::reject()
{
    if(SubwooferPlugin::instance())
    {
        SubwooferPlugin::instance()->setLevel(m_level); // restore settings
        SubwooferPlugin::instance()->setCutOff(m_cutoff); // restore settings
    }
    QDialog::reject();
}

void SettingsDialog::on_levelSlider_valueChanged(int value)
{
    m_ui.levelLabel->setText(tr("%1").arg(value / 10.0f));
    if(SubwooferPlugin::instance())
    {
        SubwooferPlugin::instance()->setLevel(value);
    }
}

void SettingsDialog::on_cutOffSlider_valueChanged(int value)
{
    m_ui.cutOffLabel->setText(tr("%1").arg(value));
    if(SubwooferPlugin::instance())
    {
        SubwooferPlugin::instance()->setCutOff(value);
    }
}
