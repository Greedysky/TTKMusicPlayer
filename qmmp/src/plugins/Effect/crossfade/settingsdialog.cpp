#include <QSettings>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.overlapSpinBox->setValue(settings.value("Crossfade/overlap",6000).toInt());
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Crossfade/overlap", m_ui.overlapSpinBox->value());
    QDialog::accept();
}
