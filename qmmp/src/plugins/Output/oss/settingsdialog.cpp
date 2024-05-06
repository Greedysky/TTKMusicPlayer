#include "settingsdialog.h"

#include <QSettings>
#include <qmmp/qmmp.h>
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

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OSS");
    m_ui.lineEdit->insert(settings.value("device", "/dev/dsp").toString());
    m_ui.lineEdit_2->insert(settings.value("mixer_device", "/dev/mixer").toString());
    m_ui.bufferSpinBox->setValue(settings.value("buffer_time", 500).toInt());
    m_ui.periodSpinBox->setValue(settings.value("period_time", 100).toInt());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OSS");
    settings.setValue("device", m_ui.lineEdit->text());
    settings.setValue("buffer_time", m_ui.bufferSpinBox->value());
    settings.setValue("period_time", m_ui.periodSpinBox->value());
    settings.setValue("mixer_device", m_ui.lineEdit_2->text());
    settings.endGroup();
    QDialog::accept();
}
