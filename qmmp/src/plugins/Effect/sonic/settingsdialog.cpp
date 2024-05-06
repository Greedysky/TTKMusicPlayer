#include "settingsdialog.h"
#include "sonicplugin.h"

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
    m_ui.ratioSlider->setRange(12, 100);  // (0.25, 2)

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.ratioSlider->setValue(settings.value("Sonic/ratio", DEFAULT_RATIO).toInt());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Sonic/ratio", m_ui.ratioSlider->value());

    if(SonicPlugin::instance())
    {
        SonicPlugin::instance()->setRatio(m_ui.ratioSlider->value());
    }
    QDialog::accept();
}

void SettingsDialog::on_ratioSlider_valueChanged(int value)
{
    m_ui.ratioLabel->setText(QString::number(value * 1.0 / DEFAULT_RATIO, 'f', 2));
}
