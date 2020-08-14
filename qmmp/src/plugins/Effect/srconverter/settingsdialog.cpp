#include <QSettings>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    ui.srSpinBox->setValue(settings.value("SRC/sample_rate",48000).toInt());
    ui.engineComboBox->setCurrentIndex(settings.value("SRC/engine", 0).toInt());
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("SRC/sample_rate",ui.srSpinBox->value());
    settings.setValue("SRC/engine", ui.engineComboBox->currentIndex());
    QDialog::accept();
}
