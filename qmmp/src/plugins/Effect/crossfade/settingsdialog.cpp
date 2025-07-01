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
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.overlapSpinBox->setValue(settings.value("Crossfade/overlap", 6000).toInt());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Crossfade/overlap", m_ui.overlapSpinBox->value());
    QDialog::accept();
}
