#include "settingsdialog.h"

#include <QSettings>
#include <qmmp/qmmp.h>

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
    settings.beginGroup("TFMX");
    m_ui.sampleRate->setCurrentIndex(settings.value("sample_rate", 1).toInt());
    m_ui.stereoPan->setValue(settings.value("panning", 75).toInt());
    m_ui.minDuration->setValue(settings.value("min_duration", 10).toInt());
    m_ui.end->setChecked(settings.value("end_shorts", true).toBool());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("TFMX");
    settings.setValue("sample_rate", m_ui.sampleRate->currentIndex());
    settings.setValue("panning", m_ui.stereoPan->value());
    settings.setValue("min_duration", m_ui.minDuration->value());
    settings.setValue("end_shorts", m_ui.end->isChecked());
    settings.endGroup();
    QDialog::accept();
}
