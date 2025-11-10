#include "settingsdialog.h"
#include "mufflerplugin.h"

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
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.ratioSlider->setValue(settings.value("Muffler/ratio", DEFAULT_RATIO).toInt());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Muffler/ratio", m_ui.ratioSlider->value());

    if(MufflerPlugin::instance())
    {
        MufflerPlugin::instance()->setRatio(m_ui.ratioSlider->value());
    }
    QDialog::accept();
}

void SettingsDialog::on_ratioSlider_valueChanged(int value)
{
    m_ui.ratioLabel->setText(QString::number(value * 1.0 / DEFAULT_RATIO, 'f', 2));
}
