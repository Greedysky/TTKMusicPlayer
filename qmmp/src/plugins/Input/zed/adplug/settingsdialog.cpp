#include "settingsdialog.h"

#include <qmmp/qmmp.h>
#include <QSettings>

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
    m_ui.emulator_combo->addItem("Tatsuyuki Satoh 0.72 (MAME, 2003)");
    m_ui.emulator_combo->addItem("Nuked OPL3 (Nuke.YKT, 2018)");
    m_ui.emulator_combo->addItem("WoodyOPL (DOSBox, 2016)");
    m_ui.emulator_combo->addItem("Ken Silverman (2001)");

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("AdPlug");
    m_ui.emulator_combo->setCurrentIndex(settings.value("emulator", 0).toInt());
    m_ui.surround->setChecked(settings.value("use_surround", false).toBool());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("AdPlug");
    settings.setValue("emulator", m_ui.emulator_combo->currentIndex());
    settings.setValue("use_surround", m_ui.surround->isChecked());
    settings.endGroup();
    QDialog::accept();
}
