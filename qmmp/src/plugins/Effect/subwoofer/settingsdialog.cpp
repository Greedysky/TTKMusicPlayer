#include "settingsdialog.h"
#include "subwooferplugin.h"

#include <QSettings>
#include <QAbstractButton>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    m_ui.delaySlider->setRange(0, 100);
    m_ui.feedSlider->setRange(0, 100);
    m_ui.cutOffSlider->setRange(50, 1500);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_delay = settings.value("Subwoofer/delay", 20).toUInt();
    m_feedback = settings.value("Subwoofer/feedback", 60).toUInt();
    m_cutoff = settings.value("Subwoofer/cutoff", 100).toUInt();

    m_ui.delaySlider->setValue(m_delay);
    m_ui.feedSlider->setValue(m_feedback);
    m_ui.cutOffSlider->setValue(m_cutoff);
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Subwoofer/delay", m_ui.delaySlider->value());
    settings.setValue("Subwoofer/feedback", m_ui.feedSlider->value());
    settings.setValue("Subwoofer/cutoff", m_ui.cutOffSlider->value());
    QDialog::accept();
}

void SettingsDialog::SettingsDialog::reject()
{
    if(SubwooferPlugin::instance())
    {
        SubwooferPlugin::instance()->setDelay(m_delay); // restore settings
        SubwooferPlugin::instance()->setFeedback(m_feedback); // restore settings
        SubwooferPlugin::instance()->setCutOff(m_cutoff); // restore settings
    }
    QDialog::reject();
}

void SettingsDialog::on_delaySlider_valueChanged(int value)
{
    m_ui.delayLabel->setText(tr("%1 ms").arg(value));
    if(SubwooferPlugin::instance())
    {
        SubwooferPlugin::instance()->setDelay(value);
    }
}

void SettingsDialog::on_feedSlider_valueChanged(int value)
{
    m_ui.feedLabel->setText(tr("%1 %").arg(value));
    if(SubwooferPlugin::instance())
    {
        SubwooferPlugin::instance()->setFeedback(value);
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
