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

    m_ui.emulator->addItem("Tatsuyuki Satoh 0.72 (MAME, 2003)");
    m_ui.emulator->addItem("Nuked OPL3 (Nuke.YKT, 2018)");
    m_ui.emulator->addItem("WoodyOPL (DOSBox, 2016)");
    m_ui.emulator->addItem("Ken Silverman (2001)");

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("AdPlug");

    const int index = m_ui.sampleRate->findData(settings.value("sample_rate", 44100).toInt());
    m_ui.sampleRate->setCurrentIndex(index);

    m_ui.emulator->setCurrentIndex(settings.value("emulator", 0).toInt());
    m_ui.surround->setChecked(settings.value("use_surround", false).toBool());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("AdPlug");

    const int index = m_ui.sampleRate->currentIndex();
    if(index > -1)
    {
        settings.setValue("sample_rate", m_ui.sampleRate->itemData(index));
    }

    settings.setValue("emulator", m_ui.emulator->currentIndex());
    settings.setValue("use_surround", m_ui.surround->isChecked());
    settings.endGroup();
    QDialog::accept();
}
