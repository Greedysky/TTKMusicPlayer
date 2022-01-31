#include "settingsdialog.h"
#include "crystalizerplugin.h"

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
    m_ui.intensitySlider->setRange(0, 100);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.intensitySlider->setValue(settings.value("Crystalizer/intensity", DEFAULT_INTENSITY).toUInt());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Crystalizer/intensity", m_ui.intensitySlider->value());
    QDialog::accept();
}

void SettingsDialog::on_intensitySlider_valueChanged(int value)
{
    m_ui.intensityLabel->setText(QString(tr("%1")).arg(value * 1.0 / DEFAULT_INTENSITY));
    if(CrystalizerPlugin::instance())
        CrystalizerPlugin::instance()->setIntensity(value);
}
