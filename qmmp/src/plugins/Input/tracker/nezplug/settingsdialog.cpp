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
    settings.beginGroup("NEZplug");
    m_ui.filter->setCurrentIndex(settings.value("filter", 0).toInt());
    m_ui.gain->setValue(settings.value("gain", 3).toInt());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("NEZplug");
    settings.setValue("filter", m_ui.filter->currentIndex());
    settings.setValue("gain", m_ui.gain->value());
    settings.endGroup();
    QDialog::accept();
}
