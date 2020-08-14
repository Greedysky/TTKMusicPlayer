#include <QSettings>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    ui.overlapSpinBox->setValue(settings.value("Crossfade/overlap",6000).toInt());
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Crossfade/overlap", ui.overlapSpinBox->value());
    QDialog::accept();
}
