#include <QSettings>
#include <QAbstractButton>
#include "echoplugin.h"
#include "settingsdialog.h"

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
    m_ui.delaySlider->setRange(0, 1000);
    m_ui.feedSlider->setRange(0, 100);
    m_ui.volumeSlider->setRange(0, 100);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.delaySlider->setValue(settings.value("Echo/delay", 500).toUInt());
    m_ui.feedSlider->setValue(settings.value("Echo/feedback", 50).toUInt());
    m_ui.volumeSlider->setValue(settings.value("Echo/volume", 50).toUInt());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Echo/delay", m_ui.delaySlider->value());
    settings.setValue("Echo/feedback", m_ui.feedSlider->value());
    settings.setValue("Echo/volume", m_ui.volumeSlider->value());
    QDialog::accept();
}

void SettingsDialog::on_delaySlider_valueChanged(int value)
{
    m_ui.delayLabel->setText(QString(tr("%1 ms")).arg(value));
    if(EchoPlugin::instance())
        EchoPlugin::instance()->setDelay(value);
}

void SettingsDialog::on_feedSlider_valueChanged(int value)
{
    m_ui.feedLabel->setText(QString(tr("%1 %")).arg(value));
    if(EchoPlugin::instance())
        EchoPlugin::instance()->setFeedback(value);
}

void SettingsDialog::on_volumeSlider_valueChanged(int value)
{
    m_ui.volumeLabel->setText(QString(tr("%1 %")).arg(value));
    if(EchoPlugin::instance())
        EchoPlugin::instance()->setVolume(value);
}
