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
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.fadeoutCheckBox->setChecked(settings.value("GME/fadeout", false).toBool());
    m_ui.fadeoutSpinBox->setValue(settings.value("GME/fadeout_length", 7000).toInt());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("GME/fadeout", m_ui.fadeoutCheckBox->isChecked());
    settings.setValue("GME/fadeout_length", m_ui.fadeoutSpinBox->value());
    QDialog::accept();
}
