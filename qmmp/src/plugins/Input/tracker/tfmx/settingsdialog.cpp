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
    m_ui.sampleRate->addItem("22050", 22050);
    m_ui.sampleRate->addItem("44100", 44100);
    m_ui.sampleRate->addItem("48000", 48000);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("TFMX");

    const int index = m_ui.sampleRate->findData(settings.value("sample_rate", 44100).toInt());
    m_ui.sampleRate->setCurrentIndex(index);

    m_ui.stereoPan->setValue(settings.value("panning", 75).toInt());
    m_ui.minDuration->setValue(settings.value("min_duration", 10).toInt());
    m_ui.end->setChecked(settings.value("end_shorts", true).toBool());
    m_ui.filter->setChecked(settings.value("filter", false).toBool());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("TFMX");

    const int index = m_ui.sampleRate->currentIndex();
    if(index > -1)
    {
        settings.setValue("sample_rate", m_ui.sampleRate->itemData(index));
    }

    settings.setValue("panning", m_ui.stereoPan->value());
    settings.setValue("min_duration", m_ui.minDuration->value());
    settings.setValue("end_shorts", m_ui.end->isChecked());
    settings.setValue("filter", m_ui.filter->isChecked());
    settings.endGroup();
    QDialog::accept();
}
